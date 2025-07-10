#pragma once

class IGameInterface
{
protected:
	friend class RenderEngine;
	void Init(HINSTANCE hInst, HWND hwnd) { _hInst = hInst; _hWnd = hwnd; }
	void Run() {}

public:
	virtual void Start() = 0;
	virtual void Update(float dt) = 0;
	virtual void Render() = 0;

protected:
	HINSTANCE _hInst;
	HWND _hWnd;

};
