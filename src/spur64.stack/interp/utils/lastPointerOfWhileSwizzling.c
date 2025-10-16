/* Extracted from interp.c:38025 (function lastPointerOfWhileSwizzling). */

/*	Answer the byte offset of the last pointer field of the given object.
        Works with CompiledMethods, as well as ordinary objects.
        Does not examine the stack pointer of contexts to be sure to swizzle
        the nils that fill contexts on snapshot.
        It is invariant that on image load no object contains a forwarding
        pointer, and the image contains no forwarders (see class comment). */

/* SpurMemoryManager>>#lastPointerOfWhileSwizzling: */

static sqInt lastPointerOfWhileSwizzling(sqInt objOop) {
  sqInt fmt;
  sqInt header;

  fmt = (byteAt((void *)(objOop + (formatFieldByteOffset())))) & (formatMask());
  assert(fmt != (forwardedFormat()));
  if (fmt <= 5 /* lastPointerFormat */) {
    return ((((assert((classIndexOf(objOop)) >
                      (isForwardedObjectClassIndexPun())),
               numSlotsOf(objOop))) -
             1) *
            BytesPerOop) +
           BaseHeaderSize;
  }
  if (fmt < (firstCompiledMethodFormat())) {
    return 0;
  }

  header = methodHeaderOf(objOop);
  return (((((/* begin literalCountOfMethodHeader: */
              assert((((header) & 7) == 1)),
              /* literalCountOfAlternateHeader: */
              ((header >> 3)) & AlternateHeaderNumLiteralsMask)) +
            LiteralStart) -
           1) *
          BytesPerOop) +
         BaseHeaderSize;
}
