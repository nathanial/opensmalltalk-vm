/* Extracted from interp.c:38059 (function lastPointerOf). */

/*	Answer the byte offset of the last pointer field of the given object.
        Works with CompiledMethods, as well as ordinary objects. */

/* SpurMemoryManager>>#lastPointerOf: */

static sqInt lastPointerOf(sqInt objOop) {
  sqInt contextSize;
  sqInt fmt;
  sqInt header;
  usqInt numSlots;
  sqInt sp;

  fmt = (byteAt((void *)(objOop + (formatFieldByteOffset())))) & (formatMask());
  assert(fmt != (forwardedFormat()));
  if (fmt <= 5 /* lastPointerFormat */) {
    if ((fmt == (indexablePointersFormat())) &&
        (((longAt((void *)(objOop))) & (classIndexMask())) ==
         ClassMethodContextCompactIndex)) {
      /* begin fetchStackPointerOf: */
      sp = fetchPointerofObject(StackPointerIndex, objOop);
      if (!((((sp) & 7) == 1))) {
        contextSize = 0;
        goto l1;
      }
      assert((ReceiverIndex + ((sp >> 3))) < (lengthOf(objOop)));
      contextSize = (sp >> 3);
      /* end fetchStackPointerOf: */
    l1:
      return (((CtxtTempFrameStart - 1) + contextSize) * BytesPerOop) +
             BaseHeaderSize;
    }

    /* contexts end at the stack pointer */
    return ((((
               assert((classIndexOf(objOop)) >
                      (isForwardedObjectClassIndexPun())),
               numSlotsOf(objOop))) -
             1) *
            BytesPerOop) +
           BaseHeaderSize;
  }
  if (fmt < (firstCompiledMethodFormat())) {
    return 0;
  }

  /* begin methodHeaderOf: */
  assert(isCompiledMethod(objOop));
  header = fetchPointerofObject(HeaderIndex, objOop);
  return (((((/* begin literalCountOfMethodHeader: */
              assert((((header) & 7) == 1)),
              /* literalCountOfAlternateHeader: */
              ((header >> 3)) & AlternateHeaderNumLiteralsMask)) +
            LiteralStart) -
           1) *
          BytesPerOop) +
         BaseHeaderSize;
}