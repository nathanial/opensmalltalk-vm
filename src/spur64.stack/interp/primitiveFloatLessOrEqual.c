/* Extracted from interp.c:16102 (function primitiveFloatLessOrEqual). */

	/* InterpreterPrimitives>>#primitiveFloatLessOrEqual */

static void
primitiveFloatLessOrEqual(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    int aBool;
    char *sp;

	aBool = primitiveFloatLessOrEqualtoArg(longAt(stackPointer + (1 * BytesPerWord)), longAt(stackPointer));
	if (!primFailCode) {
		/* begin pop:thenPushBool: */
		longAtput((sp = stackPointer + (1 * BytesPerWord)),/* booleanObjectOf: */
			(aBool
				? trueObj
				: falseObj));
		stackPointer = sp;
	}
}