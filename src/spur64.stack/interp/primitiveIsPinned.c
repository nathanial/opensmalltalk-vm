/* Extracted from interp.c:18695 (function primitiveIsPinned). */

/*	Answer if the receiver is pinned, i.e. immobile. */

	/* InterpreterPrimitives>>#primitiveIsPinned */

static void
primitiveIsPinned(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt obj;
    char *sp;
    sqInt trueOrFalse;

	obj = longAt(GIV(stackPointer));
	if ((((obj & (tagMask())) != 0))
	 || ((!((longAt((void *)(obj))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadReceiver;
		return;
	}
	trueOrFalse = ((byteAt((void *)(obj + (formatFieldByteOffset())))) & (1U << (pinnedBitByteShift()))) != 0;

	/* begin pop:thenPushBool: */
	longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),/* booleanObjectOf: */
		(trueOrFalse
			? GIV(trueObj)
			: GIV(falseObj)));
	GIV(stackPointer) = sp;
}