#pragma once

class Global : public Singleton<Global>
{
public:
	HINSTANCE hDllInst;

	HWND hwndDlg;

	bool bShowWindow = true;//�Ƿ���ʾ�˵�
};