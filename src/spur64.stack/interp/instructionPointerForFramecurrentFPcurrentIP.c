/* Extracted from interp.c:54870 (function instructionPointerForFramecurrentFPcurrentIP). */

static NoDbgRegParms sqInt
instructionPointerForFramecurrentFPcurrentIP(char *spouseFP, char *currentFP, sqInt instrPtr)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    char *callerFP;
    char *fp;
    char *theFPAbove;
    sqInt theIP;
    StackPage *thePage;

	if (spouseFP == currentFP) {
		theIP = instrPtr;
	}
	else {
		/* begin stackPageFor: */
		thePage = stackPageAtpages(pageIndexForstackMemorybytesPerPage(spouseFP, GIV(stackMemory), GIV(bytesPerPage)), GIV(pages));
		callerFP = ((char *) 0);
		fp = (thePage->headFP);
		if (fp == spouseFP) {
			theFPAbove = 0;
			goto l1;
		}
		while (((callerFP = ((char *)(longAt(fp + FoxSavedFP))))) != 0) {
			if (callerFP == spouseFP) {
				theFPAbove = fp;
				goto l1;
			}
			fp = callerFP;
		}
		error("did not find theFP in stack page");
		theFPAbove = 0;
		/* end findFrameAbove:inPage: */
l1:
		theIP = (theFPAbove
					? longAt(theFPAbove + FoxCallerSavedIP)
					: longAt((thePage->headSP)));
	}

	/* begin contextInstructionPointer:frame: */
	assert(validInstructionPointerinFrame(theIP + 1, spouseFP));
	return (((usqInt)(((theIP - (longAt(spouseFP + FoxMethod))) - BaseHeaderSize) + 2) << 3) | 1);
}