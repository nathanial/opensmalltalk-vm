/* Extracted from interp.c:58337 (function positive32BitIntegerFor). */

/*	integerValue is interpreted as POSITIVE, e.g. as the result of Bitmap>at:.
	N.B. Returning in each arm separately enables Slang inlining.
	/Don't/ return the ifTrue:ifFalse: unless Slang inlining of conditionals
	is fixed. */

	/* StackInterpreter>>#positive32BitIntegerFor: */

sqInt
positive32BitIntegerFor(unsigned int integerValue)
{
	return ((((((usqInt)integerValue)) & 0xFFFFFFFFU) << 3) | 1);
}