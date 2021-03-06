/*算法调用接口
*Jason 2015/1/30
*调用接口分为2部分
*
*1--算法属性静态变量（静态数组、静态变量）(ALGORITHM_NAME,KEY_LENGTH,BLOCK_SZIE,IV_LENGTH等等)
*2--算法执行函数
*
*/
#include "stdafx.h"
#include "BaseCrypto.h"

template<class AlgType>
bool Use_BlockCipher(bool direct,byte * key,int key_len,byte * plain,int plain_len,
	byte * cipher,int cipher_len,int mode_id,byte * iv,int iv_len) {
	if(	key ==NULL ||
		plain == NULL ||
		cipher == NULL ||
		plain_len == 0 ||
		cipher_len == 0 ||
		plain_len % AlgType::BLOCKSIZE != 0 ||
		key_len != AlgType::DEFAULT_KEYLENGTH ||
		(mode_id != ECB && (iv == NULL || iv_len == 0))
	) return false;

	switch(mode_id){
		case ECB:if(direct){ECB_Mode<AlgType>::Encryption enc(key,key_len);enc.ProcessData(cipher,plain,plain_len);}
				 else{ECB_Mode<AlgType>::Decryption dec(key,key_len);dec.ProcessData(plain,cipher,plain_len);}
		break;
		case	CBC:if(direct){CBC_Mode<AlgType>::Encryption enc(key,key_len,iv);enc.ProcessData(cipher,plain,plain_len);}
				 else{CBC_Mode<AlgType>::Decryption dec(key,key_len,iv);dec.ProcessData(plain,cipher,plain_len);}
		break;
		case CFB:if(direct){CFB_Mode<AlgType>::Encryption enc(key,key_len,iv);enc.ProcessData(cipher,plain,plain_len);}
				 else{CFB_Mode<AlgType>::Decryption dec(key,key_len,iv);dec.ProcessData(plain,cipher,plain_len);}
		break;
		case OFB:if(direct){OFB_Mode<AlgType>::Encryption enc(key,key_len,iv);enc.ProcessData(cipher,plain,plain_len);}
				 else{OFB_Mode<AlgType>::Decryption dec(key,key_len,iv);dec.ProcessData(plain,cipher,plain_len);}
		break;
	}
	return true;
}

//需要修改：针对RC4算法的调用
template <class AlgType>
bool Use_StreamCipher(_Stream_Cipher_Parameters_){
	
	if(	key == NULL ||
		plain == NULL ||
		cipher == NULL ||
		key_len == 0 ||
		key_len != AlgType::DEFAULT_KEYLENGTH ||
		plain_len == 0 ||
		cipher_len == 0 ||
		iv == NULL ||
		iv_len == 0
	) return false;
	
	if(direct){
		AlgType::Encryption enc(key,key_len,iv);
		enc.ProcessData(cipher,plain,cipher_len);
	}else{
		AlgType::Decryption dec(key,key_len,iv);
		dec.ProcessData(plain,cipher,plain_len);
	}
	return true;
};

template <class AlgType>
bool Use_Hash(byte * msg,byte * digest,int digest_len){
	if(msg == NULL || digest == NULL || digest_len == NULL) return false;
	StringSource src((const char*)msg,true,new HashFilter(*new AlgType(),new ArraySink(digest,digest_len)));
	return true;
}

template <class AlgType>
bool Use_MAC(byte *key,int key_len,byte *msg,int msg_len,byte *mac,int mac_len,byte *iv){
	if(	key == NULL ||
		msg == NULL ||
		mac == NULL ||
		key_len == 0 ||
		msg_len == 0 ||
		mac_len == 0
	) return false;
	AlgType MacAlg;
	if(MacAlg.StaticAlgorithmName().find("VMAC") == 0){
		if(iv == NULL) return false;
		MacAlg.SetKeyWithIV(key,key_len,iv);
	}else MacAlg.SetKey(key,key_len);
	MacAlg.Update(msg,msg_len);
	MacAlg.Final(mac);
	return true;
}

