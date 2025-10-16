/* Extracted from interp.c:37993 (function keyOfEphemeron). */

static NoDbgRegParms sqInt
keyOfEphemeron(sqInt objOop)
{
	assert((isNonImmediate(objOop))
	 && (isObjEphemeron(objOop)));
	return longAt((void *)((objOop + BaseHeaderSize) + (0U << (shiftForWord()))));
}