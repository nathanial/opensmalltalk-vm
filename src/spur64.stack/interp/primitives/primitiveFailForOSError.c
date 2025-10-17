/* Extracted from interp.c:15364 (function primitiveFailForOSError). */

/*	Set PrimErrOSError primitive failure and associated osErrorCode. */

/* InterpreterPrimitives>>#primitiveFailForOSError: */

sqInt primitiveFailForOSError(sqLong osErrorCode) {
  secondaryErrorCode = osErrorCode;
  return (primFailCode = PrimErrOSError);
}