/* Extracted from interp.c:61051 (function printStackReferencesTo). */

/*	useful for VM debugging */

	/* StackInterpreter>>#printStackReferencesTo: */

void
printStackReferencesTo(sqInt oop)
{
    char *callerFP;
    sqInt i;
    char *theFP;
    StackPage *thePage;
    char *theSP;

	callerFP = ((char *) 0);
	for (i = 0; i < numStackPages; i += 1) {
		/* begin stackPageAt: */
		thePage = stackPageAtpages(i, pages);
		if ((thePage->baseFP)) {
			theSP = (thePage->headSP);
			theFP = (thePage->headFP);

			/* Skip the instruction pointer on top of stack of inactive pages. */
			if (!(thePage == stackPage)) {
				theSP += BytesPerWord;
			}
			while (1) {
				while (theSP <= (theFP + FoxReceiver)) {
					if (oop == (longAt(theSP))) {
						fprintf(transcript,
								"FP:%p @ %p\n",
								theFP,
								theSP);
					}
					theSP += BytesPerWord;
				}
				if (((byteAt((theFP + FoxFrameFlags) + 2)) != 0)
				 && (oop == (longAt(theFP + FoxThisContext)))) {
					fprintf(transcript,
							"FP:%p CTXT\n",
							theFP);
				}
				if (oop == (longAt(theFP + FoxMethod))) {
					fprintf(transcript,
							"FP:%p MTHD\n",
							theFP);
				}
				if (!(((callerFP = ((char *)(longAt(theFP + FoxSavedFP))))) != 0)) break;
				theSP = (theFP + FoxCallerSavedIP) + BytesPerWord;
				theFP = callerFP;
			}

			/* a.k.a. FoxCallerContext */
			theSP = theFP + FoxCallerSavedIP;
			while (theSP <= ((thePage->baseAddress))) {
				if (oop == (longAt(theSP))) {
					fprintf(transcript,
							"FP:%p @ %p\n",
							theFP,
							theSP);
				}
				theSP += BytesPerWord;
			}
		}
	}
}