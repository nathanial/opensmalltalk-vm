/* Extracted from interp.c:55052 (function isLinkedExternalPrimitive). */

/* StackInterpreter>>#isLinkedExternalPrimitive: */

static NoDbgRegParms sqInt isLinkedExternalPrimitive(sqInt methodObj) {
  usqInt firstBytecode;
  sqInt header;
  sqInt literal;
  usqInt numSlots;
  sqInt primIdx;
  sqInt targetFunctionIndex;

  literal = 0;

  /* begin methodHeaderOf: */
  assert(isCompiledMethod(methodObj));
  header = longAt((void *)((methodObj + BaseHeaderSize) +
                           ((((usqInt)(HeaderIndex) << (shiftForWord()))))));

  /* primitiveIndexOfMethod:header: */
  if (((header & AlternateHeaderHasPrimFlag) != 0)) {
    firstBytecode =
        (methodObj +
         ((LiteralStart + (((header >> 3)) & AlternateHeaderNumLiteralsMask)) *
          BytesPerOop)) +
        BaseHeaderSize;
    primIdx = (byteAt((void *)(firstBytecode + 1))) +
              ((((usqInt)((byteAt((void *)(firstBytecode + 2)))) << 8)));
  } else {
    primIdx = 0;
  }
  return (primIdx == PrimNumberExternalCall) &&
         ((((/* begin literalCountOfMethodHeader: */
             assert((((header) & 7) == 1)),
             /* literalCountOfAlternateHeader: */
             ((header >> 3)) & AlternateHeaderNumLiteralsMask)) > 0) &&
          ((isArray((literal = longAt((void *)((methodObj + BaseHeaderSize) +
                                               (1U << (shiftForWord()))))))) &&
           ((((/* begin numSlotsOf: */
               assert((classIndexOf(literal)) >
                      (isForwardedObjectClassIndexPun())),
               (((numSlots = byteAt(
                      (void *)(literal + (numSlotsFieldByteOffset()))))) ==
                        (numSlotsMask())
                    ? ((((usqInt)((
                          (sqInt)((usqInt)((longAt(
                                      (void *)(literal - BaseHeaderSize))))
                                  << 8)))))) >>
                          8
                    : numSlots))) == 4) &&
            (((targetFunctionIndex = longAt(
                   (void *)((literal + BaseHeaderSize) +
                            ((((usqInt)(ExternalCallLiteralTargetFunctionIndex)
                               << (shiftForWord()))))))),
              ((((targetFunctionIndex) & 7) == 1)) &&
                  (((targetFunctionIndex >> 3)) > 0))))));
}