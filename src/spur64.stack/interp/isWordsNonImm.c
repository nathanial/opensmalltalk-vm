/* Extracted from interp.c:27411 (function isWordsNonImm). */

static NoDbgRegParms int
isWordsNonImm(sqInt objOop)
{
	return ((((byteAt((void *)(objOop + (formatFieldByteOffset())))) & (formatMask())) >= (firstLongFormat())) && (((byteAt((void *)(objOop + (formatFieldByteOffset())))) & (formatMask())) <= ((firstShortFormat()) - 1)));
}