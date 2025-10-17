/* Extracted from interp.c:68556 (function primitiveFullGC). */

/*	Do a full garbage collection. In SqueakV3ObjectMemory, answer the number
	of bytes available (including swap space if dynamic memory management is
	supported). In Spur, answer the size of the largest free chunk. */
/*	Do a full garbage collection. In SqueakV3ObjectMemory, answer the number
	of bytes available (including swap space if dynamic memory management is
	supported). In Spur, answer the size of the largest free chunk. */

	/* StackInterpreterPrimitives>>#primitiveFullGC */

static void
primitiveFullGC(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt integer;
    char *sp;

	/* begin externalWriteBackHeadFramePointers */
	assert((GIV(framePointer) - GIV(stackPointer)) < (LargeContextSlots * BytesPerOop));
	assert(GIV(stackPage) == (GIV(mostRecentlyUsedPage)));
	assert(!((isFree(GIV(stackPage)))));

	/* begin setHeadFP:andSP:inPage: */
	assert(GIV(stackPointer) < GIV(framePointer));
	assert((GIV(stackPointer) < ((GIV(stackPage)->baseAddress)))
	 && (GIV(stackPointer) > (((GIV(stackPage)->realStackLimit)) - (LargeContextSlots * BytesPerOop))));
	assert((GIV(framePointer) < ((GIV(stackPage)->baseAddress)))
	 && (GIV(framePointer) > (((GIV(stackPage)->realStackLimit)) - ((LargeContextSlots * BytesPerOop) / 2))));
	(GIV(stackPage)->headFP = GIV(framePointer));
	(GIV(stackPage)->headSP = GIV(stackPointer));
	assert(pageListIsWellFormed());
	integer = fullGC();

	/* begin methodReturnInteger: */
	assert(!((failed())));
	longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),(((usqInt)integer << 3) | 1));
	GIV(stackPointer) = sp;
}