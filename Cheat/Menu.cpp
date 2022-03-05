#include "pch.h"
#include "framework.h"

void Menu::Execute()
{
	std::thread(&Menu::CreateMenu, this).detach();
}

LRESULT Menu::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_PAINT:

		break;
	case WM_CREATE:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//-----------------------------------------------------------------
// Data
static ID3D11Device* g_pd3dDevice = NULL;
static ID3D11DeviceContext* g_pd3dDeviceContext = NULL;
static IDXGISwapChain* g_pSwapChain = NULL;
static ID3D11RenderTargetView* g_mainRenderTargetView = NULL;

void CreateRenderTarget()
{
	ID3D11Texture2D* pBackBuffer;
	g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
	pBackBuffer->Release();
}

bool CreateDeviceD3D(HWND hWnd)
{
	// Setup swap chain
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	UINT createDeviceFlags = 0;
	//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
	if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
		return false;

	CreateRenderTarget();
	return true;
}

void CleanupRenderTarget()
{
	if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
}

void CleanupDeviceD3D()
{
	CleanupRenderTarget();
	if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
	if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
	if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
}
//-----------------------------------------------------------------

void Menu::CreateMenu()
{
	WNDCLASSEX wc
	{
		sizeof(WNDCLASSEX),
		CS_VREDRAW | CS_HREDRAW | CS_CLASSDC,
		WndProc,
		0L,0L,GetModuleHandle(nullptr),
		nullptr,nullptr,nullptr,nullptr, " ",nullptr
	};

	RegisterClassEx(&wc);

	//获得大小
	RECT rect;
	if (!GetWindowRect(gmae_hwnd, &rect)) {
		MessageBox(nullptr, "游戏未开启", "提示", MB_OK);
		ExitProcess(1);
	}

	//打开窗口
	hWindow = CreateWindowEx(
		WS_EX_TRANSPARENT | WS_EX_LAYERED,
		wc.lpszClassName, " ", WS_POPUP,
		rect.left, rect.top,
		rect.right - rect.left,
		rect.bottom - rect.top,
		nullptr, nullptr, wc.hInstance, &wc
	);

	//初始化D3D
	if (!CreateDeviceD3D(hWindow))
	{
		CleanupDeviceD3D();
		::UnregisterClass(wc.lpszClassName, wc.hInstance);
		ExitProcess(1);
	}

	//透明窗口
	SetLayeredWindowAttributes(hWindow, RGB(0, 0, 0), 0, ULW_COLORKEY);

	//显示菜单
	ShowWindow(hWindow, SW_SHOWDEFAULT);
	UpdateWindow(hWindow);

	//ImGui初始化
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.WantSaveIniSettings = false;
	io.IniFilename = nullptr;

	ImGui::StyleColorsLight();
	ImGui_ImplWin32_Init(hWindow);
	ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while(msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		if(ImGui::Begin("11111111"))
		{
			
		}ImGui::End();


		ImGui::Render();
		g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
		ImVec4 clearColor = ImGui::ColorConvertU32ToFloat4(ImColor(0, 0, 0, 0));
		g_pd3dDeviceContext->ClearRenderTargetView(
			g_mainRenderTargetView,
			reinterpret_cast<const float*>(&clearColor)
		);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		g_pSwapChain->Present(1, 0);
	}

}
