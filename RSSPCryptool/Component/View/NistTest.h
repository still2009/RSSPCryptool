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
	afx_msg void OnBnClickedRngGenerate();
	CButton m_StartEvaluation;
	CButton m_ShowResult;
	afx_msg void OnBnClickedStart();
	CButton m_BlockFrequency;
	CButton m_Overlapping;
	CButton m_NonOverlapping;
	CButton m_Approximate;
	CButton m_Linear;
	CButton m_Serial;
	CButton m_Frequency;
	CButton m_LongestRun;
	CButton m_Cumulative;
	CButton m_DFT;
	CButton m_RandomExcursions;
	CButton m_Runs;
	CButton m_Rank;
	CButton m_RandomExcursionsVariant;
	CButton m_Universal;
	void ChooseTest();
	void SetParameter();
	void StartProcess();
	void ResultFile();
	afx_msg void OnBnClickedShowResult();
};
