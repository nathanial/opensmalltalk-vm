/* Extracted from interp.c:36170 (function headerForSlotsformatclassIndex). */

usqLong
headerForSlotsformatclassIndex(sqInt numSlots, sqInt formatField, sqInt classIndex)
{
	return ((((((usqLong) numSlots)) << (numSlotsFullShift()))) + ((((usqInt)(formatField) << (formatShift()))))) + classIndex;
}