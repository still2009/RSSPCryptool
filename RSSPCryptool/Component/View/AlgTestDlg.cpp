// AlgTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RSSPCryptool.h"
#include "AlgTestDlg.h"
#include "afxdialogex.h"

// AlgTestDlg 对话框

IMPLEMENT_DYNAMIC(AlgTestDlg, CDialogEx)

AlgTestDlg::AlgTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(AlgTestDlg::IDD, pParent)
	, cstr_input(_T("明文"))
	, cstr_key(_T("密钥"))
	, cstr_enc(_T("加密后的密文"))
	, cstr_dec(_T("解密后的明文"))
{

}

AlgTestDlg::~AlgTestDlg()
{
}

void AlgTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_TYPE, cmb_type);
	DDX_Control(pDX, IDC_COMBO_NAME, cmb_name);
	DDX_Text(pDX, IDC_EDIT_INPUT, cstr_input);
	DDX_Text(pDX, IDC_EDIT_KEY, cstr_key);
	DDX_Text(pDX, IDC_EDIT_ENC, cstr_enc);
	DDX_Text(pDX, IDC_EDIT_DEC, cstr_dec);
}


BEGIN_MESSAGE_MAP(AlgTestDlg, CDialogEx)
	ON_BN_CLICKED(ID_ENC, &AlgTestDlg::OnBnClickedEnc)
	ON_BN_CLICKED(ID_DEC, &AlgTestDlg::OnBnClickedDec)
	ON_CBN_DROPDOWN(IDC_COMBO_TYPE, &AlgTestDlg::OnCbnDropdownComboType)
	ON_CBN_DROPDOWN(IDC_COMBO_NAME, &AlgTestDlg::OnCbnDropdownComboName)
END_MESSAGE_MAP()


// AlgTestDlg 消息处理程序


void AlgTestDlg::OnBnClickedEnc()
{
	
}


void AlgTestDlg::OnBnClickedDec()
{
	// TODO: 在此添加控件通知处理程序代码
}


void AlgTestDlg::OnCbnDropdownComboType()
{
	// TODO: 在此添加控件通知处理程序代码
}


void AlgTestDlg::OnCbnDropdownComboName()
{
	// TODO: 在此添加控件通知处理程序代码
}
