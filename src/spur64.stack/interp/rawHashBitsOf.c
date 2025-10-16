/* Extracted from interp.c:44936 (function rawHashBitsOf). */

sqInt
rawHashBitsOf(sqInt objOop)
{
	return (long32At((void *)(objOop + 4))) & (identityHashHalfWordMask());
}