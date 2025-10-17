/* Extracted from interp.c:56951 (function mapStackPages). */

	/* StackInterpreter>>#mapStackPages */

static NeverInline void
mapStackPages(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    char *callerFP;
    sqInt i;
    sqInt numLivePages;
    sqInt oop;
    char *theFP;
    sqInt theIP;
    char *theIPPtr;
    StackPage *thePage;
    char *theSP;

	callerFP = ((char *) 0);
	theIP = 0;
	numLivePages = 0;
	for (i = 0; i < GIV(numStackPages); i += 1) {
		/* begin stackPageAt: */
		thePage = stackPageAtpages(i, GIV(pages));
		if ((thePage->baseFP)) {
			assert(ifCurrentStackPageHasValidHeadPointers(thePage));
			numLivePages += 1;
			theSP = (thePage->headSP);
			theFP = (thePage->headFP);

			/* Skip the instruction pointer on top of stack of inactive pages. */
			if (thePage == GIV(stackPage)) {
				theIPPtr = 0;
			}
			else {
				theIPPtr = theSP;
				theSP += BytesPerWord;
			}
			while (1) {
				assert(addressIsInPage(thePage, theFP));
				assert(addressIsInPage(thePage, theSP));
				assert((theIPPtr == 0)
				 || (addressIsInPage(thePage, theIPPtr)));
				while (theSP <= (theFP + FoxReceiver)) {
					oop = longAt(theSP);
					if (/* shouldRemapOop: */
						((!(oop & (tagMask()))))
					 && (shouldRemapObj(oop))) {
						longAtput(theSP,remapObj(oop));
					}
					theSP += BytesPerWord;
				}
				if (byteAt((theFP + FoxFrameFlags) + 2)) {
					if (shouldRemapObj(longAt(theFP + FoxThisContext))) {
						longAtput(theFP + FoxThisContext,remapObj(longAt(theFP + FoxThisContext)));
					}

					/* With SqueakV3 objectMemory or SpurPlanningCompactor can't assert since object body is yet to move. */
					if (!(GIV(gcPhaseInProgress) == SlidingCompactionInProgress)) {
						assert((isMarriedOrWidowedContext(frameContext(theFP)))
						 && ((frameOfMarriedContext(frameContext(theFP))) == theFP));
					}
				}
				if (shouldRemapObj(longAt(theFP + FoxMethod))) {
					if (theIPPtr) {
						assert((longAt(theIPPtr)) > (frameMethod(theFP)));
						theIP = (longAt(theIPPtr)) - (longAt(theFP + FoxMethod));
					}
					longAtput(theFP + FoxMethod,remapObj(longAt(theFP + FoxMethod)));
					if (theIPPtr) {
						longAtput(theIPPtr,theIP + (longAt(theFP + FoxMethod)));
					}
				}
				if (!(((callerFP = ((char *)(longAt(theFP + FoxSavedFP))))) != 0)) break;
				theSP = ((theIPPtr = theFP + FoxCallerSavedIP)) + BytesPerWord;
				theFP = callerFP;
			}

			/* a.k.a. FoxCallerSavedIP */
			theSP = theFP + FoxCallerContext;
			while (theSP <= ((thePage->baseAddress))) {
				oop = longAt(theSP);
				if (/* shouldRemapOop: */
					((!(oop & (tagMask()))))
				 && (shouldRemapObj(oop))) {
					longAtput(theSP,remapObj(oop));
				}
				theSP += BytesPerWord;
			}
		}
	}

	/* begin recordLivePagesOnMapping: */
	GIV(statNumMaps) += 1;
	GIV(statPageCountWhenMappingSum) += numLivePages;
	GIV(statMaxPageCountWhenMapping) = ((GIV(statMaxPageCountWhenMapping) < numLivePages) ? numLivePages : GIV(statMaxPageCountWhenMapping));
}