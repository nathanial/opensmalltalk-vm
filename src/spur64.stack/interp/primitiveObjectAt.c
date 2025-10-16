/* Extracted from interp.c:19932 (function primitiveObjectAt). */

static void
primitiveObjectAt(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt index;
    sqInt integerPointer;
    char *sp;
    sqInt thisReceiver;

	/* begin stackIntegerValue: */
	integerPointer = longAt(GIV(stackPointer));
	if ((((integerPointer) & 7) == 1)) {
		index = (integerPointer >> 3);
	}
	else {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
		index = 0;
	}
	if (GIV(primFailCode)) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadArgument;
		return;
	}
	thisReceiver = longAt(GIV(stackPointer) + (1 * BytesPerWord));
	if (!((index > 0)
		 && (index <= ((literalCountOf(thisReceiver)) + LiteralStart)))) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadIndex;
		return;
	}

	/* begin pop:thenPush: */
	longAtput((sp = GIV(stackPointer) + (1 * BytesPerWord)),longAt((void *)((thisReceiver + BaseHeaderSize) + ((((usqInt)((index - 1)) << (shiftForWord())))))));
	GIV(stackPointer) = sp;
}