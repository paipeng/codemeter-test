#include <stdio.h>

#ifndef TEST_H
#define TEST_H


#if defined(__GNUC__)
#define S2iEXPORT
#else
#define S2iEXPORT __declspec(dllexport)
#endif


#if defined(c_plusplus) || defined(__cplusplus)
extern "C" {
#endif

S2iEXPORT int add(int a, int b);
    
    
#if defined(c_plusplus) || defined(__cplusplus)
}
#endif

#endif /* s2icore_decode_h */
