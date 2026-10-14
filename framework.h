#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files

#include <windows.h>
#include <print>
#include "memory/shadowvmt.h"
#include "CppSDK/SDK.hpp"

namespace Globals {
	inline SDK::APlayerController* localController = nullptr;
	inline SDK::UGameViewportClient* gameViewportClient = nullptr;
	inline SDK::UCanvas* canvas = nullptr;
	inline SDK::FVector2D screenSize = SDK::FVector2D();
}

namespace Indexes {
	inline int postRender = 0x63;
}

namespace Render {
	inline SDK::UFont* defaultFont = SDK::UObject::FindObject<SDK::UFont>("Font Roboto.Roboto");
}

namespace Features {
	namespace Aim {
	}

	namespace Visuals {
		inline bool boundingBox = true;
		inline bool skeleton = true;
		inline bool chams = true;
	}

	namespace Exploits {
	}
}