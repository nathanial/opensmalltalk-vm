/* Extracted from interp.c:41552 (function objectInPastSpaceBefore). */

/*	For assertions only... This ends the recursion in setting up the bridged
	enumerations of new space that need objectBefore: for assertion checking. */

	/* SpurMemoryManager>>#objectInPastSpaceBefore: */

static NoDbgRegParms sqInt
objectInPastSpaceBefore(sqInt objOop)
{
    sqInt address;
    sqInt followingWord;
    usqInt followingWordAddress;
    usqInt numSlots;
    sqInt obj;
    sqInt prev;

	if (pastSpaceStart <= (((pastSpace).start))) {
		return null;
	}
	prev = null;
	address = ((pastSpace).start);

	/* begin objectStartingAt: */
	numSlots = byteAt((void *)(address + (numSlotsFieldByteOffset())));
	obj = (numSlots == (numSlotsMask())
				? address + BaseHeaderSize
				: address);
	while (oopisLessThan(obj, pastSpaceStart)) {
		if (oopisGreaterThanOrEqualTo(obj, objOop)) {
			return prev;
		}
		prev = obj;

		/* begin objectAfter:limit: */
		followingWordAddress = addressAfter(obj);
		if (oopisGreaterThanOrEqualTo(followingWordAddress, pastSpaceStart)) {
			obj = pastSpaceStart;
			goto l1;
		}
		followingWord = longAt((void *)(followingWordAddress));
		obj = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) == (numSlotsMask())
					? followingWordAddress + BaseHeaderSize
					: followingWordAddress);
		/* end objectAfter:limit: */
l1:;
	}
	return prev;
}