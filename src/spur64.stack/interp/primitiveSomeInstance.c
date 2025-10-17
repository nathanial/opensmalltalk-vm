/* Extracted from interp.c:24047 (function primitiveSomeInstance). */

	/* InterpreterPrimitives>>#primitiveSomeInstance */

static void
primitiveSomeInstance(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt class;
    sqInt instance;
    char *sp;

	class = longAt(GIV(stackPointer));

	/* For the mirror prims check that the class obj is actually a valid class. */
	instance = initialInstanceOf(class);
	if (instance) {
		/* begin pop:thenPush: */
		longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),instance);
		GIV(stackPointer) = sp;
	}
	else {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
	}
}