/* Extracted from interp.c:37877 (function isWordsOrBytes). */

sqInt
isWordsOrBytes(sqInt oop)
{
	return ((!(oop & (tagMask()))))
	 && (isWordsOrBytesNonImm(oop));
}