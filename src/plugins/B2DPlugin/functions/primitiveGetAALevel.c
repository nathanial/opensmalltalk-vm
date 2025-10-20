/* Extracted from B2DPlugin.c:7830 (function primitiveGetAALevel). */

EXPORT(sqInt)
primitiveGetAALevel(void) {
  sqInt failureCode;

  if (methodArgumentCount()) {
    return primitiveFailFor(PrimErrBadNumArgs);
  }
  if ((failureCode = quickLoadEngineFrom(stackValue(0)))) {
    return primitiveFailFor(failureCode);
  }
  pop(1);
  pushInteger(workBuffer[GWAALevel]);
  return 0;
}