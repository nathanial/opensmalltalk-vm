/* Extracted from interp.c:35435 (function fixedFieldsOfClassFormatMask). */

sqInt
fixedFieldsOfClassFormatMask(void)
{
	return (1U << (fixedFieldsFieldWidth())) - 1;
}