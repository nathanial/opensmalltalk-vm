/* Extracted from interp.c:14770 (function primitiveDiv). */

	/* InterpreterPrimitives>>#primitiveDiv */

static void
primitiveDiv(void)
{
    sqInt quotient;
    char *sp;

	quotient = doPrimitiveDivby(longAt(stackPointer + (1 * BytesPerWord)), longAt(stackPointer));

	/* begin pop2AndPushIntegerIfOK: */
	if (!primFailCode) {
		if ((((((usqInt)(quotient)) >> 60) + 1) & 15) <= 1) {
			/* begin pop:thenPush: */
			longAtput((sp = stackPointer + (1 * BytesPerWord)),(((usqInt)quotient << 3) | 1));
			stackPointer = sp;
		}
		else {
			/* begin success: */
			if (!primFailCode) {
				primFailCode = 1;
			}
		}
	}
}