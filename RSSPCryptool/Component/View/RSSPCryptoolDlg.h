
// RSSPCryptoolDlg.h : 头文件
//


#pragma once
#include "afxcmn.h"
#include "Manage.h"
#include "AlgTestDlg.h"
#include "SensitivityTest.h"
#include "NistTest.h"

// CRSSPCryptoolDlg 对话框
class CRSSPCryptoolDlg : public CDialogEx
{
// 构造
public:
	CRSSPCryptoolDlg(CWnd* pParent = NULL);	// 标准构造函数
	Manage m_manageDlg;
	AlgTestDlg m_algTestDlg;
	SensitivityTest m_sensitivityTestDlg;
	NistTest m_nistTestDlg;

// 对话框数据
	enum { IDD = IDD_RSSPCRYPTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_tab;
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnAboutMenu();
	afx_msg void OnDirSet();
};
