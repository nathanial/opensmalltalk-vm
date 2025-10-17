/* Extracted from interp.c:55043 (function isLargePositiveIntegerObject). */

	/* StackInterpreter>>#isLargePositiveIntegerObject: */

sqInt
isLargePositiveIntegerObject(sqInt oop)
{
	return /* isInstanceOfClassLargePositiveInteger: */
		((!(oop & (tagMask()))))
	 && (((longAt((void *)(oop))) & (classIndexMask())) == ClassLargePositiveIntegerCompactIndex);
}