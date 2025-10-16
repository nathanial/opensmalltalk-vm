/* Extracted from interp.c:28894 (function scavengeFutureSurvivorSpaceStartingAt). */

static NoDbgRegParms void
scavengeFutureSurvivorSpaceStartingAt(sqInt initialAddress)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    usqInt numSlots;
    sqInt obj;
    usqInt ptr;

	ptr = initialAddress;
	while (ptr < GIV(futureSurvivorStart)) {
		/* begin objectStartingAt: */
		numSlots = byteAt((void *)(ptr + (numSlotsFieldByteOffset())));
		obj = (numSlots == (numSlotsMask())
					? ptr + BaseHeaderSize
					: ptr);
		ptr = addressAfter(obj);
		((void) (scavengeReferentsOf(obj)));
	}
}