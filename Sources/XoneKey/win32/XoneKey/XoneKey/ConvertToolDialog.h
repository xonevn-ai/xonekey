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

class ConvertToolDialog : public BaseDialog {
private:
	HWND checkAllCaps, checkNonAllCaps, checkRemoveMark, checkCapsEarchWord, checkCapsFirstLetter, checkAlertWhenCompleted,
		checkConvertClipboard, checkCtrl, checkAlt, checkWin, checkShift, checkKey;
	HWND comboboxSource, comboboxDest;
private:
	void onComboBoxSelected(const HWND& hCombobox, const int& comboboxId);
	void onCheckboxClicked(const HWND& hWnd);
	void onCharacter(const HWND& hWnd, const UINT16& keyCode);
	void setSwitchKeyText(const HWND & hWnd, const UINT16 & keyCode);
	void setLogic(const HWND& exceptWnd);
protected:
	INT_PTR eventProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void initDialog();
public:
	ConvertToolDialog(const HINSTANCE & hInstance, const int & resourceId);
	~ConvertToolDialog();
	virtual void fillData() override;
public: //event
	void onSwitchButton();
	void onConvertButton();
};

