#pragma once

#include <cstdint>
#include <cstdlib>
#include <iostream>

class ShadowVMT {
public:
	ShadowVMT() : original(nullptr), lastHookedFunction(0), lastHookedFunctionIdx(-1) {}

	template <typename T>
	bool Hook(void* instance, void* hookFunction, int idx, T* originalFunction) {
		this->original = *reinterpret_cast<uintptr_t**>(instance);

		if (this->lastHookedFunction && this->lastHookedFunctionIdx != -1) {
			if (lastHookedFunction == this->original[this->lastHookedFunctionIdx]) {
				return false;
			}
		}

		void* originalFunctionPtr = reinterpret_cast<void*>(this->original[idx]);
		if (originalFunctionPtr != hookFunction) {
			int tableSz = 0;
			while (this->original[tableSz]) {
				++tableSz;
			}

			uintptr_t* newVtable = reinterpret_cast<uintptr_t*>(malloc(tableSz * sizeof(uintptr_t)));

			std::memcpy(newVtable, this->original, tableSz * sizeof(uintptr_t));

			*originalFunction = reinterpret_cast<T>(originalFunctionPtr);

			newVtable[idx] = reinterpret_cast<uintptr_t>(hookFunction);

			*reinterpret_cast<uintptr_t**>(instance) = newVtable;

			this->lastHookedFunction = reinterpret_cast<uintptr_t>(hookFunction);
			this->lastHookedFunctionIdx = idx;

			return true;
		}

		return false;
	}

private:
	uintptr_t* original;
	uintptr_t lastHookedFunction;
	int lastHookedFunctionIdx;
};