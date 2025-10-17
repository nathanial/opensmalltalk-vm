/* Extracted from interp.c:44080 (function printObjStackAndContents). */

	/* SpurMemoryManager>>#printObjStackAndContents: */

void
printObjStackAndContents(sqInt objStack)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	/* begin printObjStack:printContents: */
	if (objStack == GIV(nilObj)) {
		print("nil");
		cr();
	}
	else {
		printObjStackPagemyIndexpageTypeprintContents(objStack, longAt((void *)((objStack + BaseHeaderSize) + ((((usqInt)(ObjStackMyx) << (shiftForWord())))))), ObjStackMyx, 1);
	}
}