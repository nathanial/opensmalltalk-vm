/* Extracted from interp.c:69454 (function primitiveSetGCSemaphore). */

EXPORT(void)
primitiveSetGCSemaphore(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt index;
    sqInt integerPointer;

	/* begin stackIntegerValue: */
	integerPointer = longAt(GIV(stackPointer));
	if ((((integerPointer) & 7) == 1)) {
		index = (integerPointer >> 3);
	}
	else {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
		index = 0;
	}
	if (!GIV(primFailCode)) {
		GIV(gcSemaphoreIndex) = index;

		/* begin pop: */
		GIV(stackPointer) += GIV(argumentCount) * BytesPerWord;
	}
}