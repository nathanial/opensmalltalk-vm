/* Extracted from interp.c:49840 (function
 * accessorDepthForExternalPrimitiveMethod). */

/* StackInterpreter>>#accessorDepthForExternalPrimitiveMethod: */

static sqInt accessorDepthForExternalPrimitiveMethod(sqInt methodObj) {
  sqInt flags;
  sqInt lit;

  assert(isLinkedExternalPrimitive(methodObj));
  lit = fetchPointerofObject(1U, methodObj);
  flags = fetchPointerofObject(ExternalCallLiteralFlagsIndex, lit);
  return (((flags >> 3))) >> SpurPrimitiveAccessorDepthShift;
}