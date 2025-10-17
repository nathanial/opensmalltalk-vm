/* Extracted from interp.c:14770 (function primitiveDiv). */

	/* InterpreterPrimitives>>#primitiveDiv */

static void
primitiveDiv(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt quotient;
    char *sp;

	quotient = doPrimitiveDivby(longAt(GIV(stackPointer) + (1 * BytesPerWord)), longAt(GIV(stackPointer)));

	/* begin pop2AndPushIntegerIfOK: */
	if (!GIV(primFailCode)) {
		if ((((((usqInt)(quotient)) >> 60) + 1) & 15) <= 1) {
			/* begin pop:thenPush: */
			longAtput((sp = GIV(stackPointer) + (1 * BytesPerWord)),(((usqInt)quotient << 3) | 1));
			GIV(stackPointer) = sp;
		}
		else {
			/* begin success: */
			if (!GIV(primFailCode)) {
				GIV(primFailCode) = 1;
			}
		}
	}
}