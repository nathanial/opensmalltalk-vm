/* Extracted from B2DPlugin.c:7994 (function primitiveGetDepth). */

EXPORT(sqInt)
primitiveGetDepth(void) {
  sqInt failureCode;

  if (methodArgumentCount()) {
    return primitiveFailFor(PrimErrBadNumArgs);
  }
  if ((failureCode = quickLoadEngineFrom(stackValue(0)))) {
    return primitiveFailFor(failureCode);
  }
  pop(1);
  pushInteger(workBuffer[GWCurrentZ]);
  return 0;
}