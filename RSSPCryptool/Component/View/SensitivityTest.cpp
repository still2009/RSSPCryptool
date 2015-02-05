// SensitivityTest.cpp : 实现文件
//

#include "stdafx.h"
#include "RSSPCryptool.h"
#include "SensitivityTest.h"
#include "afxdialogex.h"
#include "Component\DllApi\AlgDllApi.h"
#include "Component\Logic\Sensitivity.h"


// SensitivityTest 对话框

IMPLEMENT_DYNAMIC(SensitivityTest, CDialogEx)

SensitivityTest::SensitivityTest(CWnd* pParent /*=NULL*/)
	: CDialogEx(SensitivityTest::IDD, pParent)
	, m_AlgType(_T(""))
	, m_AlgName(_T(""))
	, m_AlgMode(_T(""))
	, m_TestType(_T(""))
	, m_TextLen(0)
	, m_TestTimes(0)
{

}

SensitivityTest::~SensitivityTest()
{
}

void SensitivityTest::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_ALG_TYPE, m_AlgType);
	DDX_CBString(pDX, IDC_ALG_NAME, m_AlgName);
	DDX_CBString(pDX, IDC_ALG_MODE, m_AlgMode);
	DDX_CBString(pDX, IDC_TEST_TYPE, m_TestType);
	DDX_Text(pDX, IDC_TEXT_LEN, m_TextLen);
	DDX_Text(pDX, IDC_TEST_TIMES, m_TestTimes);
	DDX_Control(pDX, IDC_TEST, m_Test);
	DDX_Control(pDX, IDC_REPORT, m_Report);
	DDX_Control(pDX, IDC_ALG_NAME, m_Name);
	DDX_Control(pDX, IDC_ALG_TYPE, m_Type);
	DDX_Control(pDX, IDC_TEST_TCHART, m_TestTChart);
}


BEGIN_MESSAGE_MAP(SensitivityTest, CDialogEx)
	ON_CBN_SELCHANGE(IDC_ALG_TYPE, &SensitivityTest::OnCbnSelchangeAlgType)
	ON_BN_CLICKED(IDC_TEST, &SensitivityTest::OnBnClickedTest)
	ON_BN_CLICKED(IDC_REPORT, &SensitivityTest::OnBnClickedReport)
END_MESSAGE_MAP()


// SensitivityTest 消息处理程序




void SensitivityTest::OnCbnSelchangeAlgType()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if(m_AlgType.Compare("分组密码"))  //相等等于0 不等等于1
	{
		GetDlgItem(IDC_ALG_MODE)->EnableWindow(FALSE);//变灰
		GetDlgItem(IDC_STATIC_MODE)->EnableWindow(FALSE);//变灰	
	}
	else 
	{
		GetDlgItem(IDC_ALG_MODE)->EnableWindow(TRUE);//激活
		GetDlgItem(IDC_STATIC_MODE)->EnableWindow(TRUE);//激活
	}
	m_Name.ResetContent();
	AlgMng mng(&dllMng);
	for(int i = 0;i < mng.info->GetSize();i++)
	{
		if(mng.info->GetAt(i)->AlgType==(m_Type.GetCurSel()+1))
		m_Name.AddString(mng.info->GetAt(i)->AlgName);
	}
}



void SensitivityTest::OnBnClickedTest()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if(!(m_AlgType&&m_AlgName&&m_AlgMode&&m_TestType&&m_TextLen&&m_TestTimes)) 
	{
		AfxMessageBox("参数未设置或不合法！");
		return;
	}
	averageData = new double[m_TestTimes];
	varianceData = new double[m_TestTimes];
	AverageData(m_AlgType,m_AlgName,m_AlgMode,m_TestType,m_TextLen,m_TestTimes,averageData);
	average=Average(averageData,m_TestTimes);
	VarianceData(averageData,varianceData,m_TestTimes);
	variance=Variance(varianceData,m_TestTimes);

	CSeries lineSeries1 = (CSeries)m_TestTChart.Series(0);
	CSeries lineSeries2 = (CSeries)m_TestTChart.Series(1);
	CSeries lineSeries3 = (CSeries)m_TestTChart.Series(2);
	lineSeries1.Clear();
	lineSeries2.Clear();
	lineSeries3.Clear();
	for(int i=0;i<m_TestTimes;i++)
    {
        lineSeries1.AddXY((double)i,averageData[i],NULL,0);
        lineSeries2.AddXY((double)i,varianceData[i],NULL,0);
        lineSeries3.AddXY((double)i,0.5,NULL,0);
    }

	lineSeries1.put_Color(RGB(5,187,92));  //均值为绿色
	lineSeries2.put_Color(RGB(0,0,255));  //方差为蓝色
	lineSeries3.put_Color(RGB(255,0,0));  //0.5基准线为红色
	CString Temp1,Temp2;
	Temp1.Format("%lf",average);
	Temp2.Format("%lf",variance);
	CDialog::GetDlgItem(IDC_RESULT_STATIC)->SetWindowText("Average --- "+ Temp1+ "    Variance ---"+ Temp2);
	AfxMessageBox("测试完成！");
	UpdateData(FALSE);
}


void SensitivityTest::OnBnClickedReport()
{
	// TODO: 在此添加控件通知处理程序代码
	//UpdateData(TRUE);
	GenerateReport(m_AlgType,m_AlgName,m_AlgMode,m_TestType,m_TextLen,m_TestTimes,averageData,average,variance);
	::WinExec("notepad SensitivityTestReport.txt",SW_SHOW);
}
