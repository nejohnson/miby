# Miby

Miby is a small MIDI 1.0 byte-stream parser intended for embedded C projects.
It consumes one received byte at a time, keeps the parser state in a caller-owned
`miby_t`, and calls application handlers when complete MIDI messages are
received.

The library does not allocate memory. Applications decide where parser state
lives, which message handlers are compiled in, and whether System Exclusive
support is included.

## Files

- `src/miby.h`: public types, constants, access macros, and function
  declarations.
- `src/miby_config.h`: compile-time configuration defaults.
- `src/miby.c`: core MIDI byte-stream parser.
- `src/miby_cc.c`: optional helper for classifying Control Change messages.

## Basic Use

Provide a configuration header which maps the Miby handler macros to your
functions, compile `miby.c` with that header, initialise a `miby_t`, then pass
each received byte to `miby_parse()`.

```c
/* my_miby_config.h */
#ifndef MY_MIBY_CONFIG_H
#define MY_MIBY_CONFIG_H

extern void app_note_on(miby_this_t m);
#define MIBY_HND_NOTE_ON app_note_on

extern void app_clock(miby_this_t m);
#define MIBY_HND_RT_CLOCK app_clock

#endif
```

```c
#include "miby.h"

static miby_t midi;

void app_note_on(miby_this_t m)
{
    unsigned char channel = MIBY_CHAN(m); /* 1 to 16 */
    unsigned char note = MIBY_ARG0(m);
    unsigned char velocity = MIBY_ARG1(m);

    (void)channel;
    (void)note;
    (void)velocity;
}

void app_clock(miby_this_t m)
{
    (void)m;
}

void midi_init(void)
{
    miby_init(&midi, NULL);
}

void midi_rx_byte(unsigned char byte)
{
    miby_parse(&midi, byte);
}
```

Compile with the configuration header named by `MIBY_USER_CONFIG`:

```sh
cc -I src -I . -DMIBY_USER_CONFIG=\"my_miby_config.h\" -c src/miby.c
```

Compile every Miby source file used by the application with the same
`MIBY_USER_CONFIG` definition. If the Control Change helper is used, compile
`src/miby_cc.c` with the same flags.

Handlers not defined by the application default to `NULL`; messages for those
handlers are ignored.

## Public API

### `miby_init()`

```c
void miby_init(miby_t *m, void *userdata);
```

Initialises a parser instance. This clears the complete `miby_t`, opens the
channel filter to channels 1 through 16, sets the SysEx state to idle when
SysEx support is enabled, and stores `userdata` in `m->v`.

Miby does no allocation, so `miby_t` may be static, global, stack allocated, or
embedded in another object.

### `miby_parse()`

```c
void miby_parse(miby_t *m, unsigned char rxbyte);
```

Processes one MIDI byte. The parser may call zero or one non-realtime handler
for the byte, plus realtime handlers whenever realtime status bytes are seen.

Data bytes received without a current status byte are dropped. If a new status
byte arrives while a partial non-SysEx message is buffered, Miby sets the
`MIBY_ERROR_MISSING_DATA(m)` flag and resynchronises to the new status byte.
The application must clear that flag with `MIBY_CLEAR_MISSING_DATA(m)`.

### `miby_cc_init()`

```c
void miby_cc_init(miby_cc_t *cc);
```

Initialises the optional Control Change helper state. Use this only when
compiling and using `src/miby_cc.c`.

### `miby_cc()`

```c
void miby_cc(miby_this_t m);
```

Classifies a Control Change message into 7-bit, 14-bit, switch, RPN, NRPN, or
channel-mode events. This function is normally installed as the Control Change
handler:

```c
extern void miby_cc(miby_this_t m);
#define MIBY_HND_CTRL_CHG miby_cc
```

## Handler Configuration

Miby uses compile-time handler macros instead of runtime registration. Define
the macros you need in the header named by `MIBY_USER_CONFIG`.

All handlers have this shape:

```c
void handler(miby_this_t m);
```

`miby_this_t` is a pointer to the parser state. The public header defines this
type before including the user configuration header, so handler prototypes in
the configuration file may use it.

### Channel Voice Handlers