/************************分组密码算法*****************************/
bool Use_Blowfish(_Block_Cipher_Parameters_){B(Blowfish)}
bool Use_Camellia(_Block_Cipher_Parameters_){B(Camellia)}
bool Use_CAST256(_Block_Cipher_Parameters_){B(CAST256)}
bool Use_DES_EDE3(_Block_Cipher_Parameters_){B(DES_EDE3)}
bool Use_IDEA(_Block_Cipher_Parameters_){B(IDEA)}
//bool Use_LR(_Block_Cipher_Parameters_){B(LR<SHA>)}
bool Use_MARS(_Block_Cipher_Parameters_){B(MARS)}
bool Use_RC5(_Block_Cipher_Parameters_){B(RC5)}
bool Use_RC6(_Block_Cipher_Parameters_){B(RC6)}
bool Use_SEED(_Block_Cipher_Parameters_){B(SEED)}
bool Use_Serpent(_Block_Cipher_Parameters_){B(Serpent)}
bool Use_SHACAL2(_Block_Cipher_Parameters_){B(SHACAL2)}
bool Use_SKIPJACK(_Block_Cipher_Parameters_){B(SKIPJACK)}
bool Use_TEA(_Block_Cipher_Parameters_){B(TEA)}
bool Use_Twofish(_Block_Cipher_Parameters_){B(Twofish)}
bool Use_XTEA(_Block_Cipher_Parameters_){B(XTEA)}
bool Use_AES(_Block_Cipher_Parameters_){B(AES)}
//不安全的算法
bool Use_DES(_Block_Cipher_Parameters_){B(DES)}
bool Use_GOST(_Block_Cipher_Parameters_){B(GOST)}
bool Use_SHARK(_Block_Cipher_Parameters_){B(SHARK)}
bool Use_Square(_Block_Cipher_Parameters_){B(Square)}
bool Use_ThreeWay(_Block_Cipher_Parameters_){B(ThreeWay)}

/************************流密码算法*****************************/
bool Use_Sosemanuk(_Stream_Cipher_Parameters_){S(Sosemanuk)}
bool Use_Salsa20(_Stream_Cipher_Parameters_){S(Salsa20)}
bool Use_XSalsa20(_Stream_Cipher_Parameters_){S(XSalsa20)}
bool Use_SEAL(_Stream_Cipher_Parameters_){S(SEAL<>)}
bool Use_PanamaCipher(_Stream_Cipher_Parameters_){S(PanamaCipher<>)}
//不安全的算法
bool Use_RC4(_Stream_Cipher_Parameters_){
	if(	key == NULL ||
		plain == NULL ||
		cipher == NULL ||
		key_len == 0 ||
		key_len != Weak::ARC4::DEFAULT_KEYLENGTH ||
		plain_len == 0 ||
		cipher_len == 0
	) return false;
	
	if(direct){
		Weak::ARC4::Encryption enc(key,key_len);
		enc.ProcessData(cipher,plain,cipher_len);
	}else{
		Weak::ARC4::Decryption dec(key,key_len);
		dec.ProcessData(plain,cipher,plain_len);
	}
	return true;
}

/************************HASH算法*****************************/
bool Use_MD2(_Hash_Parameters_){H(Weak::MD2)}
bool Use_MD4(_Hash_Parameters_){H(Weak::MD4)}
bool Use_MD5(_Hash_Parameters_){H(Weak::MD5)}
bool Use_SHA1(_Hash_Parameters_){H(SHA1)}
bool Use_SHA256(_Hash_Parameters_){H(SHA256)}
bool Use_SHA224(_Hash_Parameters_){H(SHA224)}
bool Use_SHA512(_Hash_Parameters_){H(SHA512)}
bool Use_SHA384(_Hash_Parameters_){H(SHA384)}
bool Use_SHA3_224(_Hash_Parameters_){H(SHA3_224)}
bool Use_SHA3_256(_Hash_Parameters_){H(SHA3_256)}
bool Use_SHA3_384(_Hash_Parameters_){H(SHA3_384)}
bool Use_SHA3_512(_Hash_Parameters_){H(SHA3_512)}
bool Use_Tiger(_Hash_Parameters_){H(Tiger)}
bool Use_Whirlpool(_Hash_Parameters_){H(Whirlpool)}
bool Use_RIPEMD160(_Hash_Parameters_){H(RIPEMD160)}
bool Use_RIPEMD320(_Hash_Parameters_){H(RIPEMD320)}
bool Use_RIPEMD128(_Hash_Parameters_){H(RIPEMD128)}
bool Use_RIPEMD256(_Hash_Parameters_){H(RIPEMD256)}

/************************消息认证算法*****************************/
bool Use_CBC_MAC_AES(_MAC_Parameters_){M(CBC_MAC<AES>)}
bool Use_CMAC_AES(_MAC_Parameters_){M(CMAC<AES>)}
bool Use_DMAC_AES(_MAC_Parameters_){M(DMAC<AES>)}
bool Use_HMAC_SHA1(_MAC_Parameters_){M(HMAC<SHA1>)}
bool Use_VMAC_AES(_MAC_Parameters_){M(VMAC<AES>)}
bool Use_TTMAC(_MAC_Parameters_){M(TTMAC)}

