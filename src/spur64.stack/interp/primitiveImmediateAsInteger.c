/* Extracted from interp.c:17919 (function primitiveImmediateAsInteger). */

static void
primitiveImmediateAsInteger(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt oop;
    char *sp;
    usqInt value;

	value = 0;
	oop = longAt(GIV(stackPointer));

	/* begin immediateAsInteger:ifFail: */
	if ((((oop) & 7) == 1)) {
		value = (oop >> 3);
		goto l1;
	}
	if (((oop & (characterTag())) != 0)) {
		value = ((((usqInt)oop))) >> (numTagBits());
		goto l1;
	}
	if (((oop & (smallFloatTag())) != 0)) {
		/* begin rotatedFloatBitsOf: */
		assert(isImmediateFloat(oop));
		value = (((((usqInt)oop))) >> ((numTagBits()) + 1)) + ((((oop & (1U << (numTagBits()))) != 0)
		? ((((usqInt)-1)) << ((64 - (numTagBits())) - 1))
		: 0));
		goto l1;
	}

	/* primitiveFailFor: */
	GIV(primFailCode) = PrimErrBadReceiver;
	return;
	/* end immediateAsInteger:ifFail: */
l1:

	/* begin pop:thenPushInteger: */
	longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),((value << 3) | 1));
	GIV(stackPointer) = sp;
}