| Macro | MIDI status | Data bytes |
| --- | --- | --- |
| `MIBY_HND_NOTE_OFF` | `0x8n` | note, velocity |
| `MIBY_HND_NOTE_ON` | `0x9n` | note, velocity |
| `MIBY_HND_POLY_AT` | `0xAn` | note, pressure |
| `MIBY_HND_CTRL_CHG` | `0xBn` | controller, value |
| `MIBY_HND_PROG_CHG` | `0xCn` | program |
| `MIBY_HND_CHAN_AT` | `0xDn` | pressure |
| `MIBY_HND_PITCHBEND` | `0xEn` | LSB, MSB |

For channel messages, `MIBY_STATUSBYTE(m)` is normalised to the command nibble
only, such as `0x90` for Note On. Use `MIBY_CHAN(m)` for the MIDI channel as a
real channel number from 1 to 16, or `MIBY_CHAN_RAW(m)` for the encoded channel
number from 0 to 15.

### System Common Handlers

| Macro | MIDI status | Data bytes |
| --- | --- | --- |
| `MIBY_HND_SYS_MTC` | `0xF1` | time-code quarter-frame |
| `MIBY_HND_SYS_SONGPOS` | `0xF2` | LSB, MSB |
| `MIBY_HND_SYS_SONGSEL` | `0xF3` | song number |
| `MIBY_HND_SYS_TUNEREQ` | `0xF6` | none |
| `MIBY_HND_SYS_EX` | `0xF0` | chunked SysEx data |

Undefined statuses `0xF4` and `0xF5` are ignored. End Of Exclusive `0xF7` is
handled as part of SysEx processing and is otherwise ignored.

### System Realtime Handlers

| Macro | MIDI status |
| --- | --- |
| `MIBY_HND_RT_CLOCK` | `0xF8` |
| `MIBY_HND_RT_START` | `0xFA` |
| `MIBY_HND_RT_CONTINUE` | `0xFB` |
| `MIBY_HND_RT_STOP` | `0xFC` |
| `MIBY_HND_RT_ACT_SENSE` | `0xFE` |
| `MIBY_HND_RT_SYS_RESET` | `0xFF` |

Realtime messages are recognised before all other parser state. They can appear
between data bytes, during running status, or inside a SysEx stream without
disturbing the surrounding message.

## Reading Message Data

Handlers read the current message through macros in `miby.h`.

| Macro | Meaning |
| --- | --- |
| `MIBY_STATUSBYTE(m)` | Current status byte. Channel status bytes are normalised. |
| `MIBY_ARG0(m)` | First data byte. |
| `MIBY_ARG1(m)` | Second data byte. |
| `MIBY_CHAN(m)` | Channel as 1 to 16. |
| `MIBY_CHAN_RAW(m)` | Channel as encoded 0 to 15. |
| `MIBY_ERROR_MISSING_DATA(m)` | Nonzero when a partial message was interrupted. |
| `MIBY_CLEAR_MISSING_DATA(m)` | Clears the missing-data error flag. |

`MIBY_ARG0()` and `MIBY_ARG1()` are valid according to the message length shown
in the handler tables above.

Pitch bend and 14-bit values are transmitted least-significant 7 bits first.
For pitch bend:

```c
unsigned short bend = ((unsigned short)MIBY_ARG1(m) << 7) | MIBY_ARG0(m);
```

## Running Status

Channel messages keep their status byte after the handler returns, allowing
subsequent channel data bytes to be parsed using MIDI running status.

System Common messages clear the current status after handling. System Realtime
messages do not affect running status.

## Channel Filtering

By default, the parser accepts all 16 MIDI channels. Applications can restrict
channel voice messages to an inclusive range:

```c
MIBY_SET_BASIC_CHAN(&midi, 2);
MIBY_SET_TOP_CHAN(&midi, 4);
```

The setter macros take real MIDI channel numbers, 1 through 16. Internally Miby
stores encoded channel numbers, 0 through 15.

Filtered channel status bytes are ignored. The filter applies to Channel Voice
messages only; System Common, SysEx, and System Realtime messages are not
channel messages and are not filtered.

## System Exclusive

SysEx support is enabled by default. Define `MIBY_NO_SYSEX` in the user
configuration header to remove it:

```c
#define MIBY_NO_SYSEX
```

With SysEx enabled, incoming SysEx data is delivered to `MIBY_HND_SYS_EX` in
chunks. The handler can inspect:

