/* Extracted from interp.c:68556 (function primitiveFullGC). */

/*	Do a full garbage collection. In SqueakV3ObjectMemory, answer the number
        of bytes available (including swap space if dynamic memory management is
        supported). In Spur, answer the size of the largest free chunk. */
/*	Do a full garbage collection. In SqueakV3ObjectMemory, answer the number
        of bytes available (including swap space if dynamic memory management is
        supported). In Spur, answer the size of the largest free chunk. */

/* StackInterpreterPrimitives>>#primitiveFullGC */

static void primitiveFullGC(void) {
  sqInt integer;
  char *sp;

  externalWriteBackHeadFramePointers();
  integer = fullGC();

  /* begin methodReturnInteger: */
  assert(!((failed())));
  longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),
            (((usqInt)integer << 3) | 1));
  stackPointer = sp;
}