/************************随机数生成器*****************************/
bool Use_BBS(_RNG_Parameters_){
	BlumBlumShub rng(383,503,seed);
	rng.GenerateBlock(input,size);
	return input == NULL ? false : true;
}
bool Use_LCG(_RNG_Parameters_){
	LC_RNG rng(seed);
	rng.GenerateBlock(input,size);
	return input == NULL ? false : true;
}

/************************库信息说明*****************************/
//其他数值分别是BLOCK,STREAM,HASH,MAC,RNG
extern const int DLL_ALGORITHM_TYPE = 0;
/************************算法信息*****************************/
//数组AMOUNT中第一个数为分组密码算法的数量，以后依次为流密码，HASH,MAC,RNG的数量
extern const int AMOUNTS[] = {21,6,18,6,2};
extern const Cipher CIPHERS[] = {
	BlockInfo("AES",KL(AES),BS(AES)),BlockInfo("Blowfish",KL(Blowfish),BS(Blowfish)),
	BlockInfo("Camellia",KL(Camellia),BS(Camellia)),BlockInfo("CAST256",KL(CAST256),BS(CAST256)),
	BlockInfo("DES",KL(DES),BS(DES)),BlockInfo("MARS",KL(MARS),BS(MARS)),
	BlockInfo("IDEA",KL(IDEA),BS(IDEA)),BlockInfo("RC5",KL(RC5),BS(RC5)),
	BlockInfo("RC6",KL(RC6),BS(RC6)),BlockInfo("SEED",KL(SEED),BS(SEED)),
	BlockInfo("Serpent",KL(Serpent),BS(Serpent)),BlockInfo("SHACAL2",KL(SHACAL2),BS(SHACAL2)),
	BlockInfo("SKIPJACK",KL(SKIPJACK),BS(SKIPJACK)),BlockInfo("TEA",KL(TEA),BS(TEA)),
	BlockInfo("Twofish",KL(Twofish),BS(Twofish)),BlockInfo("XTEA",KL(XTEA),BS(XTEA)),
	BlockInfo("DES_EDE3",KL(DES_EDE3),BS(DES_EDE3)),BlockInfo("GOST",KL(GOST),BS(GOST)),
	BlockInfo("SHARK",KL(SHARK),BS(SHARK)),BlockInfo("Square",KL(Square),BS(Square)),
	BlockInfo("ThreeWay",KL(ThreeWay),BS(ThreeWay)),

	StreamInfo("Sosemanuk",KL(Sosemanuk),IL(Sosemanuk)),
	StreamInfo("Salsa20",KL(Salsa20),IL(Salsa20)),
	StreamInfo("XSalsa20",KL(XSalsa20),IL(XSalsa20)),
	StreamInfo("SEAL",KL(SEAL<>),IL(SEAL<>)),
	StreamInfo("PanamaCipher",KL(PanamaCipher<>),IL(PanamaCipher<>)),
	StreamInfo("RC4",KL(Weak::ARC4),IL(Weak::ARC4)),

	HashInfo("MD2",DS(Weak::MD2)),HashInfo("MD4",DS(Weak::MD4)),
	HashInfo("MD5",DS(Weak::MD5)),HashInfo("SHA1",DS(SHA1)),
	HashInfo("SHA256",DS(SHA256)),HashInfo("SHA224",DS(SHA224)),
	HashInfo("SHA512",DS(SHA512)),HashInfo("SHA384",DS(SHA384)),
	HashInfo("SHA3_224",DS(SHA3_224)),HashInfo("SHA3_256",DS(SHA3_256)),
	HashInfo("SHA3_384",DS(SHA3_384)),HashInfo("SHA3_512",DS(SHA3_512)),
	HashInfo("Tiger",DS(Tiger)),HashInfo("RIPEMD160",DS(RIPEMD160)),
	HashInfo("RIPEMD320",DS(RIPEMD320)),HashInfo("RIPEMD128",DS(RIPEMD128)),
	HashInfo("RIPEMD256",DS(RIPEMD256)),HashInfo("Whirlpool",DS(Whirlpool)),

	MacInfo("CBC_MAC_AES",KL(CBC_MAC<AES>),AES::BLOCKSIZE),
	MacInfo("CMAC_AES",KL(CMAC<AES>),AES::BLOCKSIZE),
	MacInfo("DMAC_AES",KL(DMAC<AES>),AES::BLOCKSIZE),
	MacInfo("HMAC_SHA1",KL(HMAC<SHA1>),HMAC<SHA1>::DIGESTSIZE),
	MacInfo("VMAC_AES",KL(AES),AES::BLOCKSIZE),

	MacInfo("TTMAC",20,20),
	RngInfo("BBS",0),RngInfo("LCG",0)
};