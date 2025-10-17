/* Extracted from interp.c:43834 (function printNonPointerDataOfon). */

/* SpurMemoryManager>>#printNonPointerDataOf:on: */

static void printNonPointerDataOfon(sqInt oop, FILE *aStream) {
  sqInt elementsPerLine;
  sqInt format;
  sqInt index;
  sqInt lastIndex;
  usqInt numSlots;
  usqInt numSlotsUsqInt;

  format = (byteAt((void *)(oop + (formatFieldByteOffset())))) & (formatMask());
  assert(((format >= (sixtyFourBitIndexableFormat())) &&
          (format <= ((firstCompiledMethodFormat()) - 1))));

  /* begin lengthOf:format: */
  /* begin numSlotsOfAny: */
  numSlotsUsqInt = byteAt((void *)(oop + (numSlotsFieldByteOffset())));
  numSlots =
      (numSlotsUsqInt == (numSlotsMask())
           ? ((((usqInt)((
                 (sqInt)((usqInt)((longAt((void *)(oop - BaseHeaderSize))))
                         << 8)))))) >>
                 8
           : numSlotsUsqInt);
  if (format <= (ephemeronFormat())) {
    lastIndex = numSlots;
    goto l1;
  }
  if (format >= (firstByteFormat())) {
    lastIndex = ((numSlots << (shiftForWord()))) - (format & 7);
    goto l1;
  }

  /* bytes, including CompiledMethod */
  if (format >= (firstShortFormat())) {
    lastIndex = ((numSlots << ((shiftForWord()) - 1))) - (format & 3);
    goto l1;
  }
  if (format >= (firstLongFormat())) {
    lastIndex = ((numSlots << ((shiftForWord()) - 2))) - (format & 1);
    goto l1;
  }
  if (format == (sixtyFourBitIndexableFormat())) {
    lastIndex = numSlots;
    goto l1;
  }

  /* fmt = self forwardedFormat */
  lastIndex = 0;
  /* end lengthOf:format: */
l1:
  if (!lastIndex) {
    return;
  }
  if (format == (sixtyFourBitIndexableFormat())) {
    lastIndex = ((32 < lastIndex) ? 32 : lastIndex);

    /* 0x/16r0123456789ABCDEF<space|cr> x 4 = 76/80 */
    elementsPerLine = 4;
    for (index = 1; index <= lastIndex; index += 1) {
      fprintf(aStream, "%18" PRIXSQINT "%c",
              ((usqLong)(long64At((void *)((oop + BaseHeaderSize) +
                                           ((((usqInt)((index - 1)) << 3))))))),
              (((index % elementsPerLine) == 0) || (index == lastIndex) ? '\n'
                                                                        : ' '));
    }
    return;
  }
  if (format < (firstShortFormat())) {
    lastIndex = ((64 < lastIndex) ? 64 : lastIndex);

    /* 0x/16r12345678<space|cr> x 8 = 80/88 */
    elementsPerLine = 8;
    for (index = 1; index <= lastIndex; index += 1) {
      fprintf(aStream, "%10p%c",
              ((void *)(long32At((void *)((oop + BaseHeaderSize) +
                                          ((((usqInt)((index - 1)) << 2))))))),
              (((index % elementsPerLine) == 0) || (index == lastIndex) ? '\n'
                                                                        : ' '));
    }
    return;
  }
  if (format < (firstByteFormat())) {
    lastIndex = ((128 < lastIndex) ? 128 : lastIndex);

    /* 0x/16r1234<space|cr> x 10 = 70/80 */
    elementsPerLine = 10;
    for (index = 1; index <= lastIndex; index += 1) {
      fprintf(aStream, "%6p%c",
              ((void *)(shortAt((void *)((oop + BaseHeaderSize) +
                                         ((((usqInt)((index - 1)) << 1))))))),
              (((index % elementsPerLine) == 0) || (index == lastIndex) ? '\n'
                                                                        : ' '));
    }
    return;
  }
  lastIndex = ((0x100 < lastIndex) ? 0x100 : lastIndex);

  /* 0x/16r12<space|cr> x 16 = 80/96 */
  elementsPerLine = 16;
  for (index = 1; index <= lastIndex; index += 1) {
    fprintf(aStream, "%4p%c",
            ((void *)(byteAt((void *)((oop + BaseHeaderSize) + (index - 1))))),
            (((index % elementsPerLine) == 0) || (index == lastIndex) ? '\n'
                                                                      : ' '));
  }
}