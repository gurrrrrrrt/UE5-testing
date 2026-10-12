#include "framework.h"
#include "hooks/postrender.h"

DWORD MainThread(HMODULE hModule) {
    std::println("injected");

    while (true) {
        SDK::UEngine* engine = SDK::UEngine::GetEngine();
        if (engine && engine->GameViewport) {
            Globals::gameViewportClient = engine->GameViewport;
            break;
        }

        Sleep(200);
    }

    if (Globals::gameViewportClient) {
        std::println("got gameViewportClient");
    }

    if (Globals::gameViewportClient && PostRender::vmt.Hook(Globals::gameViewportClient, (void*)&PostRender::Hooked_PostRender, Indexes::postRender, &PostRender::PostRenderOriginal)) {
        std::println("hooked post render");
    }

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);

        AllocConsole();

        FILE* file;
        freopen_s(&file, "CONOUT$", "w", stdout);
        freopen_s(&file, "CONOUT$", "w", stderr);
        
        if (HANDLE hThread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, nullptr)) {
            CloseHandle(hThread);
        }
    }

    return TRUE;
}