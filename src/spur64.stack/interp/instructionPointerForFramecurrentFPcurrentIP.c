/* Extracted from interp.c:54870 (function instructionPointerForFramecurrentFPcurrentIP). */

/*	Answer the bytecode pc object (i.e. SmallInteger) for an active frame. The
	bytecode pc is derived from the frame's pc. If the frame is the top frame
	on the current stack
	the frame pc is whatever the current instruction pointer is. If the frame
	is the top
	frame on some other stack the frame pc is the value on top of stack.
	Otherwise the
	frame pc is the saved pc of the frame above. Once the frame pc is found it
	must be
	mapped to a bytecode pc. */

	/* StackInterpreter>>#instructionPointerForFrame:currentFP:currentIP: */

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