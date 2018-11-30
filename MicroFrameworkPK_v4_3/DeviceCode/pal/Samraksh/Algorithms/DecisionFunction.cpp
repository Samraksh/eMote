#include "Samraksh\DecisionFunction.h"
#include <cmath>

#define _precision 20 // Q11.20
#define _fx_one (1 << _precision) // 0x100000

//extern Data_Store g_dataStoreObject;
float DecisionFunction::rho = (float)-408.947f;
float DecisionFunction::gamma = (float)0.0625;
float* DecisionFunction::weight = NULL;
UINT16 DecisionFunction::nSV = 68;
UINT16 DecisionFunction::nFeature = 13;
float* DecisionFunction::feature_min = NULL;
float* DecisionFunction::scalingFactors = NULL;
const UINT32 dataVectors[68*13] = {
193399, 95239, 82931, 983543, 8729, 130055, 73156, 1061, 0, 0, 0, 0, 0,
180061, 11399, 15827, 422681, 2314, 341395, 308879, 8490, 0, 0, 0, 0, 0,
173393, 676, 4030, 520220, 2571, 40643, 32514, 8490, 4443, 0, 4559, 0, 0,
0, 0, 0, 0, 0, 0, 0, 472, 494, 0, 506, 4315, 513,
164050, 230477, 239862, 455197, 20901, 430807, 430807, 2830, 0, 0, 0, 0, 0,
202920, 7731, 2520, 479577, 4489, 325142, 276373, 9340, 2665, 28672, 2736, 4315, 924,
6156, 13445, 10942, 983543, 12092, 455197, 430807, 531, 186, 0, 190, 4315, 193,
223410, 9018, 14503, 406428, 3962, 373912, 251983, 1698, 0, 0, 0, 0, 0,
135046, 16533, 8800, 1024186, 731, 1016060, 1016060, 2122, 0, 0, 0, 0, 0,
238740, 18219, 17479, 520220, 9538, 503967, 406428, 4246, 1367, 65536, 701, 8630, 711,
256639, 96299, 117325, 528357, 126133, 528357, 487714, 5346, 3291, 163840, 2871, 64727, 2566,
213406, 676, 1499, 260110, 406, 251983, 186961, 12736, 0, 0, 0, 0, 0,
166724, 12472, 9090, 487714, 8256, 487714, 292626, 76414, 22215, 28672, 18236, 12946, 13858,
60019, 45392, 55156, 438944, 16241, 438944, 260110, 25471, 11108, 36864, 6839, 8630, 4619,
0, 0, 0, 0, 0, 0, 0, 472, 494, 0, 506, 4315, 513,
264084, 5773, 4378, 536483, 3345, 373912, 373912, 15566, 5924, 28672, 3040, 4315, 1539,
0, 0, 0, 0, 0, 0, 0, 425, 445, 0, 456, 4315, 462,
88600, 862202, 874806, 926648, 1048576, 918521, 918521, 33962, 2665, 167940, 1823, 4315, 0,
180061, 3988, 3756, 447071, 790, 357659, 81285, 0, 0, 0, 0, 0, 0,
60019, 7058, 13868, 357659, 7714, 349522, 341395, 12736, 8887, 0, 9118, 4315, 4619,
20007, 71350, 59176, 617769, 10354, 512093, 512093, 3183, 1110, 0, 1140, 4315, 1154,
209401, 61911, 69941, 520220, 54607, 520220, 479577, 3183, 1110, 122883, 814, 21576, 825,
185514, 7347, 5797, 373912, 10749, 317016, 276373, 1886, 0, 0, 0, 0, 0,
153386, 5511, 3468, 276373, 15158, 227593, 203214, 33962, 4443, 0, 4559, 0, 0,
216741, 12286, 10386, 495840, 4748, 495840, 251983, 14858, 5554, 32768, 4559, 12946, 3464,
5265, 32556, 35641, 487714, 45072, 398291, 357659, 5493, 3136, 102400, 2414, 38836, 2445,
35011, 116109, 100445, 503967, 27529, 503967, 178824, 19104, 2222, 0, 2280, 4315, 2310,
81626, 28210, 20632, 479577, 15049, 471450, 390165, 1661, 580, 32768, 595, 8630, 201,
4001, 441199, 420060, 780340, 354838, 593379, 568999, 50943, 22215, 53248, 18236, 43151, 12318,
200068, 13633, 10879, 520220, 2112, 349522, 0, 0, 0, 0, 0, 0, 0,
190065, 5053, 4562, 471450, 2873, 365785, 365785, 2122, 0, 0, 0, 0, 0,
148845, 29765, 23923, 390165, 8219, 390165, 373912, 923, 193, 0, 198, 4315, 201,
229303, 18636, 22314, 471450, 30684, 414555, 365785, 3183, 1110, 98304, 950, 12946, 578,
330113, 59205, 58550, 528357, 36315, 528357, 512093, 3396, 1333, 126972, 912, 12946, 462,
190065, 6461, 7692, 349522, 5865, 341395, 341395, 12736, 2222, 0, 1140, 0, 0,
441471, 50647, 53845, 430807, 35732, 390165, 373912, 3265, 1026, 28672, 701, 4315, 355,
111464, 10052, 13791, 365785, 26318, 300753, 243857, 12736, 4443, 32768, 3647, 8630, 1848,
144043, 19129, 18246, 487714, 5336, 357659, 325142, 12736, 1481, 0, 0, 0, 0,
195067, 28443, 29933, 430807, 5929, 341395, 341395, 708, 0, 0, 0, 0, 0,
140048, 3577, 5716, 544609, 10287, 455197, 365785, 76414, 31102, 0, 13677, 8630, 9239,
278271, 8523, 8902, 503967, 3765, 479577, 471450, 8018, 3949, 61440, 3040, 17261, 1539,
135046, 28772, 27465, 512093, 15348, 251983, 251983, 0, 0, 0, 0, 0, 0,
43347, 9458, 10121, 414555, 16692, 414555, 390165, 20165, 8887, 49152, 7979, 17261, 4619,
185776, 11222, 9472, 398291, 14683, 276373, 268236, 7641, 2665, 0, 1823, 8630, 1848,
248083, 21504, 22517, 447071, 30196, 349522, 284500, 6368, 1666, 40961, 1140, 4315, 578,
13918, 43822, 45668, 503967, 86019, 503967, 455197, 7277, 4866, 192508, 4559, 77672, 3959,
165392, 25308, 30328, 463324, 45757, 447071, 398291, 6532, 2735, 61440, 2104, 17261, 1066,
458721, 51513, 53049, 634022, 115406, 634022, 528357, 6694, 4956, 196608, 4734, 94933, 3731,
110038, 50700, 48859, 487714, 14090, 414555, 373912, 12736, 4443, 0, 4559, 4315, 2310,
188062, 17928, 22463, 463324, 3093, 455197, 422681, 4246, 1481, 0, 1519, 4315, 1539,
181603, 6130, 6243, 430807, 8875, 430807, 268236, 2315, 404, 40961, 0, 0, 0,
65021, 11225, 19538, 292626, 284, 292626, 292626, 19104, 8887, 0, 9118, 4315, 2310,
193399, 5897, 11619, 260110, 948, 251983, 235730, 25471, 0, 0, 0, 0, 0,
51445, 17191, 21672, 1016060, 4791, 552736, 406428, 4246, 888, 0, 912, 0, 0,
45729, 14618, 14961, 430807, 11760, 365785, 308879, 8490, 2665, 40961, 1823, 4315, 924,
221501, 38869, 31259, 414555, 34787, 373912, 357659, 3538, 1110, 53248, 760, 4315, 385,
270092, 15540, 9763, 503967, 3556, 341395, 341395, 4952, 1481, 0, 760, 0, 0,
228076, 12900, 12262, 438944, 9068, 292626, 203214, 12736, 1481, 0, 1519, 4315, 1539,
0, 0, 0, 0, 0, 0, 0, 386, 404, 0, 414, 4315, 419,
160055, 2417, 1218, 341395, 0, 251983, 48771, 4246, 0, 0, 0, 0, 0,
68356, 105812, 112208, 471450, 42884, 447071, 398291, 8915, 3110, 73729, 2280, 17261, 1848,
106703, 729840, 716303, 959164, 597279, 812846, 658411, 93396, 48874, 57345, 18236, 12946, 13858,
0, 0, 0, 0, 0, 0, 0, 212, 222, 0, 228, 4315, 231,
265541, 12938, 14120, 414555, 4719, 414555, 414555, 3774, 1481, 57345, 506, 0, 0,
230079, 5302, 6466, 487714, 1973, 487714, 195088, 13797, 7775, 53248, 5699, 17261, 4619,
386012, 31771, 37902, 463324, 15916, 373912, 325142, 2830, 592, 36864, 304, 4315, 0,
28899, 28474, 30851, 512093, 21153, 512093, 308879, 13948, 6982, 61440, 6513, 30206, 4619,
100032, 13922, 10121, 211341, 5157, 195088, 195088, 19104, 2222, 0, 0, 0, 0,
};

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
		//features_normalized[i] = Double_to_Fixed((featureVector[i] - feature_min[i]) * scalingFactors[i]);
		norm_feat = (featureVector[i] - feature_min[i]) * scalingFactors[i];
		features_normalized[i] = Double_to_Fixed(norm_feat<0?0:(norm_feat>1?1:norm_feat));
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
	
	//InitDataStore();
	//Assuming that the support vector is always written to the first block in datastore
	UINT32 sv1row[nFeature] = {0};
	//UINT8 sv1row_bytes[nFeature * sizeof(UINT32)] = {0};
	ushort dataIdOffset = 0;
	ushort arrayLength = 1;
	UINT32 dataIDArray[arrayLength] = {0};
	//g_dataStoreObject.getRecordIDAfterPersistence(dataIDArray, arrayLength, dataIdOffset);
	//LPVOID givenPtr = g_dataStoreObject.getAddress(dataIDArray[0]);

	//if (givenPtr)
	{
		for (UINT16 i = 0; i < nSV; i++)
		{
			//g_dataStoreObject.readRawData(givenPtr, (void*)sv1row_bytes, i*nFeature*sizeof(UINT32), nFeature*sizeof(UINT32));
			//UINT16 offset = 0;
			for (UINT8 j = 0; j < nFeature; j++)
			{
				//sv1row[i] = Bytes_to_UINT32(sv1row_bytes, offset);
				sv1row[j] = dataVectors[i * nFeature + j];
				//offset += sizeof(UINT32);
			}
			decision += weight[i] * Kernel(sv1row, features_normalized);
			sv1row[nFeature] = {0};
			//sv1row_bytes[nFeature * sizeof(UINT32)] = {0};
		}
	}
	/*else
	{
		hal_printf("ERROR: No valid pointer to record in data store\n");
	}*/

	return decision;
}

BOOL DecisionFunction::InitDataStore()
{
	/*int eraseDataStore = false;
	if (g_dataStoreObject.init(eraseDataStore) != DS_Success)
	{
		hal_printf("ERROR: Could not initialize data store!\n");
		return false;
	}*/
}


