#include "framework.h"

void GUI::DrawFilledRect(SDK::FVector2D pos, SDK::FVector2D size, SDK::FLinearColor color) {
    for (double i = 0; i < size.Y; i++) {
        Globals::canvas->K2_DrawLine(SDK::FVector2D(pos.X, pos.Y + i), SDK::FVector2D(pos.X + size.X, pos.Y + i), 1.0f, color);
    }
}

void GUI::DrawTitleBar(SDK::FVector2D pos, SDK::FVector2D size, SDK::FString title) {
    DrawFilledRect(pos, size, SDK::FLinearColor(0.15f, 0.15f, 0.15f, 1.0f));

    SDK::FVector2D titlePos(pos.X + (size.X / 2.0f), pos.Y + (size.Y / 2.0f));

    Globals::canvas->K2_DrawText(Render::defaultFont, SDK::FString(title), titlePos, SDK::FVector2D(1.0f, 1.0f), SDK::FLinearColor(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, SDK::FLinearColor(0.0f, 0.0f, 0.0f, 1.0f), SDK::FVector2D(1.0f, 1.0f), true, true, true, SDK::FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));
}

void GUI::BeginWindow(SDK::FVector2D pos, SDK::FVector2D size, SDK::FString title) {
    windowPos = pos;

    DrawFilledRect(pos, size, SDK::FLinearColor(0.05f, 0.05f, 0.05f, 0.85f));
    DrawTitleBar(pos, SDK::FVector2D(size.X, 30.0f), title);

    cursor = SDK::FVector2D(pos.X + 15.0f, pos.Y + 45.0f);
}

bool GUI::Checkbox(SDK::FString label, bool* v) {
    SDK::FVector2D pos = cursor;
    SDK::FVector2D boxSize(16.0f, 16.0f);

    DrawFilledRect(pos, boxSize, SDK::FLinearColor(0.2f, 0.2f, 0.2f, 1.0f));

    Globals::canvas->K2_DrawLine(pos, SDK::FVector2D(pos.X + boxSize.X, pos.Y), 1.0f, SDK::FLinearColor(0.5f, 0.5f, 0.5f, 1.0f));
    Globals::canvas->K2_DrawLine(SDK::FVector2D(pos.X + boxSize.X, pos.Y), SDK::FVector2D(pos.X + boxSize.X, pos.Y + boxSize.Y), 1.0f, SDK::FLinearColor(0.5f, 0.5f, 0.5f, 1.0f));
    Globals::canvas->K2_DrawLine(SDK::FVector2D(pos.X + boxSize.X, pos.Y + boxSize.Y), SDK::FVector2D(pos.X, pos.Y + boxSize.Y), 1.0f, SDK::FLinearColor(0.5f, 0.5f, 0.5f, 1.0f));
    Globals::canvas->K2_DrawLine(SDK::FVector2D(pos.X, pos.Y + boxSize.Y), pos, 1.0f, SDK::FLinearColor(0.5f, 0.5f, 0.5f, 1.0f));

    if (*v) {
        DrawFilledRect(SDK::FVector2D(pos.X + 3.0f, pos.Y + 3.0f), SDK::FVector2D(boxSize.X - 6.0f, boxSize.Y - 6.0f), SDK::FLinearColor(0.0f, 0.6f, 1.0f, 1.0f));
    }

    SDK::FVector2D textPos(pos.X + 24.0f, pos.Y + (boxSize.Y / 2.0f));

    Globals::canvas->K2_DrawText(Render::defaultFont, label, textPos, SDK::FVector2D(1.0f, 1.0f), SDK::FLinearColor(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, SDK::FLinearColor(0.0f, 0.0f, 0.0f, 1.0f), SDK::FVector2D(1.0f, 1.0f), false, true, true, SDK::FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));

    cursor.Y += 26.0f;

    return *v;
}

bool GUI::SliderFloat(SDK::FString label, float* v, float min, float max) {
    return true;
}