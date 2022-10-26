#include "debugbreak.h"


#ifdef DEBUG
    #define FE_DEBUGBREAK()debug_break();
#else
    #define FE_DEBUGBREAK()
#endif