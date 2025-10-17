/* Extracted from interp.c:68322 (function primitiveFindHandlerContext). */

/*	Primitive. Search up the context stack for the next method context
   marked for exception handling starting at the receiver. Return nil if none
   found */

/* StackInterpreterPrimitives>>#primitiveFindHandlerContext */

static void primitiveFindHandlerContext(void) {
  sqInt handlerOrNilOrZero;
  char *sp;

  /* begin externalWriteBackHeadFramePointers */
  assert((framePointer - stackPointer) < (LargeContextSlots * BytesPerOop));
  assert(stackPage == (mostRecentlyUsedPage));
  assert(!((isFree(stackPage))));

  /* begin setHeadFP:andSP:inPage: */
  assert(stackPointer < framePointer);
  assert((stackPointer < ((stackPage->baseAddress))) &&
         (stackPointer >
          (((stackPage->realStackLimit)) - (LargeContextSlots * BytesPerOop))));
  assert((framePointer < ((stackPage->baseAddress))) &&
         (framePointer > (((stackPage->realStackLimit)) -
                          ((LargeContextSlots * BytesPerOop) / 2))));
  (stackPage->headFP = framePointer);
  (stackPage->headSP = stackPointer);
  assert(pageListIsWellFormed());
  handlerOrNilOrZero = findMethodWithPrimitiveFromContextUpToContext(
      PrimNumberHandlerMarker, longAt(stackPointer), nilObj);
  if (!handlerOrNilOrZero) {
    handlerOrNilOrZero = nilObj;
  }

  /* begin pop:thenPush: */
  longAtput((sp = stackPointer), handlerOrNilOrZero);
  stackPointer = sp;
}