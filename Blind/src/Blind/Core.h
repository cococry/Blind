#pragma once
/* Blind Engine Core Header */

/* This header defines major defines about the building and linking of the engine.
   The core defines for using Blind on engine side are defined here.*/
// ENGINE SIDE HEADER

#ifdef BLIND_DEBUG
	#define  BLIND_ENABLE_ASSERTS
#endif

#ifdef BLIND_ENABLE_ASSERTS
	#define BLIND_CLIENT_ASSERT(x, ...) {if(!(x)) {BLIND_CLIENT_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
	#define BLIND_ENGINE_ASSERT(x, ...) {if(!(x)) {BLIND_ENGINE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
#else
	#define MRN_ASSERT(x, ...)
	#define BLIND_CORE_ASSERT(x, ...)

#endif

#define BIT(x) 1 << x