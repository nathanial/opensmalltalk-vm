/* Extracted from interp.c:37925 (function isYoung). */

/*	Answer if oop is young. */

	/* SpurMemoryManager>>#isYoung: */

sqInt
isYoung(sqInt oop)
{
	return ((!(oop & (tagMask()))))
	 && (oopisLessThan(oop, oldSpaceStart));
}