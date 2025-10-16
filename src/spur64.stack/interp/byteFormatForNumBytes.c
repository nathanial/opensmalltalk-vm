/* Extracted from interp.c:31782 (function byteFormatForNumBytes). */

static NoDbgRegParms sqInt
byteFormatForNumBytes(sqInt numBytes)
{
	return (firstByteFormat()) + ((8 - numBytes) & (BytesPerWord - 1));
}