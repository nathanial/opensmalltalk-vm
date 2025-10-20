/* Extracted from B2DPlugin.c:9357 (function quickLoadEngineFromrequiredState). */

static sqInt quickLoadEngineFromrequiredState(sqInt oop, sqInt requiredState) {
  sqInt failureCode;

  if ((failureCode = quickLoadEngineFrom(oop))) {
    return failureCode;
  }
  if ((workBuffer[GWState]) == requiredState) {
    return 0;
  }

  /* stopReasonPut: */
  workBuffer[GWStopReason] = GErrorBadState;
  return GEFWrongState;
}