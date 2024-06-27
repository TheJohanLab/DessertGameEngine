#pragma once

#ifdef DGE_PLATFORM_WINDOWS

extern Dessert::Application* Dessert::CreateApplication();

int main(int argc, char** argv) {

	Dessert::Log::Init();

	DGE_PROFILE_BEGIN_SESSION("Startup", "DessertProfile-Startup.json");
	auto app = Dessert::CreateApplication();
	DGE_PROFILE_END_SESSION();

	DGE_PROFILE_BEGIN_SESSION("Startup", "DessertProfile-Runtime.json");
	app->Run();
	DGE_PROFILE_END_SESSION();

	DGE_PROFILE_BEGIN_SESSION("Startup", "DessertProfile-Shutdown.json");
	delete app;
	DGE_PROFILE_END_SESSION();

}

#endif 
