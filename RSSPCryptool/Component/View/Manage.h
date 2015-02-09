#pragma once
#include "afxwin.h"
#include "Component/DllApi/AlgDllApi.h"
#include "afxcmn.h"

// Manage 对话框

class Manage : public CDialogEx
{
	DECLARE_DYNAMIC(Manage)

public:
	Manage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Manage();

// 对话框数据
	enum { IDD = IDD_MANAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	AlgMng * mng;
	CComboBox cmb_AlgType;
	CComboBox cmb_AlgName;
	CComboBox cmb_AlgMode;
	CString curAlgType;
	CString curAlgName;
	CString curAlgMode;
	CStatic sts_CurAlg;
	CListCtrl lst_AlgInfo;
	CString strDllName;

	void initCmbAlgType();
	void initCmbAlgMode();
	void initListControl();
	void showCmbAlgName(int mode);
	void showCurInfo();
	void showAlgInfoLst(int type);

	static int modeStr2Int(CString strMode);
	static int typeStr2Int(CString strType);
	static CString int2CString(int num);

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnSwitch();
	afx_msg void OnCbnSelchangeAlgType();
	afx_msg void OnBnClickedAddDll();
};
