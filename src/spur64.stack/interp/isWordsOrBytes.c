/* Extracted from interp.c:37877 (function isWordsOrBytes). */

/*	Answer if the contains only indexable words or bytes (no oops). See
	comment in formatOf:
 */
/*	Note: Excludes CompiledMethods. */

	/* SpurMemoryManager>>#isWordsOrBytes: */

sqInt
isWordsOrBytes(sqInt oop)
{
	return ((!(oop & (tagMask()))))
	 && (isWordsOrBytesNonImm(oop));
}