/* Extracted from interp.c:69105 (function primitiveObjectPointsTo). */

 /* LRPCheck */


/*	This primitive is assumed to be fast (see e.g.
	MethodDictionary>>includesKey:) so make it so.
	N.B. Works correctly for cogged methods too. */

	/* StackInterpreterPrimitives>>#primitiveObjectPointsTo */

static void
primitiveObjectPointsTo(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    usqLong fmt;
    sqLong header;
    sqInt i;
    sqInt methodHeader;
    usqInt numSlots;
    usqInt numSlotsUsqInt;
    sqInt rcvr;
    char *sp;
    sqInt thang;
    sqInt trueOrFalse;

	thang = longAt(GIV(stackPointer));
	rcvr = longAt(GIV(stackPointer) + (1 * BytesPerWord));
	if (((rcvr & (tagMask())) != 0)) {
		/* begin pop:thenPushBool: */
		longAtput((sp = GIV(stackPointer) + (1 * BytesPerWord)),GIV(falseObj));
		GIV(stackPointer) = sp;
		return;
	}

	/* Inlined version of lastPointerOf: for speed in determining if rcvr is a context. */
	header = long64At((void *)(rcvr));
	fmt = (((usqLong)(header)) >> (formatShift())) & (formatMask());
	if (fmt <= 5 /* lastPointerFormat */) {
		if ((fmt == (indexablePointersFormat()))
		 && ((header & (classIndexMask())) == ClassMethodContextCompactIndex)) {
			if (((((longAt((void *)((rcvr + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord())))))))) & 7) == 1)) {
				/* begin externalWriteBackHeadFramePointers */
				assert((GIV(framePointer) - GIV(stackPointer)) < (LargeContextSlots * BytesPerOop));
				assert(GIV(stackPage) == (GIV(mostRecentlyUsedPage)));
				assert(!((isFree(GIV(stackPage)))));

				/* begin setHeadFP:andSP:inPage: */
				assert(GIV(stackPointer) < GIV(framePointer));
				assert((GIV(stackPointer) < ((GIV(stackPage)->baseAddress)))
				 && (GIV(stackPointer) > (((GIV(stackPage)->realStackLimit)) - (LargeContextSlots * BytesPerOop))));
				assert((GIV(framePointer) < ((GIV(stackPage)->baseAddress)))
				 && (GIV(framePointer) > (((GIV(stackPage)->realStackLimit)) - ((LargeContextSlots * BytesPerOop) / 2))));
				(GIV(stackPage)->headFP = GIV(framePointer));
				(GIV(stackPage)->headSP = GIV(stackPointer));
				assert(pageListIsWellFormed());
				if (/* isStillMarriedContext: */
					(((((longAt((void *)((rcvr + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord())))))))) & 7) == 1))
				 && (!(isWidowedContext(rcvr)))) {
					trueOrFalse = marriedContextpointsTostackDeltaForCurrentFrame(rcvr, thang, 2);

					/* begin pop:thenPushBool: */
					longAtput((sp = GIV(stackPointer) + (1 * BytesPerWord)),/* booleanObjectOf: */
						(trueOrFalse
							? GIV(trueObj)
							: GIV(falseObj)));
					GIV(stackPointer) = sp;
					return;
				}
			}

			/* contexts end at the stack pointer */
			numSlots = CtxtTempFrameStart + (fetchStackPointerOf(rcvr));
		}
		else {
			/* begin numSlotsOf: */
			assert((classIndexOf(rcvr)) > (isForwardedObjectClassIndexPun()));
			numSlots = (((numSlotsUsqInt = byteAt((void *)(rcvr + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
						? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(rcvr - BaseHeaderSize)))) << 8)))))) >> 8
						: numSlotsUsqInt);
		}
	}
	else {
		if (fmt < (firstCompiledMethodFormat())) {
			/* begin pop:thenPushBool: */
			longAtput((sp = GIV(stackPointer) + (1 * BytesPerWord)),GIV(falseObj));
			GIV(stackPointer) = sp;
			return;
		}

		/* no pointers
		   CompiledMethod: contains both pointers and bytes: */

		/* begin methodHeaderOf: */
		assert(isCompiledMethod(rcvr));
		methodHeader = longAt((void *)((rcvr + BaseHeaderSize) + ((((usqInt)(HeaderIndex) << (shiftForWord()))))));
		if (methodHeader == thang) {
			/* begin pop:thenPushBool: */
			longAtput((sp = GIV(stackPointer) + (1 * BytesPerWord)),GIV(trueObj));
			GIV(stackPointer) = sp;
			return;
		}
		numSlots = ((/* begin literalCountOfMethodHeader: */
	assert((((methodHeader) & 7) == 1)),
/* literalCountOfAlternateHeader: */
	((methodHeader >> 3)) & AlternateHeaderNumLiteralsMask)) + LiteralStart;
	}
	assert((((numSlots - 1) * BytesPerOop) + BaseHeaderSize) == (lastPointerOf(rcvr)));
	for (i = BaseHeaderSize; i <= (((numSlots - 1) * BytesPerOop) + BaseHeaderSize); i += BytesPerOop) {
		if ((longAt((void *)(rcvr + i))) == thang) {
			/* begin pop:thenPushBool: */
			longAtput((sp = GIV(stackPointer) + (1 * BytesPerWord)),GIV(trueObj));
			GIV(stackPointer) = sp;
			return;
		}
	}

	/* begin pop:thenPushBool: */
	longAtput((sp = GIV(stackPointer) + (1 * BytesPerWord)),GIV(falseObj));
	GIV(stackPointer) = sp;
}