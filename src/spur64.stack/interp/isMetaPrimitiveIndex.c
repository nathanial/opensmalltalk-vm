/* Extracted from interp.c:55131 (function isMetaPrimitiveIndex). */

/*	This virtual machine provides two primitives that executes arbitrary
	primitives, one
	for indexed primitivces and one for named primitives. These meta
	primitives are used
	in the debugger to execute primitives while simulating execution. Spur
	needs to know
	the accessor depth for a primitive so that failures due to forwarders can
	be fixed up
	and retried. This method identifies such meta primitives so that
	metaAccessorDepth can be substituted when appropriate. */

	/* StackInterpreter>>#isMetaPrimitiveIndex: */

static NoDbgRegParms sqInt
isMetaPrimitiveIndex(sqInt primIndex)
{
	return (primIndex == PrimNumberDoPrimitive)
	 || (primIndex == PrimNumberDoExternalCall);
}