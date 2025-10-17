/* Extracted from interp.c:41131 (function numPointerSlotsOf). */

/*	Answer the number of pointer fields in the given object.
        Works with CompiledMethods, as well as ordinary objects. */

/* SpurMemoryManager>>#numPointerSlotsOf: */

static usqInt numPointerSlotsOf(sqInt objOop) {
  sqInt contextSize;
  sqInt fmt;
  sqInt header;
  usqInt numLiterals;
  usqInt numSlots;
  sqInt sp;

  fmt = (byteAt((void *)(objOop + (formatFieldByteOffset())))) & (formatMask());

  /* begin numPointerSlotsOf:format: */
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
      return CtxtTempFrameStart + contextSize;
    }

    /* contexts end at the stack pointer */

    /* begin numSlotsOf: */
    assert((classIndexOf(objOop)) > (isForwardedObjectClassIndexPun()));
    return (
        ((numSlots = byteAt((void *)(objOop + (numSlotsFieldByteOffset()))))) ==
                (numSlotsMask())
            ? ((((usqInt)((
                  (sqInt)((usqInt)((longAt((void *)(objOop - BaseHeaderSize))))
                          << 8)))))) >>
                  8
            : numSlots);
  }
  if (fmt == (forwardedFormat())) {
    return 1;
  }
  if (fmt < (firstCompiledMethodFormat())) {
    return 0;
  }

  /* begin methodHeaderOf: */
  assert(isCompiledMethod(objOop));
  header = fetchPointerofObject(HeaderIndex, objOop);

  /* begin literalCountOfMethodHeader: */
  assert((((header) & 7) == 1));
  numLiterals = ((header >> 3)) & AlternateHeaderNumLiteralsMask;
  return numLiterals + LiteralStart;
}