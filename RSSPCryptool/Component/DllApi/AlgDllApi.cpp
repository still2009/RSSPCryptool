#include "AlgDllApi.h"
AlgDllApi::AlgDllApi(){
	c = new Cipher;
	path_dll = "../AlgDll/";
	path_info = "./AlgorithmInfo.txt";
	path_load = "./AlgorithmLoad.txt";
}

AlgDllApi::~AlgDllApi(){
	DLL_CLOSE(dll);
}

/*该函数将明文填充至BLOCK_SIZE的整数倍，并更新长度*/
void AlgDllApi::padding(int num){
	/*off_set代表应该填充的位数*/
	padding_num = num;
	prev_length = c->input_len;
	/*存储pading_num以便解密时还原明文*/
	int offset = c->block_size-(prev_length%c->block_size);
	if(offset % c->block_size != 0){
		/*扩展空间*/
		byte *temp = c->input;
		c->input = new byte[offset + prev_length];
		memcpy(c->input,temp,prev_length);

		/*填充空位*/
		for(int i = prev_length;i < prev_length+offset;i++){
			c->input[i] = num;
		}

		/*更新长度*/
		c->input_len = offset + prev_length;
		/*释放之前的空间*/
		//delete[] temp;
	}
}

void AlgDllApi::unpadding(){
	byte *temp = c->input;
	c->input = new byte[prev_length];
	memcpy(c->input,temp,prev_length);
	c->input_len = prev_length;
	delete[] temp;
}

string AlgDllApi::GetCurrentAlg(int type){
	AlgLoadFile.open(path_load.data(),ios::in);
	if(!AlgLoadFile.is_open()) return "";

	string *AlgName;
	char line[50];
	while(!AlgLoadFile.eof()){
		AlgLoadFile.getline(line,50);
		if(type == atoi(line)){
			AlgName = new string(line,2,50);
			AlgName->assign(AlgName->substr(0,AlgName->find_first_of(' ',0)));
		}
	}
	AlgLoadFile.close();
	return *AlgName;
}

int AlgDllApi::GetCurrentMode(){
	AlgLoadFile.open(path_load.data(),ios::in);
	if(!AlgLoadFile.is_open()) return -1;

	char line[50];
	AlgLoadFile.getline(line,50);
	AlgLoadFile.close();
	return atoi(line + (strlen(line)-2));
}

string AlgDllApi::GetDllName(string name){
	AlgInfoFile.open(path_info.data(),ios::in);
	if(!AlgInfoFile.is_open()) return "";

	char line[50];
	string temp;
	while(!AlgInfoFile.eof()){
		AlgInfoFile.getline(line,50);
		temp = string(line,0,50);
		if(temp.substr(2,temp.find_first_of(' ',2)-2).compare(name) == 0){
			AlgInfoFile.close();
			return temp.substr(temp.find_last_of(' ')+1);
		}
	}
	AlgInfoFile.close();
	return "";
}

void AlgDllApi::GetDllCipher(int AlgType,string AlgName){
	string FunName = "Use_";
	string path(path_dll);
	//加载DLL
	path.append(GetDllName(AlgName).data());
	dll = DLL_OPEN(path.data());
	FunName.append(AlgName);
	//获取结构体数组
	Cipher * cipher_list;
	int *amount = NULL;
	switch(AlgType){
	case BLOCK:cipher_list = (Cipher *)DLL_GET(dll,"BLOCK_CIPHERS");amount = (int *)DLL_GET(dll,"B_AMOUNT");
		break;
	case STREAM:cipher_list = (Cipher *)DLL_GET(dll,"STREAM_CIPHERS");amount = (int *)DLL_GET(dll,"S_AMOUNT");
		break;
	case HASH:cipher_list = (Cipher *)DLL_GET(dll,"HASH_CIPHERS");amount = (int *)DLL_GET(dll,"H_AMOUNT");
		break;
	case MAC:cipher_list = (Cipher *)DLL_GET(dll,"MAC_CIPHERS");amount = (int *)DLL_GET(dll,"M_AMOUNT");
		break;
	case RNG:cipher_list = (Cipher *)DLL_GET(dll,"RNG_CIPHERS");amount = (int *)DLL_GET(dll,"R_AMOUNT");
		break;
	}
	for(int i = 0;i < *amount;i++){
		if(strcmp(cipher_list[i].AlgName,AlgName.data()) == 0){
			memcpy(c,&cipher_list[i],sizeof(Cipher));
			c->pFun = DLL_GET(dll,FunName.data());
			break;
		}
	}
	//DLL_CLOSE(dll);
	//仔细考虑释放时间；
}

