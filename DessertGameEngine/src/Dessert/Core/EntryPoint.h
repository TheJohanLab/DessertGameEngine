#pragma once

#ifdef DGE_PLATFORM_WINDOWS

extern Dessert::Application* Dessert::CreateApplication();

int main(int argc, char** argv) {

	Dessert::Log::Init();
	DGE_CORE_WARN("Initialized Log!");

	auto app = Dessert::CreateApplication();
	app->Run();
	delete app;

}

#endif 
