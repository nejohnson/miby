/* Self-checking parser harness configuration. */

#ifndef PARSER_TESTS_H
#define PARSER_TESTS_H

extern void parser_test_rt_clock( miby_this_t );
#define MIBY_HND_RT_CLOCK parser_test_rt_clock

extern void parser_test_tunereq( miby_this_t );
#define MIBY_HND_SYS_TUNEREQ parser_test_tunereq

extern void parser_test_note_on( miby_this_t );
#define MIBY_HND_NOTE_ON parser_test_note_on

#endif /* PARSER_TESTS_H */
