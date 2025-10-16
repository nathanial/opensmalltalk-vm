/* Extracted from interp.c:68322 (function primitiveFindHandlerContext). */

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