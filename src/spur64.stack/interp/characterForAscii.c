/* Extracted from interp.c:50378 (function characterForAscii). */

sqInt
characterForAscii(sqInt ascii)
{
	return ((((usqInt)(ascii) << (numTagBits())))) + (characterTag());
}