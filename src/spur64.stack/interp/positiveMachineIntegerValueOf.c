/* Extracted from interp.c:11588 (function positiveMachineIntegerValueOf). */

usqIntptr_t
positiveMachineIntegerValueOf(sqInt oop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt value;

	if ((((oop) & 7) == 1)) {
		value = (oop >> 3);
		if (value < 0) {
			/* begin primitiveFail */
			if (!GIV(primFailCode)) {
				GIV(primFailCode) = 1;
			}
			return null;
		}
		return value;
	}

	/* don't inline the rare case */
	return positiveMachineIntegerValueOfObj(oop);
}