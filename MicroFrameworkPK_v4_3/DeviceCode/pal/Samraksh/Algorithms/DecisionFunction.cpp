#include "Samraksh\DecisionFunction.h"
#include <cmath>

#define _precision 20 // Q11.20
#define _fx_one (1 << _precision) // 0x100000

extern Data_Store g_dataStoreObject;
float DecisionFunction::rho = (float)1.8982;
float DecisionFunction::gamma = (float)5;
float* DecisionFunction::weight = NULL;
UINT16 DecisionFunction::nSV = 0;
UINT16 DecisionFunction::nFeature = 0;
float* DecisionFunction::feature_min = NULL;
float* DecisionFunction::scalingFactors = NULL;

void DecisionFunction::Initialize(UINT16 p_nSV, UINT16 p_nFeature, float p_rho, float p_gamma, float* p_weight, float* p_feature_min, float* p_scalingFactors)
{
	nSV = p_nSV;
    nFeature = p_nFeature;
    rho = p_rho;
    gamma = p_gamma;
    
    // Initialize weights, feature minima and scaling factors
    if(!weight) weight = (float*)(private_malloc(nSV*sizeof(float)));
    if(!feature_min) feature_min = (float*)private_malloc(nFeature*sizeof(float));
    if(!scalingFactors) scalingFactors = (float*)private_malloc(nFeature*sizeof(float));
    
    for(UINT16 i=0; i < nSV; i++)
    {
        weight[i] = p_weight[i];
    }
    
    for(UINT8 i=0; i < nFeature; i++)
    {
        feature_min[i] = p_feature_min[i];
        scalingFactors[i] = p_scalingFactors[i];
    }
}

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

UINT32 DecisionFunction::Bytes_to_UINT32(UINT8* value, UINT16 offset)
{
	return (UINT32)(
		value[0 + offset] << 0 |
		value[1 + offset] << 8 |
		value[2 + offset] << 16 |
		value[3 + offset] << 24);
}

void DecisionFunction::Normalize(float* featureVector, UINT32* features_normalized)
{
	for (UINT8 i = 0; i < nFeature; i++)
	{
		features_normalized[i] = Double_to_Fixed((featureVector[i] - feature_min[i]) * scalingFactors[i]);
	}
}

float DecisionFunction::Kernel(UINT32 *sv, UINT32* features_normalized)
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

double DecisionFunction::Decide(UINT32* features_normalized)
{
	double decision = -rho;
	
	InitDataStore();
	//Assuming that the support vector is always written to the first block in datastore
	UINT32 sv1row[nFeature] = {0};
	UINT8 sv1row_bytes[nFeature * sizeof(UINT32)] = {0};
	ushort dataIdOffset = 0;
	ushort arrayLength = 1;
	UINT32 dataIDArray[arrayLength] = {0};
	g_dataStoreObject.getRecordIDAfterPersistence(dataIDArray, arrayLength, dataIdOffset);
	LPVOID givenPtr = g_dataStoreObject.getAddress(dataIDArray[0]);

	if (givenPtr)
	{
		for (UINT16 i = 0; i < nSV; i++)
		{
			g_dataStoreObject.readRawData(givenPtr, (void*)sv1row_bytes, i*nFeature*sizeof(UINT32), nFeature*sizeof(UINT32));
			UINT16 offset = 0;
			for (UINT8 i = 0; i < nFeature; i++)
			{
				sv1row[i] = Bytes_to_UINT32(sv1row_bytes, offset);
				offset += sizeof(UINT32);
			}
			decision += weight[i] * Kernel(sv1row, features_normalized);
			sv1row[nFeature] = {0};
			sv1row_bytes[nFeature * sizeof(UINT32)] = {0};
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


