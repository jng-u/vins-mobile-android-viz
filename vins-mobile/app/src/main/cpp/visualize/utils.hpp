#ifndef __UTILS_H__
#define __UTILS_H__

#include <string>
#include <cstdio>
#include <cstdarg>

#define ANDROID_SYSTEM

#ifdef ANDROID_SYSTEM
#define LOG_TAG "[APPLICATION INFO LOG]"
#include <EGL/egl.h>
#include <android/log.h>
#if __ANDROID_API__ >= 24
#include <GLES3/gl32.h>
#elif __ANDROID_API__ >= 21
#include <GLES3/gl31.h>
#else
#include <GLES3/gl3.h>
#endif
#else
#endif

//#include <std_msgs/Float32MultiArray.h>

void glesLog (const char *formatStr, ...);

#endif // __UTILS_H__