/* Extracted from interp.c:19217 (function primitiveMod). */

	/* InterpreterPrimitives>>#primitiveMod */

static void
primitiveMod(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt mod;
    char *sp;

	mod = doPrimitiveModby(longAt(GIV(stackPointer) + (1 * BytesPerWord)), longAt(GIV(stackPointer)));

	/* begin pop2AndPushIntegerIfOK: */
	if (!GIV(primFailCode)) {
		if ((((((usqInt)(mod)) >> 60) + 1) & 15) <= 1) {
			/* begin pop:thenPush: */
			longAtput((sp = GIV(stackPointer) + (1 * BytesPerWord)),(((usqInt)mod << 3) | 1));
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