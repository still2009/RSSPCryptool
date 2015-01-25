#pragma once


// NistTest 对话框

class NistTest : public CDialog
{
	DECLARE_DYNAMIC(NistTest)

public:
	NistTest(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~NistTest();

// 对话框数据
	enum { IDD = IDD_NIST_TEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
