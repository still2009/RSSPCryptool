// Manage.cpp : 实现文件
//

#include "stdafx.h"
#include "RSSPCryptool.h"
#include "Manage.h"
#include "afxdialogex.h"

//控制控件内容显示函数
void Manage::initCmbAlgType(){
	cmb_AlgType.InsertString(0,"分组密码算法");
	cmb_AlgType.InsertString(1,"流密码算法");
	cmb_AlgType.InsertString(2,"HASH算法");
	cmb_AlgType.InsertString(3,"MAC算法");
	cmb_AlgType.InsertString(4,"随机数生成器");
	cmb_AlgType.SetCurSel(0);
}
void Manage::initCmbAlgMode(){
	cmb_AlgMode.InsertString(0,"ECB");
	cmb_AlgMode.InsertString(1,"CBC");
	cmb_AlgMode.InsertString(2,"CFB");
	cmb_AlgMode.InsertString(3,"OFB");
	cmb_AlgMode.InsertString(4,"CTR");
	cmb_AlgMode.SetCurSel(0);
}
void Manage::initListControl(){
	
}
void Manage::showCmbAlgName(int mode){
	//首先清空之前的字符串
	cmb_AlgName.ResetContent();
	
	//开始读取指定算法类型的算法名称
	int index = 0;
	for(int i = 0;i < mng->info->GetSize();i++){
		if(mng->info->GetAt(i)->AlgType == mode){
			cmb_AlgName.InsertString(index,mng->info->GetAt(i)->AlgName);
			index++;
		}
	}
	cmb_AlgName.SetCurSel(0);
}
void Manage::showCurInfo(){
	CString strCfg = "分组密码算法:";
	strCfg.Append(mng->GetCurrCfg(CURR_BLOCK));
	strCfg.AppendChar(',');
	strCfg.Append("密码模式:");
	strCfg.Append(mng->GetCurrCfg(CURR_MODE));
	strCfg.AppendChar(',');
	strCfg.Append("流密码算法:");
	strCfg.Append(mng->GetCurrCfg(CURR_STREAM));
	strCfg.AppendChar(',');
	strCfg.Append("HASH算法:");
	strCfg.Append(mng->GetCurrCfg(CURR_HASH));
	strCfg.AppendChar(',');
	strCfg.Append("MAC算法:");
	strCfg.Append(mng->GetCurrCfg(CURR_MAC));
	strCfg.AppendChar(',');
	strCfg.Append("随机数生成器:");
	strCfg.Append(mng->GetCurrCfg(CURR_RNG));
	strCfg.AppendChar('.');
	sts_CurAlg.SetWindowTextA(strCfg);
}

//其他函数
int Manage::modeStr2Int(CString strMode){
	if(strMode.Compare("ECB") == 0) return ECB;
	else if(strMode.Compare("CBC") == 0) return CBC;
	else if(strMode.Compare("CFB") == 0) return CFB;
	else if(strMode.Compare("OFB") == 0) return OFB;
	else if(strMode.Compare("CTR") == 0) return CTR;
	else return -1;
}
int Manage::typeStr2Int(CString strType){
	if(strType.Compare("分组密码算法") == 0) return BLOCK;
	else if(strType.Compare("流密码算法") == 0) return STREAM;
	else if(strType.Compare("HASH算法") == 0) return HASH;
	else if(strType.Compare("MAC算法") == 0) return MAC;
	else if(strType.Compare("随机数生成器") == 0) return RNG;
	else return -1;
}


// Manage 对话框
IMPLEMENT_DYNAMIC(Manage, CDialogEx)
Manage::Manage(CWnd* pParent /*=NULL*/)
	: CDialogEx(Manage::IDD, pParent)
	, curAlgType(_T(""))
	, curAlgName(_T(""))
	, curAlgMode(_T(""))
{
	mng = new AlgMng(&dllMng);
}
Manage::~Manage()
{
}
void Manage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ALG_TYPE, cmb_AlgType);
	DDX_Control(pDX, IDC_ALG_NAME, cmb_AlgName);
	DDX_Control(pDX, IDC_ALG_MODE, cmb_AlgMode);
	DDX_Control(pDX, IDC_CURR_ALG, sts_CurAlg);
}


BEGIN_MESSAGE_MAP(Manage, CDialogEx)
	ON_CBN_SELCHANGE(IDC_ALG_TYPE, &Manage::OnCbnSelchangeAlgType)
	ON_BN_CLICKED(IDC_BTN_SWITCH, &Manage::OnBnClickedBtnSwitch)
END_MESSAGE_MAP()

//初始化各个控件的状态
BOOL Manage::OnInitDialog(){
	CDialogEx::OnInitDialog();

	//算法加载与移除部分界面
	initCmbAlgType();
	initCmbAlgMode();
	showCmbAlgName(BLOCK);
	showCurInfo();
	return TRUE;
}
void Manage::OnCbnSelchangeAlgType(){
	UpdateData();
	int sel = cmb_AlgType.GetCurSel();
	//算法类型不是“分组密码算法时 不显示“密码模式”下拉条
	if(sel != 0){
		GetDlgItem(IDC_STATIC_MODE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_ALG_MODE)->ShowWindow(SW_HIDE);
	}else{
		GetDlgItem(IDC_STATIC_MODE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_ALG_MODE)->ShowWindow(SW_SHOW);
	}

	//填充AlgName下拉框
	CString selStr;
	cmb_AlgType.GetLBText(cmb_AlgType.GetCurSel(),selStr);
	showCmbAlgName(typeStr2Int(selStr));
}
void Manage::OnBnClickedBtnSwitch(){
	//获取当前选定信息
	cmb_AlgType.GetLBText(cmb_AlgType.GetCurSel(),curAlgType);
	cmb_AlgName.GetLBText(cmb_AlgName.GetCurSel(),curAlgName);
	cmb_AlgMode.GetLBText(cmb_AlgMode.GetCurSel(),curAlgMode);
	int curr_type = typeStr2Int(curAlgType);
	//确认是否要切换
	CString tip = "确定要切换到算法";
	tip.Append(curAlgName);
	if(curr_type == BLOCK){
		tip.Append("并且使用");
		tip.Append(curAlgMode);
		tip.Append("模式");
	}
	tip.Append("吗？");
	int responce = MessageBox(tip,"算法切换确认",MB_OKCANCEL);
	if(responce != IDOK) return;

	//修改当前算法信息
	mng->SetCurrCfg(curr_type,curAlgName);
	if(curr_type == BLOCK) mng->SetCurrCfg(CURR_MODE,curAlgMode);
	
	//更新“当前使用算法”界面区域内容
	showCurInfo();
	//提示切换成功
	MessageBox("算法切换成功","切换成功提示",MB_OK);
}
