
// RSSPCryptoolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RSSPCryptool.h"
#include "RSSPCryptoolDlg.h"
#include "afxdialogex.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);//DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CRSSPCryptoolDlg 对话框




CRSSPCryptoolDlg::CRSSPCryptoolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRSSPCryptoolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRSSPCryptoolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB, m_tab);
}

BEGIN_MESSAGE_MAP(CRSSPCryptoolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CRSSPCryptoolDlg::OnTcnSelchangeTab)
END_MESSAGE_MAP()


// CRSSPCryptoolDlg 消息处理程序

BOOL CRSSPCryptoolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	
	//初始化Tab Control控件
	m_tab.InsertItem(0, _T("算法管理"));
	m_tab.InsertItem(1, _T("雪崩效应测试"));
	m_tab.InsertItem(2, _T("NIST随机性测试"));

	m_manageDlg.Create(IDD_MANAGE,&m_tab);
	m_sensitivityTestDlg.Create(IDD_SENSITIVITY_TEST,&m_tab);
	m_nistTestDlg.Create(IDD_NIST_TEST,&m_tab);

	CRect tabRect;
	m_tab.GetClientRect(&tabRect);
	tabRect.top += 22;
	
	m_manageDlg.MoveWindow(tabRect.left,tabRect.top,tabRect.Width(),tabRect.Height());
	m_sensitivityTestDlg.MoveWindow(tabRect.left,tabRect.top,tabRect.Width(),tabRect.Height());
	m_nistTestDlg.MoveWindow(tabRect.left,tabRect.top,tabRect.Width(),tabRect.Height());

	m_manageDlg.ShowWindow(SW_SHOW);
	m_sensitivityTestDlg.ShowWindow(SW_HIDE);
	m_nistTestDlg.ShowWindow(SW_HIDE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CRSSPCryptoolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CRSSPCryptoolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CRSSPCryptoolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CRSSPCryptoolDlg::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	//Tab按钮切换时的操作
	*pResult = 0;
	switch(m_tab.GetCurSel()){
	case 0:
		m_manageDlg.ShowWindow(SW_SHOW);
		m_sensitivityTestDlg.ShowWindow(SW_HIDE);
		m_nistTestDlg.ShowWindow(SW_HIDE);
		break;
	case 1:
		m_manageDlg.ShowWindow(SW_HIDE);
		m_sensitivityTestDlg.ShowWindow(SW_SHOW);
		m_nistTestDlg.ShowWindow(SW_HIDE);
		break;
	case 2:
		m_manageDlg.ShowWindow(SW_HIDE);
		m_sensitivityTestDlg.ShowWindow(SW_HIDE);
		m_nistTestDlg.ShowWindow(SW_SHOW);
		break;
	}
}
