/* Extracted from interp.c:59030 (function primitiveIndexOfMethodheader). */

/*	Note: With the Squeak V3 format we now have 10 bits of primitive index,
        but they are
        in two places for temporary backward compatibility. The time to unpack
   is negligible, since the derived primitive function pointer is stored in the
        method cache. With the
        Spur format we assume a 3-byte CallPrimitive with a little-endian 16-bit
        primitive index. */

/* StackInterpreter>>#primitiveIndexOfMethod:header: */

sqInt primitiveIndexOfMethodheader(sqInt theMethod, sqInt methodHeader) {
  usqInt firstBytecode;

  return (((methodHeader & AlternateHeaderHasPrimFlag) != 0)
              ? ((firstBytecode =
                      (theMethod +
                       ((LiteralStart + (((methodHeader >> 3)) &
                                         AlternateHeaderNumLiteralsMask)) *
                        BytesPerOop)) +
                      BaseHeaderSize),
                 (byteAt((void *)(firstBytecode + 1))) +
                     ((((usqInt)((byteAt((void *)(firstBytecode + 2)))) << 8))))
              : 0);
}