/* Extracted from interp.c:33999 (function countMarkedAndUnmarkdObjects). */

/*	print the count of marked and unmarked objects.
	In addition if 1 is set in printFlags, short-print marked objects,
	and/or if 2 is set, short-print unmarked obejcts. */
/*	useful for debugging */

	/* SpurMemoryManager>>#countMarkedAndUnmarkdObjects: */

void
countMarkedAndUnmarkdObjects(sqInt printFlags)
{
    sqInt address;
    sqInt classIndex;
    sqInt followingWord;
    usqInt followingWordAddress;
    sqInt nm;
    sqInt nu;
    usqInt numSlots;
    sqInt obj;
    sqInt prevObj;
    sqInt prevPrevObj;
    sqInt startObject;

	nm = (nu = 0);

	/* begin allObjectsDo: */
	address = /* startAddressForBridgedHeapEnumeration */
			(pastSpaceStart > (((pastSpace).start))
				? ((pastSpace).start)
				: (freeStart > (((eden).start))
						? ((eden).start)
						: oldSpaceStart));

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
		if (!(oopisLessThan(obj, endOfMemory))) break;
		assert((long64At((void *)(obj))) != 0);

		/* begin isEnumerableObject: */
		classIndex = (longAt((void *)(obj))) & (classIndexMask());
		assert((classIndex == (segmentBridgePun()))
		 || ((classIndex == (isForwardedObjectClassIndexPun()))
		 || (((long64At((void *)(obj))) != 0)
		 && (classIndex < (numClassTablePages * (classTablePageSize()))))));
		if (classIndex >= (isForwardedObjectClassIndexPun())) {
			if ((byteAt((void *)(obj + (markBitsByteOffset())))) & (1U << (markedBitByteShift()))) {
				nm += 1;
				if (((printFlags & 1) != 0)) {
					shortPrintOop(obj);
				}
			}
			else {
				nu += 1;
				if (((printFlags & 2) != 0)) {
					shortPrintOop(obj);
				}
			}
		}
		prevPrevObj = prevObj;
		prevObj = obj;

		/* begin objectAfterMaybeSlimBridge:limit: */
		followingWordAddress = addressAfter(obj);
		if (oopisGreaterThanOrEqualTo(followingWordAddress, endOfMemory)) {
			obj = endOfMemory;
			goto l1;
		}
		followingWord = longAt((void *)(followingWordAddress));
		obj = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) == (numSlotsMask())
					? ((oopisLessThan(obj, oldSpaceStart))
					 && ((followingWord & 0xFFFFFFFFFFFFFFLL) == 1)
							? (followingWordAddress + BaseHeaderSize) + BaseHeaderSize
							: followingWordAddress + BaseHeaderSize)
					: followingWordAddress);
		/* end objectAfterMaybeSlimBridge:limit: */
l1:
		assert(oopisGreaterThan(obj, prevObj));
	}

	/* begin print: */
	fprintf(transcript,
			"%s",
			"n marked: ");
	fprintf(transcript,
			"%" PRIdSQINT "",
			((sqInt)nm));
	cr();

	/* begin print: */
	fprintf(transcript,
			"%s",
			"n unmarked: ");
	fprintf(transcript,
			"%" PRIdSQINT "",
			((sqInt)nu));
	cr();
}