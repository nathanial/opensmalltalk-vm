/* Extracted from interp.c:15386 (function primitiveFailForwithSecondary). */

/*	Set primFailCode primitive failure and associated secondaryErrorCode. */

	/* InterpreterPrimitives>>#primitiveFailFor:withSecondary: */

sqInt
primitiveFailForwithSecondary(sqInt reasonCode, sqLong extraErrorCode)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	secondaryErrorCode = extraErrorCode;
	return (primFailCode = reasonCode);
}