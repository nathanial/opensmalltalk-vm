/* Extracted from interp.c:35442 (function fixedFieldsOfClassFormat). */

sqInt
fixedFieldsOfClassFormat(sqInt classFormat)
{
	return classFormat & ((1U << (fixedFieldsFieldWidth())) - 1);
}