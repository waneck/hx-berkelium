#ifndef STATIC_LINK
	#define IMPLEMENT_API
#endif

#if defined(HX_WINDOWS) || defined(HX_MACOS) || defined(HX_LINUX)
#define NEKO_COMPATIBLE
#endif
#include <hx/CFFI.h>

#ifdef __cplusplus
extern "C" 
{
#endif

#ifndef HX_WINDOWS
#	include <dlfcn.h>
#else
#	undef ERROR
#	include <windows.h>
#	define dlopen(l,p)		(void*)LoadLibrary(l)
#	define dlsym(h,n)		GetProcAddress((HMODULE)h,n)
#	define dlclose(h)		FreeLibrary((HMODULE)h)
#endif

#if defined HX_WINDOWS
	#define HXFFI_EXPORT __declspec(dllexport)
#else
  #define HXFFI_EXPORT __attribute__ ((__visibility__("default")))
#endif

// Reference this to bring in all the symbols for the static library
int hxberkelium_register_prims() { return 0; }

#ifdef __cplusplus
}
#endif