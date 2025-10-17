/* Extracted from interp.c:37535 (function isOopMutable). */

	/* SpurMemoryManager>>#isOopMutable: */

sqInt
isOopMutable(sqInt oop)
{
	return ((!(oop & (tagMask()))))
	 && (!(((((usqInt)((byteAt((void *)(oop + (immutableExtraBitsByteOffset())))))) >> (immutableBitByteShift())) & 1) != 0));
}