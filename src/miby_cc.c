/* ****************************************************************************
 * Miby - MIDI Bytestream Parser for Embedded Systems
 * Copyright (C) 2017, Neil Johnson <http://www.njohnson.co.uk>
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



/*****************************************************************************/
/* Project Includes                                                          */
/*****************************************************************************/

#include "miby.h"

/*****************************************************************************/
/* Macros, constants                                                         */
/*****************************************************************************/



/*****************************************************************************/
/* Data types                                                                */
/*****************************************************************************/


/*****************************************************************************/
/* Public functions, as declared in public header.                           */
/*****************************************************************************/


void miby_cc( miby_this_t this )
{
    unsigned char status_byte = MIBY_STATUSBYTE(this);
    unsigned char arg0 = MIBY_ARG0(this);
    unsigned char arg1 = MIBY_ARG1(this);

   /*

	ARG0 is control number
	ARG1 is control value

	00-31	Continuous controllers:
		00	bank select
		01	modulation
		02	breath controller
		03	UNDEFD
		04	foot controller
		05	portamento time
		06	data entry MSB
		07	volume
		08	balance
		09	UNDEFD
		10	pan
		11	expression
		12	effect 1
		13	effect 2
		14-15	UNDEFD
		16	general purpose controller #1
		17				   #2
		18				   #3
		19				   #4
		20-31	UNDEFD
		
	32-63	LSB of 00-31 CCs

	64-69	Switches (<64 = off, >=64 = on)
		64	sustain
		65	portamento
		66	sostenuto
		67	soft pedal
		68	legato
		69	hold 2
	70-79	Sound controllers
	80-83	General purpose controllers 5-8
	84	Portamento
	85-87	UNDEFD
	88	Hi-res velocity prefix
	89-90	UNDEFD
	91-95	Effects 1-5 depth
	96	Data increment
	97	Data decrement
	98,99	NRPN (LSB,MSB)
	100,101	RPN  (LSB,MSB)
	102-119	UNDEFD
	--
	Channel Mode Messages:
	120	All sound off
	121	Reset all controllers
	122	Local on/off (off = 0, on = 127)
	123	All notes off
	124	Omni off + all notes off
	125	Omni on  + all notes off
	126	Mono on  + all notes off (value = number channels, 0 = all channels)
	127	Poly on  + all notes off

    */



}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
