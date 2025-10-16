/* Extracted from interp.c:37098 (function instSpecOfClassFormat). */

sqInt
instSpecOfClassFormat(sqInt classFormat)
{
	return (((usqInt)(classFormat)) >> (fixedFieldsFieldWidth())) & (formatMask());
}