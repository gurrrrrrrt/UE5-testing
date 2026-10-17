#pragma once

#include "../framework.h"

namespace GUI {
	inline static bool open = true;

    inline SDK::FVector2D windowPos = SDK::FVector2D();
    inline SDK::FVector2D windowSize = SDK::FVector2D();
    inline SDK::FVector2D cursor = SDK::FVector2D();

    inline static int currentTab = 0;

    inline SDK::FLinearColor backgroundColor = SDK::FLinearColor(0.035f, 0.035f, 0.035f, 0.98f);
    inline SDK::FLinearColor titleBarColor = SDK::FLinearColor(0.055f, 0.055f, 0.055f, 1.0f);
    inline SDK::FLinearColor tabBarColor = SDK::FLinearColor(0.045f, 0.045f, 0.045f, 1.0f);
    inline SDK::FLinearColor tabColor = SDK::FLinearColor(0.06f, 0.06f, 0.06f, 1.0f);
    inline SDK::FLinearColor tabHoveredColor = SDK::FLinearColor(0.09f, 0.09f, 0.09f, 1.0f);
    inline SDK::FLinearColor tabActiveColor = SDK::FLinearColor(0.04f, 0.04f, 0.04f, 1.0f);
    inline SDK::FLinearColor borderColor = SDK::FLinearColor(0.14f, 0.14f, 0.14f, 1.0f);
    inline SDK::FLinearColor separatorColor = SDK::FLinearColor(0.10f, 0.10f, 0.10f, 1.0f);
    inline SDK::FLinearColor textColor = SDK::FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
    inline SDK::FLinearColor textDisabledColor = SDK::FLinearColor(0.42f, 0.42f, 0.42f, 1.0f);
    inline SDK::FLinearColor checkboxColor = SDK::FLinearColor(0.055f, 0.055f, 0.055f, 1.0f);
    inline SDK::FLinearColor accentColor = SDK::FLinearColor(0.0f, 0.40f, 0.75f, 1.0f);

    void DrawTxt(SDK::FVector2D pos, SDK::FString label);
    void DrawFilledRect(SDK::FVector2D pos, SDK::FVector2D size, SDK::FLinearColor color);
    void DrawOutlinedRect(SDK::FVector2D pos, SDK::FVector2D size, SDK::FLinearColor color, float thickness);
    void DrawCircle(SDK::FVector2D pos, int radius, int sides, SDK::FLinearColor color);

    void DrawTitleBar(SDK::FVector2D pos, SDK::FVector2D size, SDK::FString title);
    void BeginWindow(SDK::FVector2D pos, SDK::FVector2D size, SDK::FString title);

    void Tabs(const wchar_t* names[], int arraySize, int* tab);
    bool Checkbox(SDK::FString label, bool* v);
    bool SliderFloat(SDK::FString label, float* v, float min, float max);
}