| Macro | Meaning |
| --- | --- |
| `MIBY_SYSEX_STATE(m)` | One of the `MIBY_SYSEX_*` state constants. |
| `MIBY_SYSEX_LEN(m)` | Number of bytes currently in the SysEx buffer. |
| `MIBY_SYSEX_BUF(m, i)` | Data byte at offset `i`. |
| `MIBY_SYSEX_DONE_OK(m)` | Marks a chunk as successfully consumed. |

SysEx state values are:

| State | Meaning |
| --- | --- |
| `MIBY_SYSEX_START` | First SysEx data chunk after `0xF0`. |
| `MIBY_SYSEX_MID` | Later full-size data chunks. |
| `MIBY_SYSEX_END` | SysEx ended by `0xF7`; buffer contains the final partial chunk. |
| `MIBY_SYSEX_ABORT` | SysEx was interrupted by a non-EOX status byte. |
| `MIBY_SYSEX_IDLE` | No SysEx message is active. |

The initial chunk length defaults to `MIBY_SYSEX_FIRST_BUF_LEN`, which is `3`.
Later chunks use `MIBY_RX_BUF_LEN`, which defaults to `128`.

The SysEx handler must call `MIBY_SYSEX_DONE_OK(m)` after successfully accepting
a `START` or `MID` chunk. If it leaves the buffer index nonzero after such a
chunk, the parser treats the SysEx message as rejected, returns to idle, clears
the current status, and ignores the remaining data until another status byte
resynchronises the stream.

On `MIBY_SYSEX_END` and `MIBY_SYSEX_ABORT`, the handler is called once with the
final buffered data and the appropriate state. The parser then returns to idle.
An EOX byte is consumed by the SysEx parser. A non-EOX status byte that aborts
SysEx is then processed as the start of the next MIDI message.

Example:

```c
void app_sysex(miby_this_t m)
{
    unsigned int i;

    switch (MIBY_SYSEX_STATE(m)) {
    case MIBY_SYSEX_START:
    case MIBY_SYSEX_MID:
        for (i = 0; i < MIBY_SYSEX_LEN(m); i++) {
            consume_sysex_byte(MIBY_SYSEX_BUF(m, i));
        }
        MIBY_SYSEX_DONE_OK(m);
        break;

    case MIBY_SYSEX_END:
        for (i = 0; i < MIBY_SYSEX_LEN(m); i++) {
            consume_sysex_byte(MIBY_SYSEX_BUF(m, i));
        }
        finish_sysex_message();
        break;

    case MIBY_SYSEX_ABORT:
        abort_sysex_message();
        break;
    }
}
```

## Control Change Helper

The core parser reports Control Change messages as raw controller and value
bytes through `MIBY_HND_CTRL_CHG`. If the application wants higher-level
classification, compile `src/miby_cc.c`, allocate a `miby_cc_t`, pass it as the
parser userdata, and route Control Change messages to `miby_cc()`.

```c
/* my_miby_config.h */
extern void miby_cc(miby_this_t m);
#define MIBY_HND_CTRL_CHG miby_cc

extern void app_cc_14bit(miby_this_t m);
#define MIBY_HND_CC_14BIT app_cc_14bit

extern void app_cc_switch(miby_this_t m);
#define MIBY_HND_CC_SWITCH app_cc_switch

extern void app_cc_parameter(miby_this_t m);
#define MIBY_HND_CC_PARAMETER app_cc_parameter
```

```c
static miby_t midi;
static miby_cc_t cc;

void midi_init(void)
{
    miby_cc_init(&cc);
    miby_init(&midi, &cc);
}

void app_cc_14bit(miby_this_t m)
{
    miby_cc_event_t *e = MIBY_CC_EVENT(m);

    /* e->controller is 0 to 31. e->value is a 14-bit value. */
}
```

The helper stores per-channel state so it can combine MSB and LSB controller
pairs and remember the currently selected RPN or NRPN.

### Control Change Event Types

Read the current event with:

```c
miby_cc_event_t *e = MIBY_CC_EVENT(m);
```

