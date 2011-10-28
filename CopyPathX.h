#pragma once

#include "_CopyPathX_i.c"

// ���̃��W���[�������́ADllMain�ADllRegisterServer ����� DllUnregisterServer �������I�Ɏ������܂��B
[ module(dll, uuid = "{D45236A6-A91C-4539-AE9F-29BD86B5A2D6}", 
		 name = "CopyPathX", 
		 helpstring = "CopyPathX 1.0 �^�C�v ���C�u����",
		 resource_name = "IDR_COPYPATHX") ]
class CMyModule
{
public :

	// ���W�X�g���ݒ�
	HRESULT DllRegisterServer(BOOL bRegTypeLib = TRUE )
	{
		_TCHAR		strCLSID[50];
		OLECHAR		strWideCLSID[50];
		CRegKey		key;
		USES_CONVERSION;

		// �I�u�W�F�N�g�A�^�C�v ���C�u��������у^�C�v ���C�u�������̑S�ẴC���^�[�t�F�C�X��o�^���܂�
		HRESULT hr = __super::DllRegisterServer(bRegTypeLib);
		if (SUCCEEDED(hr)) {
			if (::StringFromGUID2(CLSID_CCopyPathContextMenu, strWideCLSID, 50) > 0) {
				_tcscpy(strCLSID, OLE2CT(strWideCLSID));
				hr = key.SetValue(HKEY_CLASSES_ROOT, _T("*\\shellex\\ContextMenuHandlers\\CopyPathX\\"), strCLSID);
				hr = key.SetValue(HKEY_CLASSES_ROOT, _T("directory\\shellex\\ContextMenuHandlers\\CopyPathX\\"), strCLSID);
				hr = key.SetValue(HKEY_CLASSES_ROOT, _T("drive\\shellex\\ContextMenuHandlers\\CopyPathX\\"), strCLSID);
			}
		}
		return hr;

	}


	// DllUnregisterServer - �G���g�������W�X�g������폜���܂��B
	HRESULT DllUnregisterServer(BOOL bUnRegTypeLib = TRUE )
	{
		HRESULT hr = __super::DllUnregisterServer(bUnRegTypeLib);

		CRegKey		key;

		if (SUCCEEDED(hr)) {
			if (key.Open(HKEY_CLASSES_ROOT, _T("*\\shellex\\ContextMenuHandlers\\")) == ERROR_SUCCESS) {
				hr = key.DeleteValue(NULL);
				if (hr != ERROR_SUCCESS && hr != ERROR_FILE_NOT_FOUND)
					return hr;
				hr = key.DeleteSubKey(_T("CopyPathX"));
				if (hr != ERROR_SUCCESS && hr != ERROR_FILE_NOT_FOUND)
					return hr;
			}
			if (key.Open(HKEY_CLASSES_ROOT, _T("directory\\shellex\\ContextMenuHandlers\\")) == ERROR_SUCCESS) {
				hr = key.DeleteSubKey(_T("CopyPathX"));
				if (hr != ERROR_SUCCESS && hr != ERROR_FILE_NOT_FOUND)
					return hr;
			}
			if (key.Open(HKEY_CLASSES_ROOT, _T("drive\\shellex\\ContextMenuHandlers\\")) == ERROR_SUCCESS) {
				hr = key.DeleteSubKey(_T("CopyPathX"));
				if (hr != ERROR_SUCCESS && hr != ERROR_FILE_NOT_FOUND)
					return hr;
			}
		}


		return hr;
	}


};
