/* Extracted from interp.c:50301 (function booleanValueOf). */

/*	convert true and false (Smalltalk) to true or false(C) */

	/* StackInterpreter>>#booleanValueOf: */

sqInt
booleanValueOf(sqInt obj)
{
	if (obj == trueObj) {
		return 1;
	}
	if (obj == falseObj) {
		return 0;
	}

	/* begin success: */
	if (!primFailCode) {
		primFailCode = 1;
	}
	return null;
}