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
	, m_NumOfRNG(0)
	, m_LenOfRNG(0)
	, m_blockFrequencyBlockLength(0)
	, m_overlappingTemplateBlockLength(0)
	, m_nonOverlappingTemplateBlockLength(0)
	, m_approximateEntropyBlockLength(0)
	, m_linearComplexitySequenceLength(0)
	, m_serialBlockLength(0)
{

}

NistTest::~NistTest()
{
}

void NistTest::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NUM_OF_RNG, m_NumOfRNG);
	DDX_Text(pDX, IDC_LEN_OF_RNG, m_LenOfRNG);
	DDX_Control(pDX, IDC_RNG_GENERATE, m_RNGGenerate);
	DDX_Text(pDX, IDC_BLOCK_FREQUENCY_BLOCKLENGTH, m_blockFrequencyBlockLength);
	DDX_Text(pDX, IDC_OVERLAPPING_LENGTH, m_overlappingTemplateBlockLength);
	DDX_Text(pDX, IDC_NONOVERLAPPING_LENGTH, m_nonOverlappingTemplateBlockLength);
	DDX_Text(pDX, IDC_APPROXIMATE_LENGTH, m_approximateEntropyBlockLength);
	DDX_Text(pDX, IDC_LINEAR_LENGTH, m_linearComplexitySequenceLength);
	DDX_Text(pDX, IDC_SERIAL_LENGTH, m_serialBlockLength);
}


BEGIN_MESSAGE_MAP(NistTest, CDialog)
END_MESSAGE_MAP()


// NistTest 消息处理程序


BOOL NistTest::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	
	//默认参数初始化
	m_blockFrequencyBlockLength=128;
	m_overlappingTemplateBlockLength=9;
	m_nonOverlappingTemplateBlockLength=9;
	m_approximateEntropyBlockLength=10;
	m_linearComplexitySequenceLength=500;
	m_serialBlockLength=16;
	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
