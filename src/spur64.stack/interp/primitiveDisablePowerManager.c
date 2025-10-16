/* Extracted from interp.c:14742 (function primitiveDisablePowerManager). */

EXPORT(sqInt)
primitiveDisablePowerManager(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt integer;
    sqInt integerPointer;

	/* begin stackIntegerValue: */
	integerPointer = longAt(GIV(stackPointer));
	if ((((integerPointer) & 7) == 1)) {
		integer = (integerPointer >> 3);
	}
	else {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
		integer = 0;
	}
	if (!GIV(primFailCode)) {
		ioDisablePowerManager(integer);

		/* begin pop: */
		GIV(stackPointer) += 1 * BytesPerWord;
	}
	return 0;
}