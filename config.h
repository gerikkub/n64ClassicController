// enables serial port functions
//#define USE_SERPORT

// enables the + - up down left right buttons if defined
//#define GHWT

// trigger on either rising or falling edge (never both please)
// these only apply to the 5 pads, buttons and bass pedals are always falling edge
// pull_up_res will enable internal pull-up resistors for the pad pins
// does not effect buttons and bass pedals
//#define trig_on_rise
#define trig_on_fall
//#define pull_up_res 

// guarantee the release of a pad before the next hit if defined
//#define GUARANTEE_RELEASE

// minimum pulse time (x = actual pulse time / 0.005)
// keep at 5, raise if you see double hits
#define hit_min_time 4

// hit softness (0 = max, 7 = min, just make it 0)
#define default_hit_softness 0

// simulated degree of thumb stick movement (something under 32)
#define thumbstick_speed 16
