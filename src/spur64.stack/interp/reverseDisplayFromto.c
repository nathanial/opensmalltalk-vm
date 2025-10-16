/* Extracted from interp.c:62654 (function reverseDisplayFromto). */

static NoDbgRegParms void
reverseDisplayFromto(sqInt startIndex, sqInt endIndex)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt primFailCodeValue;
    sqInt ptr;
    sqInt reversed;
    sqInt toDoLimit;
    sqInt wordEndIndex;
    sqInt wordStartIndex;

	if (!(postGCUpdateDisplayBits())) {
		return;
	}
	if ((displayBits == 0)
	 || (((((((sqInt)displayBits)) & (tagMask())) != 0))
	 || (displayDepth <= 0))) {
		return;
	}
	wordStartIndex = ((((startIndex < 0) ? 0 : startIndex)) * displayDepth) / 32;
	wordEndIndex = ((((endIndex < displayWidth) ? endIndex : displayWidth)) * displayDepth) / 32;
	toDoLimit = (((sqInt)displayBits)) + (wordEndIndex * 4);
	for (ptr = ((((sqInt)displayBits)) + (wordStartIndex * 4)); ptr <= toDoLimit; ptr += 4) {
		reversed = (long32At((void *)(ptr))) ^ 0xFFFFFFFFU;
		long32Atput((void *)(ptr),reversed);
	}
	primFailCodeValue = GIV(primFailCode);

	/* begin initPrimCall */
	GIV(primFailCode) = 0;
	updateDisplayLeftTopRightBottom(0, 0, displayWidth, 1);
	ioForceDisplayUpdate();
	GIV(primFailCode) = primFailCodeValue;
}