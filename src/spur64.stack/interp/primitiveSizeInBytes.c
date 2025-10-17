/* Extracted from interp.c:22733 (function primitiveSizeInBytes). */

/* InterpreterPrimitives>>#primitiveSizeInBytes */

static void primitiveSizeInBytes(void) {
  usqLong byteSize;
  sqInt oop;
  char *sp;

  byteSize = /* totalByteSizeOf: */
      ((((longAt(stackPointer)) & (tagMask())) != 0)
           ? 0
           : bytesInBody(longAt(stackPointer)));
  oop = positive64BitIntegerFor(byteSize);

  /* begin pop:thenPush: */
  longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),
            oop);
  stackPointer = sp;
}