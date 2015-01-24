#pragma once


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
};
