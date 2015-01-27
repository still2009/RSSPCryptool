#pragma once
#include "afxwin.h"


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
public:
	int m_NumOfRNG;
	int m_LenOfRNG;
	CButton m_RNGGenerate;
	int m_blockFrequencyBlockLength;
	int m_overlappingTemplateBlockLength;
	int m_nonOverlappingTemplateBlockLength;
	int m_approximateEntropyBlockLength;
	int m_linearComplexitySequenceLength;
	int m_serialBlockLength;
	virtual BOOL OnInitDialog();
};
