#pragma once

class Global : public Singleton<Global>
{
public:
	HINSTANCE hDllInst;

	HWND hwndDlg;

	bool bShowWindow = true;//�Ƿ���ʾ�˵�

	//std::vector<uintptr_t> zombie;
	//std::SingleElemVector<uintptr_t> Zombie;
	
};