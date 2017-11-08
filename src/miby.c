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

/*****************************************************************************/
/* System Includes                                                           */
/*****************************************************************************/

#include <stdlib.h>
#include <string.h>

/*****************************************************************************/
/* Project Includes                                                          */
/*****************************************************************************/

#include "miby.h"

/*****************************************************************************/
/* Macros, constants                                                         */
/*****************************************************************************/

/** Byte classification **/
#define BYTE_IS_DATA(b)                 ((b) <  MIDI_STATUS_CH_NOTE_OFF)
#define BYTE_IS_SYS(b)                  ((b) >= MIDI_STATUS_SYSEX)
#define BYTE_IS_CHAN(b)                 ((b) <  MIDI_STATUS_SYSEX)
#define BYTE_IS_SYSRT(b)                ((b) >= MIDI_STATUS_RT_TIMING)

/** Bitmasks to split channel status bytes **/
#define MASK_CHAN                       (0x0F)
#define MASK_CMD                        (0xF0)

#define STATUS_NULL                     ( 0 )


/*****************************************************************************/
/* Data types                                                                */
/*****************************************************************************/

/**
    General MIDI Message Handler Table
**/

/** A handler is a function which takes a pointer to a miby_t state object and
    has no return value.
**/
typedef void (*handler_t)(miby_t *);

/** Each entry in the main handler table records a handler function and the
    (maximum) number of bytes for that message.
**/
typedef struct {
    handler_t    handler;
    unsigned int len;
} msghandler_t;

/** A simple hash - just add the two nibbles together and ignore carry **/
#define HASH(x)     ( 0x0F & ( (x) + ( (x) >> 4 ) ) )

/** The handler table contains 2^4 entries to encompass all sixteen values that
    the hash function generates.  Some entries are undefined by the MIDI 
    specification, and one entry should never be generated by the hash function.
**/
static const msghandler_t msg_table[16] = {
    { (handler_t)MIBY_HND_SYS_MTC,     1 },            /* 00 = F1 = TimeCode */
    { (handler_t)MIBY_HND_SYS_SONGPOS, 2 },            /* 01 = F2 = Song Pos */
    { (handler_t)MIBY_HND_SYS_SONGSEL, 1 },            /* 02 = F3 = Song Sel */
    { (handler_t)NULL,                 0 },            /* 03 = F4 = UNDEFIND */
    { (handler_t)NULL,                 0 },            /* 04 = F5 = UNDEFIND */
    { (handler_t)MIBY_HND_SYS_TUNEREQ, 0 },            /* 05 = F6 = Tune Req */
    { (handler_t)NULL,                 0 },            /* 06 = F7 = EOX      */
    { (handler_t)NULL,                 0 },            /* 07 = NOT USED      */
    { (handler_t)MIBY_HND_NOTE_OFF,    2 },            /* 08 = 80 = Note Off */
    { (handler_t)MIBY_HND_NOTE_ON,     2 },            /* 09 = 90 = Note On  */
    { (handler_t)MIBY_HND_POLY_AT,     2 },            /* 0A = A0 = Poly AT  */
    { (handler_t)MIBY_HND_CTRL_CHG,    2 },            /* 0B = B0 = Ctrl Chg */
    { (handler_t)MIBY_HND_PROG_CHG,    1 },            /* 0C = C0 = Prog Chg */
    { (handler_t)MIBY_HND_CHAN_AT,     1 },            /* 0D = D0 = Chan AT  */
    { (handler_t)MIBY_HND_PITCHBEND,   2 },            /* 0E = E0 = PitchBnd */
    { (handler_t)MIBY_HND_SYS_EX, MIBY_SYSEX_FIRST_BUF_LEN }  /* 0F=F0=SysEx */
};

/*****************************************************************************/
/**
    Realtime System Exclusive Handler Table
    
    This table contains the handlers in numeric order, offset by the value of
    the first realtime status byte.  Two entries are undefined by the MIDI 
    specification. 
**/
static const handler_t rt_handler_table[8] = {
    MIBY_HND_RT_CLOCK,
    NULL, /* reserved */
    MIBY_HND_RT_START,
    MIBY_HND_RT_CONTINUE,
    MIBY_HND_RT_STOP,
    NULL, /* reserved */
    MIBY_HND_RT_ACT_SENSE,
    MIBY_HND_RT_SYS_RESET
};

/*****************************************************************************/
/* Public functions, as declared in public header.                           */
/*****************************************************************************/

