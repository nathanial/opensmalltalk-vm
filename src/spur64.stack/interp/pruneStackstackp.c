/* Extracted from interp.c:72153 (function pruneStackstackp). */

static NoDbgRegParms void
pruneStackstackp(sqInt stack, sqInt stackp)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    char *callerFP;
    sqInt finger;
    char *fp;
    sqInt i;
    sqInt objOrFP;
    sqInt oop;
    char *theFP;
    char *theFPAbove;
    StackPage *thePage;
    char *theSP;
    sqInt theStack;
    sqInt toDoLimit;

	finger = 1;
	for (i = 2; i < stackp; i += 2) {
		objOrFP = longAt((void *)((stack + BaseHeaderSize) + ((((usqInt)(i) << (shiftForWord()))))));
		if (/* couldBeFramePointer: */
			(((((usqInt)(((char *) objOrFP)))) & (BytesPerWord - 1)) == 0)
		 && ((((((usqInt)(((char *) objOrFP)))) >= (((usqInt)GIV(stackMemory)))) && ((((usqInt)(((char *) objOrFP)))) <= (((usqInt)GIV(pages))))))) {
			objOrFP += smallIntegerTag();
		}

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(stack))
		 && (!(isForwarded(stack))));
		assert(validStorePointerUncheckedArgs(finger, stack, objOrFP));
		longAtput((void *)((stack + BaseHeaderSize) + ((((usqInt)(finger) << (shiftForWord()))))),objOrFP);
		finger += 1;
	}
	toDoLimit = (lengthOf(stack)) - 1;
	for (i = finger; i <= toDoLimit; i += 1) {
		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(stack))
		 && (!(isForwarded(stack))));
		assert(validStorePointerUncheckedArgs(i, stack, GIV(nilObj)));
		longAtput((void *)((stack + BaseHeaderSize) + ((((usqInt)(i) << (shiftForWord()))))),GIV(nilObj));
	}
	oop = (theStack = stack);

	/* begin pushRemappableOop: */
	assert(addressCouldBeOop(oop));
	GIV(remapBuffer)[(GIV(remapBufferCount) += 1)] = oop;
	if (!(GIV(remapBufferCount) <= RemapBufferSize)) {
		error("remapBuffer overflow");
	}
	for (i = 1; i < finger; i += 1) {
		objOrFP = longAt((void *)((theStack + BaseHeaderSize) + ((((usqInt)(i) << (shiftForWord()))))));
		if ((((objOrFP) & 7) == 1)) {
			/* begin withoutSmallIntegerTags: */
			assert((((objOrFP) & 7) == 1));
			theFP = ((char *)(objOrFP - (smallIntegerTag())));

			/* begin stackPageFor: */
			thePage = stackPageAtpages(pageIndexForstackMemorybytesPerPage(theFP, GIV(stackMemory), GIV(bytesPerPage)), GIV(pages));
			callerFP = ((char *) 0);
			fp = (thePage->headFP);
			if (fp == theFP) {
				theFPAbove = 0;
				goto l1;
			}
			while (((callerFP = ((char *)(longAt(fp + FoxSavedFP))))) != 0) {
				if (callerFP == theFP) {
					theFPAbove = fp;
					goto l1;
				}
				fp = callerFP;
			}
			error("did not find theFP in stack page");
			theFPAbove = 0;
			/* end findFrameAbove:inPage: */
l1:

			/* begin frameCallerSP: */
			assert(!(isBaseFrame(theFPAbove)));
			theSP = (theFPAbove + ((FoxCallerSavedIP + BytesPerWord) + ((((usqInt)((byteAt((theFPAbove + FoxFrameFlags) + 1))) << (shiftForWord())))))) + BytesPerWord;

			/* begin ensureFrameIsMarried:SP: */
			if (byteAt((theFP + FoxFrameFlags) + 2)) {
				assert(isContext(frameContext(theFP)));
				objOrFP = longAt(theFP + FoxThisContext);
				goto l2;
			}
			objOrFP = marryFrameSP(theFP, theSP);
			/* end ensureFrameIsMarried:SP: */
l2:
			theStack = GIV(remapBuffer)[GIV(remapBufferCount)];

			/* after a GC stack may no longer be a root. */

			/* begin storePointer:ofObject:withValue: */
			assert(validStorePointerArgs(finger, theStack, objOrFP));
			assert(isNonImmediate(theStack));
			if (oopisGreaterThanOrEqualTo(theStack, GIV(oldSpaceStart))) {
				if (/* isYoung: */
					((!(objOrFP & (tagMask()))))
				 && (oopisLessThan(objOrFP, GIV(oldSpaceStart)))) {
					/* begin possibleRootStoreInto: */
					if (!((byteAt((void *)(theStack + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
						remember(theStack);
					}
				}
			}

			/* most stores into young objects */
			longAtput((void *)((theStack + BaseHeaderSize) + ((((usqInt)(finger) << (shiftForWord()))))),objOrFP);
		}
	}

	/* begin popRemappableOop */
	oop = GIV(remapBuffer)[GIV(remapBufferCount)];
	GIV(remapBufferCount) -= 1;
}