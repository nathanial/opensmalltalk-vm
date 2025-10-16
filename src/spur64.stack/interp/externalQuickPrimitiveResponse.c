/* Extracted from interp.c:52876 (function externalQuickPrimitiveResponse). */

static sqInt
externalQuickPrimitiveResponse(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt localPrimIndex;
    sqInt oop;
    char *sp;

	assert(isPrimitiveFunctionPointerAnIndex());
	localPrimIndex = ((sqInt) primitiveFunctionPointer);
	assert((localPrimIndex > 0xFF)
	 && (localPrimIndex < 520));

	/* Quick return inst vars */
	if (localPrimIndex >= 264) {
		oop = longAt((void *)(((longAt(GIV(stackPointer))) + BaseHeaderSize) + ((((usqInt)((localPrimIndex - 264)) << (shiftForWord()))))));

		/* begin pop:thenPush: */
		longAtput((sp = GIV(stackPointer)),oop);
		GIV(stackPointer) = sp;
		return 1;
	}

	/* Quick return constants */
	if (localPrimIndex == 0x100) {
		return 1;
	}
	if (localPrimIndex == 0x101) {
		/* begin pop:thenPush: */
		longAtput((sp = GIV(stackPointer)),GIV(trueObj));
		GIV(stackPointer) = sp;
		return 1;
	}
	if (localPrimIndex == 258) {
		/* begin pop:thenPush: */
		longAtput((sp = GIV(stackPointer)),GIV(falseObj));
		GIV(stackPointer) = sp;
		return 1;
	}
	if (localPrimIndex == 259) {
		/* begin pop:thenPush: */
		longAtput((sp = GIV(stackPointer)),GIV(nilObj));
		GIV(stackPointer) = sp;
		return 1;
	}

	/* begin pop:thenPush: */
	longAtput((sp = GIV(stackPointer)),(((usqInt)(localPrimIndex - 261) << 3) | 1));
	GIV(stackPointer) = sp;
	return 1;
}