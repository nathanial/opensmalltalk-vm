/* Extracted from interp.c:51090 (function checkOkayStackPage). */

	/* StackInterpreter>>#checkOkayStackPage: */

static NoDbgRegParms sqInt
checkOkayStackPage(StackPage *thePage)
{
    char *callerFP;
    char *frameRcvrOffset;
    sqInt ok;
    sqInt oop;
    char *theFP;
    char *theSP;

	callerFP = ((char *) 0);
	theSP = (thePage->headSP);
	theFP = (thePage->headFP);
	ok = 1;

	/* Skip the instruction pointer on top of stack of inactive pages. */
	if (!(thePage == stackPage)) {
		theSP += BytesPerWord;
	}
	while (1) {
		frameRcvrOffset = theFP + FoxReceiver;
		while (theSP <= frameRcvrOffset) {
			oop = longAt(theSP);
			if (!((((oop & (tagMask())) != 0))
				 || (checkOkayFields(oop)))) {
				ok = 0;
			}
			theSP += BytesPerWord;
		}
		if (byteAt((theFP + FoxFrameFlags) + 2)) {
			assert(isContext(frameContext(theFP)));
			if (!(checkOkayFields(longAt(theFP + FoxThisContext)))) {
				ok = 0;
			}
		}
		if (!(checkOkayFields(longAt(theFP + FoxMethod)))) {
			ok = 0;
		}
		if (!(((callerFP = ((char *)(longAt(theFP + FoxSavedFP))))) != 0)) break;
		theSP = (theFP + FoxCallerSavedIP) + BytesPerWord;
		theFP = callerFP;
	}

	/* caller ip is frameCallerContext in a base frame */
	theSP = theFP + FoxCallerSavedIP;
	while (theSP <= ((thePage->baseAddress))) {
		oop = longAt(theSP);
		if (!((((oop & (tagMask())) != 0))
			 || (checkOkayFields(oop)))) {
			ok = 0;
		}
		theSP += BytesPerWord;
	}
	return ok;
}