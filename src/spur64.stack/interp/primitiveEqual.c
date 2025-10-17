/* Extracted from interp.c:15035 (function primitiveEqual). */

	/* InterpreterPrimitives>>#primitiveEqual */

static void
primitiveEqual(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt integerArgument;
    sqInt integerReceiver;
    int result;
    char *sp;

	integerArgument = longAt(stackPointer);
	integerReceiver = longAt(stackPointer + (1 * BytesPerWord));
	if ((((integerReceiver & integerArgument) & (smallIntegerTag())) != 0)) {
		/* begin pop:thenPushBool: */
		longAtput((sp = stackPointer + (1 * BytesPerWord)),/* booleanObjectOf: */
			(integerReceiver == integerArgument
				? trueObj
				: falseObj));
		stackPointer = sp;
	}
	else {
		result = (signed64BitValueOf(integerReceiver)) == (signed64BitValueOf(integerArgument));
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