/* Extracted from interp.c:21789 (function primitiveScreenDepth). */

EXPORT(sqInt)
primitiveScreenDepth(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt depth;
    char *sp;

	depth = ioScreenDepth();
	if (GIV(primFailCode)) {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
		return null;
	}

	/* begin pop:thenPushInteger: */
	longAtput((sp = GIV(stackPointer)),(((usqInt)depth << 3) | 1));
	GIV(stackPointer) = sp;
	return 0;
}