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
	CRect rect;
	lst_AlgInfo.GetClientRect(&rect);

	// 为列表视图控件添加全行选中和栅格风格
	lst_AlgInfo.SetExtendedStyle(lst_AlgInfo.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	//插入列
	lst_AlgInfo.InsertColumn(0, _T("算法类型"), LVCFMT_CENTER, rect.Width()*5.5/36,0);
	lst_AlgInfo.InsertColumn(1, _T("算法名称"), LVCFMT_CENTER, rect.Width()*5.5/36,0);
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
void Manage::showAlgInfoLst(int type){
	//删除可选列
	int columns = lst_AlgInfo.GetHeaderCtrl()->GetItemCount();
	//注意从右边的列向左边删
	for(int i = columns-1;i >= 2;i--){
		lst_AlgInfo.DeleteColumn(i);
	}
	//删除所有数据条目
	lst_AlgInfo.DeleteAllItems();
	CRect rect;
	lst_AlgInfo.GetClientRect(&rect);
	//添加相关列与数据条目
	int row = 0;
	Cipher * alg;
	switch(type){
	case BLOCK:
		lst_AlgInfo.InsertColumn(2,_T("密钥长度/bit"),LVCFMT_CENTER,rect.Width()*5/36,0);
		lst_AlgInfo.InsertColumn(3,_T("分组长度/bit"),LVCFMT_CENTER,rect.Width()*5/36,0);
		for(int i = 0;i < mng->info->GetSize();i++){
			alg = mng->info->GetAt(i);
			if(alg->AlgType == BLOCK){
				lst_AlgInfo.InsertItem(row,NULL);
				lst_AlgInfo.SetItemText(row,0,"分组密码算法");
				lst_AlgInfo.SetItemText(row,1,alg->AlgName);
				lst_AlgInfo.SetItemText(row,2,int2CString(8*alg->key_len));
				lst_AlgInfo.SetItemText(row,3,int2CString(8*alg->block_size));
				row++;
			}
		}
		break;
	case STREAM:
		lst_AlgInfo.InsertColumn(2,_T("密钥长度/bit"),LVCFMT_CENTER,rect.Width()*5/36,0);
		lst_AlgInfo.InsertColumn(3,_T("初始向量长度/bit"),LVCFMT_CENTER,rect.Width()*7/36,0);
		for(int i = 0;i < mng->info->GetSize();i++){
			alg = mng->info->GetAt(i);
			if(alg->AlgType == STREAM){
				lst_AlgInfo.InsertItem(row,NULL);
				lst_AlgInfo.SetItemText(row,0,"流密码算法");
				lst_AlgInfo.SetItemText(row,1,alg->AlgName);
				lst_AlgInfo.SetItemText(row,2,int2CString(8*alg->key_len));
				lst_AlgInfo.SetItemText(row,3,int2CString(8*alg->iv_len));
				row++;
			}
		}
		break;
	case HASH:
		lst_AlgInfo.InsertColumn(2,_T("摘要长度/bit"),LVCFMT_CENTER,rect.Width()*5/36, 0);
		for(int i = 0;i < mng->info->GetSize();i++){
			alg = mng->info->GetAt(i);
			if(alg->AlgType == HASH){
				lst_AlgInfo.InsertItem(row,NULL);
				lst_AlgInfo.SetItemText(row,0,"HASH算法");
				lst_AlgInfo.SetItemText(row,1,alg->AlgName);
				lst_AlgInfo.SetItemText(row,2,int2CString(8*alg->output_len));
				row++;
			}
		}
		break;
	case MAC:
		lst_AlgInfo.InsertColumn(2,_T("密钥长度/bit"),LVCFMT_CENTER,rect.Width()*5/36,0);
		lst_AlgInfo.InsertColumn(3,_T("消息认证码长度/bit"),LVCFMT_CENTER,rect.Width()*8/36,0);
		for(int i = 0;i < mng->info->GetSize();i++){
			alg = mng->info->GetAt(i);
			if(alg->AlgType == MAC){
				lst_AlgInfo.InsertItem(row,NULL);
				lst_AlgInfo.SetItemText(row,0,"MAC算法");
				lst_AlgInfo.SetItemText(row,1,alg->AlgName);
				lst_AlgInfo.SetItemText(row,2,int2CString(8*alg->key_len));
				lst_AlgInfo.SetItemText(row,3,int2CString(8*alg->output_len));
				row++;
			}
		}
		break;
	case RNG:
		lst_AlgInfo.InsertColumn(2,_T("种子长度/bit"),LVCFMT_CENTER,rect.Width()*5/36, 0);
		for(int i = 0;i < mng->info->GetSize();i++){
			alg = mng->info->GetAt(i);
			if(alg->AlgType == RNG){
				lst_AlgInfo.InsertItem(row,NULL);
				lst_AlgInfo.SetItemText(row,0,"随机数生成器");
				lst_AlgInfo.SetItemText(row,1,alg->AlgName);
				lst_AlgInfo.SetItemText(row,2,int2CString(8*alg->seed_len));
				row++;
			}
		}
		break;
	default:;
	}
}

//工具函数
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
CString Manage::int2CString(int num){
	CString str;
	str.Format("%d",num);
	return str;
}

// Manage 对话框
IMPLEMENT_DYNAMIC(Manage, CDialogEx)
Manage::Manage(CWnd* pParent /*=NULL*/)
	: CDialogEx(Manage::IDD, pParent)
	, curAlgType(_T(""))
	, curAlgName(_T(""))
	, curAlgMode(_T(""))
	, strDllName(_T(""))
{
	mng = new AlgMng(&dllMng);
}
Manage::~Manage()
{
}
void Manage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ALG_TYPE_MNG, cmb_AlgType);
	DDX_Control(pDX, IDC_ALG_NAME_MNG, cmb_AlgName);
	DDX_Control(pDX, IDC_ALG_MODE_MNG, cmb_AlgMode);
	DDX_Control(pDX, IDC_CURR_ALG, sts_CurAlg);
	DDX_Control(pDX, IDC_LST_INFO, lst_AlgInfo);
	DDX_Text(pDX, IDC_FILE_BROSWER, strDllName);
}


