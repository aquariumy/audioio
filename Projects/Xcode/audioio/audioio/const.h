#define MODE_COMPILE_DEBUG		(100)
#define	MODE_COMPILE_RELEASE    (200)
#define MODE_COMPILE			(MODE_COMPILE_DEBUG)

#ifndef MODE_COMPILE
	#error MODE_COMPILE is not defined.
#endif
#if		MODE_COMPILE == MODE_COMPILE_DEBUG
#elif	MODE_COMPILE == MODE_COMPILE_RELEASE
#else
	#error MODE_COMPILE is illegal value.
#endif

