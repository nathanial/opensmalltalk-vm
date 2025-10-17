/* Extracted from interp.c:26854 (function byteSwapped). */

/*	Answer the given integer with its bytes in the reverse order. */

	/* Spur64BitMemoryManager>>#byteSwapped: */

sqInt
byteSwapped(sqInt w)
{
	return SQ_SWAP_8_BYTES(w);
}