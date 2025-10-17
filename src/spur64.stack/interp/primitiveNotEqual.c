/* Extracted from interp.c:19836 (function primitiveNotEqual). */

	/* InterpreterPrimitives>>#primitiveNotEqual */

static void
primitiveNotEqual(void)
{
    sqInt integerArgument;
    sqInt integerReceiver;
    int result;
    char *sp;

	integerArgument = longAt(stackPointer);
	integerReceiver = longAt(stackPointer + (1 * BytesPerWord));
	if ((((integerReceiver & integerArgument) & (smallIntegerTag())) != 0)) {
		/* begin pop:thenPushBool: */
		longAtput((sp = stackPointer + (1 * BytesPerWord)),/* booleanObjectOf: */
			(integerReceiver != integerArgument
				? trueObj
				: falseObj));
		stackPointer = sp;
	}
	else {
		result = (signedMachineIntegerValueOf(integerReceiver)) != (signedMachineIntegerValueOf(integerArgument));
		if (!primFailCode) {
			/* begin pop:thenPushBool: */
			longAtput((sp = stackPointer + (1 * BytesPerWord)),/* booleanObjectOf: */
				(result
					? trueObj
					: falseObj));
			stackPointer = sp;
		}
	}
}