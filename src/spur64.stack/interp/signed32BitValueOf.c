/* Extracted from interp.c:25948 (function signed32BitValueOf). */

/*	Convert the given object into an integer value.
	The object may be either a SmallInteger or a four-byte LargeInteger. */

	/* InterpreterPrimitives>>#signed32BitValueOf: */

int
signed32BitValueOf(sqInt oop)
{
    sqInt value64;

	if ((((oop) & 7) == 1)) {
		value64 = (oop >> 3);
		if ((((int) value64)) != value64) {
			/* begin primitiveFail */
			if (!primFailCode) {
				primFailCode = 1;
			}
			value64 = 0;
		}
		return value64;
	}

	/* begin primitiveFail */
	if (!primFailCode) {
		primFailCode = 1;
	}
	return 0;
}