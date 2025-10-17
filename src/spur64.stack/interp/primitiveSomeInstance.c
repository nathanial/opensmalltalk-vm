/* Extracted from interp.c:24047 (function primitiveSomeInstance). */

	/* InterpreterPrimitives>>#primitiveSomeInstance */

static void
primitiveSomeInstance(void)
{
    sqInt class;
    sqInt instance;
    char *sp;

	class = longAt(stackPointer);

	/* For the mirror prims check that the class obj is actually a valid class. */
	instance = initialInstanceOf(class);
	if (instance) {
		/* begin pop:thenPush: */
		longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),instance);
		stackPointer = sp;
	}
	else {
		/* begin primitiveFail */
		if (!primFailCode) {
			primFailCode = 1;
		}
	}
}