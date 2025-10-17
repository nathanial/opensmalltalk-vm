/* Extracted from interp.c:28160 (function copyToOldSpacebytesformat). */

/*	Copy survivor to oldSpace. Answer the new oop of the object. */
/*	Should be too infrequent to lower icache density of copyAndForward: */

/* SpurGenerationScavenger>>#copyToOldSpace:bytes:format: */

static NeverInline sqInt copyToOldSpacebytesformat(
    sqInt survivor, sqInt bytesInObject, sqInt formatOfSurvivor) {
  sqInt field;
  sqInt newOop;
  sqInt newStart;
  sqInt nTenures;
  sqInt p;
  usqInt startOfSurvivor;
  sqInt toDoLimit;

  assert((formatOfSurvivor == (formatOf(survivor))) &&
         (((!(isMarked(survivor))) || (tenureCriterion == MarkOnTenure)) &&
          ((tenureCriterion == TenureToShrinkRT) ||
           ((!(isPinned(survivor))) && (!(isRemembered(survivor)))))));
  nTenures = statTenures;
  startOfSurvivor = /* startOfObject: */
      ((byteAt((void *)(survivor + (numSlotsFieldByteOffset())))) ==
               (numSlotsMask())
           ? survivor - BaseHeaderSize
           : survivor);
  newStart = allocateOldSpaceChunkOfBytes(bytesInObject);
  if (!newStart) {
    growOldSpaceByAtLeast(0);
    newStart = allocateOldSpaceChunkOfBytes(bytesInObject);
    if (!newStart) {
      error("out of memory");
    }
  }

  /* manager checkFreeSpace. */
  memcpy(((void *)newStart), ((void *)startOfSurvivor), bytesInObject);
  newOop = newStart + (survivor - startOfSurvivor);
  if (tenureCriterion >=
      (((TenureToShrinkRT < MarkOnTenure) ? TenureToShrinkRT : MarkOnTenure))) {
    if (tenureCriterion == TenureToShrinkRT) {
      /* begin rtRefCountOf:put: */
      assert(isYoungObject(newOop));
      byteAtput((void *)(newOop + (formatFieldByteOffset())),
                ((byteAt((void *)(newOop + (formatFieldByteOffset())))) &
                 (formatMask())) +
                    (0U << (rememberedBitByteShift())));
    }
    if (tenureCriterion == MarkOnTenure) {
      /* begin setIsMarkedOf:to: */
      assert(!(isFreeObject(newOop)));
      byteAtput((void *)(newOop + (markBitsByteOffset())),
                (byteAt((void *)(newOop + (markBitsByteOffset())))) |
                    (1U << (markedBitByteShift())));
    }
  }
  statTenures = nTenures + 1;
  if (/* isAnyPointerFormat: */
      (formatOfSurvivor <= 5 /* lastPointerFormat */) ||
      (formatOfSurvivor >= (firstCompiledMethodFormat()))) {
    toDoLimit = (bytesInObject - (survivor - startOfSurvivor)) - BytesPerWord;
    for (p = BaseHeaderSize; p <= toDoLimit; p += BytesPerWord) {
      field = longAt((void *)(survivor + p));
      if (/* isReallyYoung: */
          ((!(field & (tagMask())))) &&
          ((/* begin isReallyYoungObject: */ /* begin isYoungObject: */
            assert(isNonImmediate(field)),
            oopisLessThan(field, oldSpaceStart)))) {
        remember(newOop);
        return newOop;
      }
    }
  }

  /* A very quick and dirty scan to find young referents.  If we misidentify
     bytes in a CompiledMethod as young we don't care; it's unlikely, and a
     subsequent scan of the rt will filter the object out.  But it's good to
     filter here because otherwise an attempt to shrink the RT may simply fill
     it up with new objects, and here the data is likely in the cache. */
  return newOop;
}