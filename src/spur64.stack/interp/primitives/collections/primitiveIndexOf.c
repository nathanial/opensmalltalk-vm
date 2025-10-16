/* Extracted from interp.c:59042 (function primitiveIndexOf). */

/* StackInterpreter>>#primitiveIndexOf: */

sqInt primitiveIndexOf(sqInt methodPointer) {
  usqInt firstBytecode;
  sqInt methodHeader;

  methodHeader = methodHeaderOf(methodPointer);

  /* begin primitiveIndexOfMethod:header: */
  return (((methodHeader & AlternateHeaderHasPrimFlag) != 0)
              ? ((firstBytecode =
                      (methodPointer +
                       ((LiteralStart + (((methodHeader >> 3)) &
                                         AlternateHeaderNumLiteralsMask)) *
                        BytesPerOop)) +
                      BaseHeaderSize),
                 (byteAt((void *)(firstBytecode + 1))) +
                     ((((usqInt)((byteAt((void *)(firstBytecode + 2)))) << 8))))
              : 0);
}