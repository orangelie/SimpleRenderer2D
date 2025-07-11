#include "pch.h"
#include "RenderEngine.h"
#include "IGameInterface.h"
#include "Shader.h"
#include "Camera.h"

void RenderEngine::Start(HINSTANCE hInst, HWND hwnd, int w, int h, shared_ptr<IGameInterface> game)
{
	_hInst = hInst;
	_hWnd = hwnd;
	_game = game;
	_w = w;
	_h = h;

	InitializeD3D11();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	ImGui::StyleColorsDark();
	// ImGui::StyleColorsLight();

	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(DEVICE.Get(), DC.Get());

	// Rendering
	{
		_shader = make_shared<Shader>();
		_shader->Start();

		_camera = make_shared<Camera>();
		_camera->Start();
	}

	game->Init(hInst, hwnd);
	game->Start();
}

void RenderEngine::Update(float dt)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	_game->Update(dt);
}

void RenderEngine::Render()
{
	// RENDER BEGIN
	_deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), _depthStencilView.Get());
	_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), (float*)(&_clearColor));
	_deviceContext->ClearDepthStencilView(_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	_deviceContext->RSSetViewports(1, &_viewport);

	// CUSTOM
	ImGui::Begin("UI");
	_shader->Render();
	_camera->Render();
	_game->Render();
	ImGui::End();

	// IMGUI
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	// RENDER END
	CHECK(_swapChain->Present(1, 0));
}

void RenderEngine::ResizeBuffer(uint32 width, uint32 height)
{
	if (DEVICE != nullptr)
	{
		// SETUP
		_w = width;
		_h = height;

		// RTV
		_deviceContext->OMSetRenderTargets(0, nullptr, nullptr);
		_deviceContext->Flush();
		CleanupRenderTargetView();

		_swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
		CreateRenderTargetView();
		CreateRenderDSView();

		// VIEWPORT
		_viewport.Width = static_cast<float>(width);
		_viewport.Height = static_cast<float>(height);

		// IMGUI
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)width, (float)height);
		io.DisplayFramebufferScale = ImVec2(1.0F, 1.0F);

		_deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), _depthStencilView.Get());
	}
}

void RenderEngine::InitializeD3D11()
{
	// device, dc, swapChain
	DXGI_SWAP_CHAIN_DESC desc = {};
	{
		desc.BufferDesc.Width = _w;
		desc.BufferDesc.Height = _h;
		desc.BufferDesc.RefreshRate.Numerator = 120;
		desc.BufferDesc.RefreshRate.Denominator = 1;
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.BufferCount = 2;
		desc.OutputWindow = _hWnd;
		desc.Windowed = TRUE;
		desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	}

	CHECK(::D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&desc,
		_swapChain.GetAddressOf(),
		_device.GetAddressOf(),
		nullptr,
		_deviceContext.GetAddressOf()
	));

	// main rtv
	CreateRenderTargetView();

	// DSV
	CreateRenderDSView();

	_viewport.TopLeftX = 0.0F;
	_viewport.TopLeftY = 0.0F;
	_viewport.Width = static_cast<float>(_w);
	_viewport.Height = static_cast<float>(_h);
	_viewport.MinDepth = 0.0F;
	_viewport.MaxDepth = 1.0F;
}

void RenderEngine::CreateRenderTargetView()
{
	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	CHECK(_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf()));
	CHECK(_device->CreateRenderTargetView(backBuffer.Get(), nullptr, _renderTargetView.GetAddressOf()));
}

void RenderEngine::CleanupRenderTargetView()
{
	_renderTargetView.Reset();
	_depthStencilView.Reset();
	_depthStencilTexture.Reset();
}

void RenderEngine::CreateRenderDSView()
{
	{
		D3D11_TEXTURE2D_DESC desc = {};
		desc.Width = static_cast<uint32>(_w);
		desc.Height = static_cast<uint32>(_h);
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		HRESULT hr = DEVICE->CreateTexture2D(&desc, nullptr, _depthStencilTexture.GetAddressOf());
		CHECK(hr);
	}

	{
		D3D11_DEPTH_STENCIL_VIEW_DESC desc = {};
		desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice = 0;

		HRESULT hr = DEVICE->CreateDepthStencilView(_depthStencilTexture.Get(), &desc, _depthStencilView.GetAddressOf());
		CHECK(hr);
	}
}
