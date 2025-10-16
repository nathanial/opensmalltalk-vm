/* Extracted from interp.c:62757 (function saneFunctionPointerForFailureOfPrimIndex). */

static NoDbgRegParms sqInt
saneFunctionPointerForFailureOfPrimIndex(sqInt primIndex)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    void (*basePrimitive)(void);

	/* begin functionPointerFor:inClass: */
	basePrimitive = ((((usqInt)primIndex)) > MaxPrimitiveIndex
				? ((void (*)(void)) 0)
				: primitiveTable[primIndex]);
	return (primitiveFunctionPointer == basePrimitive)
	 || (((/* isCalloutPrimitiveIndex: */
		(primIndex == PrimNumberExternalCall)
	 || (primIndex == PrimNumberFFICall))
	 && (!((((usqIntptr_t) primitiveFunctionPointer)) <= MaxQuickPrimitiveIndex)))
	 || ((/* isMetaPrimitiveIndex: */
		(primIndex == PrimNumberDoPrimitive)
	 || (primIndex == PrimNumberDoExternalCall))
	 && (GIV(metaAccessorDepth) > -2)));
}