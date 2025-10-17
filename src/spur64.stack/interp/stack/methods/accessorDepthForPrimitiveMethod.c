/* Extracted from interp.c:49853 (function accessorDepthForPrimitiveMethod). */

/* StackInterpreter>>#accessorDepthForPrimitiveMethod: */

signed char accessorDepthForPrimitiveMethod(sqInt aMethodObj) {
  usqInt firstBytecode;
  sqInt methodHeader;
  sqInt primIndex;

  /* begin primitiveIndexOf: */
  /* begin methodHeaderOf: */
  assert(isCompiledMethod(aMethodObj));
  methodHeader =
      fetchPointerofObject(HeaderIndex, aMethodObj);
  if (((methodHeader & AlternateHeaderHasPrimFlag) != 0)) {
    firstBytecode =
        (aMethodObj + ((LiteralStart + (((methodHeader >> 3)) &
                                        AlternateHeaderNumLiteralsMask)) *
                       BytesPerOop)) +
        BaseHeaderSize;
    primIndex = (byteAt((void *)(firstBytecode + 1))) +
                ((((usqInt)((byteAt((void *)(firstBytecode + 2)))) << 8)));
  } else {
    primIndex = 0;
  }
  return ((primIndex == PrimNumberExternalCall) &&
                  (isLinkedExternalPrimitive(aMethodObj))
              ? accessorDepthForExternalPrimitiveMethod(aMethodObj)
              : primitiveAccessorDepthTable[primIndex]);
}