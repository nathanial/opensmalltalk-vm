/* Extracted from interp.c:17222 (function primitiveGetImmutability). */

static void
primitiveGetImmutability(void)
{
    sqInt boolean;
    sqInt rcvr;
    char *sp;

	rcvr = longAt(stackPointer);
	if ((argumentCount > 0)
	 && (/* isOopForwarded: */
		((!(rcvr & (tagMask()))))
	 && ((!((longAt((void *)(rcvr))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))))) {
		/* begin primitiveFail */
		if (!primFailCode) {
			primFailCode = 1;
		}
		return;
	}
	boolean = /* isOopImmutable: */
			(((rcvr & (tagMask())) != 0))
		 || (((((usqInt)((byteAt((void *)(rcvr + (immutableExtraBitsByteOffset())))))) >> (immutableBitByteShift())) & 1) != 0);

	/* begin methodReturnBool: */
	assert(!((failed())));
	longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),/* booleanObjectOf: */
		(boolean
			? trueObj
			: falseObj));
	stackPointer = sp;
}