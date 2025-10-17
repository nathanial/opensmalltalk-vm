/* Extracted from interp.c:62090 (function removeProcessfromList). */

/*	Attempt to remove a process from a linked list. Answer if the attempt
	succeeded. 
 */

	/* StackInterpreter>>#removeProcess:fromList: */

static NoDbgRegParms sqInt
removeProcessfromList(sqInt aProcess, sqInt aList)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt firstLink;
    sqInt lastLink;
    sqInt nextLink;
    usqInt numSlots;
    sqInt tempLink;

	nextLink = 0;
	assert(!((isForwarded(aProcess))));
	assert(!((isForwarded(aList))));

	/* any process on the list could have been becomed, so use a read barrier... */

	/* begin followObjField:ofObject: */
	firstLink = longAt((void *)((aList + BaseHeaderSize) + ((((usqInt)(FirstLinkIndex) << (shiftForWord()))))));
	assert(isNonImmediate(firstLink));
	if ((!((longAt((void *)(firstLink))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
		firstLink = fixFollowedFieldofObjectwithInitialValue(FirstLinkIndex, aList, firstLink);
	}

	/* begin followObjField:ofObject: */
	lastLink = longAt((void *)((aList + BaseHeaderSize) + ((((usqInt)(LastLinkIndex) << (shiftForWord()))))));
	assert(isNonImmediate(lastLink));
	if ((!((longAt((void *)(lastLink))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
		lastLink = fixFollowedFieldofObjectwithInitialValue(LastLinkIndex, aList, lastLink);
	}
	if (aProcess == firstLink) {
		/* begin followObjField:ofObject: */
		nextLink = longAt((void *)((aProcess + BaseHeaderSize) + ((((usqInt)(NextLinkIndex) << (shiftForWord()))))));
		assert(isNonImmediate(nextLink));
		if ((!((longAt((void *)(nextLink))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
			nextLink = fixFollowedFieldofObjectwithInitialValue(NextLinkIndex, aProcess, nextLink);
		}

		/* begin storePointer:ofObject:withValue: */
		assert(validStorePointerArgs(FirstLinkIndex, aList, nextLink));
		assert(isNonImmediate(aList));
		if (oopisGreaterThanOrEqualTo(aList, oldSpaceStart)) {
			if (/* isYoung: */
				((!(nextLink & (tagMask()))))
			 && (oopisLessThan(nextLink, oldSpaceStart))) {
				/* begin possibleRootStoreInto: */
				if (!((byteAt((void *)(aList + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
					remember(aList);
				}
			}
		}

		/* most stores into young objects */
		longAtput((void *)((aList + BaseHeaderSize) + ((((usqInt)(FirstLinkIndex) << (shiftForWord()))))),nextLink);
		if (aProcess == lastLink) {
			/* begin storePointerUnchecked:ofObject:withValue: */
			assert((isNonImmediate(aList))
			 && (!(isForwarded(aList))));
			assert(validStorePointerUncheckedArgs(LastLinkIndex, aList, nilObj));
			longAtput((void *)((aList + BaseHeaderSize) + ((((usqInt)(LastLinkIndex) << (shiftForWord()))))),nilObj);
		}
	}
	else {
		tempLink = firstLink;

		/* fail if any link doesn't look like a process... */
		while (1) {
			if (!((/* isPointers: */
					((!(tempLink & (tagMask()))))
				 && (((byteAt((void *)(tempLink + (formatFieldByteOffset())))) & (formatMask())) <= 5 /* lastPointerFormat */))
				 && (((/* begin numSlotsOf: */
					assert((classIndexOf(tempLink)) > (isForwardedObjectClassIndexPun())),
				(((numSlots = byteAt((void *)(tempLink + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
							? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(tempLink - BaseHeaderSize)))) << 8)))))) >> 8
							: numSlots))) > MyListIndex))) {
				return 0;
			}

			/* begin followObjField:ofObject: */
			nextLink = longAt((void *)((tempLink + BaseHeaderSize) + ((((usqInt)(NextLinkIndex) << (shiftForWord()))))));
			assert(isNonImmediate(nextLink));
			if ((!((longAt((void *)(nextLink))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
				nextLink = fixFollowedFieldofObjectwithInitialValue(NextLinkIndex, tempLink, nextLink);
			}
			if (nextLink == aProcess) break;
			tempLink = nextLink;
		}
		nextLink = longAt((void *)((aProcess + BaseHeaderSize) + ((((usqInt)(NextLinkIndex) << (shiftForWord()))))));

		/* begin storePointer:ofObject:withValue: */
		assert(validStorePointerArgs(NextLinkIndex, tempLink, nextLink));
		assert(isNonImmediate(tempLink));
		if (oopisGreaterThanOrEqualTo(tempLink, oldSpaceStart)) {
			if (/* isYoung: */
				((!(nextLink & (tagMask()))))
			 && (oopisLessThan(nextLink, oldSpaceStart))) {
				/* begin possibleRootStoreInto: */
				if (!((byteAt((void *)(tempLink + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
					remember(tempLink);
				}
			}
		}

		/* most stores into young objects */
		longAtput((void *)((tempLink + BaseHeaderSize) + ((((usqInt)(NextLinkIndex) << (shiftForWord()))))),nextLink);
		if (aProcess == lastLink) {
			/* begin storePointer:ofObject:withValue: */
			assert(validStorePointerArgs(LastLinkIndex, aList, tempLink));
			assert(isNonImmediate(aList));
			if (oopisGreaterThanOrEqualTo(aList, oldSpaceStart)) {
				if (/* isYoung: */
					((!(tempLink & (tagMask()))))
				 && (oopisLessThan(tempLink, oldSpaceStart))) {
					/* begin possibleRootStoreInto: */
					if (!((byteAt((void *)(aList + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
						remember(aList);
					}
				}
			}

			/* most stores into young objects */
			longAtput((void *)((aList + BaseHeaderSize) + ((((usqInt)(LastLinkIndex) << (shiftForWord()))))),tempLink);
		}
	}

	/* begin storePointerUnchecked:ofObject:withValue: */
	assert((isNonImmediate(aProcess))
	 && (!(isForwarded(aProcess))));
	assert(validStorePointerUncheckedArgs(NextLinkIndex, aProcess, nilObj));
	longAtput((void *)((aProcess + BaseHeaderSize) + ((((usqInt)(NextLinkIndex) << (shiftForWord()))))),nilObj);

	/* begin storePointerUnchecked:ofObject:withValue: */
	assert((isNonImmediate(aProcess))
	 && (!(isForwarded(aProcess))));
	assert(validStorePointerUncheckedArgs(MyListIndex, aProcess, nilObj));
	longAtput((void *)((aProcess + BaseHeaderSize) + ((((usqInt)(MyListIndex) << (shiftForWord()))))),nilObj);
	return 1;
}