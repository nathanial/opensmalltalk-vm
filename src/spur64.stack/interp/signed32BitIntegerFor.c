/* Extracted from interp.c:63393 (function signed32BitIntegerFor). */

sqInt
signed32BitIntegerFor(sqInt integerValue)
{
	return (((usqInt)(((int) integerValue)) << 3) | 1);
}