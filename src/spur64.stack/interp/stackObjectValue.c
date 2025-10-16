/* Extracted from interp.c:64306 (function stackObjectValue). */

sqInt
stackObjectValue(sqInt offset)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt oop;

	oop = longAt(GIV(stackPointer) + (offset * BytesPerWord));
	if (((oop & (tagMask())) != 0)) {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
		return null;
	}
	return oop;
}