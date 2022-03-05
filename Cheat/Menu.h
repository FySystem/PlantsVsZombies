#pragma once

class drawRect
{
public:
	int x, y, width, height;

	void GetRect();
};

class Menu : public Singleton<Menu>
{
public:
	void Execute();

	//D3D11
	ID3D11Device* g_pd3dDevice = nullptr;
	ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
	IDXGISwapChain* g_pSwapChain = nullptr;
	ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

	//游戏HWND
	HWND game_hwnd = GetCurrentProcessH();
private:
	static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	//打开菜单
	void CreateMenu();

	//主要绘制 
	void MainDraw();

	//窗口初始化
	void WindowInit();

	//ImGui初始化
	void ImGuiInit() const;

	//--------D3D--------
	bool CreateDeviceD3D(HWND hWnd);//打开设备 
	void CleanupDeviceD3D();//释放设备
	//-------------------

	WNDCLASSEX wc;

	drawRect rect;

	//窗口HWND
	HWND hWindow;
};
