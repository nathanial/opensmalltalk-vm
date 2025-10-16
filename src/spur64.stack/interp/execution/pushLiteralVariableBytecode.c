/* Extracted from interp.c (pushLiteralVariableBytecode cases). */

/* StackInterpreter>>#pushLiteralVariableBytecode:literalIndex: */

static inline sqInt pushLiteralVariableBytecodeCommon(char **localIPP,
                                                      sqInt bytecodeSetSelector,
                                                      sqInt methodOop,
                                                      sqInt literalIndex,
                                                      char **localSPP) {
  sqInt currentBytecode;
  sqInt litVar;
  sqInt literalOffset;

  currentBytecode = fetchNextBytecode(localIPP, bytecodeSetSelector);
  literalOffset = literalIndex + LiteralStart;
  litVar = fetchPointerofObject(literalOffset, methodOop);
  assert(isNonImmediate(litVar));
  if ((!((longAt((void *)(litVar))) &
         ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
    litVar = fixFollowedFieldofObjectwithInitialValue(literalOffset, methodOop,
                                                      litVar);
  }

  internalPush(localSPP, fetchPointerofObject(ValueIndex, litVar));
  return currentBytecode;
}
