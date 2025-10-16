/* Extracted from interp.c:37844 (function isWeak). */

sqInt
isWeak(sqInt oop)
{
	return ((!(oop & (tagMask()))))
	 && (isWeakNonImm(oop));
}