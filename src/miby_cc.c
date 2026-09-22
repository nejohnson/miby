/* ****************************************************************************
 * Miby - MIDI Bytestream Parser for Embedded Systems
 * Copyright (C) 2017-2026, Neil Johnson
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

#include <string.h>

/*****************************************************************************/
/* Project Includes                                                          */
/*****************************************************************************/

#include "miby.h"

/*****************************************************************************/
/* Macros, constants                                                         */
/*****************************************************************************/

#define CC_DATA_ENTRY_MSB       ( 6 )
#define CC_DATA_ENTRY_LSB       ( 38 )
#define CC_DATA_INCREMENT       ( 96 )
#define CC_DATA_DECREMENT       ( 97 )
#define CC_NRPN_LSB             ( 98 )
#define CC_NRPN_MSB             ( 99 )
#define CC_RPN_LSB              ( 100 )
#define CC_RPN_MSB              ( 101 )
#define CC_NULL_VALUE           ( 0x7F )

#define CC_FLAG_MSB             ( 0x01 )
#define CC_FLAG_LSB             ( 0x02 )
#define CC_FLAG_BOTH            ( CC_FLAG_MSB | CC_FLAG_LSB )

/*****************************************************************************/
/* Data types                                                                */
/*****************************************************************************/

typedef void (*cc_handler_t)(miby_t *, miby_cc_t *);

typedef void (*cc_user_handler_t)(miby_this_t);

/*****************************************************************************/
/* Private function declarations                                             */
/*****************************************************************************/

static void cc_handle_7bit( miby_t *m, miby_cc_t *cc );
static void cc_handle_14bit_msb( miby_t *m, miby_cc_t *cc );
static void cc_handle_14bit_lsb( miby_t *m, miby_cc_t *cc );
static void cc_handle_switch( miby_t *m, miby_cc_t *cc );
static void cc_handle_data_msb( miby_t *m, miby_cc_t *cc );
static void cc_handle_data_lsb( miby_t *m, miby_cc_t *cc );
static void cc_handle_data_increment( miby_t *m, miby_cc_t *cc );
static void cc_handle_data_decrement( miby_t *m, miby_cc_t *cc );
static void cc_handle_nrpn_lsb( miby_t *m, miby_cc_t *cc );
static void cc_handle_nrpn_msb( miby_t *m, miby_cc_t *cc );
static void cc_handle_rpn_lsb( miby_t *m, miby_cc_t *cc );
static void cc_handle_rpn_msb( miby_t *m, miby_cc_t *cc );
static void cc_handle_mode( miby_t *m, miby_cc_t *cc );

/*****************************************************************************/
/* Private data                                                              */
/*****************************************************************************/

