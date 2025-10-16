/* Extracted from interp.c:34992 (function fetchByteofObject). */

sqInt
fetchByteofObject(sqInt byteIndex, sqInt objOop)
{
	return byteAt((void *)((objOop + BaseHeaderSize) + byteIndex));
}