/* Extracted from interp.c:15364 (function primitiveFailForOSError). */

/*	Set PrimErrOSError primitive failure and associated osErrorCode. */

	/* InterpreterPrimitives>>#primitiveFailForOSError: */

sqInt
primitiveFailForOSError(sqLong osErrorCode)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	GIV(secondaryErrorCode) = osErrorCode;
	return (GIV(primFailCode) = PrimErrOSError);
}