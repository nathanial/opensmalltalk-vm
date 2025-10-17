/* Extracted from interp.c:46822 (function validObjStacks). */

	/* SpurMemoryManager>>#validObjStacks */

static sqInt
validObjStacks(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return ((markStack == nilObj)
	 || (isValidObjStack(markStack)))
	 && (((weaklingStack == nilObj)
	 || (isValidObjStack(weaklingStack)))
	 && ((mournQueue == nilObj)
	 || (isValidObjStack(mournQueue))));
}