/*****************************************************************************/
/**
    Initialise an instance of the MIBY parser.
    
    MIBY itself does no memory allocation.  The caller must create the state 
    object and pass its address to miby_init().  This allows for the state
    object to be anywhere in the standard C memory spaces- global, local, etc.
    
    @param this     Pointer to MIBY parser state object
    @param v        User-supplied opaque pointer
**/
void miby_init( miby_t *this, void *v )
{
    memset( this, 0, sizeof( *this ) );

    /* Set the MIDI channel filter to wide open to receive everything. */
    this->basic_channel = MIBY_CHAN_REAL_TO_ENCD( 1 );
    this->top_channel   = MIBY_CHAN_REAL_TO_ENCD( 16 );
    
    this->sysexstate    = MIBY_SYSEX_IDLE;    
    this->v             = v;
}

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
void miby_parse( miby_t *this, unsigned char rxbyte )
{
    /* Catch System Realtime messages early - they can appear ANYWHERE in the
     *  stream.
     */
    if ( BYTE_IS_SYSRT( rxbyte ) )
    {
        handler_t h = rt_handler_table[rxbyte - MIDI_STATUS_RT_TIMING];
        if ( h )
            (h)( this );

        return;
    }

    /* Treat data bytes and status bytes separately **************************/
    if ( BYTE_IS_DATA( rxbyte ) )
    {
        /* If there is no status byte then this data byte is an orphan, or
         *  noise, so drop it.
         */
        if ( this->statusbyte == STATUS_NULL )
            return;

        /* Append the received byte to the receive buffer */
        this->buf[this->idx++] = rxbyte;

        /* Until we have received the required number of data bytes we keep
         *  going.
         */
        if ( this->idx < this->msglen )
            return;

        /* Ok, at this point we have received all the data bytes we are
         *  expecting.
         * First check if we are in the midst of a SysEx message.
         */
        if ( this->sysexstate != MIBY_SYSEX_IDLE )
        {
            /* This is part of a SysEx message.  Go and call the handler,
             *  which will set this->idx to 0 to signal successful
             *  consumption of the SysEx data (e.g. this SysEx is for this
             *  device).
             */
            (this->handler)( this );
            if ( this->idx != 0 )
            {
                /* This SysEx chunk caused a problem, so we move back to the
                 *  IDLE state so that the rest of this SysEx message will be
                 *  ignored.  Also clear out the status byte while we're at it
                 *  forcing the parser to resync to the next status byte.
                 */
                this->sysexstate = MIBY_SYSEX_IDLE;
                this->statusbyte = STATUS_NULL;
            }
            else
            {
                /* The chunk was handled successfully so move (if not already)
                 *  to the MIDdle state and update the message length to the 
                 *  full size.
                 */
                this->sysexstate = MIBY_SYSEX_MID;
                this->msglen     = MIBY_SYSEX_BUF_LEN;
            }
        }
        else /* Not in SysEx mode */
        {
            /* This is any other message, so just call its handler */
            (this->handler)( this );

            /* Channel messages can use Running Status - reusing the previous
             *  status byte for the next message.  If this is not a Channel 
             *  message then clear the status byte.
             */
            if ( !BYTE_IS_CHAN( this->statusbyte ) )
                this->statusbyte = STATUS_NULL;
        }

        /* Message processed, reset the receive buffer */
        this->idx = 0;
    }
    else /* This is a status byte ********************************************/
    {
        unsigned char i;

        /* Reset the status byte field ready for the new one */
        this->statusbyte = STATUS_NULL;

        /* If we are in the middle of a SysEx message then we need to terminate
         *  it somehow.
         */
        if ( this->sysexstate != MIBY_SYSEX_IDLE )
        {
            /* A status byte marks the end of a SysEx message */
            this->sysexstate = MIBY_SYSEX_END;

            /* However if the status byte is not an End Of Exclusive (EOX)
             *  then we treat this as a aborted SysEx message.
             */
            if ( rxbyte != MIDI_STATUS_EOX )
                this->sysexstate = MIBY_SYSEX_ABORT;

            /* Pass the received data to the SysEx handler */
            (this->handler)( this );
            
            /* Then back to IDLE state ready for the start of the next one */
            this->sysexstate = MIBY_SYSEX_IDLE;

            /* If this status byte is EOX then we consume it here.  Any other
             *  status byte needs to be further processed.
             */
            if ( rxbyte == MIDI_STATUS_EOX )
                return;
        }
        else if ( this->idx != 0 )
        {
            /* For some reason there is data in the receive buffer that has
             *  not been processed.  We can only assume that we missed a status
             *  byte or one or more data bytes.  Flag it and carry on.
             */
             
            this->err.missing = 1;
        }

        /* Channel status bytes have a channel field in then.  Extract it and
         *  then strip out the channel field to normalise the status bytes.
         *
         * Note: this is all done in encoded format, not real format.
         */
        if ( BYTE_IS_CHAN( rxbyte ) )
        {
            unsigned char chan = rxbyte & MASK_CHAN;

            /* Filter out channel messages for channels that are currently 
                being ignored.
             */
            if ( chan < this->basic_channel || chan > this->top_channel )
                return;

            rxbyte &= MASK_CMD;
            this->msg_chan = chan;
        }

        /* Get the handler from the main message handler table.  We use a
         *  hash to map all valid status bytes into a small table.
         */
        i = HASH(rxbyte);
        this->handler = msg_table[i].handler;

        /* If there is no supplied handler for this status byte then drop the
         *   message.
         */
        if ( this->handler == NULL )
            return;

        /* For a message with a handler get the required number of data bytes */
        this->msglen = msg_table[i].len;

        /* For messages with no data bytes we handle them here directly */
        if ( this->msglen == 0 )
        {
            (this->handler)( this );
            return;
        }

        /* Prepare the parser to process this message, setting up the
         *  statusbyte field and reseting the receive buffer.
         */
        this->statusbyte = rxbyte;
        this->idx = 0;

        /* Finally, check if this is the start of a SysEx message */
        if ( this->statusbyte == MIDI_STATUS_SYSEX )
        {
            this->sysexstate = MIBY_SYSEX_START;
        }
    }
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
