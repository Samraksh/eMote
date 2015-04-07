/*
 * GlobalTime.h
 *
 *  Created on: April 05, 2013
 *      Author: Mukundan Sridharan
 */


#ifndef GLOBALTIME_H_
#define GLOBALTIME_H_

#include <tinyhal.h>

#define MAX_SAMPLES 8
#define MAX_NBR 6
#define INVALID_TIMESTAMP 0x7FFFFFFFFFFFFFFF
#define UnknownRelativeFreq 255

extern UINT16 MF_NODE_ID;

struct TSSamples {
	UINT16 nbrID;
	INT64 localTime[MAX_SAMPLES];
	INT64 offsetBtwNodes[MAX_SAMPLES];
	INT64 lastLocalTime;
	UINT8 localTimeIndex;
	UINT8 numSamples;
	INT64 localTimeAvg;
	INT64 offsetAvg;
	float avgSkew;
	float relativeFreq;
};

class Regression {
private:
	UINT8 nbrCount;

	void Compute(UINT16 nbr){
		INT64 newLocalAverage, newOffsetAverage, localSum, localAverageRest;
		INT64 offsetSum, offsetAverageRest;
		INT64 latestLocalTime, earliestLocalTime;
		INT64 simpleOffsetChangeSum, simpleCurrentOffsetDiff , simpleCurrentLocalTimeDiff;

		UINT16 nbrIndex = FindNbr(nbr);
		if(nbrIndex==255){
			return;
		}
		INT64 *nbrLocalTimes = samples[nbrIndex].localTime;
		INT64 *nbrOffset = samples[nbrIndex].offsetBtwNodes;
		UINT8 numSamples = samples[nbrIndex].numSamples;


		//We use a rough approximation first to avoid time overflow errors. The idea
		//is that all times in the table should be relatively close to each other.
		//i is the first nonempty entry in the table
	    UINT8 i,j;
	    for(i = 0; i < MAX_SAMPLES && nbrLocalTimes[i] == INVALID_TIMESTAMP; ++i);


	    UINT8 firstIdx, lastIdx;
	    firstIdx = lastIdx = i;
	    //hal_printf("fIdx init to %u\n", firstIdx);
	    newLocalAverage = nbrLocalTimes[i];
		newOffsetAverage = nbrOffset[i];

		localSum = 0;
		localAverageRest = 0;
		offsetSum =0;
		offsetAverageRest =0;
		simpleOffsetChangeSum=0;
		while( ++i < MAX_SAMPLES ) {
			if( nbrLocalTimes[i] != INVALID_TIMESTAMP ) {
				//This only works because C ISO 1999 defines the sign for modulo the
				//same as for the Dividend! This approximates dividing using only integers.

				localSum += (INT64)(nbrLocalTimes[i] - newLocalAverage) / numSamples;
				localAverageRest += (nbrLocalTimes[i] - newLocalAverage) % numSamples;
				offsetSum += (INT64)(nbrOffset[i] - newOffsetAverage) / numSamples;
				offsetAverageRest += (nbrOffset[i] - newOffsetAverage) % numSamples;

			}
		}
		newLocalAverage += localSum + localAverageRest / numSamples;
		newOffsetAverage += offsetSum + offsetAverageRest / numSamples;

		//Find latestLocalTime and earliestLocalTime in the buffer
		if (numSamples >= 2) {
			latestLocalTime = nbrLocalTimes[lastIdx];
			earliestLocalTime = nbrLocalTimes[firstIdx];
		    for (i = 0; i < MAX_SAMPLES; i++) {
		    	if( nbrLocalTimes[i] != INVALID_TIMESTAMP ) {
		    		if (nbrLocalTimes[i] > latestLocalTime) {
		    			latestLocalTime = nbrLocalTimes[i];
		    			lastIdx = i;
		    		}
		    		if (nbrLocalTimes[i] < earliestLocalTime) {
		    			earliestLocalTime = nbrLocalTimes[i];
		    			firstIdx = i;
		    		}
		    	}
		    }
		}

		//samples[nbrIndex].avgSkew  = (float)(nbrOffset[lastIdx] - nbrOffset[firstIdx]) /
		//		(float)(nbrLocalTimes[lastIdx]- nbrLocalTimes[firstIdx]);

		simpleOffsetChangeSum=0;
		for (int i =firstIdx; i<=lastIdx ; i++){
			//simpleCurrentLocalTimeDiff =nbrLocalTimes[(i+1) % MAX_SAMPLES] - nbrLocalTimes[i]
			simpleCurrentOffsetDiff = nbrOffset[(i+1) % MAX_SAMPLES] - nbrOffset[i];
			simpleOffsetChangeSum += simpleCurrentOffsetDiff;
			//samples[nbrIndex].relativeFreq += nbrLocalTimes[i]/(nbrLocalTimes[i]-nbrOffset[(i+1) % MAX_SAMPLES])
		}
		samples[nbrIndex].avgSkew  = (float)(simpleOffsetChangeSum) /
						(float)(nbrLocalTimes[lastIdx]- nbrLocalTimes[firstIdx]);


		samples[nbrIndex].relativeFreq = 0;
		for (i = firstIdx; i != lastIdx ; i = ((i+1) % MAX_SAMPLES)){
			if( nbrLocalTimes[i] != INVALID_TIMESTAMP ) {
				for  (j = ((i+1) % MAX_SAMPLES); j != firstIdx  ; j = ((j+1) % MAX_SAMPLES)) {
					if( nbrLocalTimes[j] != INVALID_TIMESTAMP ) {
						if(nbrLocalTimes[i] < nbrLocalTimes[j] ) {
							simpleCurrentLocalTimeDiff = nbrLocalTimes[(i+1) % MAX_SAMPLES] - nbrLocalTimes[i];
							simpleCurrentOffsetDiff = nbrOffset[(i+1) % MAX_SAMPLES] - nbrOffset[i];
							samples[nbrIndex].relativeFreq += (float) simpleCurrentLocalTimeDiff/((float) (simpleCurrentLocalTimeDiff - simpleCurrentOffsetDiff));
						}
						break;
					}
				}
			}
		}
		samples[nbrIndex].relativeFreq = samples[nbrIndex].relativeFreq / (numSamples-1);

		samples[nbrIndex].localTimeAvg = newLocalAverage;
		samples[nbrIndex].offsetAvg = newOffsetAverage;
		//hal_printf("GlobalTime: Avg Drift: %lu, Avg Skew: %f \n",samples[nbrIndex].offsetAvg, samples[nbrIndex].avgSkew);
	}

public:
	TSSamples samples[MAX_NBR];
	INT64 Last_Adjust_localtime;

