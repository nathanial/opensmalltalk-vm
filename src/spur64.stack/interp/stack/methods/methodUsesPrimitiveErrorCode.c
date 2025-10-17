/* Extracted from interp.c:57915 (function methodUsesPrimitiveErrorCode). */

/*	Answer if aMethodObj contains a primitive and uses the primitive error
        code.
 */

/* StackInterpreter>>#methodUsesPrimitiveErrorCode: */

static sqInt methodUsesPrimitiveErrorCode(sqInt aMethodObj) {
  usqInt firstBytecode;
  sqInt methodHeader;

  /* begin methodHeaderOf: */
  assert(isCompiledMethod(aMethodObj));
  methodHeader = fetchPointerofObject(HeaderIndex, aMethodObj);
  return ((/* primitiveIndexOfMethod:header: */
           (((methodHeader & AlternateHeaderHasPrimFlag) != 0)
                ? ((firstBytecode =
                        (aMethodObj +
                         ((LiteralStart + (((methodHeader >> 3)) &
                                           AlternateHeaderNumLiteralsMask)) *
                          BytesPerOop)) +
                        BaseHeaderSize),
                   (byteAt((void *)(firstBytecode + 1))) +
                       ((((usqInt)((byteAt((void *)(firstBytecode + 2))))
                          << 8))))
                : 0)) > 0) &&
         ((/* longStoreBytecodeForHeader: */
           ((((sqLong)methodHeader)) < 0 ? AltLongStoreBytecode
                                         : LongStoreBytecode)) ==
          (fetchByteofObject(
              ((((/* begin literalCountOfMethodHeader: */
                  assert((((methodHeader) & 7) == 1)),
                  /* literalCountOfAlternateHeader: */
                  ((methodHeader >> 3)) & AlternateHeaderNumLiteralsMask)) +
                LiteralStart) *
               BytesPerOop) +
                  3 /* sizeOfCallPrimitiveBytecode: */,
              aMethodObj)));
}