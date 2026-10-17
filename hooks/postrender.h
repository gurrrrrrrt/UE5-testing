#pragma once

#include "../framework.h"

namespace PostRender {
    inline const wchar_t* bonePairs[][2] = {
        { L"head", L"neck_01" },
        { L"neck_01", L"spine_03" },
        { L"spine_03", L"spine_01" },
        { L"spine_01", L"pelvis" },
        { L"neck_01", L"upperarm_l" },
        { L"upperarm_l", L"lowerarm_l" },
        { L"lowerarm_l", L"hand_l" },
        { L"neck_01", L"upperarm_r" },
        { L"upperarm_r", L"lowerarm_r" },
        { L"lowerarm_r", L"hand_r" },
        { L"pelvis", L"thigh_l" },
        { L"thigh_l", L"calf_l" },
        { L"calf_l", L"foot_l" },
        { L"pelvis", L"thigh_r" },
        { L"thigh_r", L"calf_r" },
        { L"calf_r", L"foot_r" }
    };

	typedef void(__fastcall* PostRender_t)(SDK::UGameViewportClient*, SDK::UCanvas*);
	extern PostRender_t PostRenderOriginal;

	void Hooked_PostRender(SDK::UGameViewportClient* viewport, SDK::UCanvas* canvas);

	inline ShadowVMT vmt;
}