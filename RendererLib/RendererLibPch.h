#pragma once
#pragma warning(disable:4996)

#define _HAS_STD_BYTE 0

#include <Windows.h>
#include <windowsx.h>

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <array>
#include <memory>
#include <algorithm>

using namespace std;

#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3d11shader.h>
#include <d3d11.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <DirectXTex/DirectXTex.h>
#include <DirectXTex/DirectXTex.inl>
#include "SimpleMath.h"

using namespace DirectX;
using namespace Microsoft::WRL;

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

// Libs
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex/DirectXTex_debug.lib")
#else
#pragma comment(lib, "DirectXTex/DirectXTex.lib")
#endif

using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;
using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;

using Color = DirectX::XMFLOAT4;

using Vec2 = DirectX::SimpleMath::Vector2;
using Vec3 = DirectX::SimpleMath::Vector3;
using Vec4 = DirectX::SimpleMath::Vector4;
using Matrix = DirectX::SimpleMath::Matrix;
using Quaternion = DirectX::SimpleMath::Quaternion;

#define DECLARE_SINGLE(classname)			\
private:									\
	classname() { }							\
public:										\
	static classname* GetInstance()			\
	{										\
		static classname s_instance;		\
		return &s_instance;					\
	}

#define GET_SINGLE(classname)	(classname::GetInstance())

#include "RenderEngine.h"
#include "GameLoader.h"
#include "Timer.h"
#include "Input.h"

#define TIMER GET_SINGLE(Timer)
#define INPUT GET_SINGLE(Input)

#define CHECK(p)	assert(SUCCEEDED(p))

#define DEVICE		(GET_SINGLE(GameLoader)->GetEngine()->GetDevice())
#define DC			(GET_SINGLE(GameLoader)->GetEngine()->GetDeviceContext())
#define SHADER		(GET_SINGLE(GameLoader)->GetEngine()->GetShader())
#define CAMERA		(GET_SINGLE(GameLoader)->GetEngine()->GetCamera())

#define WIDTH		(GET_SINGLE(GameLoader)->GetEngine()->GetWidth())
#define HEIGHT		(GET_SINGLE(GameLoader)->GetEngine()->GetHeight())