| Event type | Handler macro | Meaning |
| --- | --- | --- |
| `MIBY_CC_EVENT_7BIT` | `MIBY_HND_CC_7BIT` | Ordinary 7-bit controller. |
| `MIBY_CC_EVENT_14BIT` | `MIBY_HND_CC_14BIT` | MSB/LSB controller pair. |
| `MIBY_CC_EVENT_SWITCH` | `MIBY_HND_CC_SWITCH` | Switch controller. |
| `MIBY_CC_EVENT_PARAMETER` | `MIBY_HND_CC_PARAMETER` | RPN or NRPN data entry/increment/decrement. |
| `MIBY_CC_EVENT_MODE` | `MIBY_HND_CC_MODE` | Channel Mode message, controllers 120 to 127. |

Fields common to most events:

| Field | Meaning |
| --- | --- |
| `type` | One of the `MIBY_CC_EVENT_*` constants. |
| `channel` | Real MIDI channel number, 1 to 16. |
| `controller` | Controller number. For 14-bit events this is the MSB controller, 0 to 31. |
| `value` | 7-bit or combined 14-bit value, depending on event type. |
| `value_msb` | Most significant 7-bit value when available. |
| `value_lsb` | Least significant 7-bit value when available. |
| `flags` | `MIBY_CC_VALUE_HAS_MSB` and/or `MIBY_CC_VALUE_HAS_LSB`. |

For switch controllers, `switch_on` is nonzero when the incoming value is 64 or
greater.

For parameter events:

| Field | Meaning |
| --- | --- |
| `parameter_type` | `MIBY_CC_PARAM_RPN` or `MIBY_CC_PARAM_NRPN`. |
| `parameter` | Selected 14-bit RPN or NRPN number. |
| `action` | `MIBY_CC_ACTION_SET`, `MIBY_CC_ACTION_INCREMENT`, or `MIBY_CC_ACTION_DECREMENT`. |
| `value` | Combined data-entry value for set events; increment/decrement amount for relative events. |

RPN and NRPN selection is ignored until both selector bytes have been received.
The null selector value `0x7F, 0x7F` clears the selected parameter.

## Embedded Configuration

`miby_config.h` provides defaults suitable for hosted C and allows platform
overrides through the user configuration header.

### Read-only Tables

The parser and Control Change helper use static dispatch tables. On platforms
where these tables should live in program memory, define `MIBY_READONLY` and
`MIBY_GET_RO_WORD`.

For AVR-style program memory this resembles:

```c
#include <avr/pgmspace.h>

#define MIBY_READONLY PROGMEM
#define MIBY_GET_RO_WORD(a) pgm_read_word(&(a))
```

### SysEx Buffer Sizes

When SysEx is enabled:

```c
#define MIBY_RX_BUF_LEN 128
#define MIBY_SYSEX_FIRST_BUF_LEN 3
```

`MIBY_RX_BUF_LEN` is also the size of the parser receive buffer. If
`MIBY_NO_SYSEX` is defined, Miby forces `MIBY_RX_BUF_LEN` to `2`, removes the
SysEx state field from `miby_t`, and sets `MIBY_HND_SYS_EX` to `NULL`.

### User Data

The second argument to `miby_init()` is stored in `miby_t.v`. Miby itself does
not interpret it except through the default `MIBY_CC_GET_STATE(m)` macro used
by the Control Change helper:

```c
#define MIBY_CC_GET_STATE(m) ((miby_cc_t *)((m)->v))
```

Applications can use this pointer to hold parser-specific context for shared
handlers. If the Control Change helper is used, the pointer must resolve to a
valid `miby_cc_t` unless `MIBY_CC_GET_STATE` is overridden.

## Error Handling and Resynchronisation

Miby is designed to resynchronise from malformed input:

- Orphan data bytes are dropped.
- Undefined and unhandled status bytes are ignored.
- A new status byte during a partial non-SysEx message sets
  `MIBY_ERROR_MISSING_DATA(m)`.
- Realtime messages are dispatched immediately and do not disturb the current
  message.
- A non-EOX status byte during SysEx reports `MIBY_SYSEX_ABORT`, exits SysEx,
  and then processes that status byte normally.

The parser does not validate the musical meaning of values beyond MIDI byte
shape and message length. For example, Note On with velocity zero remains a
Note On message; applications may choose to treat it as Note Off.

## Tests and Examples

The `test/` directory contains a simple interactive decoder and a self-checking
parser test harness.

Build and run the parser tests:

```sh
make -C test test
```

Build the interactive test application:

```sh
make -C test
```

Then enter MIDI bytes as two-digit hexadecimal values without a `0x` prefix.
