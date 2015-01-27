// SensitivityTest.cpp : 实现文件
//

#include "stdafx.h"
#include "RSSPCryptool.h"
#include "SensitivityTest.h"
#include "afxdialogex.h"


// SensitivityTest 对话框

IMPLEMENT_DYNAMIC(SensitivityTest, CDialogEx)

SensitivityTest::SensitivityTest(CWnd* pParent /*=NULL*/)
	: CDialogEx(SensitivityTest::IDD, pParent)
{

}

SensitivityTest::~SensitivityTest()
{
}

void SensitivityTest::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(SensitivityTest, CDialogEx)
END_MESSAGE_MAP()


// SensitivityTest 消息处理程序
