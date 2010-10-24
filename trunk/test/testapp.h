
#ifndef TESTAPP_H
#define TESTAPP_H

#define MIBY_SYSEX_BUF_LEN	( 32 )


extern void test_rt_timing_clock( miby_this_t );
#define MIBY_HND_RT_CLOCK	test_rt_timing_clock

extern void test_rt_start( miby_this_t );
#define MIBY_HND_RT_START	test_rt_start

extern void test_rt_continue( miby_this_t );
#define MIBY_HND_RT_CONTINUE	test_rt_continue

extern void test_rt_stop( miby_this_t );
#define MIBY_HND_RT_STOP	test_rt_stop

extern void test_rt_active_sense( miby_this_t );
#define MIBY_HND_RT_ACT_SENSE	test_rt_active_sense

extern void test_rt_system_reset( miby_this_t );
#define MIBY_HND_RT_SYS_RESET	test_rt_system_reset

extern void test_tunereq( miby_this_t );
#define MIBY_HND_SYS_TIMEREQ	test_tunereq

extern void test_mtc( miby_this_t );
#define MIBY_HND_SYS_MTC	test_mtc

extern void test_songpos( miby_this_t );
#define MIBY_HND_SYS_SONGPOS	test_songpos

extern void test_songsel( miby_this_t );
#define MIBY_HND_SYS_SONGSEL	test_songsel

extern void test_sysex( miby_this_t );
#define MIBY_HND_SYS_EX		test_sysex

extern void test_note_on( miby_this_t );
#define MIBY_HND_NOTE_ON	test_note_on

extern void test_note_off( miby_this_t );
#define MIBY_HND_NOTE_OFF	test_note_off

extern void test_poly_at( miby_this_t );
#define MIBY_HND_POLY_AT	test_poly_at

extern void test_cc( miby_this_t );
#define MIBY_HND_CTRL_CHG	test_cc

extern void test_pb( miby_this_t );
#define MIBY_HND_PITCHBEND	test_pb

extern void test_pc( miby_this_t );
#define MIBY_HND_PROG_CHG	test_pc

extern void test_chanat( miby_this_t );
#define MIBY_HND_CHAN_AT	test_chanat



#endif /* TESTAPP_H */
