// NistTest.cpp : 实现文件
//

#include "stdafx.h"
#include "RSSPCryptool.h"
#include "NistTest.h"
#include "afxdialogex.h"


// NistTest 对话框

IMPLEMENT_DYNAMIC(NistTest, CDialog)

NistTest::NistTest(CWnd* pParent /*=NULL*/)
	: CDialog(NistTest::IDD, pParent)
{

}

NistTest::~NistTest()
{
}

void NistTest::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(NistTest, CDialog)
END_MESSAGE_MAP()


// NistTest 消息处理程序
