/* Extracted from interp.c:22733 (function primitiveSizeInBytes). */

/* InterpreterPrimitives>>#primitiveSizeInBytes */

static void primitiveSizeInBytes(void) {
  usqLong byteSize;
  sqInt oop;

  byteSize = /* totalByteSizeOf: */
      ((((longAt(stackPointer)) & (tagMask())) != 0)
           ? 0
           : bytesInBody(longAt(stackPointer)));
  oop = positive64BitIntegerFor(byteSize);

  /* begin pop:thenPush: */
  popthenPush(argumentCount + 1, oop);
}