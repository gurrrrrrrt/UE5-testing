#include "framework.h"
#include "zeroinput.h"

SDK::FVector2D CursorPos() {
    POINT pos;
    GetCursorPos(&pos);

    return SDK::FVector2D((float)pos.x, (float)pos.y);
}

bool MouseInZone(SDK::FVector2D pos, SDK::FVector2D size) {
    SDK::FVector2D cursor_pos = CursorPos();
    if (cursor_pos.X > pos.X && cursor_pos.Y > pos.Y) {
        if (cursor_pos.X < pos.X + size.X && cursor_pos.Y < pos.Y + size.Y) {
            return true;
        }
    }

    return false;
}

bool IsMouseClicked() {
    static bool prevPressed = false;
    bool pressed = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
    bool clicked = pressed && !prevPressed;

    prevPressed = pressed;

    return clicked;
}

bool IsMouseDown() {
    return (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
}

void GUI::DrawTxt(SDK::FVector2D pos, SDK::FString label) {
    Globals::canvas->K2_DrawText(Render::Fonts::roboto, SDK::FString(label), pos, SDK::FVector2D(1.0f, 1.0f), SDK::FLinearColor(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, SDK::FLinearColor(0.0f, 0.0f, 0.0f, 1.0f), SDK::FVector2D(1.0f, 1.0f), false, false, true, SDK::FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));
}

void GUI::DrawFilledRect(SDK::FVector2D pos, SDK::FVector2D size, SDK::FLinearColor color) {
    for (double i = 0; i < size.Y; i++) {
        Globals::canvas->K2_DrawLine(SDK::FVector2D(pos.X, pos.Y + i), SDK::FVector2D(pos.X + size.X, pos.Y + i), 1.0f, color);
    }
}

void GUI::DrawOutlinedRect(SDK::FVector2D pos, SDK::FVector2D size, SDK::FLinearColor color, float thickness = 1.0f) {
    Globals::canvas->K2_DrawLine(pos, SDK::FVector2D(pos.X + size.X, pos.Y), thickness, color);
    Globals::canvas->K2_DrawLine(pos, SDK::FVector2D(pos.X, pos.Y + size.Y), thickness, color);
    Globals::canvas->K2_DrawLine(SDK::FVector2D(pos.X, pos.Y + size.Y), SDK::FVector2D(pos.X + size.X, pos.Y + size.Y), thickness, color);
    Globals::canvas->K2_DrawLine(SDK::FVector2D(pos.X + size.X, pos.Y), SDK::FVector2D(pos.X + size.X, pos.Y + size.Y), thickness, color);
}

void GUI::DrawCircle(SDK::FVector2D pos, int radius, int sides, SDK::FLinearColor color) {
    float step = 3.1415927f * 2.0f / sides;
    int count = 0;

    SDK::FVector2D V[128];

    for (float a = 0; a < 3.1415927f * 2.0f; a += step) {
        float X1 = radius * cos(a) + pos.X;
        float Y1 = radius * sin(a) + pos.Y;
        float X2 = radius * cos(a + step) + pos.X;
        float Y2 = radius * sin(a + step) + pos.Y;

        V[count].X = X1;
        V[count].Y = Y1;
        V[count + 1].X = X2;
        V[count + 1].Y = Y2;

        Globals::canvas->K2_DrawLine(SDK::FVector2D(V[count].X, V[count].Y), SDK::FVector2D(X2, Y2), 1.0f, color);
    }
}

void GUI::DrawTitleBar(SDK::FVector2D pos, SDK::FVector2D size, SDK::FString title) {
    DrawFilledRect(pos, size, titleBarColor);
    DrawFilledRect(pos, SDK::FVector2D(size.X, 2.0f), accentColor);

    SDK::FVector2D titlePos(pos.X + (size.X / 2.0f), pos.Y + (size.Y / 2.0f));

    Globals::canvas->K2_DrawText(Render::Fonts::roboto, SDK::FString(title), titlePos, SDK::FVector2D(1.0f, 1.0f), SDK::FLinearColor(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, SDK::FLinearColor(0.0f, 0.0f, 0.0f, 1.0f), SDK::FVector2D(1.0f, 1.0f), true, true, true, SDK::FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));
}

void GUI::BeginWindow(SDK::FVector2D pos, SDK::FVector2D size, SDK::FString title) {
    static bool initialized = false;
    if (!initialized) {
        windowPos = pos;
        windowSize = size;
        initialized = true;
    }

    SDK::FVector2D titleBarSize = SDK::FVector2D(size.X, 30.0f);

    static bool isDragging = false;
    static SDK::FVector2D dragOffset = SDK::FVector2D();

    if (IsMouseDown()) {
        if (isDragging) {
            windowPos.X = CursorPos().X - dragOffset.X;
            windowPos.Y = CursorPos().Y - dragOffset.Y;
        }
        else if (MouseInZone(windowPos, titleBarSize)) {
            isDragging = true;
            dragOffset = CursorPos() - windowPos;
        }
    }
    else {
        isDragging = false;
    }

    DrawFilledRect(windowPos, size, backgroundColor);
    DrawTitleBar(windowPos, titleBarSize, title);
    DrawOutlinedRect(windowPos, size, borderColor, 1.0f);

    cursor = SDK::FVector2D(windowPos.X + 15.0f, windowPos.Y + titleBarSize.Y);
}

void GUI::Tabs(const wchar_t* names[], int arraySize, int* tab) {
    float tabWidth = windowSize.X / arraySize;
    float tabHeight = 30.0f;
    SDK::FVector2D pos = SDK::FVector2D(windowPos.X, windowPos.Y + 30.0f);

    for (int i = 0; i < arraySize; i++) {
        SDK::FVector2D tabPos = SDK::FVector2D(pos.X + (tabWidth * i), pos.Y);
        SDK::FVector2D tabSize = SDK::FVector2D(tabWidth, tabHeight);
        SDK::FLinearColor color = tabColor;

        bool hovered = MouseInZone(tabPos, tabSize);
        bool selected = (*tab == i);

        if (hovered) {
            color = tabHoveredColor;
        }

        if (selected) {
            color = tabActiveColor;
        }

        if (hovered && IsMouseClicked()) {
            *tab = i;
        }

        DrawFilledRect(tabPos, tabSize, color);
        DrawOutlinedRect(tabPos, tabSize, borderColor);

        if (selected) {
            DrawFilledRect(SDK::FVector2D(tabPos.X, tabPos.Y + tabHeight - 2.0f), SDK::FVector2D(tabWidth, 2.0f), accentColor);
        }

        SDK::FVector2D textPos = SDK::FVector2D(tabPos.X + (tabWidth / 2.0f), tabPos.Y + (tabHeight / 2.0f));
        SDK::FLinearColor textCol = selected ? textColor : textDisabledColor;

        Globals::canvas->K2_DrawText(Render::Fonts::roboto, SDK::FString(names[i]), textPos, SDK::FVector2D(1.0f, 1.0f), textCol, 0.0f, SDK::FLinearColor(0.0f, 0.0f, 0.0f, 1.0f), SDK::FVector2D(1.0f, 1.0f), true, true, true, SDK::FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));
    }

    cursor = SDK::FVector2D(windowPos.X + 15.0f, pos.Y + tabHeight + 15.0f);
}

