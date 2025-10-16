/* Extracted from interp.c:19797 (function primitiveNextObject). */

static void
primitiveNextObject(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt instance;
    char *sp;

	if ((((longAt(GIV(stackPointer))) & (tagMask())) != 0)) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadReceiver;
		return;
	}
	if ((instance = accessibleObjectAfter(longAt(GIV(stackPointer))))) {
		assert(isInMemory(instance));

		/* begin pop:thenPush: */
		longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),instance);
		GIV(stackPointer) = sp;
	}
	else {
		/* begin pop:thenPushInteger: */
		longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),(((usqInt)0 << 3) | 1));
		GIV(stackPointer) = sp;
	}
}