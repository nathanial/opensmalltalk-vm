/* Extracted from interp.c:37649 (function isUnmarked). */

/*	For debugging using printOopsSuchThat: */

	/* SpurMemoryManager>>#isUnmarked: */

int
isUnmarked(sqInt objOop)
{
	return !(((byteAt((void *)(objOop + (markBitsByteOffset())))) & (1U << (markedBitByteShift()))) != 0);
}