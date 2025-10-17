/* Extracted from interp.c:46822 (function validObjStacks). */

	/* SpurMemoryManager>>#validObjStacks */

static sqInt
validObjStacks(void)
{
	return ((markStack == nilObj)
	 || (isValidObjStack(markStack)))
	 && (((weaklingStack == nilObj)
	 || (isValidObjStack(weaklingStack)))
	 && ((mournQueue == nilObj)
	 || (isValidObjStack(mournQueue))));
}