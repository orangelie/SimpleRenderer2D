#pragma once

class IGameInterface;
class Shader;
class Camera;

class RenderEngine
{
public:
	void Start(HINSTANCE hInst, HWND hwnd, int w, int h, shared_ptr<IGameInterface> game);
	void Update(float dt);
	void Render();

	void ResizeBuffer(uint32 width, uint32 height);

	ComPtr<ID3D11Device> GetDevice() { return _device; }
	ComPtr<ID3D11DeviceContext> GetDeviceContext() { return _deviceContext; }
	
	shared_ptr<Shader> GetShader() { return _shader; }
	shared_ptr<Camera> GetCamera() { return _camera; }

	int GetWidth() { return _w; }
	int GetHeight() { return _h; }

private:
	void InitializeD3D11();
	void CreateRenderTargetView();
	void CleanupRenderTargetView();
	void CreateRenderDSView();

private:
	HINSTANCE _hInst = nullptr;
	HWND _hWnd = nullptr;
	int _w, _h;
	shared_ptr<IGameInterface> _game = nullptr;

private:
	ComPtr<ID3D11Device> _device = nullptr;
	ComPtr<ID3D11DeviceContext> _deviceContext = nullptr;
	ComPtr<IDXGISwapChain> _swapChain = nullptr;

private:
	ComPtr<ID3D11RenderTargetView> _renderTargetView = nullptr;
	ComPtr<ID3D11Texture2D> _depthStencilTexture = nullptr;
	ComPtr<ID3D11DepthStencilView> _depthStencilView = nullptr;

private:
	D3D11_VIEWPORT _viewport = {};
	Color _clearColor = { 1.0F, 1.0F, 1.0F, 1.0F };

private:
	shared_ptr<Shader> _shader = nullptr;
	shared_ptr<Camera> _camera = nullptr;

};
