#include "Component\DllApi\AlgDllApi.h"
#include <time.h>
#include "string.h"
#include "stdafx.h"
typedef unsigned char byte;
void Roll(byte* str, int index);
void CountChangedBit(byte a,byte b,int *count);
double CalcDistance(byte *a,byte *b,int len);
int TransformAlgMode(CString AlgMode);
int TransformAlgType(CString AlgType);
double CalcAverage(int AlgType, CString AlgName, int AlgMode, CString TestType, int TextLen);
void AverageData(CString AlgType, CString AlgName, CString AlgMode, CString TestType, int TextLen, int TestTimes, double* AverageData);
double Average(double *AverageData, int TestTimes);
void VarianceData(double* AverageData, double* VarianceData, int TestTimes);
double Variance(double *VarianceData, int TestTimes);
void GenerateReport(CString AlgType, CString AlgName, CString AlgMode, CString TestType, int TextLen, int TestTimes, double* AverageData, double Average, double Variance);