/* Extracted from interp.c:11610 (function primitiveAdd). */

static void
primitiveAdd(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt integerResult;
    char *sp;

	integerResult = (stackIntegerValue(1)) + (stackIntegerValue(0));

	/* begin pop2AndPushIntegerIfOK: */
	if (!GIV(primFailCode)) {
		if ((((((usqInt)(integerResult)) >> 60) + 1) & 15) <= 1) {
			/* begin pop:thenPush: */
			longAtput((sp = GIV(stackPointer) + (1 * BytesPerWord)),(((usqInt)integerResult << 3) | 1));
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