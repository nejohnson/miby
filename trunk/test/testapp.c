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
 
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>

#include "miby.h"

#define TAB			"\t\t"
#define NEWTAB		"\n" TAB

/*****************************************************************************/
/** Realtime Timing Clock                                                   **/
/*****************************************************************************/
void test_rt_timing_clock( miby_this_t this )
{
	printf( NEWTAB "[F8] " );
	printf( "SysRT: timing clock\n" );	
}

/*****************************************************************************/
/** Realtime Start                                                          **/
/*****************************************************************************/
void test_rt_start( miby_this_t this )
{
	printf( NEWTAB "[FA] " );
	printf( "SysRT: start\n" );
}

/*****************************************************************************/
/** Realtime Continue                                                       **/
/*****************************************************************************/
void test_rt_continue( miby_this_t this )
{
	printf( NEWTAB "[FB] " );
	printf( "SysRT: continue\n" );
}

/*****************************************************************************/
/** Realtime Stop                                                           **/
/*****************************************************************************/
void test_rt_stop( miby_this_t this )
{
	printf( NEWTAB "[FC] " );
	printf( "SysRT: stop\n" );
}

/*****************************************************************************/
/** Realtime Active Sense                                                   **/
/*****************************************************************************/
void test_rt_active_sense( miby_this_t this )
{
	printf( NEWTAB "[FE] " );
	printf( "SysRT: active sense\n" );
}

/*****************************************************************************/
/** Realtime System Reset                                                   **/
/*****************************************************************************/
void test_rt_system_reset( miby_this_t this )
{
	printf( NEWTAB "[FF] " );
	printf( "SysRT: system reset\n" );
}

/*****************************************************************************/
/** MIDI Time Code                                                          **/
/*****************************************************************************/
void test_mtc( miby_this_t this )
{
	assert( MIBY_STATUSBYTE(this) == 0xF1 );
	assert( MIBY_ARG0(this) < 0x80 );
	
	printf( NEWTAB "[%02X %02X] ", MIBY_STATUSBYTE(this), MIBY_ARG0(this) );
	printf( "SysCom: Timecode :: type = %02X, value = %02X\n", MIBY_ARG0(this) >> 4, MIBY_ARG0(this) & 0xF );	
}

/*****************************************************************************/
/** Song Position                                                           **/
/*****************************************************************************/
void test_songpos( miby_this_t this )
{
	assert( MIBY_STATUSBYTE(this) == 0xF2 );
	assert( MIBY_ARG0(this) < 0x80 );
	assert( MIBY_ARG1(this) < 0x80 );
	
	printf( NEWTAB "[%02X %02X %02X] ", MIBY_STATUSBYTE(this), MIBY_ARG0(this), MIBY_ARG1(this) );
	printf( "SysCom: Song Position :: LSB = %02X, MSB = %02X\n", MIBY_ARG0(this), MIBY_ARG1(this) );
}

/*****************************************************************************/
/** Song Select                                                             **/
/*****************************************************************************/
void test_songsel( miby_this_t this )
{
	assert( MIBY_STATUSBYTE(this) == 0xF3 );
	assert( MIBY_ARG0(this) < 0x80 );
	
	printf( NEWTAB "[%02X %02X] ", MIBY_STATUSBYTE(this), MIBY_ARG0(this) );
	printf( "SysCom: Song Select :: song %02X\n", MIBY_ARG0(this) );
}

/*****************************************************************************/
/** Tune Request                                                            **/
/*****************************************************************************/
void test_tunereq( miby_this_t this )
{
	assert( MIBY_STATUSBYTE(this) == 0xF6 );
	
	printf( NEWTAB "[%02X] ", MIBY_STATUSBYTE(this) );
	printf( "SysCom: Tune Request\n" );	
}

/*****************************************************************************/
/** Note Off                                                                **/
/*****************************************************************************/
void test_note_off( miby_this_t this )
{
	assert( MIBY_STATUSBYTE(this) >= 0x80 && MIBY_STATUSBYTE(this) <= 0x8F );
	assert( MIBY_ARG0(this) < 0x80 );
	assert( MIBY_ARG1(this) < 0x80 );
	
	printf( NEWTAB "[%02X %02X %02X] ", MIBY_STATUSBYTE(this), MIBY_ARG0(this), MIBY_ARG1(this) );
	printf( "<Ch.%d> ", MIBY_CHAN(this) + 1 );
	printf( "Note Off :: note = %02X, vel = %02X\n", MIBY_ARG0(this), MIBY_ARG1(this) );
}

/*****************************************************************************/
/** Note On                                                                 **/
/*****************************************************************************/
void test_note_on( miby_this_t this )
{
	assert( MIBY_STATUSBYTE(this) >= 0x90 && MIBY_STATUSBYTE(this) <= 0x9F );
	assert( MIBY_ARG0(this) < 0x80 );
	assert( MIBY_ARG1(this) < 0x80 );
	
	printf( NEWTAB "[%02X %02X %02X] ", MIBY_STATUSBYTE(this), MIBY_ARG0(this), MIBY_ARG1(this) );
	printf( "<Ch.%d> ", MIBY_CHAN(this) + 1 );
	printf( "Note On :: note = %02X, vel = %02X\n", MIBY_ARG0(this), MIBY_ARG1(this) );
}

