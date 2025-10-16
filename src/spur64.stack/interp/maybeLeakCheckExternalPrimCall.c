/* Extracted from interp.c:57633 (function maybeLeakCheckExternalPrimCall). */

static NoDbgRegParms sqInt
maybeLeakCheckExternalPrimCall(sqInt aMethodObj)
{
	if ((((checkForLeaks & GCCheckPrimCall) != 0))
	 && (((primitiveIndexOf(aMethodObj)) == PrimNumberExternalCall)
	 && (checkedPluginName == (fetchPointerofObject(0, longAt((void *)((aMethodObj + BaseHeaderSize) + (1U << (shiftForWord()))))))))) {
		runLeakCheckerFor(GCCheckPrimCall);
	}
	return 1;
}