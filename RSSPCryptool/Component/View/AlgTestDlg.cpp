// AlgTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RSSPCryptool.h"
#include "AlgTestDlg.h"
#include "afxdialogex.h"
#include "../DllApi/AlgDllApi.h"

// AlgTestDlg 对话框

IMPLEMENT_DYNAMIC(AlgTestDlg, CDialogEx)

AlgTestDlg::AlgTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(AlgTestDlg::IDD, pParent)
	, cstr_input(_T("明文"))
	, cstr_key(_T("密钥"))
	, cstr_enc(_T("加密后的密文"))
	, cstr_dec(_T("解密后的明文"))
{

}

AlgTestDlg::~AlgTestDlg()
{
}

void AlgTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_TYPE, cmb_type);
	DDX_Control(pDX, IDC_COMBO_NAME, cmb_name);
	DDX_Text(pDX, IDC_EDIT_INPUT, cstr_input);
	DDX_Text(pDX, IDC_EDIT_KEY, cstr_key);
	DDX_Text(pDX, IDC_EDIT_ENC, cstr_enc);
	DDX_Text(pDX, IDC_EDIT_DEC, cstr_dec);
}


BEGIN_MESSAGE_MAP(AlgTestDlg, CDialogEx)
	ON_BN_CLICKED(ID_ENC, &AlgTestDlg::OnBnClickedEnc)
	ON_BN_CLICKED(ID_DEC, &AlgTestDlg::OnBnClickedDec)
	ON_CBN_DROPDOWN(IDC_COMBO_TYPE, &AlgTestDlg::OnCbnDropdownComboType)
	ON_CBN_DROPDOWN(IDC_COMBO_NAME, &AlgTestDlg::OnCbnDropdownComboName)
END_MESSAGE_MAP()


// AlgTestDlg 消息处理程序


void AlgTestDlg::OnBnClickedEnc()
{
	string text = "密码库子系统SRTP答辩加密字符！！";

	AlgDllApi cipher,rng;
	rng.GetDllCipher(RNG,"BBS");
	
	/*分组密码测试*/
	puts("分组密码加解密测试---\n");
	for(int i = 0;i < 21;i++){
		cipher.GetDllCipher(BLOCK,B[i]);
		rng.RunRng(time(NULL),cipher.c->key_len);

		cipher.RunBlock(true,(byte *)text.data(),text.length(),rng.c->output,CBC,(byte *)text.data());

		cipher.c->input = NULL;
		byte * plain = cipher.RunBlock(false,cipher.c->output,text.length(),rng.c->output,CBC,(byte *)text.data());
		cout<<string((char *)plain,cipher.c->input_len)<<endl;
	}
	/*流密码测试*/
	puts("流密码加解密测试---\n");
	for(int i = 0;i <6;i++){
		cipher.GetDllCipher(STREAM,S[i]);
		rng.RunRng(time(NULL),cipher.c->key_len);

		cipher.RunStream(true,(byte *)text.data(),text.length(),rng.c->output,(byte *)text.data());

		cipher.c->input = NULL;
		byte * plain = cipher.RunStream(false,cipher.c->output,text.length(),rng.c->output,(byte *)text.data());
		cout<<string((char *)plain,cipher.c->input_len)<<endl;
	}
	/*Hash测试*/
	puts("Hash 算法测试---\n");
	for (int i = 0;i < 18;i++){
		cipher.GetDllCipher(HASH,H[i]);
		AlgDllApi::ShowHex(cipher.RunHash(text),cipher.c->output_len);
	}
	/*消息认证码*/
	puts("消息认证码生成测试---\n");
	for(int i = 0;i < 6;i++){
		cipher.GetDllCipher(MAC,M[i]);
		byte *iv = new byte[cipher.c->iv_len];

		memcpy(iv,rng.RunRng(time(NULL),16),16);

		byte *mac = cipher.RunMac(text,rng.RunRng(time(NULL),cipher.c->key_len),iv);
		AlgDllApi::ShowHex(mac,cipher.c->output_len);

	}
	system("pause");
}


void AlgTestDlg::OnBnClickedDec()
{
	// TODO: 在此添加控件通知处理程序代码
}


void AlgTestDlg::OnCbnDropdownComboType()
{
	// TODO: 在此添加控件通知处理程序代码
}


void AlgTestDlg::OnCbnDropdownComboName()
{
	// TODO: 在此添加控件通知处理程序代码
}
