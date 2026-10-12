#pragma once

#include "../framework.h"

namespace PostRender {
	typedef void(__fastcall* PostRender_t)(SDK::UGameViewportClient*, SDK::UCanvas*);
	extern PostRender_t PostRenderOriginal;

	void Hooked_PostRender(SDK::UGameViewportClient* viewport, SDK::UCanvas* canvas);

	inline ShadowVMT vmt;
}