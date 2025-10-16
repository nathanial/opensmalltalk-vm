/* Extracted from interp.c:16484 (function primitiveFormPrint). */

static void
primitiveFormPrint(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt bitsArray;
    sqInt bitsArraySize;
    sqInt depth;
    sqInt fmt;
    sqInt h;
    double hScale;
    sqInt landscapeFlag;
    usqInt numBytes;
    usqInt numSlots;
    sqInt pixelsPerWord;
    sqInt rcvr;
    sqInt successBoolean;
    double vScale;
    sqInt w;
    sqInt wordsPerLine;

	bitsArray = 0;
	depth = 0;
	h = 0;
	w = 0;

	/* begin booleanValueOf: */
	if ((longAt(GIV(stackPointer))) == GIV(trueObj)) {
		landscapeFlag = 1;
		goto l1;
	}
	if ((longAt(GIV(stackPointer))) == GIV(falseObj)) {
		landscapeFlag = 0;
		goto l1;
	}

	/* begin success: */
	if (!GIV(primFailCode)) {
		GIV(primFailCode) = 1;
	}
	landscapeFlag = null;
	/* end booleanValueOf: */
l1:
	vScale = floatValueOf(longAt(GIV(stackPointer) + (1 * BytesPerWord)));
	hScale = floatValueOf(longAt(GIV(stackPointer) + (2 * BytesPerWord)));
	rcvr = longAt(GIV(stackPointer) + (3 * BytesPerWord));
	if (!((/* isPointers: */
			((!(rcvr & (tagMask()))))
		 && (((byteAt((void *)(rcvr + (formatFieldByteOffset())))) & (formatMask())) <= 5 /* lastPointerFormat */))
		 && ((lengthOf(rcvr)) >= 4))) {
		/* begin success: */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
	}
	if (!GIV(primFailCode)) {
		bitsArray = longAt((void *)((rcvr + BaseHeaderSize) + (0U << (shiftForWord()))));
		w = fetchIntegerofObject(1, rcvr);
		h = fetchIntegerofObject(2, rcvr);
		depth = fetchIntegerofObject(3, rcvr);
		if (!((w > 0)
			 && (h > 0))) {
			/* begin success: */
			if (!GIV(primFailCode)) {
				GIV(primFailCode) = 1;
			}
		}
		pixelsPerWord = 32 / depth;
		wordsPerLine = (w + (pixelsPerWord - 1)) / pixelsPerWord;
		if (/* isWordsOrBytes: */
			((!(bitsArray & (tagMask()))))
		 && (isWordsOrBytesNonImm(bitsArray))) {
			/* begin numBytesOf: */
			fmt = (byteAt((void *)(bitsArray + (formatFieldByteOffset())))) & (formatMask());
			assert((classIndexOf(bitsArray)) > (isForwardedObjectClassIndexPun()));
			numBytes = (((numSlots = byteAt((void *)(bitsArray + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
						? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(bitsArray - BaseHeaderSize)))) << 8)))))) >> 8
						: numSlots);
			numBytes = (numBytes << (shiftForWord()));
			if (fmt >= (firstByteFormat())) {
				bitsArraySize = numBytes - (fmt & 7);
				goto l2;
			}

			/* bytes (the common case), including CompiledMethod */
			if (fmt <= (sixtyFourBitIndexableFormat())) {
				bitsArraySize = numBytes;
				goto l2;
			}
			if (fmt >= (firstShortFormat())) {
				bitsArraySize = numBytes - (((fmt & 3) << 1));
				goto l2;
			}

			/* fmt >= self firstLongFormat */
			bitsArraySize = numBytes - (((fmt & 1) << 2));
			/* end numBytesOf: */
l2:

			/* begin success: */
			if (!(bitsArraySize == ((wordsPerLine * h) * 4))) {
				if (!GIV(primFailCode)) {
					GIV(primFailCode) = 1;
				}
			}
		}
		else {
			/* begin success: */
			if (!GIV(primFailCode)) {
				GIV(primFailCode) = 1;
			}
		}
	}
	if (!GIV(primFailCode)) {
		successBoolean = ioFormPrint(bitsArray + BaseHeaderSize, w, h, depth, hScale, vScale, landscapeFlag);

		/* begin success: */
		if (!successBoolean) {
			if (!GIV(primFailCode)) {
				GIV(primFailCode) = 1;
			}
		}
		if (!GIV(primFailCode)) {
			/* begin methodReturnReceiver */
			assert(!((failed())));
			GIV(stackPointer) += GIV(argumentCount) * BytesPerWord;
		}
	}
}