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
 
#ifndef MIBY_CONFIG_H
#define MIBY_CONFIG_H

#ifdef MIBY_USER_CONFIG
#include MIBY_USER_CONFIG
#endif


/** Maximum length of SysEx Receive Buffer **/
#ifndef MIBY_SYSEX_BUF_LEN
#define MIBY_SYSEX_BUF_LEN          ( 128 )
#endif

/** Initial length of SysEx message chunk **/
/** To minimise processing overhead we treat the start of the SysEx
  * message quicker than waiting for an entire buffer's worth of
  * data, only then to find out it is not for this device. 
  * For Manufacturer messages the first 1 or 3 bytes are the 
  * Manufacturer's ID code.
  * Universal SysEx messages always have a 3 byte header, with the
  * first byte being 0x7E for non-realtime and 0x7F for realtime,
  * followed by two sub-ID bytes to specify the message type.
 **/
#ifndef MIBY_SYSEX_FIRST_BUF_LEN
#define MIBY_SYSEX_FIRST_BUF_LEN	( 3 )
#endif


/** Provide default values for handler names **/

/** - System RealTime **/
#ifndef MIBY_HND_RT_CLOCK
#define MIBY_HND_RT_CLOCK           ( NULL )
#endif

#ifndef MIBY_HND_RT_START
#define MIBY_HND_RT_START           ( NULL )
#endif

#ifndef MIBY_HND_RT_CONTINUE
#define MIBY_HND_RT_CONTINUE        ( NULL )
#endif

#ifndef MIBY_HND_RT_STOP
#define MIBY_HND_RT_STOP            ( NULL )
#endif

#ifndef MIBY_HND_RT_ACT_SENSE
#define MIBY_HND_RT_ACT_SENSE       ( NULL )
#endif

#ifndef MIBY_HND_RT_SYS_RESET
#define MIBY_HND_RT_SYS_RESET       ( NULL )
#endif

/** - System Common **/

#ifndef MIBY_HND_SYS_MTC
#define MIBY_HND_SYS_MTC            ( NULL )
#endif

#ifndef MIBY_HND_SYS_SONGPOS
#define MIBY_HND_SYS_SONGPOS        ( NULL )
#endif

#ifndef MIBY_HND_SYS_SONGSEL
#define MIBY_HND_SYS_SONGSEL        ( NULL )
#endif

#ifndef MIBY_HND_SYS_TUNEREQ
#define MIBY_HND_SYS_TUNEREQ        ( NULL )
#endif

/** - System Exclusive **/

#ifndef MIBY_HND_SYS_EX
#define MIBY_HND_SYS_EX             ( NULL )
#endif

/** - Channel Voice **/

#ifndef MIBY_HND_NOTE_ON
#define MIBY_HND_NOTE_ON            ( NULL )
#endif

#ifndef MIBY_HND_NOTE_OFF
#define MIBY_HND_NOTE_OFF           ( NULL )
#endif

#ifndef MIBY_HND_POLY_AT
#define MIBY_HND_POLY_AT            ( NULL )
#endif

#ifndef MIBY_HND_CTRL_CHG
#define MIBY_HND_CTRL_CHG           ( NULL )
#endif

#ifndef MIBY_HND_PROG_CHG
#define MIBY_HND_PROG_CHG           ( NULL )
#endif

#ifndef MIBY_HND_CHAN_AT
#define MIBY_HND_CHAN_AT            ( NULL )
#endif

#ifndef MIBY_HND_PITCHBEND
#define MIBY_HND_PITCHBEND          ( NULL )
#endif


#endif /* MIBY_CONFIG_H */

