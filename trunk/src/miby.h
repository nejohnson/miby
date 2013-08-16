/* ****************************************************************************
 * Miby - MIDI Bytestream Parser for Embedded Systems
 * Copyright (C) 2010, Neil Johnson <http://www.njohnson.co.uk>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms,
 * with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * * Neither the name of nor the names of its contributors
 *   may be used to endorse or promote products derived from this software
 *   without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ************************************************************************* */

#ifndef _MIBY_H
#define _MIBY_H

/** Forward reference **/
typedef struct miby_s *miby_this_t;

/** Pull in the configuration options **/
#include "miby_config.h"


/*****************************************************************************/
/* Macros, constants                                                         */
/*****************************************************************************/

/** Define all the Status Bytes **/
/** -- Channel **/
#define MIDI_STATUS_CH_NOTE_OFF     (0x80)
#define MIDI_STATUS_CH_NOTE_ON      (0x90)
#define MIDI_STATUS_CH_POLY_AT      (0xA0)
#define MIDI_STATUS_CH_CTRL_CHG     (0xB0)
#define MIDI_STATUS_CH_PROG_CHG     (0xC0)
#define MIDI_STATUS_CH_CHAN_AT      (0xD0)
#define MIDI_STATUS_CH_PITCHBEND    (0xE0)

/** -- System Exclusive **/
#define MIDI_STATUS_SYSEX           (0xF0)

/** -- System Common **/
#define MIDI_STATUS_TIMECODE        (0xF1)
#define MIDI_STATUS_SONG_POS        (0xF2)
#define MIDI_STATUS_SONG_SEL        (0xF3)
#define MIDI_STATUS_TUNE_REQ        (0xF6)
#define MIDI_STATUS_EOX             (0xF7)

/** -- System Realtime **/
#define MIDI_STATUS_RT_TIMING       (0xF8)
#define MIDI_STATUS_RT_START        (0xFA)
#define MIDI_STATUS_RT_CONTINUE     (0xFB)
#define MIDI_STATUS_RT_STOP         (0xFC)
#define MIDI_STATUS_RT_ACT_SENS     (0xFE)
#define MIDI_STATUS_RT_SYS_RESET    (0xFF)


/** SysEx States **/
#define MIBY_SYSEX_IDLE             ( 0 )
#define MIBY_SYSEX_START            ( 1 )
#define MIBY_SYSEX_MID              ( 2 )
#define MIBY_SYSEX_END              ( 3 )
#define MIBY_SYSEX_ABORT            ( 4 )

/** Utility macros for the miby_t state data type **/

/** Convert between real channel IDs and encoded channel IDs.
 *
 * The MIDI standard defines the sixteen MIDI channels in the
 * range 1-16.  The encoding scheme used at the data layer is
 * to map these onto the range 0-15.  Applications, and users,
 * should work with the correct MIDI channel numbers, and let
 * MIBY convert between the real number and the encoded number.
 **/
#define MIBY_CHAN_REAL_TO_ENCD(c)   ((c)-1)
#define MIBY_CHAN_ENCD_TO_REAL(c)   ((c)+1)

/** -- Get the status byte **/
#define MIBY_STATUSBYTE(x)          ((x)->statusbyte)

/** -- Get the first data byte **/
#define MIBY_ARG0(x)                ((x)->buf[0])

/** -- Get the second data byte **/
#define MIBY_ARG1(x)                ((x)->buf[1])

/** -- Get the message channel, in real numbers **/
#define MIBY_CHAN(x)                (MIBY_CHAN_ENCD_TO_REAL((x)->msg_chan))

/** -- Get the length of a SysEx chunk **/
#define MIBY_SYSEX_LEN(x)           ((x)->idx)

/** -- Get the data byte at offset y **/
#define MIBY_SYSEX_BUF(x,y)         ((x)->buf[(y)])

/** -- Get the current SysEx state **/
#define MIBY_SYSEX_STATE(x)         ((x)->sysexstate)

/** -- Signify that the SysEx message has been successfully processed **/
#define MIBY_SYSEX_DONE_OK(x)       ((x)->idx=0)

/** Error flags set by the parser.  Must be cleared by the application. **/
#define MIBY_ERROR_MISSING_DATA(x)  ((x)->err.missing)
#define MIBY_CLEAR_MISSING_DATA(x)  ((x)->err.missing=0)

/*****************************************************************************/
/* Data types                                                                */
/*****************************************************************************/

/** Miby state data type **/

typedef struct miby_s {
    unsigned char statusbyte;                 /** Current status byte      **/
    unsigned char msg_chan;                   /** For channel msgs         **/
    unsigned char sysexstate;                 /** Current SysEx state      **/
    unsigned char basic_channel;              /** Encoded basic channel    **/
    unsigned char top_channel;                /** Encoded top channel      **/
    struct {                                  /** Parser error flags:      **/
        unsigned int missing : 1;             /** - missing data           **/
    } err;
    int msglen;                               /** Maximum message length   **/
    int idx;                                  /** Index into buf           **/
    unsigned char buf[MIBY_SYSEX_BUF_LEN];    /** Receive buffer           **/
    void (*handler)(struct miby_s *);         /** Current message handler  **/
    void *v;                                  /** Opaque user data         **/
} miby_t;

/*****************************************************************************/
/* Public functions                                                          */
/*****************************************************************************/

/*****************************************************************************/
/**
    Initialise an instance of the MIBY parser.
    
    MIBY itself does no memory allocation.  The caller must create the state 
    object and pass its address to miby_init().  This allows for the state
    object to be anywhere in the standard C memory spaces: global, local, etc.
    
    @param this     Pointer to MIBY parser state object
    @param v        User-supplied opaque pointer
**/
extern void miby_init( miby_t * /* this */, void * /* v */ );

/*****************************************************************************/
/**
    MIBY - MIDI bytestream parser
    
    This is the full MIDI parser.  It processes a stream of bytes formatted
    according to "The Complete MIDI 1.0 Detailed Specification revision, 96.1".
    
    This intended to be a complete parser, calling out to handlers to process
    specific messages.  All illegal data bytes are silently dropped and the 
    parser will resync to the next valid incoming message.
    
    At the current time there is one global handler table for realtime 
    messages and one global handler table for non-realtime messages.  If a
    system requires multiple parsers with different handler behaviours then 
    there are two ways to achieve this: per-parser handler tables, or userdata
    (the "v" void * field in the state object).  Given the size of the tables
    and the intended target systems the preferred method is userdata (the "v"
    field).
    
    The parser is called for each received byte, either as it is received
    (perhaps with interrupt context) or batched up and processed from a FIFO
    queue in an RTOS.

    @param this     Pointer to MIBY parser state object
    @param rxbyte   Byte to process
**/
extern void miby_parse( miby_t * /* this */, unsigned char /* rxbyte */ );

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

#endif /* _MIBY_H */
