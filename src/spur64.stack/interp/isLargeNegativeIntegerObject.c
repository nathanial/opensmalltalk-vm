/* Extracted from interp.c:55034 (function isLargeNegativeIntegerObject). */

	/* StackInterpreter>>#isLargeNegativeIntegerObject: */

sqInt
isLargeNegativeIntegerObject(sqInt oop)
{
	return /* isInstanceOfClassLargeNegativeInteger: */
		((!(oop & (tagMask()))))
	 && (((longAt((void *)(oop))) & (classIndexMask())) == ClassLargeNegativeIntegerCompactIndex);
}