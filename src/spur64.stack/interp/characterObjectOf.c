/* Extracted from interp.c:32010 (function characterObjectOf). */

	/* SpurMemoryManager>>#characterObjectOf: */

sqInt
characterObjectOf(sqInt characterCode)
{
	return ((((usqInt)(characterCode) << (numTagBits())))) + (characterTag());
}