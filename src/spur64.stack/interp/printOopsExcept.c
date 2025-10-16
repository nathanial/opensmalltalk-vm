/* Extracted from interp.c:44200 (function printOopsExcept). */

NeverInline void
printOopsExcept(sqInt (*function)(sqInt))
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt followingWord;
    usqInt followingWordAddress;
    sqInt n;
    usqInt numSlots;
    sqInt objOop;
    sqInt prevObj;
    sqInt prevPrevObj;
    usqInt start;

	n = 0;

	/* begin allHeapEntitiesDo: */
	/* begin allOldSpaceEntitiesDo: */
	/* begin allOldSpaceEntitiesFrom:do: */
	assert(isOldObject(GIV(nilObj)));
	prevPrevObj = (prevObj = null);
	objOop = GIV(nilObj);
	while (1) {
		assert((objOop % (allocationUnit())) == 0);
		if (!(oopisLessThan(objOop, GIV(endOfMemory)))) break;
		assert((long64At((void *)(objOop))) != 0);
		if (!(function(objOop))) {
			n += 1;
			printEntity(objOop);
		}
		prevPrevObj = prevObj;
		prevObj = objOop;

		/* begin objectAfter:limit: */
		followingWordAddress = addressAfter(objOop);
		if (oopisGreaterThanOrEqualTo(followingWordAddress, GIV(endOfMemory))) {
			objOop = GIV(endOfMemory);
			goto l1;
		}
		followingWord = longAt((void *)(followingWordAddress));
		objOop = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) == (numSlotsMask())
					? followingWordAddress + BaseHeaderSize
					: followingWordAddress);
		/* end objectAfter:limit: */
l1:;
	}

	/* begin allNewSpaceEntitiesDo: */
	prevPrevObj = (prevObj = null);

	/* After a scavenge eden is empty, futureSpace is empty, and all newSpace objects are
	   in pastSpace.  Objects are allocated in eden.  So enumerate only pastSpace and eden. */
	assert((((GIV(pastSpace)).start)) < (((GIV(eden)).start)));
	start = /* startAddressForBridgedHeapEnumeration */
			(GIV(pastSpaceStart) > (((GIV(pastSpace)).start))
				? ((GIV(pastSpace)).start)
				: (GIV(freeStart) > (((GIV(eden)).start))
						? ((GIV(eden)).start)
						: GIV(oldSpaceStart)));
	if (start > GIV(freeStart)) {
		goto l3;
	}

	/* begin bridgePastSpaceAndEden */
	if (GIV(pastSpaceStart) < (((GIV(eden)).start))) {
		if ((GIV(pastSpaceStart) + BaseHeaderSize) == (((GIV(eden)).start))) {
			hackSlimBridgeToat(objectStartingAt(((GIV(eden)).start)), GIV(pastSpaceStart));

			/* And carefully check the assumption */
			assert((objectAfterMaybeSlimBridgelimit(objectInPastSpaceBefore(GIV(pastSpaceStart)), GIV(nilObj))) == (objectStartingAt(((GIV(eden)).start))));
		}
		else {
			initSegmentBridgeWithBytesat((((GIV(eden)).start)) - GIV(pastSpaceStart), GIV(pastSpaceStart));
		}
	}

	/* begin objectStartingAt: */
	numSlots = byteAt((void *)(start + (numSlotsFieldByteOffset())));
	objOop = (numSlots == (numSlotsMask())
				? start + BaseHeaderSize
				: start);
	while (oopisLessThan(objOop, GIV(freeStart))) {
		if (!(function(objOop))) {
			n += 1;
			printEntity(objOop);
		}
		prevPrevObj = prevObj;
		prevObj = objOop;

		/* begin objectAfterMaybeSlimBridge:limit: */
		followingWordAddress = addressAfter(objOop);
		if (oopisGreaterThanOrEqualTo(followingWordAddress, GIV(freeStart))) {
			objOop = GIV(freeStart);
			goto l2;
		}
		followingWord = longAt((void *)(followingWordAddress));
		objOop = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) == (numSlotsMask())
					? ((oopisLessThan(objOop, GIV(oldSpaceStart)))
					 && ((followingWord & 0xFFFFFFFFFFFFFFLL) == 1)
							? (followingWordAddress + BaseHeaderSize) + BaseHeaderSize
							: followingWordAddress + BaseHeaderSize)
					: followingWordAddress);
		/* end objectAfterMaybeSlimBridge:limit: */
l2:;
	}
	/* end allNewSpaceEntitiesDo: */
l3:
	if (n > 4) {
		printNum(n);
		print(" objects");
		cr();
	}
}