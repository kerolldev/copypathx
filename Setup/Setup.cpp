// Setup.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "Setup.h"

int setup(LPCTSTR psz, bool bUnreg);

int APIENTRY WinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	int ret = MessageBox(NULL, MSG_SETUP, "セットアップ", MB_YESNOCANCEL | MB_ICONQUESTION);
	switch(ret) {
		case IDYES:
			ret = setup(DLLNAME, false);
			break;
		case IDNO:
			ret = setup(DLLNAME, true);
			break;
		case IDCANCEL:
			break;
	}
	return ret;
}


int setup(LPCTSTR psz, bool bUnreg)
{
	int   iFail = FALSE;

	// Because we load the DLL server into our own (ie, REGISTER.EXE)
	// process space, call to initialize the COM Library.  Use the
	// SUCCEEDED macro to detect success.  If fail then exit app
	// with error message.
	if (SUCCEEDED(CoInitialize(NULL)))
	{
		HINSTANCE hMod;

		// Load the Server DLL into our process space.
		hMod = LoadLibraryA(psz);

		if (NULL != hMod)
		{
			HRESULT (STDAPICALLTYPE *pfn)(void);

			if (bUnreg)
			{
				(FARPROC&)pfn = GetProcAddress(hMod, "DllUnregisterServer");

				if (NULL != pfn) {
					iFail = FAILED((*pfn)());
				}

				if (iFail) {
					MessageBox(NULL, "DLLのレジストリ削除に失敗しました。", "セットアップ", MB_OK | MB_ICONERROR);
				}

				else
				{
					// Show a timed message box indicating registration success.
					MessageBox(NULL, MSG_SUCCESS, "セットアップ", MB_OK | MB_ICONINFORMATION);
				}
			}
			else
			{
				(FARPROC&)pfn = GetProcAddress(hMod, "DllRegisterServer");

				if (NULL != pfn) {
					iFail = FAILED((*pfn)());
				}
				if (iFail) {
					MessageBox(NULL, "DLLのレジストリ登録に失敗しました。", "セットアップ", MB_OK | MB_ICONERROR);
				}
				else
				{
					// Show a timed message box indicating registration success.
					MessageBox(NULL, MSG_SUCCESS, "セットアップ", MB_OK | MB_ICONINFORMATION);
				}
			}
			CoFreeLibrary(hMod);
		}
		else {
			MessageBox(NULL, "DLLが読み込めませんでした。", "セットアップ", MB_OK | MB_ICONERROR);
		}

		// We're exiting this app so shut down the COM Library.
		CoUninitialize();
	}
	else {
		MessageBox(NULL, "COM初期化に失敗しました。", "セットアップ", MB_OK | MB_ICONERROR);
	}

	return iFail;

}