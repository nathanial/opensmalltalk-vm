/* Extracted from interp.c:28240 (function growRememberedSet). */

	/* SpurGenerationScavenger>>#growRememberedSet */

static void
growRememberedSet(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt *base;
    sqInt fudge;
    sqInt i;
    sqInt newObj;
    sqInt nSlots;
    usqInt numSlots;
    usqInt numSlotsUsqInt;
    sqInt obj;
    usqInt p;
    usqInt toDoLimit;


	/* Don't ruin locality in remember: */
	obj = longAt((void *)((GIV(hiddenRootsObj) + BaseHeaderSize) + ((((usqInt)(RememberedSetRootIndex) << (shiftForWord()))))));

	/* begin numSlotsOf: */
	assert((classIndexOf(obj)) > (isForwardedObjectClassIndexPun()));
	numSlots = (((numSlotsUsqInt = byteAt((void *)(obj + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
				? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(obj - BaseHeaderSize)))) << 8)))))) >> 8
				: numSlotsUsqInt);
	assert(numSlots >= 0x400);
	nSlots = numSlots * 2;

	/* begin allocatePinnedSlots: */
	newObj = allocateSlotsForPinningInOldSpacebytesformatclassIndex(nSlots, /* objectBytesForSlots: */
			(nSlots
				? ((((usqInt)(nSlots) << (shiftForWord())))) + ((nSlots >= (numSlotsMask())
		? BaseHeaderSize + BaseHeaderSize
		: BaseHeaderSize))
				: 8 /* allocationUnit */ + BaseHeaderSize), sixtyFourBitIndexableFormat(), sixtyFourBitLongsClassIndexPun());
	if (newObj) {
		assert(isPinned(newObj));

		/* begin fillObj:numSlots:with: */
		assert(oopisLessThan(((newObj + BaseHeaderSize) + (nSlots * BytesPerOop)) - 1, addressAfter(newObj)));
		toDoLimit = ((usqInt)(((newObj + BaseHeaderSize) + (nSlots * BytesPerOop)) - 1));
		for (p = (((usqInt)(newObj + BaseHeaderSize))); p <= toDoLimit; p += 8 /* allocationUnit */) {
			longAtput((void *)(p),0);
		}
	}
	if (!newObj) {
		nSlots = numSlots + 0x400;

		/* begin allocatePinnedSlots: */
		newObj = allocateSlotsForPinningInOldSpacebytesformatclassIndex(nSlots, /* objectBytesForSlots: */
				(nSlots
					? ((((usqInt)(nSlots) << (shiftForWord())))) + ((nSlots >= (numSlotsMask())
		? BaseHeaderSize + BaseHeaderSize
		: BaseHeaderSize))
					: 8 /* allocationUnit */ + BaseHeaderSize), sixtyFourBitIndexableFormat(), sixtyFourBitLongsClassIndexPun());
		if (newObj) {
			assert(isPinned(newObj));

			/* begin fillObj:numSlots:with: */
			assert(oopisLessThan(((newObj + BaseHeaderSize) + (nSlots * BytesPerOop)) - 1, addressAfter(newObj)));
			toDoLimit = ((usqInt)(((newObj + BaseHeaderSize) + (nSlots * BytesPerOop)) - 1));
			for (p = (((usqInt)(newObj + BaseHeaderSize))); p <= toDoLimit; p += 8 /* allocationUnit */) {
				longAtput((void *)(p),0);
			}
		}
		if (!newObj) {
			if (!(growOldSpaceByAtLeast((numSlots + 0x400) * BytesPerOop))) {
				error("could not grow remembered set");
			}
			nSlots = numSlots + 0x400;

			/* begin allocatePinnedSlots: */
			newObj = allocateSlotsForPinningInOldSpacebytesformatclassIndex(nSlots, /* objectBytesForSlots: */
					(nSlots
						? ((((usqInt)(nSlots) << (shiftForWord())))) + ((nSlots >= (numSlotsMask())
		? BaseHeaderSize + BaseHeaderSize
		: BaseHeaderSize))
						: 8 /* allocationUnit */ + BaseHeaderSize), sixtyFourBitIndexableFormat(), sixtyFourBitLongsClassIndexPun());
			if (newObj) {
				assert(isPinned(newObj));

				/* begin fillObj:numSlots:with: */
				assert(oopisLessThan(((newObj + BaseHeaderSize) + (nSlots * BytesPerOop)) - 1, addressAfter(newObj)));
				toDoLimit = ((usqInt)(((newObj + BaseHeaderSize) + (nSlots * BytesPerOop)) - 1));
				for (p = (((usqInt)(newObj + BaseHeaderSize))); p <= toDoLimit; p += 8 /* allocationUnit */) {
					longAtput((void *)(p),0);
				}
			}
		}
	}

	/* begin rememberedSetObj: */
	assert(isOldObject(newObj));
	assert((isNonImmediate(GIV(hiddenRootsObj)))
	 && (!(isForwarded(GIV(hiddenRootsObj)))));
	assert(validStorePointerUncheckedArgs(RememberedSetRootIndex, GIV(hiddenRootsObj), newObj));
	longAtput((void *)((GIV(hiddenRootsObj) + BaseHeaderSize) + ((((usqInt)(RememberedSetRootIndex) << (shiftForWord()))))),newObj);
	base = firstIndexableField(newObj);
	for (i = 0; i < GIV(rememberedSetSize); i += 1) {
		base[i] = (GIV(rememberedSet)[i]);
	}

	/* if growing in the middle of a GC, need to preserve marked status. */
	if ((byteAt((void *)(obj + (markBitsByteOffset())))) & (1U << (markedBitByteShift()))) {
		/* begin setIsMarkedOf:to: */
		assert(!(isFreeObject(newObj)));
		byteAtput((void *)(newObj + (markBitsByteOffset())),(byteAt((void *)(newObj + (markBitsByteOffset())))) | (1U << (markedBitByteShift())));

		/* begin setIsMarkedOf:to: */
		assert(!(isFreeObject(obj)));
		byteAtput((void *)(obj + (markBitsByteOffset())),(byteAt((void *)(obj + (markBitsByteOffset())))) & (0xFF - (1U << (markedBitByteShift()))));
	}
	freeObject(obj);
	GIV(rememberedSet) = base;

	/* begin numSlotsOf: */
	assert((classIndexOf(newObj)) > (isForwardedObjectClassIndexPun()));
	GIV(rememberedSetLimit) = (((numSlotsUsqInt = byteAt((void *)(newObj + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
				? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(newObj - BaseHeaderSize)))) << 8)))))) >> 8
				: numSlotsUsqInt);

	/* begin setRememberedSetRedZone */
	fudge = ((((GIV(eden).limit)) - ((GIV(eden).start))) / BytesPerWord) / 0x400;
	GIV(rememberedSetRedZone) = ((((GIV(rememberedSetLimit) * 3) / 4) < fudge) ? fudge : ((GIV(rememberedSetLimit) * 3) / 4));
}