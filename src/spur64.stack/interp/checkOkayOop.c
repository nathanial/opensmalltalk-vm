/* Extracted from interp.c:32812 (function checkOkayOop). */

sqInt
checkOkayOop(usqInt oop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt classIndex;
    sqInt fmt;
    usqLong unusedBits;
    sqInt unusedBitsInYoungObjects;


	/* address and size checks */
	if (((oop & (tagMask())) != 0)) {
		return 1;
	}
	if (!(addressCouldBeObj(oop))) {
		fprintf(GIV(transcript),
				"oop %p is not a valid address",
				((void *)oop));
		return 0;
	}
	if (!(oopisLessThanOrEqualTo(addressAfter(oop), GIV(endOfMemory)))) {
		fprintf(GIV(transcript),
				"oop %p size would make it extend beyond the end of memory",
				((void *)oop));
		return 0;
	}

	/* header type checks */
	if (!(((classIndex = (longAt((void *)(oop))) & (classIndexMask()))) >= (firstClassIndexPun()))) {
		fprintf(GIV(transcript),
				"oop %p is a free chunk, or bridge, not an object",
				((void *)oop));
		return 0;
	}
	if (((byteAt((void *)(oop + (numSlotsFieldByteOffset())))) == (numSlotsMask()))
	 && ((byteAt((void *)((oop - BaseHeaderSize) + (numSlotsFieldByteOffset())))) != (numSlotsMask()))) {
		fprintf(GIV(transcript),
				"oop %p header has overflow header word, but overflow word does not have a saturated numSlots field",
				((void *)oop));
		return 0;
	}

	/* format check */
	fmt = (byteAt((void *)(oop + (formatFieldByteOffset())))) & (formatMask());
	if ((fmt == 6) || (fmt == 8)) {
		fprintf(GIV(transcript),
				"oop %p has an unknown format type",
				((void *)oop));
		return 0;
	}
	if ((fmt == (forwardedFormat())) != (classIndex == (isForwardedObjectClassIndexPun()))) {
		fprintf(GIV(transcript),
				"oop %p has mis-matched format/classIndex fields; only one of them is the isForwarded value",
				((void *)oop));
		return 0;
	}

	/* specific header bit checks */
	unusedBits = (0x400000) | (0x40000000000000LL);
	if ((long64At((void *)(oop))) & unusedBits) {
		fprintf(GIV(transcript),
				"oop %p has some unused header bits set; should be zero",
				((void *)oop));
		return 0;
	}
	unusedBitsInYoungObjects = (1U << (unusedBitShift())) | ((1U << (pinnedBitShift())) | (1U << (rememberedBitShift())));
	if ((longAt((void *)(oop))) & unusedBitsInYoungObjects) {
		fprintf(GIV(transcript),
				"oop %p has some header bits unused in young objects set; should be zero",
				((void *)oop));
		return 0;
	}
	return 1;
}