#ifndef STATIC_LINK
	#define IMPLEMENT_API
#endif

#if defined(HX_WINDOWS) || defined(HX_MACOS) || defined(HX_LINUX)
#define NEKO_COMPATIBLE
#endif
#include <hx/CFFI.h>
#include <berkelium/Berkelium.hpp>

#ifdef __cplusplus
extern "C" 
{
#endif

value berkelium_init()
{
	Berkelium::init(Berkelium::FileString::empty());
	return alloc_bool(true);
}

DEFINE_PRIM(berkelium_init, 0);


// Reference this to bring in all the symbols for the static library
int hxberkelium_register_prims() { return 0; }

#ifdef __cplusplus
}
#endif