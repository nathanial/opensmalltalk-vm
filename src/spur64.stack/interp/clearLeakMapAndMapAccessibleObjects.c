/* Extracted from interp.c:33496 (function clearLeakMapAndMapAccessibleObjects). */

/*	Perform an integrity/leak check using the heapMap. Set a bit at each
	object's header. */

	/* SpurMemoryManager>>#clearLeakMapAndMapAccessibleObjects */

static void
clearLeakMapAndMapAccessibleObjects(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt address;
    sqInt classIndex;
    sqInt followingWord;
    usqInt followingWordAddress;
    usqInt numSlots;
    sqInt obj;
    sqInt prevObj;
    sqInt prevPrevObj;
    sqInt startObject;

	clearHeapMap();

	/* begin allObjectsDo: */
	address = /* startAddressForBridgedHeapEnumeration */
			(GIV(pastSpaceStart) > (((GIV(pastSpace)).start))
				? ((GIV(pastSpace)).start)
				: (GIV(freeStart) > (((GIV(eden)).start))
						? ((GIV(eden)).start)
						: GIV(oldSpaceStart)));

	/* begin objectStartingAt: */
	numSlots = byteAt((void *)(address + (numSlotsFieldByteOffset())));
	startObject = (numSlots == (numSlotsMask())
				? address + BaseHeaderSize
				: address);

	/* begin allEntitiesFrom:do: */
	prevPrevObj = (prevObj = null);
	obj = startObject;
	enableObjectEnumerationFrom(startObject);
	while (1) {
		assert((obj % (allocationUnit())) == 0);
		if (!(oopisLessThan(obj, GIV(endOfMemory)))) break;
		assert((long64At((void *)(obj))) != 0);

		/* begin isEnumerableObject: */
		classIndex = (longAt((void *)(obj))) & (classIndexMask());
		assert((classIndex == (segmentBridgePun()))
		 || ((classIndex == (isForwardedObjectClassIndexPun()))
		 || (((long64At((void *)(obj))) != 0)
		 && (classIndex < (GIV(numClassTablePages) * (classTablePageSize()))))));
		if (classIndex >= (isForwardedObjectClassIndexPun())) {
			heapMapAtWordPut(pointerForOop(obj), 1);
		}
		prevPrevObj = prevObj;
		prevObj = obj;

		/* begin objectAfterMaybeSlimBridge:limit: */
		followingWordAddress = addressAfter(obj);
		if (oopisGreaterThanOrEqualTo(followingWordAddress, GIV(endOfMemory))) {
			obj = GIV(endOfMemory);
			goto l1;
		}
		followingWord = longAt((void *)(followingWordAddress));
		obj = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) == (numSlotsMask())
					? ((oopisLessThan(obj, GIV(oldSpaceStart)))
					 && ((followingWord & 0xFFFFFFFFFFFFFFLL) == 1)
							? (followingWordAddress + BaseHeaderSize) + BaseHeaderSize
							: followingWordAddress + BaseHeaderSize)
					: followingWordAddress);
		/* end objectAfterMaybeSlimBridge:limit: */
l1:
		assert(oopisGreaterThan(obj, prevObj));
	}
}