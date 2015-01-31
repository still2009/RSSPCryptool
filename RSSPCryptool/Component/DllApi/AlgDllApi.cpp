//unicode中一个字符占2字节，就是2个char的长度

#include "stdafx.h"
#include "AlgDllApi.h"
/***********************DllMng的实现******************************/
DllMng::DllMng(){
	getDllDir();
	LoadDlls();
	getAllCipher();
}
DllMng::~DllMng(){
	FreeDlls();
}

void DllMng::getDllDir(){
	char buff[100];
	int len = 0;
	cfgFile.open("Config.txt",ios::in);
	if(cfgFile.is_open()){
		while(!cfgFile.eof()){
			cfgFile.read(buff+len,1);
			if(buff[len] == ';') break;
			len++;
		}
	}else dllDir = "ERROR";
	dllDir =  CString(buff,len);
}

void DllMng::LoadDlls(){
	CString dir(dllDir);
	dir.Append(_T("*.dll"));

	bool succ = finder.FindFile(dir.GetBuffer());
	while(succ){
		succ = finder.FindNextFile();

		dir = dllDir;
		dir.Append(finder.GetFileName());

		handles.Add(DLL_OPEN(dir));
	}
	for(int i = 0;i < handles.GetSize();i++){
		cout<<handles.GetAt(i)<<endl;
	}
}

void DllMng::FreeDlls(){
	for(int i = 0;i < handles.GetSize();i++){
		DLL_CLOSE(handles.GetAt(i));
	}
	handles.RemoveAll();
}

//所有Dll中唯一的变量
void * DllMng::getAlgFun(CString name){
	void *p = NULL;
	for(int i = 0;i < handles.GetSize();i++){
		p = DLL_GET(handles.GetAt(i),name);
		if(p != NULL) break;
	}
	return p;
}

void DllMng::getAllCipher(){
	Cipher * head = NULL;

	for(int i = 0;i < handles.GetSize();i++){
		head = (Cipher *)DLL_GET(handles.GetAt(i),"CIPHERS");
		
		int amount = *(int *)DLL_GET(handles.GetAt(i),"B_AMOUNT");
		amount += *(int *)DLL_GET(handles.GetAt(i),"S_AMOUNT");
		amount += *(int *)DLL_GET(handles.GetAt(i),"H_AMOUNT");
		amount += *(int *)DLL_GET(handles.GetAt(i),"M_AMOUNT");
		amount += *(int *)DLL_GET(handles.GetAt(i),"R_AMOUNT");

		for(int j = 0;j < amount;j++) ciphers.Add(head + j);
	}
}

CArray<HCIPHER> * DllMng::getCiphers(){
	if(ciphers.GetSize() == 0) getAllCipher();
	return &ciphers;
}

/***********************AlgMng的实现******************************/
AlgMng::AlgMng(DllMng * dllMng){
	this->dllMng = dllMng;
	info = dllMng->getCiphers();
	alg = new Cipher;
}

AlgMng::~AlgMng(){

}

/*该函数将明文填充至BLOCK_SIZE的整数倍,并返回长度*/
int AlgMng::padding(byte ** msg,int o_len){
	int n_len = GetLength(CIPHER_LEN,o_len);
	byte * n_msg = new byte[n_len];
	memcpy(n_msg,*msg,o_len);
	//使用0填充
	for(int i = o_len;i < n_len;i++){
		n_msg[i] = 0;
	}
	//还需要释放原来的指针
	*msg = n_msg;
	return n_len;
}

/*解填充针对解密后的明文*/
void AlgMng::unpadding(byte ** msg,int o_len){
	byte * n_msg = new byte[o_len];
	memcpy(n_msg,*msg,o_len);
	//还需要释放原来的指针
	*msg = n_msg;
}

CString AlgMng::GetCurrentAlg(int type){
	return "nothing";
}

int AlgMng::GetCurrentMode(){
	return 0;
}

//设定马上要使用的算法
void AlgMng::setAlg(int type,CString name){
	for(int i = 0;i < info->GetSize();i++){
		Cipher * pc = info->GetAt(i);
		if(pc->AlgType == type && name.CompareNoCase(pc->AlgName) == 0){
			memcpy(alg,pc,sizeof(Cipher));
			break;
		}
	}
}

