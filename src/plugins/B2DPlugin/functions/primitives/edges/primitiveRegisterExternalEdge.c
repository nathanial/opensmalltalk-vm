/* Extracted from B2DPlugin.c:8561 (function primitiveRegisterExternalEdge). */

EXPORT(sqInt)
primitiveRegisterExternalEdge(void) {
  sqInt edge;
  sqInt failureCode;
  sqInt index;
  sqInt initialX;
  sqInt initialY;
  sqInt initialZ;
  usqInt leftFillIndex;
  usqInt rightFillIndex;
  sqInt value;

  if (!((methodArgumentCount()) == 6)) {
    return primitiveFailFor(PrimErrBadNumArgs);
  }
  if ((failureCode =
           quickLoadEngineFromrequiredState(stackValue(6), GEStateUnlocked))) {
    return primitiveFailFor(failureCode);
  }
  rightFillIndex = positive32BitValueOf(stackValue(0));
  leftFillIndex = positive32BitValueOf(stackValue(1));
  initialZ = stackIntegerValue(2);
  initialY = stackIntegerValue(3);
  initialX = stackIntegerValue(4);
  index = stackIntegerValue(5);
  if (failed()) {
    return primitiveFailFor(PrimErrBadArgument);
  }
  if (!(allocateObjEntry(GEBaseEdgeSize))) {
    return primitiveFailFor(GEFWorkTooBig);
  }

  /* Make sure the fills are okay */
  if (!((isFillOkay(leftFillIndex)) && (isFillOkay(rightFillIndex)))) {
    return primitiveFailFor(GEFWrongFill);
  }
  edge = objUsed;
  objUsed = edge + GEBaseEdgeSize;

  /* Install type and length */

  /* #objectTypeOf:put: #obj:at:put: */
  objBuffer[edge + GEObjectType] = GEPrimitiveEdge;

  /* #objectLengthOf:put: #obj:at:put: */
  objBuffer[edge + GEObjectLength] = GEBaseEdgeSize;

  /* #objectIndexOf:put: #obj:at:put: */
  objBuffer[edge + GEObjectIndex] = index;

  /* Install remaining stuff */

  /* #edgeXValueOf:put: #obj:at:put: */
  objBuffer[edge + GEXValue] = initialX;

  /* #edgeYValueOf:put: #obj:at:put: */
  objBuffer[edge + GEYValue] = initialY;

  /* #edgeZValueOf:put: #obj:at:put: */
  objBuffer[edge + GEZValue] = initialZ;
  value = transformColor(leftFillIndex);

  /* begin edgeLeftFillOf:put: */
  /* obj:at:put: */
  objBuffer[edge + GEFillIndexLeft] = value;
  value = transformColor(rightFillIndex);

  /* begin edgeRightFillOf:put: */
  /* obj:at:put: */
  objBuffer[edge + GEFillIndexRight] = value;
  if (engineStopped) {
    return primitiveFailFor(GEFEngineStopped);
  }
  if (!(failed())) {
    /* begin storeEngineStateInto: */
    /* objUsedPut: */
    workBuffer[GWObjUsed] = objUsed;
    pop(6);
  }
  return 0;
}