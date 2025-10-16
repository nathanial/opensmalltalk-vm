/* Extracted from interp.c:30993 (function allocatePinnedSlots). */

sqInt
allocatePinnedSlots(sqInt nSlots)
{
    sqInt obj;
    usqInt p;
    usqInt toDoLimit;

	obj = allocateSlotsForPinningInOldSpacebytesformatclassIndex(nSlots, /* objectBytesForSlots: */
			(nSlots
				? ((((usqInt)(nSlots) << (shiftForWord())))) + ((nSlots >= (numSlotsMask())
		? BaseHeaderSize + BaseHeaderSize
		: BaseHeaderSize))
				: 8 /* allocationUnit */ + BaseHeaderSize), sixtyFourBitIndexableFormat(), sixtyFourBitLongsClassIndexPun());
	if (obj) {
		assert(isPinned(obj));

		/* begin fillObj:numSlots:with: */
		assert(oopisLessThan(((obj + BaseHeaderSize) + (nSlots * BytesPerOop)) - 1, addressAfter(obj)));
		toDoLimit = ((usqInt)(((obj + BaseHeaderSize) + (nSlots * BytesPerOop)) - 1));
		for (p = (((usqInt)(obj + BaseHeaderSize))); p <= toDoLimit; p += 8 /* allocationUnit */) {
			longAtput((void *)(p),0);
		}
	}
	return obj;
}