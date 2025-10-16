/* Extracted from interp.c:35749 (function formatOf). */

static NoDbgRegParms sqInt
formatOf(sqInt objOop)
{
	return (byteAt((void *)(objOop + (formatFieldByteOffset())))) & (formatMask());
}