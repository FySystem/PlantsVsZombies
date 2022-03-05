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

	//��ϷHWND
	HWND game_hwnd = GetCurrentProcessH();
private:
	static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	//�򿪲˵�
	void CreateMenu();

	//���ڳ�ʼ��
	void WindowInit();

	//ImGui��ʼ��
	void ImGuiInit() const;

	//--------D3D--------
	bool CreateDeviceD3D(HWND hWnd);//���豸 
	void CleanupDeviceD3D();//�ͷ��豸
	//-------------------

	WNDCLASSEX wc;

	drawRect rect;

	//����HWND
	HWND hWindow;
};
