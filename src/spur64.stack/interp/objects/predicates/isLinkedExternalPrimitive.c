/* Extracted from interp.c:55052 (function isLinkedExternalPrimitive). */

/* StackInterpreter>>#isLinkedExternalPrimitive: */

static sqInt isLinkedExternalPrimitive(sqInt methodObj) {
  usqInt firstBytecode;
  sqInt header;
  sqInt literal;
  usqInt numSlots;
  sqInt primIdx;
  sqInt targetFunctionIndex;

  literal = 0;

  header = methodHeaderOf(methodObj);

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
          ((isArray((literal = fetchPointerofObject(1U, methodObj)))) &&
           ((((assert((classIndexOf(literal)) >
                      (isForwardedObjectClassIndexPun())),
               numSlotsOf(literal))) == 4) &&
            (((targetFunctionIndex = fetchPointerofObject(
                   ExternalCallLiteralTargetFunctionIndex, literal)),
              ((((targetFunctionIndex) & 7) == 1)) &&
                  (((targetFunctionIndex >> 3)) > 0))))));
}