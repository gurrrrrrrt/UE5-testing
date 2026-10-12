#include "postrender.h"
#include "../gui/framework.h"

PostRender::PostRender_t PostRender::PostRenderOriginal = nullptr;

void PostRender::Hooked_PostRender(SDK::UGameViewportClient* viewport, SDK::UCanvas* canvas) {
    SDK::UWorld* world = viewport->World;

    SDK::UGameInstance* gameInstance = viewport->GameInstance;
    if (!gameInstance) {
        return PostRenderOriginal(viewport, canvas);
    }

    SDK::ULocalPlayer* localPlayer = gameInstance->LocalPlayers[0];
    if (!localPlayer) {
        return PostRenderOriginal(viewport, canvas);
    }

    SDK::APlayerController* controller = localPlayer->PlayerController;
    if (!controller) {
        return PostRenderOriginal(viewport, canvas);
    }

    if (GetAsyncKeyState(VK_INSERT) & 1) {
        GUI::open = !GUI::open;
    }

    if (canvas) {
        Globals::canvas = canvas;

        if (GUI::open) {
            GUI::BeginWindow(SDK::FVector2D(100.0f, 100.0f), SDK::FVector2D(300.0f, 400.0f), SDK::FString(L"jewengine5"));

            GUI::Checkbox(SDK::FString(L"Bounding Box"), &Features::boundingBox);
        }
    }

    return PostRenderOriginal(viewport, canvas);
}