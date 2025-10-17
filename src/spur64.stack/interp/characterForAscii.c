/* Extracted from interp.c:50378 (function characterForAscii). */

/*	Above ObjectMemory, arg must lie in range 0-255! */

	/* StackInterpreter>>#characterForAscii: */

sqInt
characterForAscii(sqInt ascii)
{
	return ((((usqInt)(ascii) << (numTagBits())))) + (characterTag());
}