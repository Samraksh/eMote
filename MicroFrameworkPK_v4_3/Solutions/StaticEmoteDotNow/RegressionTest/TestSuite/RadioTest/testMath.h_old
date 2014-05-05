
#define PRECISION 0.001
#define SUCCESS 1
#define FAIL 0

class testMath
{
	
public:
	int multiplier;
	int increment;
	int modulus;
	int seedValue;
	bool prngInitialized;
	long randomValue;
	

	// Initializes the prng gives the ability to initialize multiplier, increment, modulus and seedValue
	void prng_init(int Multiplier, int Increment, int Modulus, int SeedValue)
	{
		prngInitialized = true;
		multiplier = Multiplier;
		increment = Increment;
		modulus = Modulus;
		seedValue = SeedValue;
	}

	// Uses preinitialized multiplier, increment and modulus values
	void prng_init(int SeedValue)
	{
		prngInitialized = true;
		multiplier = 1140671485; // 2 ^ 24
		increment = 12820163;
		modulus = 16777216;
		seedValue = SeedValue;
		randomValue = (double) SeedValue;
	}

	long prng()
	{

		if(!prngInitialized)
		{
			hal_printf("\nRandom Number Generator Not Initialized");
			return -1;
		}
		
		//printf("\n%ld",multiplier * randomValue);

		randomValue = (long) (( multiplier * randomValue + increment) % (modulus));
		if(randomValue < 0)
			return prng();
		return randomValue;

	}
	
	// Generates the sqrroot using the newtons law
	static float sqrroot(int number)
	{
		float initial = 1;
		float currValue = initial;
		float newValue = 0;

		while(true)
		{
			newValue = currValue - ((currValue * currValue) - number)/(2 * currValue);
			if(newValue > currValue)
			{
				if((newValue - currValue) < PRECISION)
					return newValue;
				else
					currValue = newValue;
			}
			else if(currValue > newValue)
			{
				if((currValue - newValue) < PRECISION)
					return currValue;
				else
					currValue = newValue;
			}
		}
	}
};


