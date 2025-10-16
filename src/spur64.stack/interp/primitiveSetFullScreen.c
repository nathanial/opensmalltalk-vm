/* Extracted from interp.c:22022 (function primitiveSetFullScreen). */

static void
primitiveSetFullScreen(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt argOop;

	argOop = longAt(GIV(stackPointer));
	if (argOop == GIV(trueObj)) {
		ioSetFullScreen(1);
	}
	else {
		if (argOop == GIV(falseObj)) {
			ioSetFullScreen(0);
		}
		else {
			/* begin primitiveFail */
			if (!GIV(primFailCode)) {
				GIV(primFailCode) = 1;
			}
		}
	}
	if (!GIV(primFailCode)) {
		/* begin setFullScreenFlag: */
		GIV(fullScreenFlag) = argOop == GIV(trueObj);
		assert(!((failed())));
		GIV(stackPointer) += GIV(argumentCount) * BytesPerWord;
	}
}