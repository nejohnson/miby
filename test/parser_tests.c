/* ****************************************************************************
 * Miby - MIDI Bytestream Parser for Embedded Systems
 * Copyright (C) 2026, Neil Johnson
 * All rights reserved.
 * ************************************************************************* */

#include <stdio.h>
#include <string.h>

#include "miby.h"

typedef struct {
    unsigned int note_on_count;
    unsigned int rt_clock_count;
    unsigned int tunereq_count;
    unsigned char last_status;
    unsigned char last_chan;
    unsigned char last_arg0;
    unsigned char last_arg1;
} parser_test_state_t;

static unsigned int failures;

#define CHECK(expr)                                                         \
    do {                                                                    \
        if ( !(expr) ) {                                                    \
            printf( "FAIL %s:%d: %s\n", __FILE__, __LINE__, #expr );        \
            failures++;                                                     \
        }                                                                   \
    } while ( 0 )

static parser_test_state_t state;

static void reset_state( void )
{
    memset( &state, 0, sizeof( state ) );
}

void parser_test_rt_clock( miby_this_t this )
{
    (void)this;
    state.rt_clock_count++;
}

void parser_test_tunereq( miby_this_t this )
{
    state.tunereq_count++;
    state.last_status = MIBY_STATUSBYTE( this );
}

void parser_test_note_on( miby_this_t this )
{
    state.note_on_count++;
    state.last_status = MIBY_STATUSBYTE( this );
    state.last_chan = MIBY_CHAN( this );
    state.last_arg0 = MIBY_ARG0( this );
    state.last_arg1 = MIBY_ARG1( this );
}

static void parse_bytes( miby_t *m, const unsigned char *bytes, unsigned int len )
{
    unsigned int i;

    for ( i = 0; i < len; i++ )
        miby_parse( m, bytes[i] );
}

static void tune_request_reports_status( void )
{
    miby_t m;
    const unsigned char msg[] = { 0xF6 };

    reset_state();
    miby_init( &m, NULL );
    parse_bytes( &m, msg, sizeof( msg ) );

    CHECK( state.tunereq_count == 1 );
    CHECK( state.last_status == 0xF6 );
    CHECK( MIBY_STATUSBYTE( &m ) == 0 );
}

static void unhandled_status_clears_partial_message( void )
{
    miby_t m;

    reset_state();
    miby_init( &m, NULL );

    miby_parse( &m, 0x90 );
    miby_parse( &m, 0x40 );
    miby_parse( &m, 0xF4 );
    CHECK( MIBY_ERROR_MISSING_DATA( &m ) == 1 );

    MIBY_CLEAR_MISSING_DATA( &m );
    miby_parse( &m, 0xF4 );
    CHECK( MIBY_ERROR_MISSING_DATA( &m ) == 0 );
}

static void realtime_does_not_disrupt_running_status( void )
{
    miby_t m;
    const unsigned char msg[] = {
        0x90, 0x40, 0x7F,
        0xF8,
        0x41, 0x7E
    };

    reset_state();
    miby_init( &m, NULL );
    parse_bytes( &m, msg, sizeof( msg ) );

    CHECK( state.note_on_count == 2 );
    CHECK( state.rt_clock_count == 1 );
    CHECK( state.last_status == 0x90 );
    CHECK( state.last_chan == 1 );
    CHECK( state.last_arg0 == 0x41 );
    CHECK( state.last_arg1 == 0x7E );
}

static void channel_setters_take_real_channel_numbers( void )
{
    miby_t m;
    const unsigned char msg[] = {
        0x90, 0x40, 0x7F,
        0x91, 0x41, 0x7E
    };

    reset_state();
    miby_init( &m, NULL );
    MIBY_SET_BASIC_CHAN( &m, 2 );
    MIBY_SET_TOP_CHAN( &m, 2 );
    parse_bytes( &m, msg, sizeof( msg ) );

    CHECK( state.note_on_count == 1 );
    CHECK( state.last_chan == 2 );
    CHECK( state.last_arg0 == 0x41 );
    CHECK( state.last_arg1 == 0x7E );
}

int main( void )
{
    tune_request_reports_status();
    unhandled_status_clears_partial_message();
    realtime_does_not_disrupt_running_status();
    channel_setters_take_real_channel_numbers();

    if ( failures != 0 ) {
        printf( "%u parser test failure(s)\n", failures );
        return 1;
    }

    printf( "parser tests passed\n" );
    return 0;
}