int AlgMng::GetLength(int itemType,int plain_len){
	switch(itemType){
	case BLOCK_SIZE:return alg->block_size;
	case IV_LEN:return alg->iv_len;
	case KEY_LEN:return alg->key_len;
	case DIGEST_LEN:
	case MAC_LEN:return alg->output_len;
	case SEED_LEN:return alg->seed_len;
	case CIPHER_LEN:return (plain_len/alg->block_size + 1)*alg->block_size;
	default:return -1;
	}
}
//输入数据，获取输出。
/*当direct为true时,代表加密，input为明文，返回密文。
**当direct为false时，代表解密，input为密文。返回明文。
*/
//加密、解密失败时函数返回NULL
//密文长度不一定等于明文长度，因为分组密码可能会填充明文
//解密后的明文会是填充后的明文，可以根据填充字符和长度去掉填充字段。
byte * AlgMng::RunCipher(bool direct,byte* input,int len,byte* key,int mode,byte *iv){
	pBlockCipher Block = (pBlockCipher)(dllMng->getAlgFun("Use_" + CString(alg->AlgName)));
	byte * output = NULL;
	bool succ;

	//获取标准密文长度(分组长度整数倍)
	int n_len = padding(&input,len);
	output = new byte[n_len];

	if(direct) succ = (*Block)(direct,key,alg->key_len,input,n_len,output,n_len,mode,iv,alg->block_size);
	else succ = (*Block)(direct,key,alg->key_len,output,n_len,input,n_len,mode,iv,alg->block_size);
	//unpadding(&output,len);
	return succ ? output : NULL;
}
byte * AlgMng::RunCipher(bool direct,byte* input,int len,byte* key,byte* iv){
	pStreamCipher Stream = (pStreamCipher)(dllMng->getAlgFun("Use_" + CString(alg->AlgName)));
	byte * output = NULL;
	bool succ;

	output = new byte[len];
	if(direct) succ = (*Stream)(direct,key,alg->key_len,input,len,output,len,iv,alg->iv_len);
	else succ = (*Stream)(direct,key,alg->key_len,output,len,input,len,iv,alg->iv_len);

	return succ ? output : NULL;
}
byte * AlgMng::RunCipher(byte * msg){
	pHashFuction Hash = (pHashFuction)(dllMng->getAlgFun("Use_" + CString(alg->AlgName)));
	byte * output = NULL;
	bool succ;
	output = new byte[alg->output_len];

	succ = (*Hash)(msg,output,alg->output_len);
	return succ ? output : NULL;
}
byte * AlgMng::RunCipher(byte * input,int len,byte* key,byte *iv){
	pMacConstructor Mac = (pMacConstructor)(dllMng->getAlgFun("Use_" + CString(alg->AlgName)));
	byte * output = NULL;
	bool succ;
	output = new byte[alg->output_len];

	succ = (*Mac)(key,alg->key_len,input,len,output,alg->output_len,iv);
	return succ ? output : NULL;
}
byte * AlgMng::RunCipher(int seed,int size){
	pRng Rng = (pRng)(dllMng->getAlgFun("Use_" + CString(alg->AlgName)));
	byte * output = new byte[size];
	bool succ;

	succ = (*Rng)(seed,size,output);
	return succ ? output : NULL;
}

//int _tmain(int argc, _TCHAR* argv[])
//{
//	char text[16] = {'1','2','3','4','5','6','7','8','9','0','1','2','3','4','5','6'};
//	string plain = "Hello World!";
//	int len = plain.length();
//	byte *p = (byte *)plain.data();
//	AlgMng mng(&dllMng);
//	byte * output,* dec;
//
//	for(int i = 0;i < mng.info->GetSize();i++){
//		cout<<mng.info->GetAt(i)->AlgName<<endl;
//	}
//	//STREAM CIPHER
//	mng.setAlg(STREAM,"Sosemanuk");
//	output = mng.RunCipher(true,p,len,(byte *)text,(byte *)text);
//	dec = mng.RunCipher(false,output,len,(byte *)text,(byte *)text);
//
//	hex(output,len);
//	printf("\n");
//	hex(dec,len);
//	printf("\n");
//
//	//BLOCK CIPHER
//	mng.setAlg(BLOCK,"AES");
//	output = mng.RunCipher(true,p,len,(byte *)text,ECB);
//	dec = mng.RunCipher(false,output,mng.GetLength(CIPHER_LEN,len),(byte *)text,ECB);
//
//	hex(output,mng.GetLength(CIPHER_LEN,len));
//	printf("\n");
//	hex(dec,mng.GetLength(CIPHER_LEN,len));
//	printf("\n");
//
//	system("pause");
//	return 0;
//
//}