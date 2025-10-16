/* Extracted from interp.c:51231 (function checkStackIntegrity). */

static sqInt
checkStackIntegrity(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    char *callerFP;
    char *frameRcvrOffset;
    sqInt i;
    sqInt ok;
    usqInt oop;
    char *theFP;
    StackPage *thePage;
    char *theSP;

	callerFP = ((char *) 0);
	ok = 1;
	for (i = 0; i < GIV(numStackPages); i += 1) {
		/* begin stackPageAt: */
		thePage = stackPageAtpages(i, GIV(pages));
		if (!(isFree(thePage))) {
			if (thePage == GIV(stackPage)) {
				theSP = GIV(stackPointer);
				theFP = GIV(framePointer);
			}
			else {
				theSP = (thePage->headSP);
				theFP = (thePage->headFP);
			}

			/* Skip the instruction pointer on top of stack of inactive pages. */
			if (!(thePage == GIV(stackPage))) {
				theSP += BytesPerWord;
			}
			while (1) {
				frameRcvrOffset = theFP + FoxReceiver;
				while (theSP <= frameRcvrOffset) {
					oop = longAt(theSP);
					if (((!(oop & (tagMask()))))
					 && ((heapMapAtWord(pointerForOop(oop))) == 0)) {
						printFrameThingandFrameat("object leak in frame temp", theFP, theSP);
						ok = 0;
					}
					theSP += BytesPerWord;
				}
				if (byteAt((theFP + FoxFrameFlags) + 2)) {
					oop = longAt(theFP + FoxThisContext);
					if ((((oop & (tagMask())) != 0))
					 || ((heapMapAtWord(pointerForOop(oop))) == 0)) {
						printFrameThingandFrameat("object leak in frame ctxt", theFP, theFP + FoxThisContext);
						ok = 0;
					}
					if (!(/* isContext: */
							((!(oop & (tagMask()))))
						 && (((longAt((void *)(oop))) & (classIndexMask())) == ClassMethodContextCompactIndex))) {
						printFrameThingandFrameat("frame ctxt should be context", theFP, theFP + FoxThisContext);
						ok = 0;
					}
					if (!((/* isContext: */
							((!(oop & (tagMask()))))
						 && (((longAt((void *)(oop))) & (classIndexMask())) == ClassMethodContextCompactIndex))
						 && (((((longAt((void *)((oop + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord())))))))) & 7) == 1)))) {
						printFrameThingandFrameat("frame ctxt should be married", theFP, theFP + FoxThisContext);
						ok = 0;
					}
					if (!((/* isContext: */
							((!(oop & (tagMask()))))
						 && (((longAt((void *)(oop))) & (classIndexMask())) == ClassMethodContextCompactIndex))
						 && ((frameOfMarriedContext(oop)) == theFP))) {
						printFrameThingandFrameat("frame ctxt should be married to this frame ", theFP, theFP + FoxThisContext);
						ok = 0;
					}
				}
				oop = longAt(theFP + FoxMethod);
				if ((((oop & (tagMask())) != 0))
				 || ((heapMapAtWord(pointerForOop(oop))) == 0)) {
					printFrameThingandFrameat("object leak in iframe mthd", theFP, theFP + FoxMethod);
					ok = 0;
				}
				if (!(((callerFP = ((char *)(longAt(theFP + FoxSavedFP))))) != 0)) break;
				theSP = (theFP + FoxCallerSavedIP) + BytesPerWord;
				theFP = callerFP;
			}
			theSP = (theFP + FoxCallerSavedIP) + BytesPerWord;
			while (theSP <= ((thePage->baseAddress))) {
				oop = longAt(theSP);
				if (((!(oop & (tagMask()))))
				 && ((heapMapAtWord(pointerForOop(oop))) == 0)) {
					printFrameThingandFrameat("object leak in frame arg", theFP, theSP);
					ok = 0;
				}
				theSP += BytesPerWord;
			}
		}
	}
	return ok;
}