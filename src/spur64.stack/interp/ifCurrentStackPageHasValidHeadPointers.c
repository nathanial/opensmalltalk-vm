/* Extracted from interp.c:54749 (function ifCurrentStackPageHasValidHeadPointers). */

/*	If thePage is the stackPage and the stackPointer and/or the framePointer
	are pointing within it,
	answer if thePage's heapSP and headFP are equal to the stackPointer and
	framePointer respectively.
 */

	/* StackInterpreter>>#ifCurrentStackPageHasValidHeadPointers: */

static NoDbgRegParms sqInt
ifCurrentStackPageHasValidHeadPointers(StackPage *thePage)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	if (thePage == GIV(stackPage)) {
		if (/* addressIsInPage: */
			(((thePage->lastAddress)) < GIV(framePointer))
		 && (GIV(framePointer) < ((thePage->baseAddress)))) {
			if (((thePage->headFP)) != GIV(framePointer)) {
				return 0;
			}
		}
		if (/* addressIsInPage: */
			(((thePage->lastAddress)) < GIV(stackPointer))
		 && (GIV(stackPointer) < ((thePage->baseAddress)))) {
			if (((thePage->headSP)) != GIV(stackPointer)) {
				return 0;
			}
		}
	}
	return 1;
}