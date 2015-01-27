#pragma once


// SensitivityTest 对话框

class SensitivityTest : public CDialogEx
{
	DECLARE_DYNAMIC(SensitivityTest)

public:
	SensitivityTest(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SensitivityTest();

// 对话框数据
	enum { IDD = IDD_SENSITIVITY_TEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
