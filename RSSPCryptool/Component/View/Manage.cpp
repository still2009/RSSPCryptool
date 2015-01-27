// Manage.cpp : 实现文件
//

#include "stdafx.h"
#include "RSSPCryptool.h"
#include "Manage.h"
#include "afxdialogex.h"


// Manage 对话框

IMPLEMENT_DYNAMIC(Manage, CDialogEx)

Manage::Manage(CWnd* pParent /*=NULL*/)
	: CDialogEx(Manage::IDD, pParent)
{

}

Manage::~Manage()
{
}

void Manage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Manage, CDialogEx)
END_MESSAGE_MAP()


// Manage 消息处理程序
