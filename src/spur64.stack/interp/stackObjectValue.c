/* Extracted from interp.c:64306 (function stackObjectValue). */

/*	Ensures that the given object is a real object, not a SmallInteger. */
/*	In the StackInterpreter stacks grow down. */

	/* StackInterpreter>>#stackObjectValue: */

sqInt
stackObjectValue(sqInt offset)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt oop;

	oop = longAt(stackPointer + (offset * BytesPerWord));
	if (((oop & (tagMask())) != 0)) {
		/* begin primitiveFail */
		if (!primFailCode) {
			primFailCode = 1;
		}
		return null;
	}
	return oop;
}