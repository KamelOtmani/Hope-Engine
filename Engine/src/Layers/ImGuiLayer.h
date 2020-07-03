#pragma once
#include "Layer.h"

#include "Events/AppEvent.h"
#include "Events/KeyboardEvent.h"
#include "Events/MouseEvent.h"

namespace HEngine {

    class HOPE_API ImGuiLayer :  public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        void OnAttach() override;
        void OnDetach() override;		
        virtual void OnImGuiRender() override;

        void Begin();
        void End();

    private:
        float m_Time = 0;
    };
}

