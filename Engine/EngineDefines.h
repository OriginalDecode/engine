#pragma once

#define SAFE_DELETE(ptr) if(ptr){ delete ptr; ptr = nullptr;}
#define SAFE_DELETEA(ptr) delete[] ptr; ptr = nullptr
#define SAFE_RELEASE(dxPtr) if(dxPtr != nullptr){ dxPtr->Release(); dxPtr = nullptr; }
#define DELETE_MAP(map) for(auto it = map.begin(); it != map.end(); it++) { SAFE_DELETE(it->second); }
#define PI 3.14159265359

#define DEFERREDCONTEXT

#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#define MIN(a,b) (((a) < (b)) ? (a) : (b))



