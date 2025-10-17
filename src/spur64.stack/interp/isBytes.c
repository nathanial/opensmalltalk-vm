/* Extracted from interp.c:37160 (function isBytes). */

/*	Answer true if the argument contains indexable bytes. See comment in
	formatOf: 
 */
/*	Note: Includes CompiledMethods. */

	/* SpurMemoryManager>>#isBytes: */

sqInt
isBytes(sqInt oop)
{
	return ((!(oop & (tagMask()))))
	 && (((byteAt((void *)(oop + (formatFieldByteOffset())))) & (formatMask())) >= (firstByteFormat()));
}