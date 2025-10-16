/* Extracted from interp.c:55131 (function isMetaPrimitiveIndex). */

static NoDbgRegParms sqInt
isMetaPrimitiveIndex(sqInt primIndex)
{
	return (primIndex == PrimNumberDoPrimitive)
	 || (primIndex == PrimNumberDoExternalCall);
}