MIBY_READONLY
static const cc_handler_t cc_table[128] = {
    cc_handle_14bit_msb, cc_handle_14bit_msb,
    cc_handle_14bit_msb, cc_handle_14bit_msb,
    cc_handle_14bit_msb, cc_handle_14bit_msb,
    cc_handle_data_msb,  cc_handle_14bit_msb,
    cc_handle_14bit_msb, cc_handle_14bit_msb,
    cc_handle_14bit_msb, cc_handle_14bit_msb,
    cc_handle_14bit_msb, cc_handle_14bit_msb,
    cc_handle_14bit_msb, cc_handle_14bit_msb,
    cc_handle_14bit_msb, cc_handle_14bit_msb,
    cc_handle_14bit_msb, cc_handle_14bit_msb,
    cc_handle_14bit_msb, cc_handle_14bit_msb,
    cc_handle_14bit_msb, cc_handle_14bit_msb,
    cc_handle_14bit_msb, cc_handle_14bit_msb,
    cc_handle_14bit_msb, cc_handle_14bit_msb,
    cc_handle_14bit_msb, cc_handle_14bit_msb,
    cc_handle_14bit_msb, cc_handle_14bit_msb,
    cc_handle_14bit_lsb, cc_handle_14bit_lsb,
    cc_handle_14bit_lsb, cc_handle_14bit_lsb,
    cc_handle_14bit_lsb, cc_handle_14bit_lsb,
    cc_handle_data_lsb,  cc_handle_14bit_lsb,
    cc_handle_14bit_lsb, cc_handle_14bit_lsb,
    cc_handle_14bit_lsb, cc_handle_14bit_lsb,
    cc_handle_14bit_lsb, cc_handle_14bit_lsb,
    cc_handle_14bit_lsb, cc_handle_14bit_lsb,
    cc_handle_14bit_lsb, cc_handle_14bit_lsb,
    cc_handle_14bit_lsb, cc_handle_14bit_lsb,
    cc_handle_14bit_lsb, cc_handle_14bit_lsb,
    cc_handle_14bit_lsb, cc_handle_14bit_lsb,
    cc_handle_14bit_lsb, cc_handle_14bit_lsb,
    cc_handle_14bit_lsb, cc_handle_14bit_lsb,
    cc_handle_14bit_lsb, cc_handle_14bit_lsb,
    cc_handle_14bit_lsb, cc_handle_14bit_lsb,
    cc_handle_switch,    cc_handle_switch,
    cc_handle_switch,    cc_handle_switch,
    cc_handle_switch,    cc_handle_switch,
    cc_handle_7bit,      cc_handle_7bit,
    cc_handle_7bit,      cc_handle_7bit,
    cc_handle_7bit,      cc_handle_7bit,
    cc_handle_7bit,      cc_handle_7bit,
    cc_handle_7bit,      cc_handle_7bit,
    cc_handle_7bit,      cc_handle_7bit,
    cc_handle_7bit,      cc_handle_7bit,
    cc_handle_7bit,      cc_handle_7bit,
    cc_handle_7bit,      cc_handle_7bit,
    cc_handle_7bit,      cc_handle_7bit,
    cc_handle_7bit,      cc_handle_7bit,
    cc_handle_7bit,      cc_handle_7bit,
    cc_handle_7bit,      cc_handle_7bit,
    cc_handle_data_increment, cc_handle_data_decrement,
    cc_handle_nrpn_lsb,  cc_handle_nrpn_msb,
    cc_handle_rpn_lsb,   cc_handle_rpn_msb,
    cc_handle_7bit,      cc_handle_7bit,
    cc_handle_7bit,      cc_handle_7bit,
    cc_handle_7bit,      cc_handle_7bit,
    cc_handle_7bit,      cc_handle_7bit,
    cc_handle_7bit,      cc_handle_7bit,
    cc_handle_7bit,      cc_handle_7bit,
    cc_handle_7bit,      cc_handle_7bit,
    cc_handle_7bit,      cc_handle_7bit,
    cc_handle_7bit,      cc_handle_7bit,
    cc_handle_mode,      cc_handle_mode,
    cc_handle_mode,      cc_handle_mode,
    cc_handle_mode,      cc_handle_mode,
    cc_handle_mode,      cc_handle_mode
};

/*****************************************************************************/
/* Private functions                                                         */
/*****************************************************************************/

static unsigned short cc_14bit( unsigned char msb, unsigned char lsb )
{
    return (unsigned short)( ( (unsigned short)msb << 7 ) | lsb );
}

static unsigned long cc_bit( unsigned char controller )
{
    return (unsigned long)1 << controller;
}

static miby_cc_channel_t *cc_channel( miby_t *m, miby_cc_t *cc )
{
    return &cc->channel[MIBY_CHAN_RAW( m )];
}

static void cc_clear_event( miby_cc_event_t *e )
{
    memset( e, 0, sizeof( *e ) );
}

static void cc_call( miby_t *m, cc_user_handler_t handler )
{
    if ( handler )
        (handler)( m );
}

static void cc_event_common( miby_t *m, miby_cc_t *cc, unsigned char type )
{
    miby_cc_event_t *e = &cc->event;

    cc_clear_event( e );
    e->type       = type;
    e->action     = MIBY_CC_ACTION_SET;
    e->channel    = MIBY_CHAN( m );
    e->controller = MIBY_ARG0( m );
    e->value      = MIBY_ARG1( m );
    e->value_msb  = MIBY_ARG1( m );
}

static void cc_handle_7bit( miby_t *m, miby_cc_t *cc )
{
    cc_event_common( m, cc, MIBY_CC_EVENT_7BIT );
    cc_call( m, (cc_user_handler_t)MIBY_HND_CC_7BIT );
}

