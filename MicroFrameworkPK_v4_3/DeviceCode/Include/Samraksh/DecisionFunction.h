#include <tinyhal.h>
#include <Samraksh\DataStore\Datastore.h>
#include <Samraksh\HAL_util.h>
#include <cstdlib>

class DecisionFunction
{
	static const UINT16 nSV = 305;
	static const UINT16 nFeature = 12;

	static float rho;
	static float gamma;
	
	static float weight[nSV];
	static float feature_min[nFeature];
	static float scalingFactors[nFeature];

	static float Kernel(UINT32*, UINT32*);
	static BOOL InitDataStore();

	static UINT32 Bytes_to_UINT32(UINT8*, UINT16);

    // Fixed point functions
    static INT32 Double_to_Fixed(double);
    static UINT64 Fixed_Point_Mult(INT32, INT32);
    static double Fixed_to_Double(UINT64);

public:
	static void Normalize(float*, UINT32*);
	static double Decide(UINT32*);
};

float DecisionFunction::rho = (float)1.8982;
float DecisionFunction::gamma = (float)5;

float DecisionFunction::weight[] = { 45.3179, 100, 37.2111, 62.339, 23.6049, 10.214, 100, 20.8927, 100, 14.8125, 100, 72.3573, 100, 2.7467, 3.2113, 11.5475, 100, 100, 38.0853, 10.2794, 100, 100, 39.325, 0.82681, 16.425, 100, 24.7883, 100, 89.7547, 100, 67.8498, 100, 1.6664, 100, 4.6063, 100, 51.6598, 100, 11.8902, 50.9415, 100, 10.1132, 39.6788, 26.7678, 29.3656, 23.0826, 4.8856, 100, 100, 2.8382, 100, 73.7081, 3.7688, 4.2869, 100, 58.3415, 36.1982, 4.3762, 45.2888, 100, 30.5753, 72.4488, 100, 66.6348, 29.8444, 65.9437, 19.2592, 100, 27.097, 2.8956, 100, 100, 100, 42.7856, 100, 44.8362, 58.6795, 100, 29.6645, 12.8607, 6.6399, 72.3494, 35.2729, 74.4404, 10.4785, 52.6469, 15.1341, 78.4004, 100, 100, 100, 54.0343, 100, 100, 58.5493, 32.8313, 1.1825, 93.2915, 51.1955, 2.8293, 13.0914, 88.1233, 100, 35.3094, 3.2723, 76.5911, 100, 50.3435, 7.0347, 100, 100, 78.8862, 100, 12.8327, 100, 95.0199, 2.0949, 2.4798, 100, 28.6728, 76.1068, 10.7909, 54.9354, 100, 100, 40.6945, 21.2803, 100, 86.1012, 20.9871, 12.4707, 72.4488, 37.6406, 14.5519, 21.604, 18.4597, 35.4954, 100, 13.8337, 15.0797, 100, 1.9311, 17.536, 100, 9.4933, 57.117, 55.2889, 40.5419, 89.4944, 97.1698, 16.4413, 12.6871, 100, -7.6978, -100, -100, -100, -100, -100, -100, -59.0625, -6.2342, -14.73, -11.9713, -85.3406, -3.8988, -91.7245, -0.072354, -74.2061, -17.0527, -100, -10.0595, -27.151, -100, -5.9004, -100, -100, -3.1696, -27.5292, -4.7762, -100, -5.4488, -100, -25.0169, -61.1735, -16.3381, -41.9145, -100, -100, -43.8892, -100, -100, -48.3424, -100, -77.1996, -9.6622, -43.0998, -100, -24.9221, -100, -6.7043, -16.8742, -28.8998, -100, -45.4974, -100, -16.5241, -3.2821, -100, -100, -88.7518, -46.1943, -1.6, -100, -15.3227, -100, -100, -56.3659, -53.1459, -4.4329, -100, -100, -31.8179, -80.6984, -57.8104, -0.36888, -55.9421, -5.1483, -38.4281, -100, -100, -11.872, -18.9387, -3.0281, -3.8693, -34.4296, -15.2556, -3.0206, -100, -44.8078, -12.1343, -100, -42.5527, -46.2366, -26.1684, -10.9376, -37.1955, -100, -100, -100, -13.8479, -35.8014, -2.374, -89.7047, -47.0547, -100, -100, -100, -100, -19.0503, -33.8813, -22.4559, -86.7429, -40.648, -100, -7.438, -100, -18.8494, -100, -16.8991, -17.4661, -68.7594, -65.578, -6.2014, -99.879, -100, -14.4173, -6.2079, -43.1554, -100, -100, -100, -12.867, -54.8777, -18.4204, -12.4855, -62.1768, -100, -11.5221, -100, -28.5773, -100, -11.215, -33.608, -74.3012, -79.3675, -100, -86.9026, -44.6418, -93.3171, -100, -72.9061, -77.5182, -14.1099, -4.7221 };
float DecisionFunction::feature_min[] = { 487071.4276, 3352.9162, 80, 8, 1.7564, 60, -148, 41, 14, 3, -1, -41 };
float DecisionFunction::scalingFactors[] = { 2.0944e-14, 2.2021e-06, 5.5047e-07, 0.0040323, 7.0137e-05, 0.014706, 0.0043353, 0.011494, 0.0087719, 2.8774e-06, 0.0077519, 0.0096774 };
