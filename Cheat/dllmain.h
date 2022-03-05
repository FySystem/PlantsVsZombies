#pragma once

class Global : public Singleton<Global>
{
public:
	HINSTANCE hDllInst;

	HWND hwndDlg;

	bool bShowWindow = true;// «∑Òœ‘ æ≤Àµ•

	//std::vector<uintptr_t> zombie;
	//std::SingleElemVector<uintptr_t> Zombie;
	
};