/* Extracted from interp.c:64399 (function stackPointerIndexForFrame). */

static NoDbgRegParms usqInt
stackPointerIndexForFrame(char *theFP)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    StackPage *thePage;
    char *theSP;

	/* begin stackPageFor: */
	thePage = stackPageAtpages(pageIndexForstackMemorybytesPerPage(theFP, GIV(stackMemory), GIV(bytesPerPage)), GIV(pages));
	theSP = findSPOfon(theFP, thePage);
	return (((usqInt)(((theFP + FoxReceiver) - theSP))) >> (shiftForWord())) + (byteAt((theFP + FoxFrameFlags) + 1));
}