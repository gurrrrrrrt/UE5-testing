#pragma once

#include "../framework.h"

// todo: add mouse input handling for moving menu and clicking/dragging widgets

namespace GUI {
	inline static bool open = true;

    inline SDK::FVector2D windowPos;
    inline SDK::FVector2D cursor;

    void DrawTxt(SDK::FVector2D pos, SDK::FString label);
    void DrawFilledRect(SDK::FVector2D pos, SDK::FVector2D size, SDK::FLinearColor color);
    void DrawTitleBar(SDK::FVector2D pos, SDK::FVector2D size, SDK::FString title);
    void BeginWindow(SDK::FVector2D pos, SDK::FVector2D size, SDK::FString title);
    
    bool Checkbox(SDK::FString label, bool* v);
    bool SliderFloat(SDK::FString label, float* v, float min, float max);
}