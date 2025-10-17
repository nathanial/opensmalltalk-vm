/* Extracted from interp.c:37231 (function isEmptyObjStack). */

	/* SpurMemoryManager>>#isEmptyObjStack: */

static NoDbgRegParms sqInt
isEmptyObjStack(sqInt objStack)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	if (objStack == GIV(nilObj)) {
		return 1;
	}
	eassert(isValidObjStack(objStack));
	return (0 == (longAt((void *)((objStack + BaseHeaderSize) + ((((usqInt)(ObjStackTopx) << (shiftForWord()))))))))
	 && (0 == (longAt((void *)((objStack + BaseHeaderSize) + ((((usqInt)(ObjStackNextx) << (shiftForWord()))))))));
}