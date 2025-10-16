/* Extracted from interp.c:50185 (function assertValidExecutionPointersimbarline). */

static NoDbgRegParms void
assertValidExecutionPointersimbarline(usqInt lip, char *lfp, char *lsp, sqInt inInterpreter, sqInt ln)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	assertl(inInterpreter, ln);
	assertl(GIV(stackPage) == (GIV(mostRecentlyUsedPage)), ln);

	/* begin assertValidStackLimits: */
	assertl((GIV(stackLimit) == ((GIV(stackPage)->realStackLimit)))
	 || (GIV(stackLimit) == (allOnesAsCharStar())), ln);
	assertl((((GIV(stackPage)->stackLimit)) == ((GIV(stackPage)->realStackLimit)))
	 || (((GIV(stackPage)->stackLimit)) == (allOnesAsCharStar())), ln);
	assertl(addressIsInPage(GIV(stackPage), lfp), ln);
	assertl(lsp < lfp, ln);
	assertl(lfp > lsp, ln);
	assertl(lsp >= (((GIV(stackPage)->realStackLimit)) - (stackLimitOffset())), ln);
	assertl(((lfp - lsp) / BytesPerOop) < LargeContextSlots, ln);
	assertl(validInstructionPointerinFrame(lip, lfp), ln);
	assertl((frameIsBlockActivation(lfp))
	 || ((pushedReceiverOrClosureOfFrame(lfp)) == (frameReceiver(lfp))), ln);
	assertl(GIV(method) == (frameMethod(lfp)), ln);
	assertl((methodUsesAlternateBytecodeSet(GIV(method))) == (GIV(bytecodeSetSelector) == 0x100), ln);
}