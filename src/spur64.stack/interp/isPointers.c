/* Extracted from interp.c:37555 (function isPointers). */

/*	Answer if the argument has only fields that can hold oops. See comment in
	formatOf: 
 */

	/* SpurMemoryManager>>#isPointers: */

sqInt
isPointers(sqInt oop)
{
	return ((!(oop & (tagMask()))))
	 && (((byteAt((void *)(oop + (formatFieldByteOffset())))) & (formatMask())) <= 5 /* lastPointerFormat */);
}