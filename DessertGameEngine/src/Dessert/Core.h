#pragma once

#include <memory>

#ifdef DGE_PLATFORM_WINDOWS

#if DGE_DYNAMIC_LINK
	#ifdef DGE_BUILD_DLL
		#define DESSERT_API __declspec(dllexport)
	#else
		#define DESSERT_API __declspec(dllimport)
	#endif
#else
	#define DESSERT_API
#endif

#else 
	#error Dessert only support Windows!
#endif 

#ifdef DGE_DEBUG
	#define DGE_ENABLE_ASSERTS
#endif

#ifdef DGE_ENABLE_ASSERTS
	#define DGE_ASSERT(x, ...) { if(!(x)) { DGE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define DGE_CORE_ASSERT(x, ...) { if(!(x)) { DGE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define DGE_ASSERT(x, ...)
	#define DGE_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define BIND_EVENT_FUNC(fn) std::bind(&fn, this, std::placeholders::_1)


namespace Dessert {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;
}