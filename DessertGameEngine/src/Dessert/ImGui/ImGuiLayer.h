#pragma once

#include "Dessert/Core/Layer.h"
#include "Dessert/Events/MouseEvent.h"
#include "Dessert/Events/ApplicationEvent.h"
#include "Dessert/Events/KeyEvent.h"


namespace Dessert {

	class DESSERT_API ImGuiLayer: public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		//virtual void OnImGuiRender() override;

		void Begin();
		void End();

	private:
		float m_Time = 0.0f;
	};

}

