/* Extracted from interp.c:49911 (function addLastLinktoList). */

/*	Add the given process to the end of the given linked list
	and set the backpointer of process to its new list. */

	/* StackInterpreter>>#addLastLink:toList: */

static NoDbgRegParms void
addLastLinktoList(sqInt proc, sqInt aList)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt lastLink;

	assert(!((isForwarded(proc))));
	assert(!((isForwarded(aList))));
	assert((fetchPointerofObject(NextLinkIndex, proc)) == (nilObject()));

	/* begin isEmptyList: */
	assert(!(isForwarded(aList)));
	if ((longAt((void *)((aList + BaseHeaderSize) + ((((usqInt)(FirstLinkIndex) << (shiftForWord()))))))) == nilObj) {
		/* begin storePointer:ofObject:withValue: */
		assert(validStorePointerArgs(FirstLinkIndex, aList, proc));
		assert(isNonImmediate(aList));
		if (oopisGreaterThanOrEqualTo(aList, oldSpaceStart)) {
			if (/* isYoung: */
				((!(proc & (tagMask()))))
			 && (oopisLessThan(proc, oldSpaceStart))) {
				/* begin possibleRootStoreInto: */
				if (!((byteAt((void *)(aList + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
					remember(aList);
				}
			}
		}

		/* most stores into young objects */
		longAtput((void *)((aList + BaseHeaderSize) + ((((usqInt)(FirstLinkIndex) << (shiftForWord()))))),proc);
	}
	else {
		lastLink = longAt((void *)((aList + BaseHeaderSize) + ((((usqInt)(LastLinkIndex) << (shiftForWord()))))));
		assert(lastLink != proc);

		/* begin storePointer:ofObject:withValue: */
		assert(validStorePointerArgs(NextLinkIndex, lastLink, proc));
		assert(isNonImmediate(lastLink));
		if (oopisGreaterThanOrEqualTo(lastLink, oldSpaceStart)) {
			if (/* isYoung: */
				((!(proc & (tagMask()))))
			 && (oopisLessThan(proc, oldSpaceStart))) {
				/* begin possibleRootStoreInto: */
				if (!((byteAt((void *)(lastLink + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
					remember(lastLink);
				}
			}
		}

		/* most stores into young objects */
		longAtput((void *)((lastLink + BaseHeaderSize) + ((((usqInt)(NextLinkIndex) << (shiftForWord()))))),proc);
	}

	/* begin storePointer:ofObject:withValue: */
	assert(validStorePointerArgs(LastLinkIndex, aList, proc));
	assert(isNonImmediate(aList));
	if (oopisGreaterThanOrEqualTo(aList, oldSpaceStart)) {
		if (/* isYoung: */
			((!(proc & (tagMask()))))
		 && (oopisLessThan(proc, oldSpaceStart))) {
			/* begin possibleRootStoreInto: */
			if (!((byteAt((void *)(aList + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
				remember(aList);
			}
		}
	}

	/* most stores into young objects */
	longAtput((void *)((aList + BaseHeaderSize) + ((((usqInt)(LastLinkIndex) << (shiftForWord()))))),proc);

	/* begin storePointer:ofObject:withValue: */
	assert(validStorePointerArgs(MyListIndex, proc, aList));
	assert(isNonImmediate(proc));
	if (oopisGreaterThanOrEqualTo(proc, oldSpaceStart)) {
		if (/* isYoung: */
			((!(aList & (tagMask()))))
		 && (oopisLessThan(aList, oldSpaceStart))) {
			/* begin possibleRootStoreInto: */
			if (!((byteAt((void *)(proc + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
				remember(proc);
			}
		}
	}

	/* most stores into young objects */
	longAtput((void *)((proc + BaseHeaderSize) + ((((usqInt)(MyListIndex) << (shiftForWord()))))),aList);
}