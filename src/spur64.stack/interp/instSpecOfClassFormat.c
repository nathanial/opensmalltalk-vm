/* Extracted from interp.c:37098 (function instSpecOfClassFormat). */

	/* SpurMemoryManager>>#instSpecOfClassFormat: */

sqInt
instSpecOfClassFormat(sqInt classFormat)
{
	return (((usqInt)(classFormat)) >> (fixedFieldsFieldWidth())) & (formatMask());
}