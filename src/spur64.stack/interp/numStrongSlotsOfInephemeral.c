/* Extracted from interp.c:41254 (function numStrongSlotsOfInephemeral). */

/*	Answer the number of strong pointer fields in the given object,
        which is expected not to be an active ephemeron.
        Works with CompiledMethods as well as ordinary objects. */

/* SpurMemoryManager>>#numStrongSlotsOfInephemeral: */

static sqInt numStrongSlotsOfInephemeral(sqInt objOop) {
  sqInt contextSize;
  sqInt fmt;
  sqInt header;
  usqInt numLiterals;
  usqInt numSlots;
  usqInt numSlotsUsqInt;
  sqInt objOopSqInt;
  sqInt sp;

  fmt = (byteAt((void *)(objOop + (formatFieldByteOffset())))) & (formatMask());
  assert((fmt != (ephemeronFormat())) ||
         ((isImmediate(keyOfEphemeron(objOop))) ||
          (isMarked(keyOfEphemeron(objOop)))));
  if (fmt <= 5 /* lastPointerFormat */) {
    /* begin numSlotsOf: */
    assert((classIndexOf(objOop)) > (isForwardedObjectClassIndexPun()));
    numSlots =
        (((numSlotsUsqInt =
               byteAt((void *)(objOop + (numSlotsFieldByteOffset()))))) ==
                 (numSlotsMask())
             ? ((((usqInt)((
                   (sqInt)((usqInt)((longAt((void *)(objOop - BaseHeaderSize))))
                           << 8)))))) >>
                   8
             : numSlotsUsqInt);
    if (fmt <= (arrayFormat())) {
      return numSlots;
    }
    if (fmt == (indexablePointersFormat())) {
      if (((longAt((void *)(objOop))) & (classIndexMask())) ==
          ClassMethodContextCompactIndex) {
        setTraceFlagOnContextsFramesPageIfNeeded(objOop);

        /* contexts end at the stack pointer */

        /* begin fetchStackPointerOf: */
        sp = longAt(
            (void *)((objOop + BaseHeaderSize) +
                     ((((usqInt)(StackPointerIndex) << (shiftForWord()))))));
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
      return numSlots;
    }
    if (fmt == (weakArrayFormat())) {
      objOopSqInt = fetchClassOfNonImm(objOop);

      /* begin fixedFieldsOfClass: */
      return (((longAt((void *)((objOopSqInt + BaseHeaderSize) +
                                ((((usqInt)(InstanceSpecificationIndex)
                                   << (shiftForWord()))))))) >>
               3)) &
             ((1U << (fixedFieldsFieldWidth())) - 1);
    }
    return numSlots;
  }
  if (fmt == (forwardedFormat())) {
    return 1;
  }
  if (fmt < (firstCompiledMethodFormat())) {
    return 0;
  }

  /* begin methodHeaderOf: */
  assert(isCompiledMethod(objOop));
  header = longAt((void *)((objOop + BaseHeaderSize) +
                           ((((usqInt)(HeaderIndex) << (shiftForWord()))))));

  /* begin literalCountOfMethodHeader: */
  assert((((header) & 7) == 1));
  numLiterals = ((header >> 3)) & AlternateHeaderNumLiteralsMask;
  return numLiterals + LiteralStart;
}