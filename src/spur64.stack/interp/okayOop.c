/* Extracted from interp.c:42015 (function okayOop). */

/*	Verify that the given oop is legitimate. Check address, header, and size
	but not class.
 */

	/* SpurMemoryManager>>#okayOop: */

static NoDbgRegParms sqInt
okayOop(sqInt signedOop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt classIndex;
    sqInt fmt;
    usqInt oop;
    usqLong unusedBits;
    sqInt unusedBitsInYoungObjects;

	oop = ((usqInt) signedOop);

	/* address and size checks */
	if (((oop & (tagMask())) != 0)) {
		return 1;
	}
	if (!(addressCouldBeObj(oop))) {
		error("oop is not a valid address");
		return 0;
	}
	if (!(oopisLessThanOrEqualTo(addressAfter(oop), GIV(endOfMemory)))) {
		error("oop size would make it extend beyond the end of memory");
		return 0;
	}

	/* header type checks */
	if (!(((classIndex = (longAt((void *)(oop))) & (classIndexMask()))) >= (firstClassIndexPun()))) {
		error("oop is a free chunk, or bridge, not an object");
		return 0;
	}
	if (((byteAt((void *)(oop + (numSlotsFieldByteOffset())))) == (numSlotsMask()))
	 && ((byteAt((void *)((oop - BaseHeaderSize) + (numSlotsFieldByteOffset())))) != (numSlotsMask()))) {
		error("oop header has overflow header word, but overflow word does not have a saturated numSlots field");
		return 0;
	}

	/* format check */
	fmt = (byteAt((void *)(oop + (formatFieldByteOffset())))) & (formatMask());
	if ((fmt == 6) || (fmt == 8)) {
		error("oop has an unknown format type");
		return 0;
	}
	if ((fmt == (forwardedFormat())) != (classIndex == (isForwardedObjectClassIndexPun()))) {
		error("oop has mis-matched format/classIndex fields; only one of them is the isForwarded value");
		return 0;
	}

	/* specific header bit checks */
	unusedBits = (0x400000) | (0x40000000000000LL);
	if ((long64At((void *)(oop))) & unusedBits) {
		error("some unused header bits are set; should be zero");
		return 0;
	}
	unusedBitsInYoungObjects = ((1ULL << (greyBitShift())) | (1U << (pinnedBitShift()))) | (1U << (rememberedBitShift()));
	if ((longAt((void *)(oop))) & unusedBitsInYoungObjects) {
		error("some header bits unused in young objects are set; should be zero");
		return 0;
	}
	return 1;
}