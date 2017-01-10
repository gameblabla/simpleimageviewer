#include <stdint.h>

uint32_t __udivmodsi4(uint32_t num, uint32_t den, uint32_t * rem_p);
signed int __aeabi_idiv(signed int num, signed int den);

/*
* 32-bit signed integer divide.
*/
signed int __aeabi_idiv(signed int num, signed int den)
{
	signed int minus = 0;
	signed int v;
	
	if (num < 0)
	{
		num = -num;
		minus = 1;
	}
	if (den < 0)
	{
		den = -den;
		minus ^= 1;
	}
	
	v = __udivmodsi4(num, den, 0);
	if (minus)
		v = -v;
	
	return v;
}

uint32_t __udivmodsi4(uint32_t num, uint32_t den, uint32_t * rem_p)
{
	uint32_t quot = 0, qbit = 1;
	
	if (den == 0) 
	{
		return 0; /* If trap returns... */
	}
	
	/* Left-justify denominator and count shift */
	while ((int32_t) den >= 0) 
	{
		den <<= 1;
		qbit <<= 1;
	}
	
	while (qbit) 
	{
		if (den <= num)
		{
			num -= den;
			quot += qbit;
		}
		den >>= 1;
		qbit >>= 1;
	}
	
	if (rem_p)
		*rem_p = num;
	
	return quot;
}

unsigned int __aeabi_uidiv(unsigned int num, unsigned int den)
{
	return __udivmodsi4(num, den, 0);
}
