/* Extracted from interp.c:41552 (function objectInPastSpaceBefore). */

static NoDbgRegParms sqInt
objectInPastSpaceBefore(sqInt objOop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt address;
    sqInt followingWord;
    usqInt followingWordAddress;
    usqInt numSlots;
    sqInt obj;
    sqInt prev;

	if (GIV(pastSpaceStart) <= (((GIV(pastSpace)).start))) {
		return null;
	}
	prev = null;
	address = ((GIV(pastSpace)).start);

	/* begin objectStartingAt: */
	numSlots = byteAt((void *)(address + (numSlotsFieldByteOffset())));
	obj = (numSlots == (numSlotsMask())
				? address + BaseHeaderSize
				: address);
	while (oopisLessThan(obj, GIV(pastSpaceStart))) {
		if (oopisGreaterThanOrEqualTo(obj, objOop)) {
			return prev;
		}
		prev = obj;

		/* begin objectAfter:limit: */
		followingWordAddress = addressAfter(obj);
		if (oopisGreaterThanOrEqualTo(followingWordAddress, GIV(pastSpaceStart))) {
			obj = GIV(pastSpaceStart);
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