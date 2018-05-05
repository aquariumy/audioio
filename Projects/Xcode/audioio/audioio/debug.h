#import "const.h"

#if MODE_COMPILE == MODE_COMPILE_RELEASE
    #define LOG_D(message)
    #define LOG_D_STR(message, str)
    #define LOG_D_VALUE(message, value)
    #define LOG_D_UVALUE(message, value)
    #define LOG_D_HVALUE(message, value)
    #define LOG_D_FVALUE(message, value)
    #define LOG_W(message)
    #define LOG_W_STR(message, str)
    #define LOG_W_VALUE(message, value)
    #define LOG_W_UVALUE(message, value)
    #define LOG_W_HVALUE(message, value)
    #define LOG_W_FVALUE(message, value)
#else // MODE_COMPILE == MODE_COMPILE_DEBUG
    #define LOG_D(message)                    printf("%s %s\n",        __FUNCTION__, message);
    #define LOG_D_STR(message, str)           printf("%s %s : %s\n",   __FUNCTION__, message, str);
    #define LOG_D_VALUE(message, value)       printf("%s %s : %d\n",   __FUNCTION__, message, value);
    #define LOG_D_UVALUE(message, value)      printf("%s %s : %u\n",   __FUNCTION__, message, value);
    #define LOG_D_HVALUE(message, value)      printf("%s %s : 0x%x\n", __FUNCTION__, message, value);
    #define LOG_D_FVALUE(message, value)      printf("%s %s : %f\n",   __FUNCTION__, message, value);
    #define LOG_W(message)                    printf("!!!warning!!! %s %s\n",        __FUNCTION__, message);
    #define LOG_W_STR(message, str)           printf("!!!warning!!! %s %s : %s\n",   __FUNCTION__, message, str);
    #define LOG_W_VALUE(message, value)       printf("!!!warning!!! %s %s : %d\n",   __FUNCTION__, message, value);
    #define LOG_W_UVALUE(message, value)      printf("!!!warning!!! %s %s : %u\n",   __FUNCTION__, message, value);
    #define LOG_W_HVALUE(message, value)      printf("!!!warning!!! %s %s : 0x%x\n", __FUNCTION__, message, value);
    #define LOG_W_FVALUE(message, value)      printf("!!!warning!!! %s %s : %f\n",   __FUNCTION__, message, value);
#endif

