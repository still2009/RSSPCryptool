// DllDirSetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RSSPCryptool.h"
#include "DllDirSetDlg.h"
#include "afxdialogex.h"
#include "Component\DllApi\AlgDllApi.h"


// DllDirSetDlg 对话框

IMPLEMENT_DYNAMIC(DllDirSetDlg, CDialogEx)

DllDirSetDlg::DllDirSetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(DllDirSetDlg::IDD, pParent)
	, strDirPath(_T(""))
{

}

DllDirSetDlg::~DllDirSetDlg()
{
}

void DllDirSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DIR_SET_BROSWER, eb_DllDirSel);
	DDX_Text(pDX, IDC_DIR_SET_BROSWER, strDirPath);
}


BEGIN_MESSAGE_MAP(DllDirSetDlg, CDialogEx)
	ON_BN_CLICKED(IDOK_DIRSET, &DllDirSetDlg::OnBnClickedDirset)
	ON_BN_CLICKED(ID_CANCEL_DIRSET, &DllDirSetDlg::OnBnClickedCancelDirset)
END_MESSAGE_MAP()


// DllDirSetDlg 消息处理程序
void DllDirSetDlg::OnBnClickedDirset(){
	UpdateData();
	strDirPath.AppendChar('\\');
	int resp = MessageBox("确定要将目录" + strDirPath + "作为算法DLL目录吗？","目录切换确认",MB_OKCANCEL);
	if(resp == IDCANCEL) return;
	dllMng.cfgList.SetAt(DLL_DIR,strDirPath);
	dllMng.UpdateCfgFile();
	MessageBox("目录切换成功,将重新加载算法DLL","设置成功提示",MB_OK);
	dllMng.FreeDlls();
	dllMng.LoadDlls();
	dllMng.getAllCipher();
	mngDlg->OnCbnSelchangeAlgType();
}

void DllDirSetDlg::OnBnClickedCancelDirset(){
	SendMessage(WM_CLOSE);
}


BOOL DllDirSetDlg::OnInitDialog(){
	CDialogEx::OnInitDialog();
	eb_DllDirSel.EnableFolderBrowseButton();
	

	return TRUE;
}
