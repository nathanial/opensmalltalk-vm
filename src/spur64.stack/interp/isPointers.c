/* Extracted from interp.c:37555 (function isPointers). */

sqInt
isPointers(sqInt oop)
{
	return ((!(oop & (tagMask()))))
	 && (((byteAt((void *)(oop + (formatFieldByteOffset())))) & (formatMask())) <= 5 /* lastPointerFormat */);
}