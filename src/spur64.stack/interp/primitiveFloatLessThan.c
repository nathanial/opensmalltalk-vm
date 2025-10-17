/* Extracted from interp.c:16120 (function primitiveFloatLessThan). */

	/* InterpreterPrimitives>>#primitiveFloatLessThan */

static void
primitiveFloatLessThan(void)
{
    int aBool;
    char *sp;

	aBool = primitiveFloatLessthanArg(longAt(stackPointer + (1 * BytesPerWord)), longAt(stackPointer));
	if (!primFailCode) {
		/* begin pop:thenPushBool: */
		longAtput((sp = stackPointer + (1 * BytesPerWord)),/* booleanObjectOf: */
			(aBool
				? trueObj
				: falseObj));
		stackPointer = sp;
	}
}