	UINT8 FindNbr(UINT16 nbr){
		for(int i=0; i < MAX_NBR; i++){
			if (samples[i].nbrID == nbr){
				return i;
			}
		}
		return 255;
	}


	//Mean algorithm. My global value is just the mean of all  global values
	INT64 Local2Global(INT64 time){
		INT64 sum;
		INT64 globalSum = 0;
		INT64 globalAverageRest = 0;
		for (int i=0; i< MAX_NBR; i++){

			if(samples[i].nbrID!=0xFFFF){
				INT64 x = NbrTime(samples[i].nbrID, time);
				//INT64 x = samples[i].lastLocalTime + (INT64) (samples[i].avgSkew * (time - samples[i].localTimeAvg));
				globalSum += (INT64) x/(nbrCount+1);
				globalAverageRest += x % (nbrCount+1);
			}

		}
		//add your value
		globalSum += (INT64) time/(nbrCount+1);
		globalAverageRest += time % (nbrCount+1);
		globalSum += globalAverageRest /(nbrCount+1);
		return globalSum;
	}

	INT64 NbrTime(UINT16 nbr, INT64 time){
		INT64 retTime=time;
		INT64 adjskew =0;
		UINT16 nbrIndex = FindNbr(nbr);

		if(nbrIndex != 255){
			//implementing ftsp logic
			//Look at http://docs.tinyos.net/tinywiki/index.php/FTSP
			adjskew = (INT64) (samples[nbrIndex].avgSkew * (time - samples[nbrIndex].localTimeAvg));
			retTime += samples[nbrIndex].offsetAvg + adjskew ;
		}
		//hal_printf("Local: %lld, AvgOffset: %lld, AdjSkew: %lld, Nbr: %lld \n", time, samples[nbrIndex].offsetAvg, adjskew, retTime);
		return retTime;
	}
	INT64 SkewAdjustment(UINT16 nbr, INT64 curTime){
		INT64 ret;
		UINT8 nbrIndex = FindNbr(nbr);
		ret = (INT64) (samples[nbrIndex].avgSkew * (curTime - samples[nbrIndex].localTimeAvg));
		ret = (INT64) (samples[nbrIndex].avgSkew * (curTime - Last_Adjust_localtime));
		return ret ;
	}

