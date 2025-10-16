/* Extracted from interp.c:58337 (function positive32BitIntegerFor). */

sqInt
positive32BitIntegerFor(unsigned int integerValue)
{
	return ((((((usqInt)integerValue)) & 0xFFFFFFFFU) << 3) | 1);
}