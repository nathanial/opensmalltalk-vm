/* Extracted from interp.c:37858 (function isWordsOrBytesNonImm). */

static NoDbgRegParms sqInt
isWordsOrBytesNonImm(sqInt objOop)
{
    sqInt format;

	format = (byteAt((void *)(objOop + (formatFieldByteOffset())))) & (formatMask());

	/* begin isPureBitsFormat: */
	return (format >= (sixtyFourBitIndexableFormat()))
	 && (format < (firstCompiledMethodFormat()));
}