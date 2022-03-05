#pragma once
#include <memory>
/* Blind Engine Core Header */

/* This header defines major macros about the building and linking of the engine.
   The core defines for using Blind on engine side are defined here.*/
// ENGINE SIDE HEADER

#ifdef BLIND_PLATFORM_WINDOWS
	#ifdef BLIND_DYNAMIC_LINK
		#ifdef BLIND_DLL
			#define BLIND_API __declspec(dllexport)
		#else 
			#define   __declspec(dllimport)
		#endif
	#else
		#define BLIND_API
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

namespace Blind
{
	template <typename T>
	using Scope = std::unique_ptr<T>;
	template <typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template <typename T>
	using Ref = std::shared_ptr<T>;
	template <typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}