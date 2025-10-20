/* Extracted from B2DPlugin.c:9373 (function quickLoadEngineFromrequiredStateor). */

static sqInt quickLoadEngineFromrequiredStateor(sqInt oop, sqInt requiredState,
                                                sqInt alternativeState) {
  sqInt failureCode;

  if ((failureCode = quickLoadEngineFrom(oop))) {
    return failureCode;
  }
  if ((workBuffer[GWState]) == requiredState) {
    return 0;
  }
  if ((workBuffer[GWState]) == alternativeState) {
    return 0;
  }

  /* stopReasonPut: */
  workBuffer[GWStopReason] = GErrorBadState;
  return GEFWrongState;
}