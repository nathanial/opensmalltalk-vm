/* Extracted from interp.c:16240 (function primitiveFloatNotEqual). */

static void
primitiveFloatNotEqual(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    int aBool;
    char *sp;

	aBool = primitiveFloatEqualtoArg(longAt(GIV(stackPointer) + (1 * BytesPerWord)), longAt(GIV(stackPointer)));
	if (!GIV(primFailCode)) {
		/* begin pop:thenPushBool: */
		longAtput((sp = GIV(stackPointer) + (1 * BytesPerWord)),/* booleanObjectOf: */
			(aBool
				? GIV(falseObj)
				: GIV(trueObj)));
		GIV(stackPointer) = sp;
	}
}