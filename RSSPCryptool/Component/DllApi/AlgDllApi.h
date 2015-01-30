#pragma once
#define WINDOWS_DLL

#ifdef LINUX_DLL
	#define DLL_OPEN(x) dlopen(x,RTL_NOW)
	#define DLL_GET(x,y) dlsym(x,y)
	#define DLL_CLOSE() dlclose()
#endif
#ifdef WINDOWS_DLL
	#define DLL_OPEN(x) LoadLibrary(x)
	#define DLL_GET(x,y) GetProcAddress(x,y)
	#define DLL_CLOSE(x) FreeLibrary(x)
#endif

using namespace std;
/************************************************************************/
/* version 2014/11/8      MAC: VMAC needs initial vector                 */
/************************************
************************************/
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
	char *DllName;
	int AlgType; char * AlgName;

	int key_len;
	int output_len;int iv_len;
	int block_size;int seed_len;
} Cipher;
typedef Cipher * HCIPHER;

typedef bool (*pBlockCipher)(bool,byte *,int,byte *,int,byte *,int,int,byte *,int);
typedef bool (*pStreamCipher)(bool,byte *,int,byte *,int,byte *,int,byte *,int);
typedef bool (*pHashFuction)(byte *,byte *,int);
typedef bool (*pMacConstructor)(byte *,int,byte *,int,byte *,int,byte *);
typedef byte * (*pRng)(int seed,int size);



const CString B[] = {
	"Blowfish","Camellia","CAST256",
	"DES","IDEA","MARS",
	"RC5","RC6","SEED",
	"Serpent","SHACAL2","SKIPJACK",
	"TEA","Twofish","XTEA",
	"DES_EDE3","GOST","SHARK",
	"Square","ThreeWay","AES"
};
const CString S[] = {
	"Sosemanuk","Salsa20","XSalsa20",
	"SEAL","PanamaCipher","RC4"
};
const CString H[] = {
	"MD2","MD4","MD5",
	"SHA1","SHA256","SHA224",
	"SHA512","SHA384","SHA3_224",
	"SHA3_256","SHA3_384","SHA3_512",
	"Tiger","RIPEMD160","RIPEMD320",
	"RIPEMD128","RIPEMD256","Whirlpool"
};
const CString M[] = {
	"CBC_MAC_AES","CMAC_AES","DMAC_AES",
	"HMAC_SHA1","VMAC_AES","TTMAC"
};
const CString R[] = {
	"BBS","LCG"
};

//算法信息一次性全部加载。
//配置文件设定：Config.txt
//算法与Dll文件映射关系（工作量小，所以使用动态判断）
//给定Dll文件目录-->列出目录下的所有Dll-->获取每个Dll的HINSTANCE存储到数组中
//①从config文件中读取dll文件目录②获得目录下每个Dll文件的文件名③使用DLL_OPEN获得所有句柄
class DllMng{
private:
	fstream cfgFile;//配置文件对象
	CFileFind finder;//文件查找对象
	CArray<HINSTANCE> handles;//所有dll的句柄数组
	CArray<HCIPHER> ciphers;//所有算法信息
	CString dllDir;//dll文件目录

	//Dll的操作
	void getDllDir();//获取dll文件目录
	void LoadDlls();//获得所有句柄
	void FreeDlls();//释放所有句柄

	void getAllCipher();//获取所有算法库中的所有算法信息结构体Cipher的指针
public:
	DllMng();
	~DllMng();
	
	//对上层提供的服务
	void * getAlgFun(CString name);//搜寻在所有dll中唯一的变量
	CArray<HCIPHER> * getCiphers();
};

/***********************全局唯一DllMng对象*************************/
//DllMng dllMng;


/*算法库DLL的调用接口
**使用算法加解密
*/
class AlgMng{
private:
	//填充，解填充
	int padding(byte ** msg,int o_len);//按照分组长度填充，返回新长度
	void unpadding(byte ** msg,int o_len);//还原

	//算法信息
	CArray<HCIPHER> * info;
	Cipher * alg;
	//库管理对象指针
	DllMng * dllMng;
public:
	AlgMng(DllMng * dllMng);
	~AlgMng();

	//获取算法配置（切换）信息
	CString GetCurrentAlg(int type);
	int GetCurrentMode();

	//算法调用相关
	void setAlg(int type,CString name);
	//密钥长度，分组长度，hash摘要长度
	int GetLength(int itemType);
	byte * RunCipher(bool direct,byte* input,int len,byte* key,int mode,byte *iv = NULL);
};