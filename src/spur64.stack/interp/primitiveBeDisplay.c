/* Extracted from interp.c:12628 (function primitiveBeDisplay). */

static void
primitiveBeDisplay(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt bitsOop;
    void *bitsOrHandle;
    sqInt depthOop;
    sqInt heightOop;
    sqInt pinnedBits;
    sqInt rcvr;
    sqInt widthOop;

	rcvr = longAt(GIV(stackPointer));
	if (!((/* isPointers: */
			((!(rcvr & (tagMask()))))
		 && (((byteAt((void *)(rcvr + (formatFieldByteOffset())))) & (formatMask())) <= 5 /* lastPointerFormat */))
		 && (((lengthOf(rcvr)) >= 4)
		 && (((bitsOop = longAt((void *)((rcvr + BaseHeaderSize) + (0U << (shiftForWord()))))),
		((/* isWordsOrBytes: */
				((!(bitsOop & (tagMask()))))
			 && (isWordsOrBytesNonImm(bitsOop)))
			 || ((((bitsOop) & 7) == 1)))
			 && (((((((widthOop = longAt((void *)((rcvr + BaseHeaderSize) + (1U << (shiftForWord()))))))) & 7) == 1))
			 && (((((((heightOop = longAt((void *)((rcvr + BaseHeaderSize) + (2U << (shiftForWord()))))))) & 7) == 1))
			 && ((((((depthOop = longAt((void *)((rcvr + BaseHeaderSize) + (3U << (shiftForWord()))))))) & 7) == 1))))))))) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadReceiver;
		return;
	}

	/* begin splObj:put: */
	/* begin storePointer:ofObject:withValue: */
	assert(validStorePointerArgs(TheDisplay, GIV(specialObjectsOop), rcvr));
	assert(isNonImmediate(GIV(specialObjectsOop)));
	if (oopisGreaterThanOrEqualTo(GIV(specialObjectsOop), GIV(oldSpaceStart))) {
		if (/* isYoung: */
			((!(rcvr & (tagMask()))))
		 && (oopisLessThan(rcvr, GIV(oldSpaceStart)))) {
			/* begin possibleRootStoreInto: */
			if (!((byteAt((void *)(GIV(specialObjectsOop) + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
				remember(GIV(specialObjectsOop));
			}
		}
	}

	/* most stores into young objects */
	longAtput((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(TheDisplay) << (shiftForWord()))))),rcvr);
	if (((!(bitsOop & (tagMask()))))
	 && (!(((byteAt((void *)(bitsOop + (formatFieldByteOffset())))) & (1U << (pinnedBitByteShift()))) != 0))) {
		/* Answers 0 if memory required to pin but not enough memory available. */
		pinnedBits = pinObject(bitsOop);
		if (pinnedBits) {
			bitsOop = pinnedBits;
		}
	}
	bitsOrHandle = ((!(bitsOop & (tagMask())))
				? firstIndexableField(bitsOop)
				: ((void *)bitsOop));

	/* begin ioBeDisplay:width:height:depth: */
	displayBits = bitsOrHandle;
	displayWidth = (widthOop >> 3);
	displayHeight = (heightOop >> 3);
	displayDepth = (depthOop >> 3);
	ioNoteDisplayChangedwidthheightdepth(bitsOrHandle, (widthOop >> 3), (heightOop >> 3), (depthOop >> 3));
}