/* Extracted from interp.c:59843 (function printFrame). */

/*	useful for VM debugging */

	/* StackInterpreter>>#printFrame: */

sqInt
printFrame(char *theFP)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    char *aFrame;
    char *frameAbove;
    char *prevFrame;
    char *startFrame;
    StackPage *thePage;
    char *theSP;

	if (!(/* couldBeFramePointer: */
			(((((usqInt)theFP)) & (BytesPerWord - 1)) == 0)
		 && ((((((usqInt)theFP)) >= (((usqInt)GIV(stackMemory)))) && ((((usqInt)theFP)) <= (((usqInt)GIV(pages)))))))) {
		if ((addressCouldBeObj(((sqInt)theFP)))
		 && ((isInMemory(((sqInt)theFP)))
		 && ((((longAt((void *)(((sqInt)theFP)))) & (classIndexMask())) == ClassMethodContextCompactIndex)
		 && (checkIsStillMarriedContextcurrentFP(((sqInt)theFP), GIV(framePointer)))))) {
			return printFrame(frameOfMarriedContext(((sqInt)theFP)));
		}
		fprintf(GIV(transcript),
				"%p is not in the stack zone?!\n",
				theFP);
		return null;
	}
	frameAbove = null;
	if (theFP == GIV(framePointer)) {
		theSP = GIV(stackPointer);
	}
	else {
		/* begin stackPageFor: */
		thePage = stackPageAtpages(pageIndexForstackMemorybytesPerPage(theFP, GIV(stackMemory), GIV(bytesPerPage)), GIV(pages));
		if (isFree(thePage)) {
			fprintf(GIV(transcript),
					"%p is on a free page?!\n",
					theFP);
			return null;
		}
		if ((thePage != GIV(stackPage))
		 && (theFP == ((thePage->headFP)))) {
			theSP = (thePage->headSP);
		}
		else {
			startFrame = ((thePage == GIV(stackPage))
				 && (((GIV(framePointer) >= ((thePage->realStackLimit))) && (GIV(framePointer) <= ((thePage->baseAddress)))))
						? GIV(framePointer)
						: (thePage->headFP));

			/* begin safeFindFrameAbove:on:startingFrom: */
			prevFrame = ((char *) 0);
			assert(!(isFree(thePage)));
			if (startFrame == theFP) {
				frameAbove = null;
				goto l1;
			}
			aFrame = startFrame;
			while (1) {
				prevFrame = aFrame;
				aFrame = ((char *)(longAt(aFrame + FoxSavedFP)));
				if (!(aFrame != 0)) break;
				if (theFP == aFrame) {
					frameAbove = prevFrame;
					goto l1;
				}
			}
			frameAbove = null;
			/* end safeFindFrameAbove:on:startingFrom: */
l1:
			theSP = (frameAbove
						? frameCallerSP(frameAbove)
						: 0);
		}
	}
	if (!theSP) {
		fprintf(GIV(transcript), "could not find sp; using bogus value\n");
		theSP = theFP + FoxReceiver;
	}
	printFrameWithSP(theFP, theSP);
	if (frameAbove) {
		/* begin printFrameThing:at: */
		printFrameThingatextraString("frame pc", frameAbove + FoxCallerSavedIP, ((char *)null));
	}
	return 0;
}