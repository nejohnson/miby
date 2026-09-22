/* Self-checking parser harness configuration. */

#ifndef PARSER_TESTS_H
#define PARSER_TESTS_H

extern void parser_test_rt_clock( miby_this_t );
#define MIBY_HND_RT_CLOCK parser_test_rt_clock

extern void parser_test_tunereq( miby_this_t );
#define MIBY_HND_SYS_TUNEREQ parser_test_tunereq

extern void parser_test_note_on( miby_this_t );
#define MIBY_HND_NOTE_ON parser_test_note_on

extern void miby_cc( miby_this_t );
#define MIBY_HND_CTRL_CHG miby_cc

extern void parser_test_cc_14bit( miby_this_t );
#define MIBY_HND_CC_14BIT parser_test_cc_14bit

extern void parser_test_cc_switch( miby_this_t );
#define MIBY_HND_CC_SWITCH parser_test_cc_switch

extern void parser_test_cc_parameter( miby_this_t );
#define MIBY_HND_CC_PARAMETER parser_test_cc_parameter

#endif /* PARSER_TESTS_H */
