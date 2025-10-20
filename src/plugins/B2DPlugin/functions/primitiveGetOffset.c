/* Extracted from B2DPlugin.c:8042 (function primitiveGetOffset). */

EXPORT(sqInt)
primitiveGetOffset(void) {
  sqInt failureCode;
  sqInt pointOop;

  if (methodArgumentCount()) {
    return primitiveFailFor(PrimErrBadNumArgs);
  }
  if ((failureCode = quickLoadEngineFrom(stackValue(0)))) {
    return primitiveFailFor(failureCode);
  }
  pointOop = makePointwithxValueyValue(workBuffer[GWDestOffsetX],
                                       workBuffer[GWDestOffsetY]);
  popthenPush(1, pointOop);
  return 0;
}