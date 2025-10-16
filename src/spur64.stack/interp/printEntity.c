/* Extracted from interp.c:42785 (function printEntity). */

void
printEntity(sqInt oop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    usqInt n;
    sqInt printFlags;

	printFlags = 0;
	printHex(oop);

	/* begin space */
	printChar(' ');
	if (!(addressCouldBeObj(oop))) {
		print((((oop & (tagMask())) != 0)
				? "immediate"
				: "unknown"));
		return;
	}
	print((((longAt((void *)(oop))) & (classIndexMask())) == (isFreeObjectClassIndexPun())
			? "free"
			: (((longAt((void *)(oop))) & (classIndexMask())) == (segmentBridgePun())
					? "bridge"
					: ((!((longAt((void *)(oop))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))
							? "forwarder"
							: (((longAt((void *)(oop))) & (classIndexMask())) <= (lastClassIndexPun())
									? ((printFlags = 1),
									"pun/obj stack")
									: ((printFlags = 1),
									"object"))))));

	/* begin space */
	printChar(' ');
	n = byteAt((void *)(oop + (numSlotsFieldByteOffset())));

	/* begin printHexnpnp: */
	fprintf(GIV(transcript),
			"%" PRIxSQINT "",
			n);
	print("/");
	n = bytesInBody(oop);

	/* begin printHexnpnp: */
	fprintf(GIV(transcript),
			"%" PRIxSQINT "",
			n);
	print("/");
	printNum(bytesInBody(oop));
	if (printFlags) {
		/* begin space */
		printChar(' ');
		print((((byteAt((void *)(oop + (formatFieldByteOffset())))) & (formatMask())) <= 15
				? "f:0"
				: "f:"));
		n = (byteAt((void *)(oop + (formatFieldByteOffset())))) & (formatMask());

		/* begin printHexnpnp: */
		fprintf(GIV(transcript),
				"%" PRIxSQINT "",
				n);
		print(((byteAt((void *)(oop + (markBitsByteOffset())))) & (1U << (greyBitByteShift()))
				? " g"
				: " ."));
		print(((((usqInt)((byteAt((void *)(oop + (immutableExtraBitsByteOffset())))))) >> (immutableBitByteShift())) & 1
				? "i"
				: "."));
		print(((byteAt((void *)(oop + (markBitsByteOffset())))) & (1U << (markedBitByteShift()))
				? "m"
				: "."));
		print(((byteAt((void *)(oop + (formatFieldByteOffset())))) & (1U << (pinnedBitByteShift()))
				? "p"
				: "."));
		print(((byteAt((void *)(oop + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift()))
				? "r"
				: "."));
	}
	cr();
}