static void cc_handle_14bit_msb( miby_t *m, miby_cc_t *cc )
{
    unsigned char controller = MIBY_ARG0( m );
    miby_cc_channel_t *ch = cc_channel( m, cc );
    unsigned char value = MIBY_ARG1( m );
    unsigned char lsb = ch->cc_lsb[controller];

    ch->cc_msb[controller] = value;
    ch->cc_msb_valid |= cc_bit( controller );

    cc_event_common( m, cc, MIBY_CC_EVENT_14BIT );
    cc->event.controller = controller;
    cc->event.value_msb = value;
    cc->event.value_lsb = lsb;
    cc->event.flags = MIBY_CC_VALUE_HAS_MSB;

    if ( ch->cc_lsb_valid & cc_bit( controller ) )
        cc->event.flags |= MIBY_CC_VALUE_HAS_LSB;

    cc->event.value = cc_14bit( value, lsb );
    cc_call( m, (cc_user_handler_t)MIBY_HND_CC_14BIT );
}

static void cc_handle_14bit_lsb( miby_t *m, miby_cc_t *cc )
{
    unsigned char controller = MIBY_ARG0( m ) - 32;
    miby_cc_channel_t *ch = cc_channel( m, cc );
    unsigned char value = MIBY_ARG1( m );

    ch->cc_lsb[controller] = value;
    ch->cc_lsb_valid |= cc_bit( controller );

    if ( !( ch->cc_msb_valid & cc_bit( controller ) ) )
        return;

    cc_event_common( m, cc, MIBY_CC_EVENT_14BIT );
    cc->event.controller = controller;
    cc->event.value_msb = ch->cc_msb[controller];
    cc->event.value_lsb = value;
    cc->event.flags = MIBY_CC_VALUE_HAS_MSB | MIBY_CC_VALUE_HAS_LSB;
    cc->event.value = cc_14bit( ch->cc_msb[controller], value );
    cc_call( m, (cc_user_handler_t)MIBY_HND_CC_14BIT );
}

static void cc_handle_switch( miby_t *m, miby_cc_t *cc )
{
    cc_event_common( m, cc, MIBY_CC_EVENT_SWITCH );
    cc->event.switch_on = ( MIBY_ARG1( m ) >= 64 );
    cc_call( m, (cc_user_handler_t)MIBY_HND_CC_SWITCH );
}

static void cc_update_selected_parameter( miby_cc_channel_t *ch )
{
    unsigned char selected = MIBY_CC_PARAM_NONE;
    unsigned short number = 0;

    if ( ch->selected_param == MIBY_CC_PARAM_RPN &&
         ( ch->rpn_valid & CC_FLAG_BOTH ) == CC_FLAG_BOTH )
    {
        if ( ch->rpn_msb != CC_NULL_VALUE || ch->rpn_lsb != CC_NULL_VALUE )
        {
            selected = MIBY_CC_PARAM_RPN;
            number = cc_14bit( ch->rpn_msb, ch->rpn_lsb );
        }
    }
    else if ( ch->selected_param == MIBY_CC_PARAM_NRPN &&
              ( ch->nrpn_valid & CC_FLAG_BOTH ) == CC_FLAG_BOTH )
    {
        if ( ch->nrpn_msb != CC_NULL_VALUE || ch->nrpn_lsb != CC_NULL_VALUE )
        {
            selected = MIBY_CC_PARAM_NRPN;
            number = cc_14bit( ch->nrpn_msb, ch->nrpn_lsb );
        }
    }

    if ( selected != ch->selected_param || number != ch->selected_number )
        ch->data_valid = 0;

    ch->selected_param = selected;
    ch->selected_number = number;
}

static void cc_handle_rpn_msb( miby_t *m, miby_cc_t *cc )
{
    miby_cc_channel_t *ch = cc_channel( m, cc );

    ch->selected_param = MIBY_CC_PARAM_RPN;
    ch->rpn_msb = MIBY_ARG1( m );
    ch->rpn_valid |= CC_FLAG_MSB;
    cc_update_selected_parameter( ch );
}

static void cc_handle_rpn_lsb( miby_t *m, miby_cc_t *cc )
{
    miby_cc_channel_t *ch = cc_channel( m, cc );

    ch->selected_param = MIBY_CC_PARAM_RPN;
    ch->rpn_lsb = MIBY_ARG1( m );
    ch->rpn_valid |= CC_FLAG_LSB;
    cc_update_selected_parameter( ch );
}

