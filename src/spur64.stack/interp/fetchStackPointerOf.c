/* Extracted from interp.c:53077 (function fetchStackPointerOf). */

static NoDbgRegParms sqInt
fetchStackPointerOf(sqInt aContext)
{
    sqInt sp;

	sp = longAt((void *)((aContext + BaseHeaderSize) + ((((usqInt)(StackPointerIndex) << (shiftForWord()))))));
	if (!((((sp) & 7) == 1))) {
		return 0;
	}
	assert((ReceiverIndex + ((sp >> 3))) < (lengthOf(aContext)));
	return (sp >> 3);
}