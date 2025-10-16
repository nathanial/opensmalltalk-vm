/* Extracted from interp.c:12019 (function primitiveAsCharacter). */

/* InterpreterPrimitives>>#primitiveAsCharacter */

static void primitiveAsCharacter(void) {
  sqInt characterCode;
  sqInt characterObject;

  characterCode = longAt(stackPointer);
  if (!(((((characterCode) & 7) == 1)) &&
        (((characterCode = (characterCode >> 3)),
          /* isInRangeCharacterCode: */
          ((characterCode >= 0) && (characterCode <= (0x3FFFFFFF))))))) {
    /* primitiveFailFor: */
    primFailCode = (argumentCount ? PrimErrBadArgument : PrimErrBadReceiver);
    return;
  }
  characterObject =
      ((((usqInt)(characterCode) << (numTagBits())))) + (characterTag());

  popthenPush(argumentCount + 1, characterObject);
}