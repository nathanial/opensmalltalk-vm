/* Extracted from interp.c:55629 (function longPrintOop). */

/*	useful for VM debugging */

/* StackInterpreter>>#longPrintOop: */

void longPrintOop(sqInt oop) {
  sqInt aClass;
  int byte;
  char *className;
  sqInt classPointer;
  sqInt cls;
  sqInt column;
  sqInt contextSize;
  sqInt fieldOop;
  sqInt fmt;
  sqInt fmtSqInt;
  sqInt header;
  sqInt i;
  usqInt index;
  sqInt lastIndex;
  usqInt lastPointer;
  sqInt length;
  sqInt n;
  usqInt numLiterals;
  usqInt numSlots;
  sqInt objOop;
  sqInt oopClass;
  sqInt sp;
  sqInt tagBits;

  length = 0;
  if (((oop & (tagMask())) != 0)) {
    printImmediateObjecton(oop, transcript);
    return;
  }
  if (!(addressCouldBeObj(oop))) {
    printCantBeObjecton(oop, transcript);
    return;
  }
  if (((longAt((void *)(oop))) & (classIndexMask())) ==
      (isFreeObjectClassIndexPun())) {
    printFreeObjecton(oop, transcript);
    return;
  }
  if ((!((longAt((void *)(oop))) &
         ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
    printForwarderon(oop, transcript);
    return;
  }
  if ((cls = fetchClassOfNonImm(oop))) {
    className = nameOfClasslengthInto(cls, (&length));
    fprintf(transcript, "%p: a(n) %.*s", ((void *)oop), ((int)length),
            className);
    fprintf(transcript, "(%" PRIxSQINT "=>%p)",
            (longAt((void *)(oop))) & (classIndexMask()), ((void *)cls));
  } else {
    fprintf(transcript, "%p has a nil class!!\n", ((void *)oop));
  }
  fmt = (byteAt((void *)(oop + (formatFieldByteOffset())))) & (formatMask());
  fprintf(transcript, " format %" PRIxSQINT "", fmt);
  if (fmt > 5 /* lastPointerFormat */) {
    fprintf(transcript, " nbytes %" PRIdSQINT "", numBytesOf(oop));
  } else {
    if (/* isIndexableFormat: */
        (fmt >= (arrayFormat())) &&
        ((fmt <= (weakArrayFormat())) ||
         (fmt >= (sixtyFourBitIndexableFormat())))) {
      /* begin lengthOf: */
      fmtSqInt =
          (byteAt((void *)(oop + (formatFieldByteOffset())))) & (formatMask());
      numSlots = numSlotsOfAny(oop);
      if (fmtSqInt <= (ephemeronFormat())) {
        length = numSlots;
        goto l1;
      }
      if (fmtSqInt >= (firstByteFormat())) {
        length = ((numSlots << (shiftForWord()))) - (fmtSqInt & 7);
        goto l1;
      }

      /* bytes, including CompiledMethod */
      if (fmtSqInt >= (firstShortFormat())) {
        length = ((numSlots << ((shiftForWord()) - 1))) - (fmtSqInt & 3);
        goto l1;
      }
      if (fmtSqInt >= (firstLongFormat())) {
        length = ((numSlots << ((shiftForWord()) - 2))) - (fmtSqInt & 1);
        goto l1;
      }
      if (fmtSqInt == (sixtyFourBitIndexableFormat())) {
        length = numSlots;
        goto l1;
      }

      /* fmt = self forwardedFormat */
      length = 0;
      /* end lengthOf: */
    l1:
      fprintf(transcript, " size %" PRIdSQINT "",
              length - (fixedFieldsOfformatlength(oop, fmt, length)));
    }
  }

  /* begin printHeaderTypeOf:on: */
  fprintf(
      transcript, " hdr%d %c%c%c%c%c",
      ((byteAt((void *)(oop + (numSlotsFieldByteOffset())))) == (numSlotsMask())
           ? 16
           : 8),
      ((((usqInt)((
            byteAt((void *)(oop + (immutableExtraBitsByteOffset())))))) >>
        (immutableBitByteShift())) &
               1
           ? 'i'
           : '.'),
      ((byteAt((void *)(oop + (formatFieldByteOffset())))) &
               (1U << (rememberedBitByteShift()))
           ? 'r'
           : '.'),
      ((byteAt((void *)(oop + (formatFieldByteOffset())))) &
               (1U << (pinnedBitByteShift()))
           ? 'p'
           : '.'),
      ((byteAt((void *)(oop + (markBitsByteOffset())))) &
               (1U << (markedBitByteShift()))
           ? 'm'
           : '.'),
      ((byteAt((void *)(oop + (markBitsByteOffset())))) &
               (1U << (greyBitByteShift()))
           ? 'g'
           : '.'));
  fprintf(transcript, " hash %" PRIxSQINT "\n",
          (long32At((void *)(oop + 4))) & (identityHashHalfWordMask()));
  if (/* isPureBitsFormat: */
      (fmt >= (sixtyFourBitIndexableFormat())) &&
      (fmt < (firstCompiledMethodFormat()))) {
    /* begin is:KindOfClass: */
    oopClass = /* fetchClassOf: */
        ((tagBits = oop & (tagMask()))
             ? fetchPointerofObject(tagBits, classTableFirstPage)
             : fetchClassOfNonImm(oop));
    while ((oopClass != nilObj) &&
           ((/* isPointers: */
             ((!(oopClass & (tagMask())))) &&
             (((byteAt((void *)(oopClass + (formatFieldByteOffset())))) &
               (formatMask())) <= 5 /* lastPointerFormat */)) &&
            ((numSlotsOfAny(oopClass)) > InstanceSpecificationIndex))) {
      if (oopClass == (fetchPointerofObject(ClassAlien, specialObjectsOop))) {
        fprintf(transcript, " datasize %" PRIdSQINT " %s @ %p\n",
                longAt((void *)(oop + BaseHeaderSize)),
                ((longAt((void *)(oop + BaseHeaderSize))) < 0
                     ? "indirect"
                     : (longAt((void *)(oop + BaseHeaderSize)) ? "direct"
                                                               : "pointer")),
                startOfAlienData(oop));
        return;
      }

      /* begin superclassOf: */
      /* begin followObjField:ofObject: */
      objOop = fetchPointerofObject(SuperclassIndex, oopClass);
      assert(isNonImmediate(objOop));
      if ((!((longAt((void *)(objOop))) &
             ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
        objOop = fixFollowedFieldofObjectwithInitialValue(SuperclassIndex,
                                                          oopClass, objOop);
      }
      oopClass = objOop;
    }
    classPointer = fetchPointerofObject(ClassByteString, specialObjectsOop);

    /* begin superclassOf: */
    /* begin followObjField:ofObject: */
    objOop = fetchPointerofObject(SuperclassIndex, classPointer);
    assert(isNonImmediate(objOop));
    if ((!((longAt((void *)(objOop))) &
           ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
      objOop = fixFollowedFieldofObjectwithInitialValue(SuperclassIndex,
                                                        classPointer, objOop);
    }
    aClass = objOop;

    /* begin is:KindOfClass: */
    oopClass = /* fetchClassOf: */
        ((tagBits = oop & (tagMask()))
             ? fetchPointerofObject(tagBits, classTableFirstPage)
             : fetchClassOfNonImm(oop));
    while ((oopClass != nilObj) &&
           ((/* isPointers: */
             ((!(oopClass & (tagMask())))) &&
             (((byteAt((void *)(oopClass + (formatFieldByteOffset())))) &
               (formatMask())) <= 5 /* lastPointerFormat */)) &&
            ((numSlotsOfAny(oopClass)) > InstanceSpecificationIndex))) {
      if (oopClass == aClass) {
        printStringDataOfon(oop, transcript);
        return;
      }

      /* begin superclassOf: */
      /* begin followObjField:ofObject: */
      objOop = fetchPointerofObject(SuperclassIndex, oopClass);
      assert(isNonImmediate(objOop));
      if ((!((longAt((void *)(objOop))) &
             ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
        objOop = fixFollowedFieldofObjectwithInitialValue(SuperclassIndex,
                                                          oopClass, objOop);
      }
      oopClass = objOop;
    }
    printNonPointerDataOfon(oop, transcript);
    return;
  }

  /* This will answer false if splObj: ClassAlien is nilObject */

  /* begin numPointerSlotsOf: */
  fmtSqInt =
      (byteAt((void *)(oop + (formatFieldByteOffset())))) & (formatMask());
  if (fmtSqInt <= 5 /* lastPointerFormat */) {
    if ((fmtSqInt == (indexablePointersFormat())) &&
        (((longAt((void *)(oop))) & (classIndexMask())) ==
         ClassMethodContextCompactIndex)) {
      /* begin fetchStackPointerOf: */
      sp = fetchPointerofObject(StackPointerIndex, oop);
      if (!((((sp) & 7) == 1))) {
        contextSize = 0;
        goto l2;
      }
      assert((ReceiverIndex + ((sp >> 3))) < (lengthOf(oop)));
      contextSize = (sp >> 3);
      /* end fetchStackPointerOf: */
    l2:
      lastPointer = CtxtTempFrameStart + contextSize;
      goto l3;
    }

    /* contexts end at the stack pointer */

    lastPointer = numSlotsOf(oop);
    goto l3;
  }
  if (fmtSqInt == (forwardedFormat())) {
    lastPointer = 1;
    goto l3;
  }
  if (fmtSqInt < (firstCompiledMethodFormat())) {
    lastPointer = 0;
    goto l3;
  }

  header = methodHeaderOf(oop);

  /* begin literalCountOfMethodHeader: */
  assert((((header) & 7) == 1));
  numLiterals = ((header >> 3)) & AlternateHeaderNumLiteralsMask;
  lastPointer = numLiterals + LiteralStart;
  /* end numPointerSlotsOf: */
l3:
  lastIndex = ((0x100 < lastPointer) ? 0x100 : lastPointer);
  if (lastIndex > 0) {
    for (i = 1; i <= lastIndex; i += 1) {
      fieldOop = fetchPointerofObject(i - 1, oop);

      /* begin space */
      printChar(' ');
      fprintf(transcript, "%" PRIdSQINT "", ((sqInt)(i - 1)));
      printChar(' ');
      printHex(fieldOop);

      /* begin space */
      printChar(' ');
      if ((i == 1) && (((byteAt((void *)(oop + (formatFieldByteOffset())))) &
                        (formatMask())) >= (firstCompiledMethodFormat()))) {
        /* begin printDecodeMethodHeaderOop: */
        /* begin printOopShort: */
        printOopShortInner(fieldOop);
        fflush(transcript);
        if (((fieldOop & AlternateHeaderHasPrimFlag) != 0)) {
          /* begin print: */
          fprintf(transcript, "%s", " hasPrim");
        }
        if (fieldOop & LargeContextBit) {
          /* begin print: */
          fprintf(transcript, "%s", " largeFrame");
        }
        if (((fieldOop >> 3)) < 0) {
          /* begin print: */
          fprintf(transcript, "%s", " altSet");
        }

        /* begin print: */
        fprintf(transcript, "%s", " nLits ");
        /* begin literalCountOfMethodHeader: */
        assert((((fieldOop) & 7) == 1));
        n = ((fieldOop >> 3)) & AlternateHeaderNumLiteralsMask;

        /* begin printNum: */
        fprintf(transcript, "%" PRIdSQINT "", ((sqInt)n));
        fprintf(transcript, "%s", " nArgs ");
        fprintf(transcript, "%" PRIdSQINT "",
                ((sqInt)((((usqInt)(fieldOop)) >> MethodHeaderArgCountShift) &
                         15)));
        fprintf(transcript, "%s", " nTemps ");
        fprintf(transcript, "%" PRIdSQINT "",
                ((sqInt)((((usqInt)(fieldOop)) >> MethodHeaderTempCountShift) &
                         0x3F)));
      } else {
        printOopShortInner(fieldOop);
      }
      cr();
    }
  }
  if (((byteAt((void *)(oop + (formatFieldByteOffset())))) & (formatMask())) >=
      (firstCompiledMethodFormat())) {
    lastPointer = (((literalCountOf(oop)) + LiteralStart) * BytesPerOop) + 1;

    /* begin lengthOf: */
    fmtSqInt =
        (byteAt((void *)(oop + (formatFieldByteOffset())))) & (formatMask());
    numSlots = numSlotsOfAny(oop);
    if (fmtSqInt <= (ephemeronFormat())) {
      lastIndex = numSlots;
      goto l4;
    }
    if (fmtSqInt >= (firstByteFormat())) {
      lastIndex = ((numSlots << (shiftForWord()))) - (fmtSqInt & 7);
      goto l4;
    }

    /* bytes, including CompiledMethod */
    if (fmtSqInt >= (firstShortFormat())) {
      lastIndex = ((numSlots << ((shiftForWord()) - 1))) - (fmtSqInt & 3);
      goto l4;
    }
    if (fmtSqInt >= (firstLongFormat())) {
      lastIndex = ((numSlots << ((shiftForWord()) - 2))) - (fmtSqInt & 1);
      goto l4;
    }
    if (fmtSqInt == (sixtyFourBitIndexableFormat())) {
      lastIndex = numSlots;
      goto l4;
    }

    /* fmt = self forwardedFormat */
    lastIndex = 0;
    /* end lengthOf: */
  l4:
    if ((lastIndex - lastPointer) > 0x100) {
      lastIndex = lastPointer + 0x100;
    }
    column = 1;
    for (index = lastPointer; index <= lastIndex; index += 1) {
      if (column == 1) {
        fprintf(transcript, "%10p ",
                ((void *)(((oop + BaseHeaderSize) + index) - 1)));
      }
      byte = ((int)(byteAt((void *)((oop + BaseHeaderSize) + (index - 1)))));
      fprintf(transcript, "%02x/%-+3d%c", byte, byte,
              (column == 8 ? '\n' : ' '));
      if (((column += 1)) > 8) {
        column = 1;
      }
    }
    if ((lengthOf(oop)) > lastIndex) {
      /* begin print: */
      fprintf(transcript, "%s", "...");
    }
    if (((column >= 2) && (column <= 7))) {
      cr();
    }
  } else {
    if (lastPointer > lastIndex) {
      /* begin print: */
      fprintf(transcript, "%s", "...");
      cr();
    }
  }
}