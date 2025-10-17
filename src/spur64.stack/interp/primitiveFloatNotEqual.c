/* Extracted from interp.c:16240 (function primitiveFloatNotEqual). */

	/* InterpreterPrimitives>>#primitiveFloatNotEqual */

static void
primitiveFloatNotEqual(void)
{
    int aBool;
    char *sp;

	aBool = primitiveFloatEqualtoArg(longAt(stackPointer + (1 * BytesPerWord)), longAt(stackPointer));
	if (!primFailCode) {
		/* begin pop:thenPushBool: */
		longAtput((sp = stackPointer + (1 * BytesPerWord)),/* booleanObjectOf: */
			(aBool
				? falseObj
				: trueObj));
		stackPointer = sp;
	}
}