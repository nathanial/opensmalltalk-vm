/* Extracted from interp.c:46046 (function storeLong32ofObjectwithValue). */

sqInt
storeLong32ofObjectwithValue(sqInt fieldIndex, sqInt obj, sqInt valueWord)
{
	return long32Atput((void *)((obj + BaseHeaderSize) + ((((usqInt)(fieldIndex) << 2)))),valueWord);
}