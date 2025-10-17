/* Extracted from interp.c:15376 (function primitiveFailFor). */

/*	Set specific primitive failure.
	N.B. primitiveFailFor: PrimNoErr is expected to clear the primFailCode. */

	/* InterpreterPrimitives>>#primitiveFailFor: */

sqInt
primitiveFailFor(sqInt reasonCode)
{
	return (primFailCode = reasonCode);
}