bool AlgDllApi::RunCipher(){
	pBlockCipher Block;
	pStreamCipher Stream;
	pHashFuction Hash;
	pMacConstructor Mac;
	pRng Rng;

	switch(c->AlgType){
	case BLOCK:
		Block = (pBlockCipher)(c->pFun);
		return (*Block)(c->direct,c->key,c->key_len,c->input,c->input_len,
			c->output,c->output_len,c->mode,c->iv,c->block_size);
	case STREAM:
		Stream = (pStreamCipher)(c->pFun);
		return (*Stream)(c->direct,c->key,c->key_len,c->input,c->input_len,
			c->output,c->output_len,c->iv,c->iv_len);
	case HASH:
		Hash = (pHashFuction)(c->pFun);
		return (*Hash)(c->input,c->output,c->output_len);
	case MAC:
		Mac = (pMacConstructor)(c->pFun);
		return (*Mac)(c->key,c->key_len,c->input,c->input_len,c->output,c->output_len,c->iv);
	case RNG:
		Rng = (pRng)(c->pFun);
		c->output = (*Rng)(c->seed,c->output_len);

		if(c->output != NULL) return true;
		else return false;
	default: return false;
	}
}

//输入数据，获取输出。
/*当direct为true时,代表加密，input为明文，返回密文。
**当direct为false时，代表解密，input为密文。返回明文。
*/
byte * AlgDllApi::RunBlock(bool direct,byte* input,int len,byte* key,int mode,byte *iv){
	c->key = key;
	c->mode = mode;
	c->direct = direct;
	c->iv = iv;
	if(direct){
		c->input = input;
		c->input_len = len;

		c->output_len = c->input_len;
		c->output = new byte[c->output_len];
		padding();
		RunCipher();
		return c->output;
	}else{
		c->output = input;
		c->output_len = len;

		c->input_len = c->output_len;
		c->input = new byte[c->input_len];

		/*密文不为空说明之前加密是明文一定是BLOCK_SIZE整数倍，因此解密前先padding*/
		padding();
		RunCipher();
		unpadding();
		return c->input;
	}
}

byte * AlgDllApi::RunStream(bool direct,byte* input,int len,byte* key,byte* iv){
	c->key = key;
	c->iv = iv;
	c->direct = direct;
	if(direct){
		c->input = input;
		c->input_len = len;

		c->output_len = c->input_len;
		c->output = new byte[c->output_len];

		RunCipher();
		return c->output;
	}else{
		c->output = input;
		c->output_len = len;

		c->input_len = c->output_len;
		c->input = new byte[c->input_len];

		RunCipher();
		return c->input;
	}
}

byte * AlgDllApi::RunHash(string msg){
	c->input = (byte *)msg.data();
	c->input_len = msg.length();
	c->output = new byte[c->output_len];
	RunCipher();
	return c->output;
}

byte * AlgDllApi::RunMac(string input,byte* key,byte * iv){

	c->input = (byte *)input.data();
	c->input_len = input.length();
	c->key = key;
	c->iv = iv;
	c->output = new byte[c->output_len];
	/*Run之后没有生成mac，考虑iv*/
	RunCipher();
	return c->output;
}

byte * AlgDllApi::RunRng(int seed,int size){
	c->seed = seed;
	c->output_len = size;
	c->output = new byte[c->output_len];
	RunCipher();
	return c->output;
}

void AlgDllApi::ShowHex(byte *arr,int len){
	if(arr == NULL){
		puts("IT IS NULL!!\n");
		return;
	}
	for(int i = 0;i < len;i++){
		printf("%02X",arr[i]);
		if(i != len-1) printf("-");
	}
	cout<<endl;
}
//CString AlgDllApi::Byte2HexString(byte *array,int len){
//	char *str = new char[len*2];
//	for(int i = 0;i < len;i++){
//		sprintf(str+i,"%x",array[i]);
//	}
//	return CString(str,len);
//}

//char* AlgDllApi::GetAnsiString(CString &s){
//	int nSize = 2*s.GetLength();
//	char *pAnsiString = new char[nSize+1];
//	wcstombs(pAnsiString, s, nSize+1);
//	return pAnsiString;
//}
