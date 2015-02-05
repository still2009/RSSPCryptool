#include "stdafx.h"
#include "Component\DllApi\AlgDllApi.h"

void Roll(byte* str, int index)
{
	byte flag = 0x80 >> (index %8);
	str[index/8]=str[index/8]^flag;
}
void CountChangedBit(byte a,byte b,int *count)
{
	a = a ^ b;
	do 
	{
		if (a & 0x01) (*count)++;
	}while (a = a >> 1);
}
double CalcDistance(byte *a,byte *b,int len)  //比较a和b两个字符串的汉明距离
{
	int count = 0;
	for (int i = 0;i < len;i++) CountChangedBit(a[i],b[i],&count);
	return (double)count;
}
int TransformAlgType(CString AlgType)
{
	if(!AlgType.Compare("分组密码")) return 1;
	else if(!AlgType.Compare("流密码")) return 2;
	else if(!AlgType.Compare("HASH")) return 3;
	else if(!AlgType.Compare("消息认证码")) return 4;

}
int TransformAlgMode(CString AlgMode)
{
	if(!AlgMode.Compare("ECB")) return 1;
	else if(!AlgMode.Compare("CBC")) return 2;
	else if(!AlgMode.Compare("CFB")) return 3;
	else if(!AlgMode.Compare("OFB")) return 4;
	else if(!AlgMode.Compare("CTR")) return 5;

}
double CalcAverage(int AlgType, CString AlgName, int AlgMode, CString TestType, int TextLen) //计算一次敏感度测试的均值
{
	double Distance = 0.0;
	AlgMng mng1(&dllMng);   //用于生成测试需要的相关参数
	AlgMng mng2(&dllMng);   //用于生成测试的算法
	if(!TestType.Compare("明文敏感度测试"))
	{
		if(AlgType==1)
		{
			mng1.setAlg(AlgType,AlgName);
			mng2.setAlg(5,"BBS");
			byte *input_pre=mng2.RunCipher(time(NULL),TextLen);
			byte *iv_pre=mng2.RunCipher(time(NULL)+117,mng1.GetLength(2));
			byte *key_pre=mng2.RunCipher(time(NULL)+TextLen,mng1.GetLength(3));  
			byte *output_pre=mng1.RunCipher(true,input_pre,TextLen,key_pre,AlgMode,iv_pre);
			byte *output_test=new byte[mng1.GetLength(7,TextLen)];
			byte *input_test=new byte[TextLen];
			for(int i=0;i<=TextLen*8;i++)
			{
				memcpy(input_test,input_pre,TextLen);
				Roll(input_test,i);
				byte *output_test=mng1.RunCipher(true,input_test,TextLen,key_pre,AlgMode,iv_pre);
				Distance+=CalcDistance(output_test,output_pre,mng1.GetLength(7,TextLen));
			}
			return Distance/(mng1.GetLength(7,TextLen)*8)/(TextLen*8);
		}
		else if(AlgType==2)
		{
			mng1.setAlg(AlgType,AlgName);
			mng2.setAlg(5,"BBS");
			byte *input_pre=mng2.RunCipher(time(NULL),TextLen);
			byte *iv_pre=mng2.RunCipher(time(NULL)+117,mng1.GetLength(2));
			byte *key_pre=mng2.RunCipher(time(NULL)+TextLen,mng1.GetLength(3));  
			byte *output_pre=mng1.RunCipher(true,input_pre,TextLen,key_pre,iv_pre);
			byte *output_test=new byte[TextLen];
			byte *input_test=new byte[TextLen];
			for(int i=0;i<=TextLen*8;i++)
			{
				memcpy(input_test,input_pre,TextLen);
				Roll(input_test,i);
				byte *output_test=mng1.RunCipher(true,input_test,TextLen,key_pre,iv_pre);
				Distance+=CalcDistance(output_test,output_pre,TextLen);
			}
			return Distance/TextLen/TextLen/64;
		}
	}
	else if(!TestType.Compare("密钥敏感度测试"))
	{
		if(AlgType==1)
		{
			mng1.setAlg(AlgType,AlgName);
			mng2.setAlg(5,"BBS");
			byte *input_pre=mng2.RunCipher(time(NULL),TextLen);
			byte *iv_pre=mng2.RunCipher(time(NULL)+117,mng1.GetLength(2));
			byte *key_pre=mng2.RunCipher(time(NULL)+TextLen,mng1.GetLength(3));  
			byte *output_pre=mng1.RunCipher(true,input_pre,TextLen,key_pre,AlgMode,iv_pre);
			byte *output_test=new byte[mng1.GetLength(7)];
			byte *key_test= new byte[mng1.GetLength(3)];
			for(int i=0;i<=mng1.GetLength(3)*8;i++)
			{
				memcpy(key_test,key_pre,mng1.GetLength(3));
				Roll(key_test,i);
				byte *output_test=mng1.RunCipher(true,input_pre,TextLen,key_test,AlgMode,iv_pre);
				Distance+=CalcDistance(output_test,output_pre,mng1.GetLength(7,TextLen));
			}
			return Distance/mng1.GetLength(3)/TextLen/64;
		}
		else if(AlgType==2)
		{
			mng1.setAlg(AlgType,AlgName);
			mng2.setAlg(5,"BBS");
			byte *input_pre=mng2.RunCipher(time(NULL),TextLen);
			byte *iv_pre=mng2.RunCipher(time(NULL)+117,mng1.GetLength(2));
			byte *key_pre=mng2.RunCipher(time(NULL)+TextLen,mng1.GetLength(3));  
			byte *output_pre=mng1.RunCipher(true,input_pre,TextLen,key_pre,iv_pre);
			byte *output_test=new byte[TextLen];
			byte *key_test= new byte[mng1.GetLength(3)];
			for(int i=0;i<=mng1.GetLength(3)*8;i++)
			{
				memcpy(key_test,key_pre,mng1.GetLength(3));
				Roll(key_test,i);
				byte *output_test=mng1.RunCipher(true,input_pre,TextLen,key_test,iv_pre);
				Distance+=CalcDistance(output_test,output_pre,TextLen);
			}
			return Distance/mng1.GetLength(3)/TextLen/64;
		}
	}
}
void AverageData(CString AlgType, CString AlgName, CString AlgMode, CString TestType, int TextLen, int TestTimes, double* AverageData)
{
	for(int n=0;n<TestTimes;n++)
	{
		AverageData[n]=CalcAverage(TransformAlgType(AlgType),AlgName,TransformAlgMode(AlgMode),TestType,TextLen);
	}
}
double Average(double *AverageData, int TestTimes)
{
	double Average=0.0;
	for(int n=0;n<=TestTimes;n++)
	{
		Average+=AverageData[n];
	}
	return Average/TestTimes;
}
void VarianceData(double* AverageData, double* VarianceData, int TestTimes)
{
	int aver=Average(AverageData,TestTimes);
	for(int n=0;n<=TestTimes;n++)
	{
		VarianceData[n]=(AverageData[n]-aver)*(AverageData[n]-aver);
	}
}
double Variance(double *VarianceData, int TestTimes)
{
	double Variance=0.0;
	for(int n=0;n<=TestTimes;n++)
	{
		Variance+=VarianceData[n];
	}
	return Variance/TestTimes;
}
void GenerateReport(CString AlgType, CString AlgName, CString AlgMode, CString TestType, int TextLen, int TestTimes, double* AverageData, double Average, double Variance)
{
	fstream report;
	report.open ("SensitivityTestReport.txt",ios::out | ios::trunc);
	report<<"-----------------------------------Sensitivity Test Report-----------------------------------\n";
	if(!AlgType.Compare("分组密码")) report<<"Algorithm Type --- "<<AlgType<<"\tAlgorithm Name --- "<<AlgName<<"\tCipher Mode --- "<<AlgMode<<"\n";
	else report<<"Algorithm Type --- "<<AlgType<<"\tAlgorithm Name --- "<<AlgName<<"\n";
	report<<"Test Type --- "<<TestType<<"\tText Length --- "<<TextLen<<"\tTest Times --- "<<TestTimes<<"\n";
	report<<"Average --- "<<Average<<"\tVariance --- "<<Variance<<"\n";
	report<<"\n\n---------------------------------------------------------------------------------------------\n";
	report<<"---------------------------------------Primary Data------------------------------------------\n\n";
	for(int i=0;i<TestTimes;i++)
	{
		report<<AverageData[i]<<"\t";
	}
	report<<"\n\n---------------------------------------------------------------------------------------------";
	report.close();

	
}
