/* Extracted from interp.c:17552 (function primitiveGrowMemoryByAtLeast). */

	/* InterpreterPrimitives>>#primitiveGrowMemoryByAtLeast */

static void
primitiveGrowMemoryByAtLeast(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt ammount;
    usqInt segSize;
    char *sp;

	ammount = longAt(GIV(stackPointer));
	if (!((((ammount) & 7) == 1))) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadArgument;
		return;
	}
	if ((segSize = growOldSpaceByAtLeast((ammount >> 3)))) {
		/* begin pop:thenPushInteger: */
		longAtput((sp = GIV(stackPointer) + (1 * BytesPerWord)),((segSize << 3) | 1));
		GIV(stackPointer) = sp;
	}
	else {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrNoMemory;
	}
}