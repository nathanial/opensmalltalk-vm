/* Extracted from interp.c:37511 (function isOopCompiledMethod). */

sqInt
isOopCompiledMethod(sqInt oop)
{
	return ((!(oop & (tagMask()))))
	 && (((byteAt((void *)(oop + (formatFieldByteOffset())))) & (formatMask())) >= (firstCompiledMethodFormat()));
}