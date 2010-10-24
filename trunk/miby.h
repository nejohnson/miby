
#ifndef _MIBY_H
#define _MIBY_H

typedef struct miby_s *miby_this_t;

#include "miby_config.h"









typedef struct miby_s {
    unsigned char statusbyte; 
    unsigned char msg_chan;
    unsigned char sysexstate;
    unsigned char basic_channel;
    unsigned char top_channel;	  
    int msglen;
    int ptr;
    void (*handler)(miby_this_t);
    void *v;
    unsigned char buf[MIBY_SYSEX_BUF_LEN];
} miby_t;

#define MIBY_SYSEX_IDLE			( 0 )
#define MIBY_SYSEX_START     		( 1 )
#define MIBY_SYSEX_MID       		( 2 )
#define MIBY_SYSEX_END       		( 3 )
#define MIBY_SYSEX_ABORT     		( 4 )


#define MIBY_STATUSBYTE(x)		((x)->statusbyte)
#define MIBY_ARG0(x)			((x)->buf[0])
#define MIBY_ARG1(x)			((x)->buf[1])
#define MIBY_CHAN(x)			((x)->msg_chan)
#define MIBY_SYSEX_LEN(x)		((x)->ptr)
#define MIBY_SYSEX_BUF(x,y)		((x)->buf[(y)])
#define MIBY_SYSEX_STATE(x)		((x)->sysexstate)
#define MIBY_SYSEX_DONE_OK(x)		((x)->ptr=0)




extern void miby_init( miby_t *, void * );
extern void miby_parse( miby_t *, unsigned char );





#endif /* _MIBY_H */
