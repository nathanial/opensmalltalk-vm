/* Extracted from interp.c:40556 (function markWeaklingsAndMarkAndFireEphemerons). */

/*	After the initial scan-mark is complete ephemerons can be processed.
	Weaklings have accumulated on the weaklingStack, but more may be
	uncovered during ephemeron processing. So trace the strong slots
	of the weaklings, and as ephemerons are processed ensure any newly
	reached weaklings are also traced. */

	/* SpurMemoryManager>>#markWeaklingsAndMarkAndFireEphemerons */

static void
markWeaklingsAndMarkAndFireEphemerons(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt ephemeron;
    sqInt field;
    sqInt i;
    sqInt iSqInt;
    sqInt key;
    sqInt numOnThisPage;
    sqInt numToEnumerate;
    sqInt numToEnumerateOnThisPage;
    sqInt numTracedWeaklings;
    sqInt objStackPage;
    usqInt p;
    sqInt size;
    sqInt toDoLimit;
    sqInt topIndex;
    sqInt weakling;

	numTracedWeaklings = 0;
	while (1) {
		markAndTraceUntracedReachableStackPages();

		/* Make sure all reached weaklings have their strong slots traced before firing ephemerons... */
		do {
			/* begin markAndTraceWeaklingsFrom: */
			/* begin objStack:from:do: */
			eassert(isValidObjStack(GIV(weaklingStack)));
			size = longAt((void *)((GIV(weaklingStack) + BaseHeaderSize) + ((((usqInt)(ObjStackTopx) << (shiftForWord()))))));
			objStackPage = longAt((void *)((GIV(weaklingStack) + BaseHeaderSize) + ((((usqInt)(ObjStackNextx) << (shiftForWord()))))));
			while (objStackPage != 0) {
				size += ObjStackLimit;
				assert((fetchPointerofObject(ObjStackTopx, objStackPage)) == ObjStackLimit);
				objStackPage = longAt((void *)((objStackPage + BaseHeaderSize) + ((((usqInt)(ObjStackNextx) << (shiftForWord()))))));
			}
			numToEnumerate = size - numTracedWeaklings;
			objStackPage = GIV(weaklingStack);
			while (numToEnumerate > 0) {
				numOnThisPage = longAt((void *)((objStackPage + BaseHeaderSize) + ((((usqInt)(ObjStackTopx) << (shiftForWord()))))));
				numToEnumerateOnThisPage = ((numToEnumerate < numOnThisPage) ? numToEnumerate : numOnThisPage);
				topIndex = (numOnThisPage + ObjStackFixedSlots) - 1;
				for (iSqInt = topIndex; iSqInt >= ((topIndex - numToEnumerateOnThisPage) + 1); iSqInt += -1) {
					assert(isWeak(fetchPointerofObject(iSqInt, objStackPage)));
					weakling = longAt((void *)((objStackPage + BaseHeaderSize) + ((((usqInt)(iSqInt) << (shiftForWord()))))));
					assert(!((isForwarded(weakling))));
					markAndTraceClassOf(weakling);

					/* N.B. generateToByDoLimitExpression:negative:on: guards against (unsigned)0 - 1 going +ve */
					toDoLimit = (numStrongSlotsOfWeakling(weakling)) - 1;
					for (i = 0; i <= toDoLimit; i += 1) {
						/* begin followOopField:ofObject: */
						field = longAt((void *)((weakling + BaseHeaderSize) + ((((usqInt)(i) << (shiftForWord()))))));
						if (/* isOopForwarded: */
							((!(field & (tagMask()))))
						 && ((!((longAt((void *)(field))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
							field = fixFollowedFieldofObjectwithInitialValue(i, weakling, field);
						}
						if (!((((field & (tagMask())) != 0))
							 || (((byteAt((void *)(field + (markBitsByteOffset())))) & (1U << (markedBitByteShift()))) != 0))) {
							markAndTrace(field);
						}
					}
				}
				numToEnumerate -= numToEnumerateOnThisPage;
				objStackPage = longAt((void *)((objStackPage + BaseHeaderSize) + ((((usqInt)(ObjStackNextx) << (shiftForWord()))))));
			}
			numTracedWeaklings = size;
		} while((sizeOfObjStack(GIV(weaklingStack))) > numTracedWeaklings);
		if (((GIV(unscannedEphemerons).top)) < ((GIV(unscannedEphemerons).start))) {
			markAndTraceUntracedReachableStackPages();
			freeUntracedStackPages();
			return;
		}
		if (!(markInactiveEphemerons())) {
			/* begin fireAllUnscannedEphemerons */
			assert(!(noUnscannedEphemerons()));
			assert(allUnscannedEphemeronsAreActive());
			for (p = ((GIV(unscannedEphemerons).start)); p <= ((GIV(unscannedEphemerons).top)); p += BytesPerOop) {
				fireEphemeron(longAt((void *)(p)));
			}
		}

		/* begin markAllUnscannedEphemerons */
		assert(allUnscannedEphemeronsAreActive());
		while (((GIV(unscannedEphemerons).top)) >= ((GIV(unscannedEphemerons).start))) {
			ephemeron = longAt((void *)((GIV(unscannedEphemerons).start)));
			longAtput((void *)((GIV(unscannedEphemerons).start)),longAt((void *)((GIV(unscannedEphemerons).top))));
			(GIV(unscannedEphemerons).top = ((GIV(unscannedEphemerons).top)) - BytesPerOop);

			/* begin followedKeyOfMaybeFiredEphemeron: */
			assert((isNonImmediate(ephemeron))
			 && (isMaybeFiredEphemeron(ephemeron)));

			/* begin followOopField:ofObject: */
			key = longAt((void *)((ephemeron + BaseHeaderSize) + (0U << (shiftForWord()))));
			if (/* isOopForwarded: */
				((!(key & (tagMask()))))
			 && ((!((longAt((void *)(key))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
				key = fixFollowedFieldofObjectwithInitialValue(0, ephemeron, key);
			}

			/* begin setIsMarkedOf:to: */
			assert(!(isFreeObject(ephemeron)));
			byteAtput((void *)(ephemeron + (markBitsByteOffset())),(byteAt((void *)(ephemeron + (markBitsByteOffset())))) & (0xFF - (1U << (markedBitByteShift()))));
			markAndTrace(key);
			markAndTrace(ephemeron);
		}
	}
}