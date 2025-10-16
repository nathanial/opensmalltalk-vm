/* Extracted from interp.c:37192 (function isCompiledMethod). */

int
isCompiledMethod(sqInt objOop)
{
	return ((byteAt((void *)(objOop + (formatFieldByteOffset())))) & (formatMask())) >= (firstCompiledMethodFormat());
}