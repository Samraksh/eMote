/*
 * SeedGenerator.h
 *
 *  Created on: May 1, 2013
 *      Author: Mukundan
 */

#ifndef SEEDGENERATOR_H_
#define SEEDGENERATOR_H_

//This implementation of the class copied from Wenjie's tinyos code.
//This is a bad generator, this needs to replaced
class SeedGenerator {

public:
	/* Return the next 16 bit random number */
	static UINT16 Rand(UINT16 *seedRet, UINT16 nodeID) {
		UINT16 mask = 137 * 29 * (nodeID + 1);
		bool endbit;
		UINT16 tmpseed;

		//Rest of the function needs to be atomic
		//GLOBAL_LOCK();
		tmpseed = *seedRet;
		endbit = ((tmpseed & 0x8000) != 0);
		tmpseed <<= 1;
		if (endbit) {
			tmpseed ^= 0x100b;
		}
		tmpseed++;
		*seedRet = tmpseed;
		tmpseed = tmpseed ^ mask;

		return tmpseed;
	}

	static UINT16 RandWithMask(UINT16 *seedRet, UINT16 mask) {
		bool endbit;
		UINT16 tmpseed;

		//Rest of the function needs to be atomic
		//GLOBAL_LOCK();
		tmpseed = *seedRet;
		endbit = ((tmpseed & 0x8000) != 0);
		tmpseed <<= 1;
		if (endbit) {
			tmpseed ^= 0x100b;
		}
		tmpseed++;
		*seedRet = tmpseed;
		tmpseed = tmpseed ^ mask;
		return tmpseed;
	}


	static UINT16 RandWithMask_Dummy(UINT16 *seedRet, UINT16 mask) {
		UINT16 rv;
		rv = *seedRet;
		*seedRet = (*seedRet+1) * (*seedRet+10) ;
		return rv;

	}
};


#endif /* SEEDGENERATOR_H_ */
