/* Extracted from interp.c:17552 (function primitiveGrowMemoryByAtLeast). */

	/* InterpreterPrimitives>>#primitiveGrowMemoryByAtLeast */

static void
primitiveGrowMemoryByAtLeast(void)
{
    sqInt ammount;
    usqInt segSize;
    char *sp;

	ammount = longAt(stackPointer);
	if (!((((ammount) & 7) == 1))) {
		/* primitiveFailFor: */
		primFailCode = PrimErrBadArgument;
		return;
	}
	if ((segSize = growOldSpaceByAtLeast((ammount >> 3)))) {
		/* begin pop:thenPushInteger: */
		longAtput((sp = stackPointer + (1 * BytesPerWord)),((segSize << 3) | 1));
		stackPointer = sp;
	}
	else {
		/* primitiveFailFor: */
		primFailCode = PrimErrNoMemory;
	}
}