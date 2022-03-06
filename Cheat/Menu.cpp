#include "pch.h"
#include "framework.h"

void drawRect::GetRect()
{
	const std::shared_ptr<RECT> rect = std::make_shared<RECT>();
	GetWindowRect(Menu::get()->game_hwnd, rect.get());

	x = rect->left;
	y = rect->top + 25;

	width = rect->right - rect->left;
	height = rect->bottom - rect->top;
}

void Menu::Execute()
{
	std::thread(&Menu::CreateMenu, this).detach();
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT Menu::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))return true;

	switch (msg)
	{
	case WM_SIZE:
		if (get()->g_pd3dDevice != nullptr && wParam != SIZE_MINIMIZED)
		{
			if (get()->g_mainRenderTargetView) { get()->g_mainRenderTargetView->Release(); get()->g_mainRenderTargetView = nullptr; }
			get()->g_pSwapChain->ResizeBuffers(0, LOWORD(lParam), HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
			ID3D11Texture2D* pBackBuffer;
			get()->g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
			get()->g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &get()->g_mainRenderTargetView);
			pBackBuffer->Release();
		}
		return false;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return false;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return false;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void Menu::CreateMenu()
{
	//窗口初始化
	WindowInit();

	//初始化D3D
	if (!CreateDeviceD3D(hWindow))
	{
		CleanupDeviceD3D();
		OutputDebugStringEx("打开D3D设备失败\r\n");
		::UnregisterClass(wc.lpszClassName, wc.hInstance);
		ExitProcess(1);
	}

	//显示菜单
	ShowWindow(hWindow, SW_SHOW);
	UpdateWindow(hWindow);

	//透明窗口
	SetLayeredWindowAttributes(hWindow, RGB(0, 0, 0), 0, ULW_COLORKEY);

	// imgui窗口透明，也可以解决D3D绘制锯齿
	DWM_BLURBEHIND bb = { 0 };
	const HRGN hRgn = CreateRectRgn(0, 0, -1, -1);
	bb.dwFlags = DWM_BB_ENABLE | DWM_BB_BLURREGION;
	bb.hRgnBlur = hRgn;
	bb.fEnable = TRUE;
	DwmEnableBlurBehindWindow(hWindow, &bb);

	//ImGui初始化
	ImGuiInit();

	//绘制
	MainDraw();
}

uintptr_t temp = 0;
uintptr_t JmpBack = 0x0052D9E8; //后台运行跳转回去地址
__declspec(naked)
void GetZombieAddr() {
	
	__asm {
		fld dword ptr[ecx + 0x2C]
		push edi
		fisub[ecx + 0x08]

		mov temp, ecx

		jmp JmpBack
	}
}
float fTest;
void Test()
{
	if (ImGui::BeginChild(u8"透视", ImVec2(0, 0), true)) {
		ImGui::SliderFloat(u8"Test", &fTest,0,1000);
	}ImGui::EndChild();
}

void Menu::MainDraw()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	BYTE oldGetZombieAddress[7];
	BYTE newGetZombieAddress[7];
	const std::set<uintptr_t> zomibe;

	drawMenu::get()->visuals_tab_window = Test;

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		rect.GetRect();
		SetWindowPos(hWindow, HWND_TOPMOST,
			rect.x, rect.y, rect.width, rect.height, SWP_SHOWWINDOW);
		MoveWindow(hWindow, rect.x, rect.y, rect.width, rect.height, true);

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		drawMenu::get()->Execute();

		//测试
		memcpy(&oldGetZombieAddress, reinterpret_cast<void const*>(0x0052D9E1), sizeof(oldGetZombieAddress));
		newGetZombieAddress[0] = '\xE9';
		*reinterpret_cast<uintptr_t*>(newGetZombieAddress + 1) = reinterpret_cast<uintptr_t>(GetZombieAddr) - 0x0052D9E1 - 5;

		newGetZombieAddress[5] = '\x90';
		newGetZombieAddress[6] = '\x90';

		//测试 写入已经被修改的代码覆盖原本的 (这里需要优化用 WriteProcessMemory 不实际)
		WriteProcessMemory(
			GetCurrentProcess(),
			reinterpret_cast<LPVOID>(0x0052D9E1), newGetZombieAddress, 
			sizeof(newGetZombieAddress), nullptr
		);

		//if (temp)
		//{
		//	Zomibe.insert(temp);
		//	//Global::get()->Zombie.push_back(temp);
		//}

		//for (const auto& it : Zomibe)
		//{
		//	Memory::Write<float>(it + 0x2C, 800);
		//}


		ImGui::Render();
		g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
		ImVec4 clearColor = ImGui::ColorConvertU32ToFloat4(ImColor(0, 0, 0, 0));
		g_pd3dDeviceContext->ClearRenderTargetView(
			g_mainRenderTargetView,reinterpret_cast<const float*>(&clearColor)
		);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		g_pSwapChain->Present(1, 0);
	}
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	CleanupDeviceD3D();
	DestroyWindow(hWindow);
	UnregisterClass(wc.lpszClassName, wc.hInstance);
}

void Menu::WindowInit()
{
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(nullptr);
	wc.hIcon = wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = " ";
	wc.lpszClassName = " ";

	RegisterClassEx(&wc);

	//获得大小
	rect.GetRect();

	// WS_EX_LAYERED 可使用SetLayeredWindowAttributes函数
	// WS_EX_TRANSPARENT 鼠标会穿透 (这个开了可能导致菜单无法移动)
	// WS_EX_TOPMOST 窗口置顶
	// WS_EX_TOOLWINDOW 任务栏不显示
	//打开窗口
	hWindow = CreateWindowEx(
		WS_EX_LAYERED | WS_EX_TOPMOST,
		wc.lpszClassName, " ", 
		WS_POPUP | WS_VISIBLE | WS_EX_TRANSPARENT,
		rect.x, rect.y,rect.width,rect.height,
		nullptr, nullptr, wc.hInstance, &wc
	);
}

void Menu::ImGuiInit() const
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	Renderer::get()->io = ImGui::GetIO();
	Renderer::get()->io.WantSaveIniSettings = false;
	Renderer::get()->io.IniFilename = nullptr;

	Renderer::SetStyle();
	ImGui_ImplWin32_Init(hWindow);
	ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
}

bool Menu::CreateDeviceD3D(HWND hWnd)
{
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

	D3D_FEATURE_LEVEL featureLevel;
	constexpr D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
	if (D3D11CreateDeviceAndSwapChain(
		nullptr, D3D_DRIVER_TYPE_HARDWARE, 
		nullptr, 0, featureLevelArray,
		2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, 
		&featureLevel, &g_pd3dDeviceContext) != S_OK
		)return false;

	ID3D11Texture2D* pBackBuffer;
	g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
	pBackBuffer->Release();
	return true;
}
void Menu::CleanupDeviceD3D()
{
	if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
	if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
	if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
	if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}