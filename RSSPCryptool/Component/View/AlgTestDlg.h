#pragma once
#include "afxwin.h"
#include "Component\DllApi\AlgDllApi.h"
#include "Manage.h"

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
	AlgMng * rngAlg;
	AlgMng * cipherAlg;
	CComboBox cmb_type;
	CComboBox cmb_name;
	CComboBox cmb_mode;
	CString cstr_input;
	CString cstr_key;
	CString cstr_enc;
	CString cstr_dec;
	CString cstr_iv;
	CString cur_alg_type;
	CString cur_alg_mode;
	CString cur_alg_name;
public:
	afx_msg void OnBnClickedEnc();
	afx_msg void OnBnClickedDec();
	afx_msg void OnCbnDropdownComboType();
	afx_msg void OnCbnDropdownComboName();
	afx_msg void OnBnClickedBtnGenplain();
	afx_msg void OnBnClickedBtnGenkey();
	afx_msg void OnBnClickedBtnGeniv();
	afx_msg void OnCbnSelchangeComboType();

	void initCmbAlgType();
	void initCmbAlgMode();
	void showCmbAlgName(int type);
	byte * getRandBytes(CString algName,int size);
	virtual BOOL OnInitDialog();
};
