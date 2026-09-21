#include <assert.h>



#ifdef PLATFORM_PC

#include <platform/os-entry-point/desktop_entry_point.cpp>

#elif PLATFORM_ANDROID


#include <platform/os-entry-point/android_entry_point.cpp>


#elif PLATFORM_TEST

//Do nothing

#else

	#error "Neither PLATFORM_PC nor PLATFORM_ANDROID is defined!"

#endif