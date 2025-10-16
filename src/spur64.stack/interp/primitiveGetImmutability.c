/* Extracted from interp.c:17222 (function primitiveGetImmutability). */

static void
primitiveGetImmutability(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt boolean;
    sqInt rcvr;
    char *sp;

	rcvr = longAt(GIV(stackPointer));
	if ((GIV(argumentCount) > 0)
	 && (/* isOopForwarded: */
		((!(rcvr & (tagMask()))))
	 && ((!((longAt((void *)(rcvr))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))))) {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
		return;
	}
	boolean = /* isOopImmutable: */
			(((rcvr & (tagMask())) != 0))
		 || (((((usqInt)((byteAt((void *)(rcvr + (immutableExtraBitsByteOffset())))))) >> (immutableBitByteShift())) & 1) != 0);

	/* begin methodReturnBool: */
	assert(!((failed())));
	longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),/* booleanObjectOf: */
		(boolean
			? GIV(trueObj)
			: GIV(falseObj)));
	GIV(stackPointer) = sp;
}