/* Extracted from interp.c:15230 (function primitiveFail). */

/*	Set general (unspecified) primitive failure. Don't overwrite an error code
	that has already been set. */
/*	Use returnTypeC: #sqInt because that's the way it is defined in sq.h.
	Use no explicit return so that Slang doesn't fail an inlining type-check
	when a primitive with return type void uses ^self primitiveFail to exit. */

	/* InterpreterPrimitives>>#primitiveFail */

sqInt
primitiveFail(void)
{
	if (!primFailCode) {
		primFailCode = 1;
	}
	return 0;
}