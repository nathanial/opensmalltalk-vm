/* Extracted from interp.c:55025 (function isLargeIntegerObject). */

	/* StackInterpreter>>#isLargeIntegerObject: */

sqInt
isLargeIntegerObject(sqInt oop)
{
	return /* isLargeIntegerInstance: */
		((!(oop & (tagMask()))))
	 && ((((usqInt)(((longAt((void *)(oop))) & (classIndexMask())) - ClassLargeNegativeIntegerCompactIndex))) <= 1);
}