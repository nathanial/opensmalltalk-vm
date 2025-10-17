/* Extracted from interp.c:43371 (function printHeaderOf). */

/*	useful for VM debugging */
/*	N.B. No safety bounds checks!! We need to look e.g. at corpses. */

	/* SpurMemoryManager>>#printHeaderOf: */

void
printHeaderOf(sqInt objOop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    usqInt n;

	/* printHexnp: */
	fprintf(transcript,
			"%p",
			((void *)objOop));
	if ((byteAt((void *)(objOop + (numSlotsFieldByteOffset())))) == (numSlotsMask())) {
		print(" hdr16 slotf ");
		n = byteAt((void *)(objOop + (numSlotsFieldByteOffset())));

		/* begin printHexnp: */
		fprintf(transcript,
				"%p",
				((void *)n));
		print(" slotc ");

		/* printHexnp: */
		fprintf(transcript,
				"%p",
				((void *)(((((usqInt)(((sqInt)((usqInt)((longAt((void *)(objOop - BaseHeaderSize)))) << 8)))))) >> 8)));

		/* begin space */
		printChar(' ');
	}
	else {
		print(" hdr8 slots ");
		n = numSlotsOfAny(objOop);

		/* begin printHexnp: */
		fprintf(transcript,
				"%p",
				((void *)n));
	}
	printChar(' ');
	printChar(((byteAt((void *)(objOop + (markBitsByteOffset())))) & (1U << (markedBitByteShift()))
			? 'M'
			: 'm'));
	printChar(((byteAt((void *)(objOop + (markBitsByteOffset())))) & (1U << (greyBitByteShift()))
			? 'G'
			: 'g'));
	printChar(((byteAt((void *)(objOop + (formatFieldByteOffset())))) & (1U << (pinnedBitByteShift()))
			? 'P'
			: 'p'));
	printChar(((byteAt((void *)(objOop + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift()))
			? 'R'
			: 'r'));
	printChar(((((usqInt)((byteAt((void *)(objOop + (immutableExtraBitsByteOffset())))))) >> (immutableBitByteShift())) & 1
			? 'I'
			: 'i'));
	print(" hash ");
	n = (long32At((void *)(objOop + 4))) & (identityHashHalfWordMask());

	/* begin printHexnp: */
	fprintf(transcript,
			"%p",
			((void *)n));
	print(" fmt ");
	n = (byteAt((void *)(objOop + (formatFieldByteOffset())))) & (formatMask());

	/* begin printHexnp: */
	fprintf(transcript,
			"%p",
			((void *)n));
	print(" cidx ");
	n = (longAt((void *)(objOop))) & (classIndexMask());

	/* begin printHexnp: */
	fprintf(transcript,
			"%p",
			((void *)n));
	cr();
}