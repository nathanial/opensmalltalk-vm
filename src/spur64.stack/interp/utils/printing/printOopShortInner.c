/* Extracted from interp.c:60352 (function printOopShortInner). */

/* StackInterpreter>>#printOopShortInner: */

static void printOopShortInner(sqInt oop) {
  usqLong bits;
  sqInt classLookupKey;
  sqInt classOop;
  double f;
  char *name;
  sqInt nameLen;
  sqInt objOop;
  double result;
  sqInt tagBits;
  sqInt target;

  if (((oop & (tagMask())) != 0)) {
    if (((oop & (characterTag())) != 0)) {
      fprintf(transcript, "$%c(%" PRIxSQINT ")",
              ((int)(((((usqInt)oop))) >> (numTagBits()))),
              ((((usqInt)oop))) >> (numTagBits()));
      return;
    }
    if ((((oop) & 7) == 1)) {
      fprintf(transcript, "%" PRIdSQINT "(0x%" PRIxSQINT ")", (oop >> 3),
              (oop >> 3));
      return;
    }
    if (((oop & (smallFloatTag())) != 0)) {
      fprintf(transcript, "%g(0x%" PRIxSQINT ")", noFailFloatValueOf(oop), oop);
      return;
    }
    fprintf(transcript, "unknown immediate %p", ((void *)oop));
    return;
  }
  if (!(addressCouldBeObj(oop))) {
    print((oop & 7 /* (allocationUnit - 1) */
               ? " is misaligned"
               : whereIs(oop)));
    return;
  }
  if (((longAt((void *)(oop))) & (classIndexMask())) ==
      (isFreeObjectClassIndexPun())) {
    print(" is a free chunk");
    return;
  }
  if ((!((longAt((void *)(oop))) &
         ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
    target = followForwarded(oop);
    fprintf(transcript, " is a forwarder to %p", ((void *)target));
    return;
  }
  if (/* isFloatInstance: */
      ((tagBits = oop & (tagMask()))
           ? tagBits == (smallFloatTag())
           : ((longAt((void *)(oop))) & (classIndexMask())) ==
                 ClassFloatCompactIndex)) {
    /* begin noFailFloatValueOf: */
    assert(isFloatInstance(oop));
    if (oop & (tagMask())) {
      result = smallFloatValueOf(oop);
    } else {
      fetchFloatAtinto(oop + BaseHeaderSize, result);
    }
    f = result;

    /* begin printFloat: */
    fprintf(transcript, "%g", f);
    return;
  }
  classOop = fetchClassOfNonImm(oop);
  if (!(addressCouldBeObj(classOop))) {
    print("a ??");
    return;
  }
  if ((numSlotsOf(classOop)) == metaclassNumSlots) {
    printNameOfClasscount(oop, 5);
    return;
  }
  if (oop == nilObj) {
    print("nil");
    return;
  }
  if (oop == trueObj) {
    print("true");
    return;
  }
  if (oop == falseObj) {
    print("false");
    return;
  }
  nameLen = lengthOfNameOfClass(classOop);
  if (!nameLen) {
    print("a ??");
    return;
  }
  name = nameOfClass(classOop);
  if ((/* isBytes: */
       ((!(oop & (tagMask())))) &&
       (((byteAt((void *)(oop + (formatFieldByteOffset())))) &
         (formatMask())) >= (firstByteFormat()))) &&
      (nameLen >= 6)) {
    if (!(strncmp((name + nameLen) - 6, "String", 6))) {
      printChar('\'');
      printStringOf(oop);
      printChar('\'');
      return;
    }

    /* strncmp is weird */
    if (!(strncmp((name + nameLen) - 6, "Symbol", 6))) {
      printChar('#');
      printStringOf(oop);
      return;
    }
  }
  if ((nameLen == 9) && ((strncmp(name, "Character", 9)) == 0)) {
    printChar('$');
    printChar(((fetchPointerofObject(0U, oop)) >> 3));
    return;
  }
  fprintf(transcript, "a(n) %.*s", ((int)nameLen), name);

  /* Try to spot association-like things; they're all subclasses of LookupKey */
  if ((((byteAt((void *)(oop + (formatFieldByteOffset())))) & (formatMask())) <=
       5 /* lastPointerFormat */) &&
      (((instanceSizeOf(classOop)) == (ValueIndex + 1)) &&
       (isBytes(fetchPointerofObject(KeyIndex, oop))))) {
    classLookupKey = fetchClassOfNonImm(splObj(SchedulerAssociation));
    while (1) {
      if (classLookupKey == nilObj) {
        return;
      }
      if ((instanceSizeOf(classLookupKey)) == (KeyIndex + 1))
        break;
      /* begin superclassOf: */
      /* begin followObjField:ofObject: */
      objOop = fetchPointerofObject(SuperclassIndex, classLookupKey);
      assert(isNonImmediate(objOop));
      if ((!((longAt((void *)(objOop))) &
             ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
        objOop = fixFollowedFieldofObjectwithInitialValue(
            SuperclassIndex, classLookupKey, objOop);
      }
      classLookupKey = objOop;
    }
    if (includesBehaviorThatOf(classOop, classLookupKey)) {
      /* begin space */
      printChar(' ');
      printOopShortInner(fetchPointerofObject(KeyIndex, oop));
      print(" -> ");
      fprintf(transcript, "%p",
              ((void *)(fetchPointerofObject(ValueIndex, oop))));
    }
  }
}