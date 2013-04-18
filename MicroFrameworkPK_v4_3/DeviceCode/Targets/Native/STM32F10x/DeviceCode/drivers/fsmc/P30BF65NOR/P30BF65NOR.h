/*
 *
 *		Name	: P30BF65NOR.h
 *
 *		Author  : nived.sivadas@samraksh.com
 *
 *      Description  : contains the api for the numonyx axcell p30-65nm flash memory
 *
 */

#define FLASH_RESET 107


struct P30BF65NOR_Driver
{

	static const GPIO_PIN kreset = FLASH_RESET;
	static const GPIO_PIN knoe = FLASH_NOE;
	static const GPIO_PIN kwait = FLASH_WAIT;
	static const GPIO_PIN knwe	= FLASH_NWE;

public:

	void Initialize(void);

	void ReadID();

};

