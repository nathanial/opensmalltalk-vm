/* Extracted from interp.c:60531 (function printOop). */

/*	useful for VM debugging */

/* StackInterpreter>>#printOop: */

void printOop(sqInt oop) {
  sqInt byte;
  sqInt bytecodesPerLine;
  char *className;
  sqInt cls;
  sqInt column;
  sqInt fmt;
  sqInt fmtSqInt;
  sqInt index;
  sqInt lastIndex;
  sqInt length;
  usqInt numSlots;
  usqInt startIP;
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
  if (!((cls = fetchClassOfNonImm(oop)))) {
    fprintf(transcript, "%p has a nil class!!\n", ((void *)oop));
    return;
  }
  className = nameOfClasslengthInto(cls, (&length));
  fprintf(transcript, "%p: a(n) %.*s", ((void *)oop), ((int)length), className);
  if (/* isFloatInstance: */
      ((tagBits = oop & (tagMask()))
           ? tagBits == (smallFloatTag())
           : ((longAt((void *)(oop))) & (classIndexMask())) ==
                 ClassFloatCompactIndex)) {
    fprintf(transcript, "\n%g\n", noFailFloatValueOf(oop));
    return;
  }
  fmt = (byteAt((void *)(oop + (formatFieldByteOffset())))) & (formatMask());
  if (fmt > 5 /* lastPointerFormat */) {
    fprintf(transcript, " nbytes %" PRIdSQINT "", numBytesOf(oop));
  }
  cr();
  if (/* isPureBitsFormat: */
      (fmt >= (sixtyFourBitIndexableFormat())) &&
      (fmt < (firstCompiledMethodFormat()))) {
    if (isKindOfClass(oop,
                      fetchPointerofObject(ClassAlien, specialObjectsOop))) {
      fprintf(transcript, " datasize %" PRIdSQINT " %s @ %p\n",
              longAt((void *)(oop + BaseHeaderSize)),
              ((longAt((void *)(oop + BaseHeaderSize))) < 0
                   ? "indirect"
                   : (longAt((void *)(oop + BaseHeaderSize)) ? "direct"
                                                             : "pointer")),
              startOfAlienData(oop));
      return;
    }
    if (isKindOfClass(oop, superclassOf(fetchPointerofObject(
                               ClassByteString, specialObjectsOop)))) {
      printStringDataOfon(oop, transcript);
      return;
    }
    printNonPointerDataOfon(oop, transcript);
    return;
  }

  /* This will answer false if splObj: ClassAlien is nilObject */
  startIP = (fmt >= (firstCompiledMethodFormat())
                 ? (((literalCountOf(oop)) + LiteralStart) * BytesPerOop) /
                       BytesPerWord
                 : numSlotsOf(oop));
  lastIndex = ((0x100 < startIP) ? 0x100 : startIP);
  if (lastIndex > 0) {
    for (index = 1; index <= lastIndex; index += 1) {
      printHex(fetchPointerofObject(index - 1, oop));

      /* begin space */
      printChar(' ');
      if (!(index % 5 /* elementsPerPrintOopLine */)) {
        cr();
      }
    }
    if (lastIndex % 5 /* elementsPerPrintOopLine */) {
      cr();
    }
  }
  if (((byteAt((void *)(oop + (formatFieldByteOffset())))) & (formatMask())) >=
      (firstCompiledMethodFormat())) {
    startIP = (((literalCountOf(oop)) + LiteralStart) * BytesPerOop) + 1;

    /* begin lengthOf: */
    lastIndex = lengthOf(oop);
    /* end lengthOf: */
  l1:
    if ((lastIndex - startIP) > 0x100) {
      lastIndex = startIP + 0x100;
    }
    bytecodesPerLine = 8;
    column = 1;
    for (index = startIP; index <= lastIndex; index += 1) {
      if (column == 1) {
        fprintf(transcript, "%10p",
                ((void *)(((oop + BaseHeaderSize) + index) - 1)));
      }
      byte = byteAt((void *)((oop + BaseHeaderSize) + (index - 1)));
      fprintf(transcript, " %02x/%-+3d", ((int)byte), ((int)byte));
      column += 1;
      if (column > bytecodesPerLine) {
        column = 1;
        cr();
      }
    }
    if ((lengthOf(oop)) > lastIndex) {
      print("...");
    }
    if (!(column == 1)) {
      cr();
    }
  } else {
    if (startIP > 64) {
      print("...");
      cr();
    }
  }
}