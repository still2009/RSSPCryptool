// SensitivityTest.cpp : 实现文件
//

#include "stdafx.h"
#include "RSSPCryptool.h"
#include "SensitivityTest.h"
#include "afxdialogex.h"
#include "Component\DllApi\AlgDllApi.h"


// SensitivityTest 对话框

IMPLEMENT_DYNAMIC(SensitivityTest, CDialogEx)

SensitivityTest::SensitivityTest(CWnd* pParent /*=NULL*/)
	: CDialogEx(SensitivityTest::IDD, pParent)
	, m_AlgType(_T(""))
	, m_AlgName(_T(""))
	, m_AlgMode(_T(""))
	, m_TestType(_T(""))
	, m_TextLen(0)
	, m_TestTimes(0)
{

}

SensitivityTest::~SensitivityTest()
{
}

void SensitivityTest::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_ALG_TYPE, m_AlgType);
	DDX_CBString(pDX, IDC_ALG_NAME, m_AlgName);
	DDX_CBString(pDX, IDC_ALG_MODE, m_AlgMode);
	DDX_CBString(pDX, IDC_TEST_TYPE, m_TestType);
	DDX_Text(pDX, IDC_TEXT_LEN, m_TextLen);
	DDX_Text(pDX, IDC_TEST_TIMES, m_TestTimes);
	DDX_Control(pDX, IDC_TEST, m_Test);
	DDX_Control(pDX, IDC_REPORT, m_Report);
	DDX_Control(pDX, IDC_ALG_NAME, m_Name);
	DDX_Control(pDX, IDC_ALG_TYPE, m_Type);
}


BEGIN_MESSAGE_MAP(SensitivityTest, CDialogEx)
	ON_CBN_SELCHANGE(IDC_ALG_TYPE, &SensitivityTest::OnCbnSelchangeAlgType)
END_MESSAGE_MAP()


// SensitivityTest 消息处理程序




void SensitivityTest::OnCbnSelchangeAlgType()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if(m_AlgType.Compare("分组密码"))  //相等等于0 不等等于1
	{
		GetDlgItem(IDC_ALG_MODE)->EnableWindow(FALSE);//变灰
		GetDlgItem(IDC_STATIC_MODE)->EnableWindow(FALSE);//变灰	
	}
	else 
	{
		GetDlgItem(IDC_ALG_MODE)->EnableWindow(TRUE);//激活
		GetDlgItem(IDC_STATIC_MODE)->EnableWindow(TRUE);//激活
	}
	m_Name.ResetContent();
	AlgMng mng(&dllMng);
	for(int i = 0;i < mng.info->GetSize();i++)
	{
		if(mng.info->GetAt(i)->AlgType==(m_Type.GetCurSel()+1))
		m_Name.AddString(mng.info->GetAt(i)->AlgName);
	}
}

