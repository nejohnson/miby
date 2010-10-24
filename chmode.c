

#define CHMODE_ALL_SND_OFF		(120)
#define CHMODE_RESET_ALL_CTRLS		(121)
#define CHMODE_LOCAL_CTRL		(122)
#define CHMODE_ALL_NOTES_OFF		(123)
#define CHMODE_OMNI_OFF			(124)
#define CHMODE_OMNI_ON			(125)
#define CHMODE_MONO_ON			(126)
#define CHMODE_POLY_ON			(127)




#define CHANNEL_OMNI_BIT            ( 0 )
#define CHANNEL_OMNI_ON             ( 1 << CHANNEL_OMNI_BIT )
#define CHANNEL_OMNI_OFF            ( 0 << CHANNEL_OMNI_BIT )

#define CHANNEL_MP_BIT              ( 1 )
#define CHANNEL_POLY_MODE           ( 1 << CHANNEL_MP_BIT )
#define CHANNEL_MONO_MODE           ( 0 << CHANNEL_MP_BIT )

#define CHANNEL_OMNI_ON_POLY        ( CHANNEL_OMNI_ON  | CHANNEL_POLY_MODE )
#define CHANNEL_OMNI_ON_MONO        ( CHANNEL_OMNI_ON  | CHANNEL_MONO_MODE )
#define CHANNEL_OMNI_OFF_POLY       ( CHANNEL_OMNI_OFF | CHANNEL_POLY_MODE )
#define CHANNEL_OMNI_OFF_MONO       ( CHANNEL_OMNI_OFF | CHANNEL_MONO_MODE )



static unsigned char basic_channel = 0;
static unsigned char btm_channel = 0;
static unsigned char top_channel = 0x0F;

void set_midi_basic_channel( unsigned char chan )
{
    basic_channel = chan;
}

void set_midi_channel_mode( unsigned char mode, unsigned char mono_voices )
{
    switch ( mode )
    {
    case CHANNEL_OMNI_ON_POLY:
    case CHANNEL_OMNI_ON_MONO:
        btm_channel = 0;
        top_channel = 0x0F;
        break;
        
    case CHANNEL_OMNI_OFF_POLY:
        btm_channel = top_channel = basic_channel;
        break;
        
    case CHANNEL_OMNI_OFF_MONO: 
        btm_channel = basic_channel;
        top_channel = basic_channel + mono_voices - 1;
        break;
    }
}
