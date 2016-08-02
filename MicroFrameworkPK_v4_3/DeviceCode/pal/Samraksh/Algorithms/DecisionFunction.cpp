#include "Samraksh\DecisionFunction.h"
#include <cmath>

#define _precision 20 // Q11.20
#define _fx_one (1 << _precision) // 0x100000

extern Data_Store g_dataStoreObject;

INT32 DecisionFunction::Double_to_Fixed(double d) {

    double temp = d * _fx_one;
    temp += (temp >= 0) ? 0.5f : -0.5f;
    return (INT32)temp;
}

UINT64 DecisionFunction::Fixed_Point_Mult(INT32 tmp, INT32 tmp2) {
    UINT64 product = ((INT64)tmp * (INT64)tmp2) >> _precision;
    return product;
}

double DecisionFunction::Fixed_to_Double(UINT64 l) {
    return (double)l/_fx_one;
}

void DecisionFunction::Normalize(float* featureVector, INT32* features_normalized)
{
	for (UINT8 i = 0; i < nFeature; i++)
	{
		features_normalized[i] = Double_to_Fixed((featureVector[i] - feature_min[i]) * scalingFactors[i]);
	}
}

float DecisionFunction::Kernel(UINT32 *sv, INT32* features_normalized)
{
    float res = 0;
	UINT64 sum = 0;

    for (UINT8 i = 0; i < nFeature; i++)
	{
		INT32 tmp = features_normalized[i] - sv[i];
		sum += Fixed_Point_Mult(tmp, tmp);
	}

	double d_sum = Fixed_to_Double(sum);

	res = (float)exp(-gamma*d_sum);
    return res;
}

double DecisionFunction::Decide(INT32* features_normalized)
{
	double decision = -rho;
	RECORD_ID rId = 1;
	
	InitDataStore();
	LPVOID givenPtr = g_dataStoreObject.getAddress(rId); // Hopefully the indexing is corrent, since I wrote at dataRefArray[0] in C#
	UINT32 sv1row[nFeature];

	if (givenPtr)
	{
		for (UINT16 i = 0; i < nSV; i++)
		{
			g_dataStoreObject.readRawData(givenPtr, (void*)sv1row, i*nFeature, nFeature);
			decision += weight[i] * Kernel(sv1row, features_normalized);
		}
	}
	else
	{
		hal_printf("ERROR: No valid pointer to record in data store\n");
	}

	return decision;
}

BOOL DecisionFunction::InitDataStore()
{
	int eraseDataStore = false;
	if (g_dataStoreObject.init(eraseDataStore) != DS_Success)
	{
		hal_printf("ERROR: Could not initialize data store!\n");
		return false;
	}
}
