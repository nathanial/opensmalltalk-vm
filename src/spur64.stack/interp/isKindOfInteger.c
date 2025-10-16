/* Extracted from interp.c:55015 (function isKindOfInteger). */

sqInt
isKindOfInteger(sqInt oop)
{
	return ((((oop) & 7) == 1))
	 || (/* isLargeIntegerInstance: */
		((!(oop & (tagMask()))))
	 && ((((usqInt)(((longAt((void *)(oop))) & (classIndexMask())) - ClassLargeNegativeIntegerCompactIndex))) <= 1));
}