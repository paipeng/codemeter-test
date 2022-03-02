#include <stdio.h>

#ifndef TEST_H
#define TEST_H


#if defined(__GNUC__)
#define EXPORT
#else
#define EXPORT __declspec(dllexport)
#endif


#if defined(c_plusplus) || defined(__cplusplus)
extern "C" {
#endif

EXPORT int add(int a, int b);
    
    
#if defined(c_plusplus) || defined(__cplusplus)
}
#endif

#endif /* test_h */
