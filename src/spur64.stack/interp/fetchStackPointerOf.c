/* Extracted from interp.c:53077 (function fetchStackPointerOf). */

/*	Return the stackPointer of a Context or BlockContext.
	Does not deal with married contexts. Use only for debug
	printing or object tracing functions. To obtain an accurate
	stack pointer use stackPointerForMaybeMarriedContext: */

	/* StackInterpreter>>#fetchStackPointerOf: */

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