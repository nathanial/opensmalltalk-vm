/* Extracted from interp.c:17454 (function primitiveGreaterOrEqual). */

	/* InterpreterPrimitives>>#primitiveGreaterOrEqual */

static void
primitiveGreaterOrEqual(void)
{
    sqInt integerArgument;
    sqInt integerReceiver;
    char *sp;

	integerArgument = longAt(stackPointer);
	integerReceiver = longAt(stackPointer + (1 * BytesPerWord));
	if ((((integerReceiver & integerArgument) & (smallIntegerTag())) != 0)) {
		/* begin pop:thenPushBool: */
		longAtput((sp = stackPointer + (1 * BytesPerWord)),/* booleanObjectOf: */
			(integerReceiver >= integerArgument
				? trueObj
				: falseObj));
		stackPointer = sp;
	}
	else {
		/* begin primitiveFail */
		if (!primFailCode) {
			primFailCode = 1;
		}
	}
}