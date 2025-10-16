/* Extracted from interp.c:42461 (function postBecomeScanClassTable). */

static NoDbgRegParms void
postBecomeScanClassTable(sqInt effectsFlags)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt classOrNil;
    sqInt i;
    sqInt j;
    usqInt numSlots;
    sqInt page;
    sqInt referent;
    sqInt toDoLimit;

	assert(validClassTableRootPages());
	if (!(((effectsFlags & BecamePointerObjectFlag) != 0))) {
		return;
	}
	for (i = 0; i < GIV(numClassTablePages); i += 1) {
		page = longAt((void *)((GIV(hiddenRootsObj) + BaseHeaderSize) + ((((usqInt)(i) << (shiftForWord()))))));
		assert(!(isForwarded(page)));
		toDoLimit = ((/* begin numSlotsOf: */
	assert((classIndexOf(page)) > (isForwardedObjectClassIndexPun())),
(((numSlots = byteAt((void *)(page + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
			? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(page - BaseHeaderSize)))) << 8)))))) >> 8
			: numSlots))) - 1;
		for (j = 0; j <= toDoLimit; j += 1) {
			classOrNil = longAt((void *)((page + BaseHeaderSize) + ((((usqInt)(j) << (shiftForWord()))))));
			if (classOrNil != GIV(nilObj)) {
				if ((!((longAt((void *)(classOrNil))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
					/* begin followForwarded: */
					assert(isUnambiguouslyForwarder(classOrNil));
					referent = longAt((void *)((classOrNil + BaseHeaderSize) + (0U << (shiftForWord()))));
					while (/* isOopForwarded: */
						((!(referent & (tagMask()))))
					 && ((!((longAt((void *)(referent))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
						referent = longAt((void *)((referent + BaseHeaderSize) + (0U << (shiftForWord()))));
					}
					classOrNil = referent;

					/* begin storePointer:ofObject:withValue: */
					assert(validStorePointerArgs(j, page, classOrNil));
					assert(isNonImmediate(page));
					if (oopisGreaterThanOrEqualTo(page, GIV(oldSpaceStart))) {
						if (/* isYoung: */
							((!(classOrNil & (tagMask()))))
						 && (oopisLessThan(classOrNil, GIV(oldSpaceStart)))) {
							/* begin possibleRootStoreInto: */
							if (!((byteAt((void *)(page + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
								remember(page);
							}
						}
					}

					/* most stores into young objects */
					longAtput((void *)((page + BaseHeaderSize) + ((((usqInt)(j) << (shiftForWord()))))),classOrNil);
				}
				if (!((long32At((void *)(classOrNil + 4))) & (identityHashHalfWordMask()))) {
					/* begin storePointerUnchecked:ofObject:withValue: */
					assert((isNonImmediate(page))
					 && (!(isForwarded(page))));
					assert(validStorePointerUncheckedArgs(j, page, GIV(nilObj)));
					longAtput((void *)((page + BaseHeaderSize) + ((((usqInt)(j) << (shiftForWord()))))),GIV(nilObj));

					/* If the removed class is before the classTableIndex, set the
					   classTableIndex to point to the empty slot so as to reuse it asap. */
					if ((((((usqInt)(i) << (classTableMajorIndexShift())))) + j) < GIV(classTableIndex)) {
						GIV(classTableIndex) = ((((usqInt)(i) << (classTableMajorIndexShift())))) + j;
					}
				}
			}
		}
	}

	/* classTableIndex must never index the first page, which is reserved for classes known to the VM. */
	assert(GIV(classTableIndex) >= (1U << (classTableMajorIndexShift())));
}