/*****************************************************************************/
/** Polyphonic Aftertouch                                                   **/
/*****************************************************************************/
void test_poly_at( miby_this_t this )
{
	assert( MIBY_STATUSBYTE(this) >= 0xA0 && MIBY_STATUSBYTE(this) <= 0xAF );
	assert( MIBY_ARG0(this) < 0x80 );
	assert( MIBY_ARG1(this) < 0x80 );
	
	printf( NEWTAB "[%02X %02X %02X] ", MIBY_STATUSBYTE(this), MIBY_ARG0(this), MIBY_ARG1(this) );
	printf( "<Ch.%d> ", MIBY_CHAN(this) + 1 );
	printf( "Poly Touch :: note = %02X, pressure = %02X\n", MIBY_ARG0(this), MIBY_ARG1(this) );
}

/*****************************************************************************/
/** Continuous Controller                                                   **/
/*****************************************************************************/
void test_cc( miby_this_t this )
{
	assert( MIBY_STATUSBYTE(this) >= 0xB0 && MIBY_STATUSBYTE(this) <= 0xBF );
	assert( MIBY_ARG0(this) < 0x80 );
	assert( MIBY_ARG1(this) < 0x80 );
	
	printf( NEWTAB "[%02X %02X %02X] ", MIBY_STATUSBYTE(this), MIBY_ARG0(this), MIBY_ARG1(this) );
	printf( "<Ch.%d> ", MIBY_CHAN(this) + 1 );
	printf( "Ctrl Change :: control # = %02X, value = %02X\n", MIBY_ARG0(this), MIBY_ARG1(this) );
}

/*****************************************************************************/
/** Program Change                                                          **/
/*****************************************************************************/
void test_pc( miby_this_t this )
{
	assert( MIBY_STATUSBYTE(this) >= 0xC0 && MIBY_STATUSBYTE(this) <= 0xCF );
	assert( MIBY_ARG0(this) < 0x80 );
	
	printf( NEWTAB "[%02X %02X] ", MIBY_STATUSBYTE(this), MIBY_ARG0(this) );
	printf( "<Ch.%d> ", MIBY_CHAN(this) + 1 );
	printf( "Prog Change :: program # = %02X\n", MIBY_ARG0(this) );
}

/*****************************************************************************/
/** Channel Aftertouch                                                      **/
/*****************************************************************************/
void test_chanat( miby_this_t this )
{
	assert( MIBY_STATUSBYTE(this) >= 0xD0 && MIBY_STATUSBYTE(this) <= 0xDF );
	assert( MIBY_ARG0(this) < 0x80 );
	
	printf( NEWTAB "[%02X %02X] ", MIBY_STATUSBYTE(this), MIBY_ARG0(this) );
	printf( "<Ch.%d> ", (MIBY_STATUSBYTE(this) & 0xF) + 1 );
	printf( "Chan Touch :: pressure = %02X\n", MIBY_ARG0(this) );
}

/*****************************************************************************/
/** PitchBend                                                               **/
/*****************************************************************************/
void test_pb( miby_this_t this )
{
	assert( MIBY_STATUSBYTE(this) >= 0xE0 && MIBY_STATUSBYTE(this) <= 0xEF );
	assert( MIBY_ARG0(this) < 0x80 );
	assert( MIBY_ARG1(this) < 0x80 );
	
	printf( NEWTAB "[%02X %02X %02X] ", MIBY_STATUSBYTE(this), MIBY_ARG0(this), MIBY_ARG1(this) );
	printf( "<Ch.%d> ", MIBY_CHAN(this) + 1 );
	printf( "Pitchbend :: lsb = %02X, msb = %02X (%04X)\n", 
					MIBY_ARG0(this), MIBY_ARG1(this),
					( MIBY_ARG1(this) << 7 ) + MIBY_ARG0(this) );
}

/*****************************************************************************/
/** System Exclusive                                                        **/
/*****************************************************************************/
void test_sysex( miby_this_t this )
{
	int i;
	static char *sysex_state[] = { "IDLE", "START", "MID", "END", "ABORT" };
	
	printf( NEWTAB "[F0] SYSEX chunk: state = %02X (%s), length = %02X bytes\n", 
				MIBY_SYSEX_STATE(this),
				sysex_state[MIBY_SYSEX_STATE(this)], 
				MIBY_SYSEX_LEN(this) );
	printf( TAB "\t[" );
	for ( i = 0; i < MIBY_SYSEX_LEN(this); i++ )
	{
		if ( i )
			printf( " " );
		printf( "%02X", MIBY_SYSEX_BUF(this,i) );
		if ( i % 8 == 7 )
			printf( NEWTAB "\t" );
	}
	printf( "]" );
	
	MIBY_SYSEX_DONE_OK(this);
}	

/*****************************************************************************/
/**
	Test application.
	
	Enter MIDI bytes in 2-digit hexadecimal.  Do not prepend "0x".
	Or pipe from the output of another program, e.g., cat.
**/
/*****************************************************************************/
int main( void )
{
	miby_t m;
	
	printf( "MIBY test app\nEnter hex values (no \"0x\") -1 to end\n" );
	
	miby_init( &m, NULL );
	
	while ( 1 )
	{
		int c;
		
		scanf ( "%x", &c );		
		if ( c < 0 )
			break;

		printf("[%2.2X]", c);
		miby_parse( &m, c);
		if ( m.err.missing )
		{
			printf( "*** MISSING DATA ***\n" );
			m.err.missing = 0;
		}
		printf("\n");
	}
	
	return 0;
}
/*****************************************************************************/
