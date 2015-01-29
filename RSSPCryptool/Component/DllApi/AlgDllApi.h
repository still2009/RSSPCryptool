#pragma once
#define WINDOWS_DLL

#ifdef LINUX_DLL
	#define DLL_OPEN(x) dlopen(x,RTL_NOW)
	#define DLL_GET(x,y) dlsym(x,y)
	#define DLL_CLOSE() dlclose();
#endif
#ifdef WINDOWS_DLL
	#define DLL_OPEN(x) LoadLibraryA(x);
	#define DLL_GET(x,y) GetProcAddress(x,y)
	#define DLL_CLOSE(x) FreeLibrary(x);
#endif

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cstring>
#include <windows.h>
#include <string>
using namespace std;
/************************************************************************/
/* version 2014/11/8      MAC: VMAC needs initial vector                 */
/************************************************************************/
enum Mode{
	ECB = 1,
	CBC,
	CFB,
	OFB,
	CTR
};
enum AlgType{
	BLOCK = 1,
	STREAM,
	HASH,
	MAC,
	RNG
};
typedef unsigned char byte;
typedef struct {
	int AlgType; char * AlgName;
	char *DllPath; void * pFun;

	bool direct;
	byte * input; int input_len;
	byte * key; int key_len;
	byte * output; int output_len;
	byte * iv; int iv_len;
	int mode; int block_size;
	int seed;
} Cipher;
typedef bool (*pBlockCipher)(bool,byte *,int,byte *,int,byte *,int,int,byte *,int);
typedef bool (*pStreamCipher)(bool,byte *,int,byte *,int,byte *,int,byte *,int);
typedef bool (*pHashFuction)(byte *,byte *,int);
typedef bool (*pMacConstructor)(byte *,int,byte *,int,byte *,int,byte *);
typedef byte * (*pRng)(int seed,int size);



const string B[] = {
	"Blowfish","Camellia","CAST256",
	"DES","IDEA","MARS",
	"RC5","RC6","SEED",
	"Serpent","SHACAL2","SKIPJACK",
	"TEA","Twofish","XTEA",
	"DES_EDE3","GOST","SHARK",
	"Square","ThreeWay","AES"
};
const string S[] = {
	"Sosemanuk","Salsa20","XSalsa20",
	"SEAL","PanamaCipher","RC4"
};
const string H[] = {
	"MD2","MD4","MD5",
	"SHA1","SHA256","SHA224",
	"SHA512","SHA384","SHA3_224",
	"SHA3_256","SHA3_384","SHA3_512",
	"Tiger","RIPEMD160","RIPEMD320",
	"RIPEMD128","RIPEMD256","Whirlpool"
};
const string M[] = {
	"CBC_MAC_AES","CMAC_AES","DMAC_AES",
	"HMAC_SHA1","VMAC_AES","TTMAC"
};
const string R[] = {
	"BBS","LCG"
};



/*算法库DLL的调用接口
**库的加载与释放
**使用算法加解密
*/
class AlgDllApi{
private:
	fstream AlgLoadFile;
	fstream AlgInfoFile;
	HINSTANCE dll;
	/*填充，解填充相关的参数*/
	int padding_num;
	int prev_length;
	void padding(int num = 0);
	void unpadding();
public:
	string path_load;
	string path_info;
	string path_dll;

	Cipher *c;

	AlgDllApi();
	~AlgDllApi();

	string GetCurrentAlg(int type);

	int GetCurrentMode();

	string GetDllName(string name);

	void GetDllCipher(int AlgType,string AlgName);

	bool RunCipher();

	byte * RunBlock(bool direct,byte* input,int len,byte* key,int mode,byte *iv = NULL);

	byte * RunStream(bool direct,byte* input,int len,byte* key,byte* iv);

	byte * RunHash(string msg);

	byte * RunMac(string input,byte* key,byte *iv = NULL);

	byte * RunRng(int seed,int size);

	static void ShowHex(byte *arr,int len);
	//CString Byte2HexString(byte *array,int len);
	
	//char *GetAnsiString(const CString &s);
};