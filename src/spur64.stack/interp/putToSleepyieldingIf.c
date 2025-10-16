/* Extracted from interp.c:61300 (function putToSleepyieldingIf). */

static NoDbgRegParms void
putToSleepyieldingIf(sqInt aProcess, sqInt yieldImplicitly)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt firstLink;
    sqInt objOop;
    sqInt oop;
    sqInt priority;
    sqInt processList;
    sqInt processLists;

	assert((GIV(framePointer) - GIV(stackPointer)) < (LargeContextSlots * BytesPerOop));

	/* begin quickFetchInteger:ofObject: */
	oop = longAt((void *)((aProcess + BaseHeaderSize) + ((((usqInt)(PriorityIndex) << (shiftForWord()))))));
	assert((((oop) & 7) == 1));
	priority = (oop >> 3);
	if ((GIV(highestRunnableProcessPriority) != 0)
	 && (priority > GIV(highestRunnableProcessPriority))) {
		GIV(highestRunnableProcessPriority) = priority;
	}
	objOop = longAt((void *)(((longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(SchedulerAssociation) << (shiftForWord()))))))) + BaseHeaderSize) + ((((usqInt)(ValueIndex) << (shiftForWord()))))));

	/* begin fetchPointer:ofObject: */
	processLists = longAt((void *)((objOop + BaseHeaderSize) + ((((usqInt)(ProcessListsIndex) << (shiftForWord()))))));
	processList = longAt((void *)((processLists + BaseHeaderSize) + ((((usqInt)((priority - 1)) << (shiftForWord()))))));
	if (yieldImplicitly) {
		addLastLinktoList(aProcess, processList);
	}
	else {
		/* begin addFirstLink:toList: */
		assert(!(isForwarded(processList)));
		assert((fetchPointerofObject(NextLinkIndex, aProcess)) == (nilObject()));
		firstLink = longAt((void *)((processList + BaseHeaderSize) + ((((usqInt)(FirstLinkIndex) << (shiftForWord()))))));
		assert(firstLink != aProcess);

		/* begin storePointer:ofObject:withValue: */
		assert(validStorePointerArgs(FirstLinkIndex, processList, aProcess));
		assert(isNonImmediate(processList));
		if (oopisGreaterThanOrEqualTo(processList, GIV(oldSpaceStart))) {
			if (/* isYoung: */
				((!(aProcess & (tagMask()))))
			 && (oopisLessThan(aProcess, GIV(oldSpaceStart)))) {
				/* begin possibleRootStoreInto: */
				if (!((byteAt((void *)(processList + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
					remember(processList);
				}
			}
		}

		/* most stores into young objects */
		longAtput((void *)((processList + BaseHeaderSize) + ((((usqInt)(FirstLinkIndex) << (shiftForWord()))))),aProcess);
		if (firstLink == GIV(nilObj)) {
			/* begin storePointer:ofObject:withValue: */
			assert(validStorePointerArgs(LastLinkIndex, processList, aProcess));
			assert(isNonImmediate(processList));
			if (oopisGreaterThanOrEqualTo(processList, GIV(oldSpaceStart))) {
				if (/* isYoung: */
					((!(aProcess & (tagMask()))))
				 && (oopisLessThan(aProcess, GIV(oldSpaceStart)))) {
					/* begin possibleRootStoreInto: */
					if (!((byteAt((void *)(processList + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
						remember(processList);
					}
				}
			}

			/* most stores into young objects */
			longAtput((void *)((processList + BaseHeaderSize) + ((((usqInt)(LastLinkIndex) << (shiftForWord()))))),aProcess);
		}
		else {
			/* begin storePointer:ofObject:withValue: */
			assert(validStorePointerArgs(NextLinkIndex, aProcess, firstLink));
			assert(isNonImmediate(aProcess));
			if (oopisGreaterThanOrEqualTo(aProcess, GIV(oldSpaceStart))) {
				if (/* isYoung: */
					((!(firstLink & (tagMask()))))
				 && (oopisLessThan(firstLink, GIV(oldSpaceStart)))) {
					/* begin possibleRootStoreInto: */
					if (!((byteAt((void *)(aProcess + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
						remember(aProcess);
					}
				}
			}

			/* most stores into young objects */
			longAtput((void *)((aProcess + BaseHeaderSize) + ((((usqInt)(NextLinkIndex) << (shiftForWord()))))),firstLink);
		}

		/* a.k.a. (self isEmptyList: aList) */

		/* begin storePointer:ofObject:withValue: */
		assert(validStorePointerArgs(MyListIndex, aProcess, processList));
		assert(isNonImmediate(aProcess));
		if (oopisGreaterThanOrEqualTo(aProcess, GIV(oldSpaceStart))) {
			if (/* isYoung: */
				((!(processList & (tagMask()))))
			 && (oopisLessThan(processList, GIV(oldSpaceStart)))) {
				/* begin possibleRootStoreInto: */
				if (!((byteAt((void *)(aProcess + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
					remember(aProcess);
				}
			}
		}

		/* most stores into young objects */
		longAtput((void *)((aProcess + BaseHeaderSize) + ((((usqInt)(MyListIndex) << (shiftForWord()))))),processList);
	}
}