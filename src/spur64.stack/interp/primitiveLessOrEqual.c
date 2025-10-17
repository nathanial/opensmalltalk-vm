/* Extracted from interp.c:18720 (function primitiveLessOrEqual). */

	/* InterpreterPrimitives>>#primitiveLessOrEqual */

static void
primitiveLessOrEqual(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt integerArgument;
    sqInt integerReceiver;
    char *sp;

	integerArgument = longAt(GIV(stackPointer));
	integerReceiver = longAt(GIV(stackPointer) + (1 * BytesPerWord));
	if ((((integerReceiver & integerArgument) & (smallIntegerTag())) != 0)) {
		/* begin pop:thenPushBool: */
		longAtput((sp = GIV(stackPointer) + (1 * BytesPerWord)),/* booleanObjectOf: */
			(integerReceiver <= integerArgument
				? GIV(trueObj)
				: GIV(falseObj)));
		GIV(stackPointer) = sp;
	}
	else {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
	}
}