	void Insert(UINT16 nbr,INT64 nbr_ltime, INT64 nbr_loffset, INT64 localtime){
		UINT16 nbrIndex = FindNbr(nbr);
		//Add new neighbor if not found
		if (nbrIndex ==255){
			nbrIndex=nbrCount;
			nbrCount++;
			//No space in regression table
			if(nbrCount > MAX_NBR){
				return;
			}
		}
		if (samples[nbrIndex].lastLocalTime == nbr_ltime) { //BK:If receiving the same message again don't insert
			return;
		}
		else{
			samples[nbrIndex].nbrID = nbr;
			samples[nbrIndex].lastLocalTime = nbr_ltime;
			samples[nbrIndex].localTime[samples[nbrIndex].localTimeIndex] = nbr_ltime;
			samples[nbrIndex].offsetBtwNodes[samples[nbrIndex].localTimeIndex] = nbr_loffset;
			samples[nbrIndex].localTimeIndex++;
			samples[nbrIndex].localTimeIndex =samples[nbrIndex].localTimeIndex % MAX_SAMPLES;
			if(samples[nbrIndex].numSamples < MAX_SAMPLES){
				samples[nbrIndex].numSamples++;
			}
			Compute(nbr);
		}

	}
	void Clean(UINT16 nbr){
		UINT16 nbrIndex = FindNbr(nbr);
		samples[nbrIndex].nbrID=0xFFFF;
		for(int i=0; i< MAX_SAMPLES; i++){
			samples[nbrIndex].localTime[i] =INVALID_TIMESTAMP;
		}
		//memset(samples[nbrIndex].localTime,INVALID_TIMESTAMP,MAX_SAMPLES);
		samples[nbrIndex].numSamples=0;
	}
	void Init(){
		nbrCount=0;
		for (int ii=0; ii< MAX_NBR; ii++){
			samples[ii].nbrID=0xFFFF;
			for(int i=0; i< MAX_SAMPLES; i++){
				samples[ii].localTime[i] =INVALID_TIMESTAMP;
			}
		}
	}

	float FindRelativeFreq(UINT16 nbr) {
		float rv = UnknownRelativeFreq;
		UINT16 nbrIndex = FindNbr(nbr);
		if(nbrIndex==255){
			return rv;
		}
		return(samples[nbrIndex].relativeFreq);
	};
};



class GlobalTime {
public:
	static INT64 offset;
	static float skew;
	static Regression regressgt;
	static BOOL synced;
	static UINT16 leader;

	void Init();

	INT64 Read(){
		INT64 rtn;
		rtn = HAL_Time_CurrentTime();
		//if(leader!=MF_NODE_ID || leader!=0xFFFF){
		//	return regressgt.NbrTime(leader,rtn);
		//}
		return  rtn;
	};
	float GetSkew();
	BOOL Adjust(UINT64 value, INT64 localtime, BOOL add);
	//INT64 Local2Global(INT64 local);
	//INT64 Global2Local(INT64 global);
	INT64 Local2NbrTime(UINT16 nbr, INT64 localtime);
	INT64 Nbr2LocalTime(UINT16 nbr, INT64 nbrtime);
};



INT64 GlobalTime::Local2NbrTime(UINT16 nbr, INT64 time){
	return regressgt.NbrTime(nbr,time);
}

/* static INT64 GlobalTime::Read(){
	INT64 rtn;
	rtn = HAL_Time_CurrentTime();
	if(leader!=MF_NODE_ID || leader!=0xFFFF){
		return regressgt.NbrTime(leader,rtn);
	}
	return  rtn;
} */
void GlobalTime::Init(){
	regressgt.Init();
	leader = 0xFFFF;
	offset=0;
}
float GlobalTime::GetSkew(){
	return skew;
}
BOOL GlobalTime::Adjust(UINT64 value, INT64 localtime, BOOL add){
	regressgt.Last_Adjust_localtime = localtime;
	if(add){
		offset+=value;
	}else{
		offset = offset - value;
	}
	return TRUE;
}

/*
INT64 GlobalTime::Local2Global(INT64 local){

	return regressgt.Local2Global(local);
}
*/
INT64 GlobalTime::Nbr2LocalTime(UINT16 nbr, INT64 nbrTime){
	return nbrTime - offset;
}

#endif //GLOBALTIME_H_
