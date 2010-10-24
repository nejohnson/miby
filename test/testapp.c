
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>

#include "miby.h"


void test_rt_timing_clock( miby_this_t this )
{
	printf( "\n\t\t[F8] " );
	printf( "SysRT: timing clock\n" );	
}

void test_rt_start( miby_this_t this )
{
	printf( "\n\t\t[FA] " );
	printf( "SysRT: start\n" );
}

void test_rt_continue( miby_this_t this )
{
	printf( "\n\t\t[FB] " );
	printf( "SysRT: continue\n" );
}

void test_rt_stop( miby_this_t this )
{
	printf( "\n\t\t[FC] " );
	printf( "SysRT: stop\n" );
}

void test_rt_active_sense( miby_this_t this )
{
	printf( "\n\t\t[FE] " );
	printf( "SysRT: active sense\n" );
}

void test_rt_system_reset( miby_this_t this )
{
	printf( "\n\t\t[FF] " );
	printf( "SysRT: system reset\n" );
}

void test_mtc( miby_this_t this )
{
	assert( MIBY_STATUSBYTE(this) == 0xF1 );
	assert( MIBY_ARG0(this) < 0x80 );
	
	printf( "\n\t\t[%02X %02X] ", MIBY_STATUSBYTE(this), MIBY_ARG0(this) );
	printf( "SysCom: Timecode :: type = %02X, value = %02X\n", MIBY_ARG0(this) >> 4, MIBY_ARG0(this) & 0xF );	
}

void test_songpos( miby_this_t this )
{
	assert( MIBY_STATUSBYTE(this) == 0xF2 );
	assert( MIBY_ARG0(this) < 0x80 );
	assert( MIBY_ARG1(this) < 0x80 );
	
	printf( "\n\t\t[%02X %02X %02X] ", MIBY_STATUSBYTE(this), MIBY_ARG0(this), MIBY_ARG1(this) );
	printf( "SysCom: Song Position :: LSB = %02X, MSB = %02X\n", MIBY_ARG0(this), MIBY_ARG1(this) );
}

void test_songsel( miby_this_t this )
{
	assert( MIBY_STATUSBYTE(this) == 0xF3 );
	assert( MIBY_ARG0(this) < 0x80 );
	
	printf( "\n\t\t[%02X %02X] ", MIBY_STATUSBYTE(this), MIBY_ARG0(this) );
	printf( "SysCom: Song Select :: song %02X\n", MIBY_ARG0(this) );
}

void test_tunereq( miby_this_t this )
{
	assert( MIBY_STATUSBYTE(this) == 0xF6 );
	
	printf( "\n\t\t[%02X] ", MIBY_STATUSBYTE(this) );
	printf( "SysCom: Tune Request\n" );	
}

void test_note_off( miby_this_t this )
{
	assert( MIBY_STATUSBYTE(this) >= 0x80 && MIBY_STATUSBYTE(this) <= 0x8F );
	assert( MIBY_ARG0(this) < 0x80 );
	assert( MIBY_ARG1(this) < 0x80 );
	
	printf( "\n\t\t[%02X %02X %02X] ", MIBY_STATUSBYTE(this), MIBY_ARG0(this), MIBY_ARG1(this) );
	printf( "<Ch.%d> ", MIBY_CHAN(this) + 1 );
	printf( "Note Off :: note = %02X, vel = %02X\n", MIBY_ARG0(this), MIBY_ARG1(this) );
}

void test_note_on( miby_this_t this )
{
	assert( MIBY_STATUSBYTE(this) >= 0x90 && MIBY_STATUSBYTE(this) <= 0x9F );
	assert( MIBY_ARG0(this) < 0x80 );
	assert( MIBY_ARG1(this) < 0x80 );
	
	printf( "\n\t\t[%02X %02X %02X] ", MIBY_STATUSBYTE(this), MIBY_ARG0(this), MIBY_ARG1(this) );
	printf( "<Ch.%d> ", MIBY_CHAN(this) + 1 );
	printf( "Note On :: note = %02X, vel = %02X\n", MIBY_ARG0(this), MIBY_ARG1(this) );
}