BEGIN_MESSAGE_MAP(Manage, CDialogEx)
	ON_CBN_SELCHANGE(IDC_ALG_TYPE_MNG, &Manage::OnCbnSelchangeAlgType)
	ON_BN_CLICKED(IDC_BTN_SWITCH, &Manage::OnBnClickedBtnSwitch)
	ON_BN_CLICKED(IDC_ADD_DLL, &Manage::OnBnClickedAddDll)
END_MESSAGE_MAP()

//初始化各个控件的状态
BOOL Manage::OnInitDialog(){
	CDialogEx::OnInitDialog();

	//算法加载与移除部分界面
	initCmbAlgType();
	initCmbAlgMode();
	showCmbAlgName(BLOCK);
	showCurInfo();
	initListControl();
	showAlgInfoLst(BLOCK);
	return TRUE;
}
void Manage::OnCbnSelchangeAlgType(){
	UpdateData();
	int sel = cmb_AlgType.GetCurSel();
	//算法类型不是“分组密码算法时 不显示“密码模式”下拉条
	if(sel != 0){
		GetDlgItem(IDC_STATIC_MODE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_ALG_MODE_MNG)->ShowWindow(SW_HIDE);
	}else{
		GetDlgItem(IDC_STATIC_MODE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_ALG_MODE_MNG)->ShowWindow(SW_SHOW);
	}

	//填充AlgName下拉框
	CString selStr;
	cmb_AlgType.GetLBText(cmb_AlgType.GetCurSel(),selStr);
	showCmbAlgName(typeStr2Int(selStr));
	//显示该类型所有算法的信息
	showAlgInfoLst(typeStr2Int(selStr));
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
void Manage::OnBnClickedAddDll(){
	UpdateData();
	if(dllMng.checkDll(strDllName)){
		if(dllMng.addDll(strDllName)){
			dllMng.FreeDlls();
			dllMng.LoadDlls();
			dllMng.getAllCipher();
			OnCbnSelchangeAlgType();
			MessageBox("算法Dll文件添加成功","添加成功提示",MB_OK);
		}else{
			MessageBox("算法Dll文件可能已经存在或者程序无法访问该文件","DLL文件复制失败",MB_OK);
		}
	}else{
		MessageBox("算法Dll文件不符合格式","DLL文件格式错误",MB_OK);
	}

}