static void cc_handle_nrpn_msb( miby_t *m, miby_cc_t *cc )
{
    miby_cc_channel_t *ch = cc_channel( m, cc );

    ch->selected_param = MIBY_CC_PARAM_NRPN;
    ch->nrpn_msb = MIBY_ARG1( m );
    ch->nrpn_valid |= CC_FLAG_MSB;
    cc_update_selected_parameter( ch );
}

static void cc_handle_nrpn_lsb( miby_t *m, miby_cc_t *cc )
{
    miby_cc_channel_t *ch = cc_channel( m, cc );

    ch->selected_param = MIBY_CC_PARAM_NRPN;
    ch->nrpn_lsb = MIBY_ARG1( m );
    ch->nrpn_valid |= CC_FLAG_LSB;
    cc_update_selected_parameter( ch );
}

static void cc_parameter_event( miby_t *m,
                                miby_cc_t *cc,
                                miby_cc_channel_t *ch,
                                unsigned char action )
{
    if ( ch->selected_param == MIBY_CC_PARAM_NONE )
        return;

    cc_event_common( m, cc, MIBY_CC_EVENT_PARAMETER );
    cc->event.action = action;
    cc->event.parameter_type = ch->selected_param;
    cc->event.parameter = ch->selected_number;

    if ( action == MIBY_CC_ACTION_SET )
    {
        cc->event.value_msb = ch->data_msb;
        cc->event.value_lsb = ch->data_lsb;
        cc->event.flags = ch->data_valid;
        cc->event.value = cc_14bit( ch->data_msb, ch->data_lsb );
    }
    else
    {
        cc->event.value = MIBY_ARG1( m );
        cc->event.value_msb = MIBY_ARG1( m );
    }

    cc_call( m, (cc_user_handler_t)MIBY_HND_CC_PARAMETER );
}

static void cc_handle_data_msb( miby_t *m, miby_cc_t *cc )
{
    miby_cc_channel_t *ch = cc_channel( m, cc );

    if ( ch->selected_param == MIBY_CC_PARAM_NONE )
        return;

    ch->data_msb = MIBY_ARG1( m );
    ch->data_valid |= MIBY_CC_VALUE_HAS_MSB;
    cc_parameter_event( m, cc, ch, MIBY_CC_ACTION_SET );
}

static void cc_handle_data_lsb( miby_t *m, miby_cc_t *cc )
{
    miby_cc_channel_t *ch = cc_channel( m, cc );

    if ( ch->selected_param == MIBY_CC_PARAM_NONE )
        return;

    ch->data_lsb = MIBY_ARG1( m );
    ch->data_valid |= MIBY_CC_VALUE_HAS_LSB;

    if ( ch->data_valid & MIBY_CC_VALUE_HAS_MSB )
        cc_parameter_event( m, cc, ch, MIBY_CC_ACTION_SET );
}

static void cc_handle_data_increment( miby_t *m, miby_cc_t *cc )
{
    cc_parameter_event( m, cc, cc_channel( m, cc ), MIBY_CC_ACTION_INCREMENT );
}

static void cc_handle_data_decrement( miby_t *m, miby_cc_t *cc )
{
    cc_parameter_event( m, cc, cc_channel( m, cc ), MIBY_CC_ACTION_DECREMENT );
}

static void cc_handle_mode( miby_t *m, miby_cc_t *cc )
{
    cc_event_common( m, cc, MIBY_CC_EVENT_MODE );
    cc_call( m, (cc_user_handler_t)MIBY_HND_CC_MODE );
}

/*****************************************************************************/
/* Public functions, as declared in public header.                           */
/*****************************************************************************/

void miby_cc_init( miby_cc_t *cc )
{
    memset( cc, 0, sizeof( *cc ) );
}

void miby_cc( miby_this_t this )
{
    miby_cc_t *cc = MIBY_CC_GET_STATE( this );
    unsigned char controller = MIBY_ARG0( this );
    cc_handler_t handler;

    if ( cc == 0 || controller >= 128 )
        return;

    handler = (cc_handler_t)MIBY_GET_RO_WORD( cc_table[controller] );
    if ( handler )
        (handler)( this, cc );
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