void test_poly_at( miby_this_t this )
{
	assert( MIBY_STATUSBYTE(this) >= 0xA0 && MIBY_STATUSBYTE(this) <= 0xAF );
	assert( MIBY_ARG0(this) < 0x80 );
	assert( MIBY_ARG1(this) < 0x80 );
	
	printf( "\n\t\t[%02X %02X %02X] ", MIBY_STATUSBYTE(this), MIBY_ARG0(this), MIBY_ARG1(this) );
	printf( "<Ch.%d> ", MIBY_CHAN(this) + 1 );
	printf( "Poly Touch :: note = %02X, pressure = %02X\n", MIBY_ARG0(this), MIBY_ARG1(this) );
}

void test_cc( miby_this_t this )
{
	assert( MIBY_STATUSBYTE(this) >= 0xB0 && MIBY_STATUSBYTE(this) <= 0xBF );
	assert( MIBY_ARG0(this) < 0x80 );
	assert( MIBY_ARG1(this) < 0x80 );
	
	printf( "\n\t\t[%02X %02X %02X] ", MIBY_STATUSBYTE(this), MIBY_ARG0(this), MIBY_ARG1(this) );
	printf( "<Ch.%d> ", MIBY_CHAN(this) + 1 );
	printf( "Ctrl Change :: control # = %02X, value = %02X\n", MIBY_ARG0(this), MIBY_ARG1(this) );
}

void test_pc( miby_this_t this )
{
	assert( MIBY_STATUSBYTE(this) >= 0xC0 && MIBY_STATUSBYTE(this) <= 0xCF );
	assert( MIBY_ARG0(this) < 0x80 );
	
	printf( "\n\t\t[%02X %02X] ", MIBY_STATUSBYTE(this), MIBY_ARG0(this) );
	printf( "<Ch.%d> ", MIBY_CHAN(this) + 1 );
	printf( "Prog Change :: program # = %02X\n", MIBY_ARG0(this) );
}

void test_chanat( miby_this_t this )
{
	assert( MIBY_STATUSBYTE(this) >= 0xD0 && MIBY_STATUSBYTE(this) <= 0xDF );
	assert( MIBY_ARG0(this) < 0x80 );
	
	printf( "\n\t\t[%02X %02X] ", MIBY_STATUSBYTE(this), MIBY_ARG0(this) );
	printf( "<Ch.%d> ", (MIBY_STATUSBYTE(this) & 0xF) + 1 );
	printf( "Chan Touch :: pressure = %02X\n", MIBY_ARG0(this) );
}

void test_pb( miby_this_t this )
{
	assert( MIBY_STATUSBYTE(this) >= 0xE0 && MIBY_STATUSBYTE(this) <= 0xEF );
	assert( MIBY_ARG0(this) < 0x80 );
	assert( MIBY_ARG1(this) < 0x80 );
	
	printf( "\n\t\t[%02X %02X %02X] ", MIBY_STATUSBYTE(this), MIBY_ARG0(this), MIBY_ARG1(this) );
	printf( "<Ch.%d> ", MIBY_CHAN(this) + 1 );
	printf( "Pitchbend :: lsb = %02X, msb = %02X\n", MIBY_ARG0(this), MIBY_ARG1(this) );
}



void test_sysex( miby_this_t this )
{
	int i;
	static char *sysex_state[] = { "IDLE", "START", "MID", "END", "ABORT" };
	
	printf( "\n\t\t[F0] SYSEX chunk: state = %02X (%s), length = %02X bytes\n", 
				MIBY_SYSEX_STATE(this),
				sysex_state[MIBY_SYSEX_STATE(this)], 
				MIBY_SYSEX_LEN(this) );
	printf( "\t\t\t[" );
	for ( i = 0; i < MIBY_SYSEX_LEN(this); i++ )
	{
		printf( "%02X ", MIBY_SYSEX_BUF(this,i) );
		if ( i % 8 == 7 )
			printf( "\n\t\t\t" );		
	}
	printf( "]" );
	
	MIBY_SYSEX_DONE_OK(this);
}	



int main( void )
{
	miby_t m;
	
	printf( "MIBY test app\n" );
	
	miby_init( &m, NULL );
	
	
	{
		int  i;
		for(i=0;i<1000000000;i++)
		{
			unsigned char c = rand();
			//printf("[%2.2X]", c);
			miby_parse( &m, c);
		}
	}
	
	return 0;
}


