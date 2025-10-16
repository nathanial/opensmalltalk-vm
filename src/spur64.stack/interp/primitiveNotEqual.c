/* Extracted from interp.c:19836 (function primitiveNotEqual). */

static void
primitiveNotEqual(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt integerArgument;
    sqInt integerReceiver;
    int result;
    char *sp;

	integerArgument = longAt(GIV(stackPointer));
	integerReceiver = longAt(GIV(stackPointer) + (1 * BytesPerWord));
	if ((((integerReceiver & integerArgument) & (smallIntegerTag())) != 0)) {
		/* begin pop:thenPushBool: */
		longAtput((sp = GIV(stackPointer) + (1 * BytesPerWord)),/* booleanObjectOf: */
			(integerReceiver != integerArgument
				? GIV(trueObj)
				: GIV(falseObj)));
		GIV(stackPointer) = sp;
	}
	else {
		result = (signedMachineIntegerValueOf(integerReceiver)) != (signedMachineIntegerValueOf(integerArgument));
		if (!GIV(primFailCode)) {
			/* begin pop:thenPushBool: */
			longAtput((sp = GIV(stackPointer) + (1 * BytesPerWord)),/* booleanObjectOf: */
				(result
					? GIV(trueObj)
					: GIV(falseObj)));
			GIV(stackPointer) = sp;
		}
	}
}