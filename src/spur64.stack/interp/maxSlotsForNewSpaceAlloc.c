/* Extracted from interp.c:40679 (function maxSlotsForNewSpaceAlloc). */

sqInt
maxSlotsForNewSpaceAlloc(void)
{
	return (1U << (fixedFieldsFieldWidth())) - 1;
}