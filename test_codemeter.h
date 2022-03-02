#ifndef __TEST_CODEMETER__
#define __TEST_CODEMETER__

#define LED_NONE    0
#define LED_GREEN   1
#define LED_RED     2
#define LED_ORANGE    (LED_GREEN | LED_RED)



#if defined(__GNUC__)
#define EXPORT   
#else
#define EXPORT __declspec(dllexport)   
#endif


#if defined(c_plusplus) || defined(__cplusplus)
extern "C" {
#endif

EXPORT void set_codemeter_led(int state);


#if defined(c_plusplus) || defined(__cplusplus)
}
#endif

#endif // __TEST_CODEMETER__
