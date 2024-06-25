#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"


namespace Dessert {

	class DESSERT_API Log
	{
	public:
		Log();
		~Log();

		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}



//Core log macros
#define DGE_CORE_TRACE(...) ::Dessert::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define DGE_CORE_INFO(...)  ::Dessert::Log::GetCoreLogger()->info(__VA_ARGS__)
#define DGE_CORE_WARN(...)  ::Dessert::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define DGE_CORE_ERROR(...) ::Dessert::Log::GetCoreLogger()->error(__VA_ARGS__)
#define DGE_CORE_FATAL(...) ::Dessert::Log::GetCoreLogger()->fatal(__VA_ARGS__)

//Client log macros
#define DGE_TRACE(...) ::Dessert::Log::GetClientLogger()->trace(__VA_ARGS__)
#define DGE_INFO(...)  ::Dessert::Log::GetClientLogger()->info(__VA_ARGS__)
#define DGE_WARN(...)  ::Dessert::Log::GetClientLogger()->warn(__VA_ARGS__)
#define DGE_ERROR(...) ::Dessert::Log::GetClientLogger()->error(__VA_ARGS__)
#define DGE_FATAL(...) ::Dessert::Log::GetClientLogger()->fatal(__VA_ARGS__)

