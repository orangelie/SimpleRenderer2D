#pragma once

class RenderEngine;
class IGameInterface;

class GameLoader
{
private:
	GameLoader() { }
public:										
	static GameLoader* GetInstance()
	{
		static GameLoader s_instance;
		return &s_instance;
	}

public:
	void Init(HINSTANCE hInst, int width, int height, shared_ptr<IGameInterface> game);
	void Run();

	shared_ptr<RenderEngine> GetEngine() { return _game; }

private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	HINSTANCE _hInst;
	HWND _hWnd;

	shared_ptr<RenderEngine> _game = nullptr;

};
