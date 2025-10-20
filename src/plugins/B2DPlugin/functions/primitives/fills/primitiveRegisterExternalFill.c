/* Extracted from B2DPlugin.c:8644 (function primitiveRegisterExternalFill). */

EXPORT(sqInt)
primitiveRegisterExternalFill(void) {
  sqInt failureCode;
  sqInt fill;
  sqInt index;

  if (!((methodArgumentCount()) == 1)) {
    return primitiveFailFor(PrimErrBadNumArgs);
  }
  if ((failureCode =
           quickLoadEngineFromrequiredState(stackValue(1), GEStateUnlocked))) {
    return primitiveFailFor(failureCode);
  }
  index = stackIntegerValue(0);
  if (failed()) {
    return primitiveFailFor(PrimErrBadArgument);
  }

  /* Note: We *must* not allocate any fill with index 0 */
  fill = 0;
  while (fill == 0) {
    if (!(allocateObjEntry(GEBaseEdgeSize))) {
      return primitiveFailFor(GEFWorkTooBig);
    }
    fill = objUsed;
    objUsed = fill + GEBaseFillSize;

    /* Install type and length */

    /* #objectTypeOf:put: #obj:at:put: */
    objBuffer[fill + GEObjectType] = GEPrimitiveFill;

    /* #objectLengthOf:put: #obj:at:put: */
    objBuffer[fill + GEObjectLength] = GEBaseFillSize;

    /* #objectIndexOf:put: #obj:at:put: */
    objBuffer[fill + GEObjectIndex] = index;
  }
  if (!(failed())) {
    /* begin storeEngineStateInto: */
    /* objUsedPut: */
    workBuffer[GWObjUsed] = objUsed;
    pop(2);
    pushInteger(fill);
  }
  return 0;
}