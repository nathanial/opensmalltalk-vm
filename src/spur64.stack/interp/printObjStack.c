/* Extracted from interp.c:44183 (function printObjStack). */

/*	useful for debugging */

	/* SpurMemoryManager>>#printObjStack: */

void
printObjStack(sqInt objStack)
{
	/* begin printObjStack:printContents: */
	if (objStack == nilObj) {
		print("nil");
		cr();
	}
	else {
		printObjStackPagemyIndexpageTypeprintContents(objStack, longAt((void *)((objStack + BaseHeaderSize) + ((((usqInt)(ObjStackMyx) << (shiftForWord())))))), ObjStackMyx, 0);
	}
}