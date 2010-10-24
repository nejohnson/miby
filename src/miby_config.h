
#ifndef MIBY_CONFIG_H
#define MIBY_CONFIG_H

#ifdef MIBY_USER_CONFIG
#include MIBY_USER_CONFIG
#endif


/** Length of SysEx Receive Buffer **/
#ifndef MIBY_SYSEX_BUF_LEN
#define MIBY_SYSEX_BUF_LEN   		( 128 )
#endif

/** Provide default values for handler names **/

/** - System RealTime **/
#ifndef MIBY_HND_RT_CLOCK
#define MIBY_HND_RT_CLOCK		( NULL )
#endif

#ifndef MIBY_HND_RT_START
#define MIBY_HND_RT_START		( NULL )
#endif

#ifndef MIBY_HND_RT_CONTINUE
#define MIBY_HND_RT_CONTINUE		( NULL )
#endif

#ifndef MIBY_HND_RT_STOP
#define MIBY_HND_RT_STOP		( NULL )
#endif

#ifndef MIBY_HND_RT_ACT_SENSE
#define MIBY_HND_RT_ACT_SENSE		( NULL )
#endif

#ifndef MIBY_HND_RT_SYS_RESET
#define MIBY_HND_RT_SYS_RESET		( NULL )
#endif

/** - System Common **/

#ifndef MIBY_HND_SYS_MTC
#define MIBY_HND_SYS_MTC		( NULL )
#endif

#ifndef MIBY_HND_SYS_SONGPOS
#define MIBY_HND_SYS_SONGPOS		( NULL )
#endif

#ifndef MIBY_HND_SYS_SONGSEL
#define MIBY_HND_SYS_SONGSEL		( NULL )
#endif

#ifndef MIBY_HND_SYS_TUNEREQ
#define MIBY_HND_SYS_TUNEREQ		( NULL )
#endif

/** - System Exclusive **/

#ifndef MIBY_HND_SYS_EX
#define MIBY_HND_SYS_EX			( NULL )
#endif

/** - Channel Voice **/

#ifndef MIBY_HND_NOTE_ON
#define MIBY_HND_NOTE_ON		( NULL )
#endif

#ifndef MIBY_HND_NOTE_OFF
#define MIBY_HND_NOTE_OFF		( NULL )
#endif

#ifndef MIBY_HND_POLY_AT
#define MIBY_HND_POLY_AT		( NULL )
#endif

#ifndef MIBY_HND_CTRL_CHG
#define MIBY_HND_CTRL_CHG		( NULL )
#endif

#ifndef MIBY_HND_PROG_CHG
#define MIBY_HND_PROG_CHG		( NULL )
#endif

#ifndef MIBY_HND_CHAN_AT
#define MIBY_HND_CHAN_AT		( NULL )
#endif

#ifndef MIBY_HND_PITCHBEND
#define MIBY_HND_PITCHBEND		( NULL )
#endif

/** - Channel Mode **/

#ifndef MIBY_HND_CH_ALL_SND_OFF
#define MIBY_HND_CH_ALL_SND_OFF		( NULL )
#endif

#ifndef MIBY_HND_CH_RESET_ALL_CTRL
#define MIBY_HND_CH_RESET_ALL_CTRL	( NULL )
#endif

#ifndef MIBY_HND_CH_LOCAL
#define MIBY_HND_CH_LOCAL		( NULL )
#endif

#ifndef MIBY_HND_CH_ALL_NOTES_OFF
#define MIBY_HND_CH_ALL_NOTES_OFF	( NULL )
#endif

#ifndef MIBY_HND_CH_OMNI_OFF
#define MIBY_HND_CH_OMNI_OFF		( NULL )
#endif

#ifndef MIBY_HND_CH_OMNI_ON
#define MIBY_HND_CH_OMNI_ON		( NULL )
#endif

#ifndef MIBY_HND_CH_MONO_ON
#define MIBY_HND_CH_MONO_ON		( NULL )
#endif

#ifndef MIBY_HND_CH_POLY_ON
#define MIBY_HND_CH_POLY_ON		( NULL )
#endif


#endif /* MIBY_CONFIG_H */

