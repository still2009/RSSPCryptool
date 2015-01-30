#include "AlgDllApi.h"
#include <ctime>
//#include <string>
void ShowHex(byte *arr,int len){
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

void ApiTest(){
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
		ShowHex(cipher.RunHash(text),cipher.c->output_len);
	}
	/*消息认证码*/
	puts("消息认证码生成测试---\n");
	for(int i = 0;i < 6;i++){
		cipher.GetDllCipher(MAC,M[i]);
		byte *iv = new byte[cipher.c->iv_len];

		memcpy(iv,rng.RunRng(time(NULL),16),16);

		byte *mac = cipher.RunMac(text,rng.RunRng(time(NULL),cipher.c->key_len),iv);
		ShowHex(mac,cipher.c->output_len);

	}
	system("pause");
}
