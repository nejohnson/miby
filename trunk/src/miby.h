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

typedef struct miby_s *miby_this_t;

#include "miby_config.h"









typedef struct miby_s {
    unsigned char statusbyte;
    unsigned char msg_chan;
    unsigned char sysexstate;
    unsigned char basic_channel;
    unsigned char top_channel;	
    int msglen;
    int ptr;
    void (*handler)(miby_this_t);
    void *v;
    unsigned char buf[MIBY_SYSEX_BUF_LEN];
} miby_t;

#define MIBY_SYSEX_IDLE             ( 0 )
#define MIBY_SYSEX_START            ( 1 )
#define MIBY_SYSEX_MID              ( 2 )
#define MIBY_SYSEX_END              ( 3 )
#define MIBY_SYSEX_ABORT            ( 4 )


#define MIBY_STATUSBYTE(x)          ((x)->statusbyte)
#define MIBY_ARG0(x)                ((x)->buf[0])
#define MIBY_ARG1(x)                ((x)->buf[1])
#define MIBY_CHAN(x)                ((x)->msg_chan)
#define MIBY_SYSEX_LEN(x)           ((x)->ptr)
#define MIBY_SYSEX_BUF(x,y)         ((x)->buf[(y)])
#define MIBY_SYSEX_STATE(x)         ((x)->sysexstate)
#define MIBY_SYSEX_DONE_OK(x)       ((x)->ptr=0)




extern void miby_init( miby_t *, void * );
extern void miby_parse( miby_t *, unsigned char );





#endif /* _MIBY_H */