bool GUI::Checkbox(SDK::FString label, bool* v) {
    SDK::FVector2D pos = cursor;
    SDK::FVector2D boxSize = SDK::FVector2D(16.0f, 16.0f);
    SDK::FVector2D interactiveSize = SDK::FVector2D(boxSize.X + 150.0f, boxSize.Y);

    bool hovered = MouseInZone(pos, interactiveSize);
    if (hovered && IsMouseClicked()) {
        *v = !(*v);
    }

    DrawFilledRect(pos, boxSize, checkboxColor);
    DrawOutlinedRect(pos, boxSize, hovered ? textDisabledColor : borderColor);

    if (*v) {
        DrawFilledRect(SDK::FVector2D(pos.X + 3.0f, pos.Y + 3.0f), SDK::FVector2D(boxSize.X - 6.0f, boxSize.Y - 6.0f), accentColor);
    }

    SDK::FVector2D textPos = SDK::FVector2D(pos.X + 24.0f, pos.Y + (boxSize.Y / 2.0f));
    SDK::FLinearColor textCol = hovered || *v ? textColor : textDisabledColor;

    Globals::canvas->K2_DrawText(Render::Fonts::roboto, label, textPos, SDK::FVector2D(1.0f, 1.0f), textCol, 0.0f, SDK::FLinearColor(0.0f, 0.0f, 0.0f, 1.0f), SDK::FVector2D(1.0f, 1.0f), false, true, true, SDK::FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));

    cursor.Y += 26.0f;

    return *v;
}

bool GUI::SliderFloat(SDK::FString label, float* v, float min, float max) {
    return true;
}