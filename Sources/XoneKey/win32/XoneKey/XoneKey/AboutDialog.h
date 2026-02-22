/*----------------------------------------------------------
XoneKey - The Cross platform Open source Vietnamese Keyboard application.

Copyright (C) 2025 Daniel Do
Contact: diep@xone.vn
Github: https://github.com/vituocgia/XoneKey
Fanpage: https://www.facebook.com/XoneKeyVN

This file is belong to the XoneKey project, Win32 version
which is released under GPL license.
You can fork, modify, improve this program. If you
redistribute your new version, it MUST be open source.
-----------------------------------------------------------*/
#pragma once
#include "BaseDialog.h"

class AboutDialog :	public BaseDialog {
protected:
	INT_PTR eventProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void initDialog();
public:
	AboutDialog(const HINSTANCE & hInstance, const int & resourceId);
	~AboutDialog();
	virtual void fillData() override;
	// Handle WM_USER+1001 messages for update check
    void handleUpdateCheckResult(WPARAM wParam, LPARAM lParam);
private:
	HWND hUpdateButton;
	void onUpdateButton();
};