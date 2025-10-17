/* Extracted from interp.c:16048 (function primitiveFloatEqual). */

	/* InterpreterPrimitives>>#primitiveFloatEqual */

static void
primitiveFloatEqual(void)
{
    int aBool;
    char *sp;

	aBool = primitiveFloatEqualtoArg(longAt(stackPointer + (1 * BytesPerWord)), longAt(stackPointer));
	if (!primFailCode) {
		/* begin pop:thenPushBool: */
		longAtput((sp = stackPointer + (1 * BytesPerWord)),/* booleanObjectOf: */
			(aBool
				? trueObj
				: falseObj));
		stackPointer = sp;
	}
}