/* Extracted from interp.c:46822 (function validObjStacks). */

static sqInt
validObjStacks(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return ((GIV(markStack) == GIV(nilObj))
	 || (isValidObjStack(GIV(markStack))))
	 && (((GIV(weaklingStack) == GIV(nilObj))
	 || (isValidObjStack(GIV(weaklingStack))))
	 && ((GIV(mournQueue) == GIV(nilObj))
	 || (isValidObjStack(GIV(mournQueue)))));
}