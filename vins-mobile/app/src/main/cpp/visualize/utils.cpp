#include "utils.hpp"

void glesLog (const char *formatStr, ...)
{
   va_list params;
   char buf[100];

   va_start(params, formatStr);
   vsprintf(buf, formatStr, params);
#ifdef ANDROID_SYSTEM
   __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "%s\n", buf);
#elif WIN32
   printf("%s\n", buf);
#endif
   va_end(params);
}
