/* Extracted from interp.c:27808 (function allWeakSurvivorsOnWeakList). */

	/* SpurGenerationScavenger>>#allWeakSurvivorsOnWeakList */

static sqInt
allWeakSurvivorsOnWeakList(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt address;
    usqInt corpse;
    sqInt followingWord;
    usqInt followingWordAddress;
    sqInt format;
    sqInt limit;
    sqInt listOffset;
    usqInt numSlots;
    sqInt objOop;
    sqInt prevObj;
    sqInt prevPrevObj;

	/* begin allFutureSpaceEntitiesDo: */
	prevPrevObj = (prevObj = null);
	address = (GIV(futureSpace).start);

	/* begin objectStartingAt: */
	numSlots = byteAt((void *)(address + (numSlotsFieldByteOffset())));
	objOop = (numSlots == (numSlotsMask())
				? address + BaseHeaderSize
				: address);
	limit = GIV(futureSurvivorStart);
	while (oopisLessThan(objOop, limit)) {
		/* begin isWeakNonImm: */
		format = (byteAt((void *)(objOop + (formatFieldByteOffset())))) & (formatMask());
		if (format == (weakArrayFormat())) {
			/* begin is:onWeaklingList: */
			corpse = ((((usqInt)((GIV(weakList) - 1)) << 3 /* shiftForAllocationUnit */))) + GIV(newSpaceStart);
			while (corpse) {
				if (objOop == (followForwarded(corpse))) {
					goto l1;
				}

				/* begin nextCorpseOrNil: */
				assert(isYoung(corpse));
				listOffset = ((((usqInt)(((long32At((void *)(corpse + 4))) & (identityHashHalfWordMask()))) << (formatFieldWidthShift())))) + ((byteAt((void *)(corpse + (formatFieldByteOffset())))) & (formatMask()));
				corpse = ((sqInt) ((listOffset
		? ((((usqInt)((listOffset - 1)) << 3 /* shiftForAllocationUnit */))) + GIV(newSpaceStart)
		: 0)));
			}
			return 0;
			return 0;
l1:;
		}
		prevPrevObj = prevObj;
		prevObj = objOop;

		/* begin objectAfter:limit: */
		followingWordAddress = addressAfter(objOop);
		if (oopisGreaterThanOrEqualTo(followingWordAddress, limit)) {
			objOop = limit;
			goto l2;
		}
		followingWord = longAt((void *)(followingWordAddress));
		objOop = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) == (numSlotsMask())
					? followingWordAddress + BaseHeaderSize
					: followingWordAddress);
		/* end objectAfter:limit: */
l2:;
	}
	return 1;
}