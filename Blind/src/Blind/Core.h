#pragma once
/* Blind Engine Core Header */

/* This header defines major macros about the building and linking of the engine.
   The core defines for using Blind on engine side are defined here.*/
// ENGINE SIDE HEADER

#ifdef BLIND_PLATFORM_WINDOWS
	#ifdef BLIND_DLL
		#define BLIND_API __declspec(dllexport)
	#else 
		#define BLIND_API __declspec(dllimport)
	#endif
#endif // BLIND_PLATFORM_WINDOWS

#ifdef BLIND_DEBUG
	#define  BLIND_ENABLE_ASSERTS
#endif

#ifdef BLIND_ENABLE_ASSERTS
	#define BLIND_CLIENT_ASSERT(x, ...) {if(!(x)) {BLIND_CLIENT_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
	#define BLIND_ENGINE_ASSERT(x, ...) {if(!(x)) {BLIND_ENGINE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
#else
	#define BLIND_CLIENT_ASSERT(x, ...)
	#define BLIND_ENGINE_ASSERT(x, ...)

#endif

#define BIT(x) 1 << x

#define BIND_EVENT_FUNCTION(x) std::bind(&x, this, std::placeholders::_1)