/* Extracted from interp.c:35535 (function followForwardedObjectFieldstoDepth).
 */

/*	Follow pointers in the object to depth.
        Answer if any forwarders were found.
        How to avoid cyclic structures?? A temporary mark bit? eem 6/22/2020 no
        need since depth is always finite. */

/* SpurMemoryManager>>#followForwardedObjectFields:toDepth: */

static NoDbgRegParms sqInt followForwardedObjectFieldstoDepth(sqInt objOop,
                                                              sqInt depth) {
  sqInt contextSize;
  sqInt fmt;
  sqInt found;
  sqInt header;
  sqInt i;
  usqInt numLiterals;
  usqInt numSlots;
  usqInt numSlotsUsqInt;
  sqInt oop;
  sqInt referent;
  sqInt sp;

  found = 0;
  assert((isPointers(objOop)) || (isOopCompiledMethod(objOop)));

  /* begin numPointerSlotsOf: */
  fmt = (byteAt((void *)(objOop + (formatFieldByteOffset())))) & (formatMask());
  if (fmt <= 5 /* lastPointerFormat */) {
    if ((fmt == (indexablePointersFormat())) &&
        (((longAt((void *)(objOop))) & (classIndexMask())) ==
         ClassMethodContextCompactIndex)) {
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
      numSlots = CtxtTempFrameStart + contextSize;
      goto l2;
    }

    /* contexts end at the stack pointer */

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
    goto l2;
  }
  if (fmt == (forwardedFormat())) {
    numSlots = 1;
    goto l2;
  }
  if (fmt < (firstCompiledMethodFormat())) {
    numSlots = 0;
    goto l2;
  }

  /* begin methodHeaderOf: */
  assert(isCompiledMethod(objOop));
  header = longAt((void *)((objOop + BaseHeaderSize) +
                           ((((usqInt)(HeaderIndex) << (shiftForWord()))))));

  /* begin literalCountOfMethodHeader: */
  assert((((header) & 7) == 1));
  numLiterals = ((header >> 3)) & AlternateHeaderNumLiteralsMask;
  numSlots = numLiterals + LiteralStart;
  /* end numPointerSlotsOf: */
l2:
  for (i = 0; i < numSlots; i += 1) {
    oop = longAt((void *)((objOop + BaseHeaderSize) +
                          ((((usqInt)(i) << (shiftForWord()))))));
    if ((!(oop & (tagMask())))) {
      if ((!((longAt((void *)(oop))) &
             ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
        found = 1;

        /* begin followForwarded: */
        assert(isUnambiguouslyForwarder(oop));
        referent =
            longAt((void *)((oop + BaseHeaderSize) + (0U << (shiftForWord()))));
        while (/* isOopForwarded: */
               ((!(referent & (tagMask())))) &&
               ((!((longAt((void *)(referent))) &
                   ((classIndexMask()) -
                    (isForwardedObjectClassIndexPun())))))) {
          referent = longAt(
              (void *)((referent + BaseHeaderSize) + (0U << (shiftForWord()))));
        }
        oop = referent;

        /* begin storePointer:ofObject:withValue: */
        assert(validStorePointerArgs(i, objOop, oop));
        assert(isNonImmediate(objOop));
        if (oopisGreaterThanOrEqualTo(objOop, oldSpaceStart)) {
          if (/* isYoung: */
              ((!(oop & (tagMask())))) && (oopisLessThan(oop, oldSpaceStart))) {
            /* begin possibleRootStoreInto: */
            if (!((byteAt((void *)(objOop + (formatFieldByteOffset())))) &
                  (1U << (rememberedBitByteShift())))) {
              remember(objOop);
            }
          }
        }

        /* most stores into young objects */
        longAtput((void *)((objOop + BaseHeaderSize) +
                           ((((usqInt)(i) << (shiftForWord()))))),
                  oop);
      }
      if ((depth > 0) &&
          ((/* hasPointerFields: */
            ((!(oop & (tagMask())))) && (hasPointerFieldsNonImm(oop))) &&
           (followForwardedObjectFieldstoDepth(oop, depth - 1)))) {
        found = 1;
      }
    }
  }
  return found;
}