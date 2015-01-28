/************************************************************************/
/* Version 2014/11/8  Bug:fixed                     */
/************************************************************************/
#ifdef BASECRYPTO_EXPORTS
#define BASECRYPTO_API __declspec(dllexport)
#else
#define BASECRYPTO_API __declspec(dllimport)
#endif

//Algorithms
#include "HBlockCiphers.h"
#include "HStreamCiphers.h"
#include "HHashFuctions.h"
#include "Hmacs.h"
#include "HRngs.h"
//filters
#include "cryptopp\filters.h"

//standard headers
#include <iostream>

//namespaces
using namespace std;
using namespace CryptoPP;
#pragma comment(lib,"cryptlib.lib")

//类型说明
enum AlgType{
	BLOCK = 1,
	STREAM,
	HASH,
	MAC,
	RNG
};
enum Mode{
	ECB = 1,
	CBC,
	CFB,
	OFB,
	CTR
};
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

//结构体赋值简化宏定义
#define BlockInfo(name,key_len,block_size) {BLOCK,name,NULL,NULL,true,NULL,0,NULL,key_len,NULL,0,NULL,0,0,block_size,0}
#define StreamInfo(name,key_len,iv_len) {STREAM,name,NULL,NULL,true,NULL,0,NULL,key_len,NULL,0,NULL,iv_len,0,0,0}
#define HashInfo(name,digest_size) {HASH,name,NULL,NULL,true,NULL,0,NULL,0,NULL,digest_size,NULL,0,0,0,0}
#define MacInfo(name,key_len,mac_len) {MAC,name,NULL,NULL,true,NULL,0,NULL,key_len,NULL,mac_len,NULL,0,0,0,0}
#define RngInfo(name) {RNG,name,NULL,NULL,true,NULL,0,NULL,0,NULL,0,NULL,0,0,0,0}
//简化的宏定义
#define _Block_Cipher_Parameters_ bool direct,byte * key,int key_len,byte * plain,int plain_len,byte * cipher,int cipher_len,int mode_id = ECB,byte * iv = NULL,int iv_len = 0
#define B(x) return Use_BlockCipher<x>(direct,key,key_len,plain,plain_len,cipher,cipher_len,mode_id,iv,iv_len);

#define _Stream_Cipher_Parameters_ bool direct,byte * key,int key_len,byte * plain,int plain_len,byte * cipher,int cipher_len,byte * iv,int iv_len
#define S(x) return Use_StreamCipher<x>(direct,key,key_len,plain,plain_len,cipher,cipher_len,iv,iv_len);

#define _Hash_Parameters_ byte * msg,byte * digest,int digest_len
#define H(x) return Use_Hash<x>(msg,digest,digest_len);

#define _MAC_Parameters_ byte *key,int key_len,byte *msg,int msg_len,byte *mac,int mac_len,byte *iv
#define M(x) return Use_MAC<x>(key,key_len,msg,msg_len,mac,mac_len,iv);

#define _RNG_Parameters_ int seed,int size

#define KL(x) x::DEFAULT_KEYLENGTH
#define BS(x) x::BLOCKSIZE
#define IL(x) x::IV_LENGTH
#define DS(x) x::DIGESTSIZE