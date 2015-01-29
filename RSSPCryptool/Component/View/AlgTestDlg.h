#pragma once
#include "afxwin.h"


// AlgTestDlg 对话框

class AlgTestDlg : public CDialogEx
{
	DECLARE_DYNAMIC(AlgTestDlg)

public:
	AlgTestDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~AlgTestDlg();

// 对话框数据
	enum { IDD = IDD_ALG_TEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CComboBox cmb_type;
	CComboBox cmb_name;
	CString cstr_input;
	CString cstr_key;
	CString cstr_enc;
	CString cstr_dec;
public:
	afx_msg void OnBnClickedEnc();
	afx_msg void OnBnClickedDec();
	afx_msg void OnCbnDropdownComboType();
	afx_msg void OnCbnDropdownComboName();
};
