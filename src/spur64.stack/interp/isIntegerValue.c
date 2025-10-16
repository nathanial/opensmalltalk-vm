/* Extracted from interp.c:27356 (function isIntegerValue). */

int
isIntegerValue(sqInt intValue)
{
	return (((((usqInt)(intValue)) >> 60) + 1) & 15) <= 1;
}