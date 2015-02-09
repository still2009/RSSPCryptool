// AlgTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RSSPCryptool.h"
#include "AlgTestDlg.h"
#include "afxdialogex.h"

//工具函数
byte * AlgTestDlg::getRandBytes(CString algName,int size){
	rngAlg->setAlg(RNG,algName);
	return rngAlg->RunCipher(time(0)*time(0),size);
}
void AlgTestDlg::initCmbAlgType(){
	cmb_type.InsertString(0,"分组密码算法");
	cmb_type.InsertString(1,"流密码算法");
	cmb_type.InsertString(2,"HASH算法");
	cmb_type.InsertString(3,"MAC算法");
	cmb_type.InsertString(4,"随机数生成器");
	cmb_type.SetCurSel(0);
}
void AlgTestDlg::initCmbAlgMode(){
	cmb_mode.InsertString(0,"ECB");
	cmb_mode.InsertString(1,"CBC");
	cmb_mode.InsertString(2,"CFB");
	cmb_mode.InsertString(3,"OFB");
	cmb_mode.InsertString(4,"CTR");
	cmb_mode.SetCurSel(0);
}
void AlgTestDlg::showCmbAlgName(int type){
	//首先清空之前的字符串
	cmb_name.ResetContent();
	
	//开始读取指定算法类型的算法名称
	int index = 0;
	for(int i = 0;i < rngAlg->info->GetSize();i++){
		if(rngAlg->info->GetAt(i)->AlgType == type){
			cmb_name.InsertString(index,rngAlg->info->GetAt(i)->AlgName);
			index++;
		}
	}
	cmb_name.SetCurSel(0);
}

// AlgTestDlg 对话框
IMPLEMENT_DYNAMIC(AlgTestDlg, CDialogEx)
AlgTestDlg::AlgTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(AlgTestDlg::IDD, pParent)
	, cstr_input(_T("明文"))
	, cstr_key(_T("密钥"))
	, cstr_enc(_T("加密后的密文"))
	, cstr_dec(_T("解密后的明文"))
	, cstr_iv(_T("初始向量"))
{
	rngAlg = new AlgMng(&dllMng);
	cipherAlg = new AlgMng(&dllMng);
}
AlgTestDlg::~AlgTestDlg(){

}
void AlgTestDlg::DoDataExchange(CDataExchange* pDX){
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_TYPE, cmb_type);
	DDX_Control(pDX, IDC_COMBO_NAME, cmb_name);
	DDX_Text(pDX, IDC_EDIT_INPUT, cstr_input);
	DDX_Text(pDX, IDC_EDIT_KEY, cstr_key);
	DDX_Text(pDX, IDC_EDIT_ENC, cstr_enc);
	DDX_Text(pDX, IDC_EDIT_DEC, cstr_dec);
	DDX_Text(pDX, IDC_EDIT_DEC2, cstr_iv);
	DDX_Control(pDX, IDC_COMBO_MODE, cmb_mode);
}


BEGIN_MESSAGE_MAP(AlgTestDlg, CDialogEx)
	ON_BN_CLICKED(ID_ENC, &AlgTestDlg::OnBnClickedEnc)
	ON_BN_CLICKED(ID_DEC, &AlgTestDlg::OnBnClickedDec)
	ON_BN_CLICKED(IDC_BTN_GENKEY, &AlgTestDlg::OnBnClickedBtnGenkey)
	ON_BN_CLICKED(ID_BTN_GENIV, &AlgTestDlg::OnBnClickedBtnGeniv)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, &AlgTestDlg::OnCbnSelchangeComboType)
END_MESSAGE_MAP()


// AlgTestDlg 消息处理程序
void AlgTestDlg::OnBnClickedEnc(){
	UpdateData();
	byte * cipher;
	if(cur_alg_type.Compare("流密码算法") == 0){
		cipher = cipherAlg->RunCipher(true,CString2Byte(cstr_input),
			cstr_input.GetAllocLength(),CString2Byte(cstr_key),
			CString2Byte(cstr_iv));
		cstr_enc = Byte2CString(cipher,cstr_input.GetAllocLength());
	}
	UpdateData(false);
}
void AlgTestDlg::OnBnClickedDec(){
	UpdateData();
	byte * plain;
	if(cur_alg_type.Compare("流密码算法") == 0){
		plain = cipherAlg->RunCipher(false,CString2Byte(cstr_enc),
			cstr_enc.GetAllocLength(),CString2Byte(cstr_key),
			CString2Byte(cstr_iv));
		cstr_dec = Byte2CString(plain,cstr_enc.GetAllocLength());
	}
	UpdateData(false);
}
void AlgTestDlg::OnBnClickedBtnGenkey(){
	cmb_name.GetLBText(cmb_name.GetCurSel(),cur_alg_name);
	cmb_type.GetLBText(cmb_type.GetCurSel(),cur_alg_type);

	cipherAlg->setAlg(Manage::typeStr2Int(cur_alg_type),cur_alg_name);
	int len = cipherAlg->GetLength(KEY_LEN);
	CString randStr = Byte2CString(getRandBytes("BBS",len),len);
	cstr_key = randStr;
	UpdateData(false);
}
void AlgTestDlg::OnBnClickedBtnGeniv(){
	cmb_name.GetLBText(cmb_name.GetCurSel(),cur_alg_name);
	cmb_type.GetLBText(cmb_type.GetCurSel(),cur_alg_type);
	int type = Manage::typeStr2Int(cur_alg_type);
	int len;
	cipherAlg->setAlg(Manage::typeStr2Int(cur_alg_type),cur_alg_name);
	if(type == STREAM) len = cipherAlg->GetLength(IV_LEN);
	else if (type == BLOCK) len = cipherAlg->GetLength(BLOCK_SIZE);
	CString randStr = Byte2CString(getRandBytes("BBS",len),len);
	cstr_iv = randStr;
	UpdateData(false);
}
void AlgTestDlg::OnCbnSelchangeComboType(){
	UpdateData();
	int sel = cmb_type.GetCurSel();
	//算法类型不是“分组密码算法时 不显示“密码模式”下拉条
	if(sel != 0){
		GetDlgItem(IDC_STAT_MODE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_MODE)->ShowWindow(SW_HIDE);
	}else{
		GetDlgItem(IDC_STAT_MODE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_COMBO_MODE)->ShowWindow(SW_SHOW);
	}

	//填充AlgName下拉框
	CString selStr;
	cmb_type.GetLBText(cmb_type.GetCurSel(),selStr);
	showCmbAlgName(Manage::typeStr2Int(selStr));
}


BOOL AlgTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	initCmbAlgType();
	initCmbAlgMode();
	showCmbAlgName(BLOCK);
	return TRUE;
}