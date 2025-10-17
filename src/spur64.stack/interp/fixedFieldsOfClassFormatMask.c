/* Extracted from interp.c:35435 (function fixedFieldsOfClassFormatMask). */

	/* SpurMemoryManager>>#fixedFieldsOfClassFormatMask */

sqInt
fixedFieldsOfClassFormatMask(void)
{
	return (1U << (fixedFieldsFieldWidth())) - 1;
}