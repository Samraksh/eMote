#include "Samraksh\DecisionFunction.h"
#include <cmath>

extern Data_Store g_dataStoreObject;

void DecisionFunction::Normalize(float* featureVector, INT32* features_normalized)
{
	for (UINT8 i = 0; i < nFeature; i++)
	{
		features_normalized[i] = (INT32)((featureVector[i] - feature_min[i]) * scalingFactors[i] * 10000000); // Multiplying by 10^7 to maintain parity with INT32 support vectors
	}
}

float DecisionFunction::Kernel(UINT32 *sv, INT32* features_normalized)
{
	UINT32 sum = 0;
	for (UINT8 i = 0; i < nFeature; i++)
	{
		UINT32 tmp = features_normalized[i] - sv[i];
		sum += tmp * tmp;
	}

	sum /= ((double)10000000); // Scaling down, typecasting to double for exponentiation

	return (float)exp(-gamma*sum);
}

float DecisionFunction::Decide(INT32* features_normalized)
{
	float decision = -rho;
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
