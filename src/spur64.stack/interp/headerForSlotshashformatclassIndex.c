/* Extracted from interp.c:36204 (function headerForSlotshashformatclassIndex). */

static NoDbgRegParms usqLong
headerForSlotshashformatclassIndex(sqInt numSlots, sqInt hash, sqInt formatField, sqInt classIndex)
{
	return (((((((usqLong) numSlots)) << (numSlotsFullShift()))) + (((((usqLong) hash)) << (identityHashFullWordShift())))) + ((((usqInt)(formatField) << (formatShift()))))) + classIndex;
}