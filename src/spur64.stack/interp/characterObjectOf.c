/* Extracted from interp.c:32010 (function characterObjectOf). */

sqInt
characterObjectOf(sqInt characterCode)
{
	return ((((usqInt)(characterCode) << (numTagBits())))) + (characterTag());
}