#pragma once

class Menu : public Singleton<Menu>
{
public:
	void Execute();
private:
	static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	//�򿪲˵�
	void CreateMenu();

	//����HWND
	HWND hWindow;

	//��ϷHWND
	HWND gmae_hwnd = FindWindow(nullptr, "PvZ-Eagrace Max By Henry-joestar   (B&Eteam)  QQgroup:821960155");
};
