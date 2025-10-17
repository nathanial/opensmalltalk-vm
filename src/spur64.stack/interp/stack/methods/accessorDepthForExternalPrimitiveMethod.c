/* Extracted from interp.c:49840 (function
 * accessorDepthForExternalPrimitiveMethod). */

/* StackInterpreter>>#accessorDepthForExternalPrimitiveMethod: */

static sqInt
accessorDepthForExternalPrimitiveMethod(sqInt methodObj) {
  sqInt flags;
  sqInt lit;

  assert(isLinkedExternalPrimitive(methodObj));
  lit =
      longAt((void *)((methodObj + BaseHeaderSize) + (1U << (shiftForWord()))));
  flags = longAt((void *)((lit + BaseHeaderSize) +
                          ((((usqInt)(ExternalCallLiteralFlagsIndex)
                             << (shiftForWord()))))));
  return (((flags >> 3))) >> SpurPrimitiveAccessorDepthShift;
}