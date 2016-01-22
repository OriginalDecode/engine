#pragma once

#define SAFE_DELETE(ptr) delete ptr; ptr = nullptr
#define SAFE_RELEASE(dxPtr) if(dxPtr != nullptr){ dxPtr->Release(); dxPtr = nullptr; }

#define DEFERREDCONTEXT