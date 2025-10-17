/* Extracted from interp.c:68322 (function primitiveFindHandlerContext). */

/*	Primitive. Search up the context stack for the next method context marked
	for exception handling starting at the receiver. Return nil if none found */

	/* StackInterpreterPrimitives>>#primitiveFindHandlerContext */

static void
primitiveFindHandlerContext(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt handlerOrNilOrZero;
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
	handlerOrNilOrZero = findMethodWithPrimitiveFromContextUpToContext(PrimNumberHandlerMarker, longAt(GIV(stackPointer)), GIV(nilObj));
	if (!handlerOrNilOrZero) {
		handlerOrNilOrZero = GIV(nilObj);
	}

	/* begin pop:thenPush: */
	longAtput((sp = GIV(stackPointer)),handlerOrNilOrZero);
	GIV(stackPointer) = sp;
}