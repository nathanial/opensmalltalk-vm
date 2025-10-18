/* Extracted from interp.c (storeAndPopTemporaryVariableBytecode cases). */

/* StackInterpreter>>#storeAndPopTemporaryVariableBytecode:index: */

static sqInt storeAndPopTemporaryVariableBytecodeCommon(
    char **localIPP, sqInt bytecodeSetSelector, char *localFP, sqInt tempIndex,
    char **localSPP) {
  sqInt currentBytecode;
  sqInt frameNumArgs;
  sqInt value;
  sqInt destinationOffset;

  currentBytecode = fetchNextBytecode(localIPP, bytecodeSetSelector);
  frameNumArgs = byteAt((localFP + FoxFrameFlags) + 1);
  value = longAt(*localSPP);
  if (tempIndex < frameNumArgs) {
    destinationOffset = frameNumArgs - tempIndex;
    longAtput((localFP + FoxCallerSavedIP) + (destinationOffset * BytesPerWord),
              value);
  } else {
    destinationOffset = frameNumArgs - tempIndex;
    longAtput(((localFP + FoxReceiver) - BytesPerWord) +
                  (destinationOffset * BytesPerWord),
              value);
  }

  *localSPP += BytesPerOop;
  return currentBytecode;
}
