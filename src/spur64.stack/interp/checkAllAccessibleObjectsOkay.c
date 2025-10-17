/* Extracted from interp.c:50389 (function checkAllAccessibleObjectsOkay). */

/*	Ensure that all accessible objects in the heap are okay. */
/*	useful for VM debugging */

	/* StackInterpreter>>#checkAllAccessibleObjectsOkay */

sqInt
checkAllAccessibleObjectsOkay(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt address;
    sqInt followingWord;
    usqInt followingWordAddress;
    usqInt numSlots;
    sqInt obj;
    sqInt ok;
    sqInt prevObj;
    sqInt prevPrevObj;
    sqInt startObject;

	ok = 1;

	/* begin allObjectsDoSafely: */
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
		if (((longAt((void *)(obj))) & (classIndexMask())) > (lastClassIndexPun())) {
			ok = ok && (checkOkayFields(obj));
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
	return ok;
}