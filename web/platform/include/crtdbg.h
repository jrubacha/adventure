/* MSVC <crtdbg.h> shim: debug-heap and assert macros, which are no-ops in
   the release build the browser version is made from. */
#include "../platform.h"
#ifndef _ASSERT
#define _ASSERT(expr)	((void)0)
#define _ASSERTE(expr)	((void)0)
#endif
