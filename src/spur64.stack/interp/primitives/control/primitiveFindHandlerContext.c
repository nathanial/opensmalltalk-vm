/* Extracted from interp.c:68322 (function primitiveFindHandlerContext). */

/*	Primitive. Search up the context stack for the next method context
   marked for exception handling starting at the receiver. Return nil if none
   found */

/* StackInterpreterPrimitives>>#primitiveFindHandlerContext */

static void primitiveFindHandlerContext(void) {
  sqInt handlerOrNilOrZero;

  externalWriteBackHeadFramePointers();
  handlerOrNilOrZero = findMethodWithPrimitiveFromContextUpToContext(
      PrimNumberHandlerMarker, longAt(stackPointer), nilObj);
  if (!handlerOrNilOrZero) {
    handlerOrNilOrZero = nilObj;
  }

  popthenPush(1, handlerOrNilOrZero);
}
