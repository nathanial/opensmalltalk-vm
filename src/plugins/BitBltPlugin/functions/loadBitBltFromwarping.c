/* Extracted from BitBltPlugin.c:3009 (function loadBitBltFromwarping). */

static sqInt loadBitBltFromwarping(sqInt bbObj, sqInt aBool) {
  sqInt cmOop;
  sqInt cmSize;
  sqInt destBitsSize;
  sqInt fieldOop;
  double floatValue;
  sqInt halftoneBits;
  sqInt mapOop;
  sqInt oldStyle;
  sqInt oop;
  sqInt sourceBitsSize;

  bitBltOop = bbObj;
  isWarping = aBool;
  bitBltIsReceiver = bbObj == (stackValue(methodArgumentCount()));
  numGCsOnInvocation = statNumGCs();
  combinationRule = fetchIntegerofObject(BBRuleIndex, bitBltOop);
  if ((failed()) ||
      (((combinationRule < 0) || (combinationRule > (OpTableSize - 2))) ||
       (((combinationRule >= 16) && (combinationRule <= 17))))) {
    return 0;
  }
  if (/* noSourceCombinationRule */
      (combinationRule == 0) ||
      ((combinationRule == 5) ||
       ((combinationRule == 10) || (combinationRule == 15)))) {
    noSource = (noHalftone = 1);
  } else {
    sourceForm = fetchPointerofObject(BBSourceFormIndex, bitBltOop);
    noSource = sourceForm == (nilObject());
    halftoneForm = fetchPointerofObject(BBHalftoneFormIndex, bitBltOop);
    noHalftone = halftoneForm == (nilObject());
  }
  destForm = fetchPointerofObject(BBDestFormIndex, bbObj);

  /* begin loadBitBltDestForm */
  if (!((isPointers(destForm)) && ((slotSizeOf(destForm)) >= 4))) {
    return 0;
  }
  destBits = fetchPointerofObject(FormBitsIndex, destForm);
  destWidth = fetchIntegerofObject(FormWidthIndex, destForm);
  destHeight = fetchIntegerofObject(FormHeightIndex, destForm);
  if (!((destWidth >= 0) && (destHeight >= 0))) {
    return 0;
  }
  destDepth = fetchIntegerofObject(FormDepthIndex, destForm);
  if (!((destMSB = destDepth > 0))) {
    destDepth = 0 - destDepth;
  }

  /* Ignore an integer bits handle for Display in which case
     the appropriate values will be obtained by calling ioLockSurfaceBits(). */
  if (isIntegerObject(destBits)) {
    if (!querySurfaceFn) {
      if (!(loadSurfacePlugin())) {
        return 0;
      }
    }
    if (!(querySurfaceFn(integerValueOf(destBits), (&destWidth), (&destHeight),
                         (&destDepth), (&destMSB)))) {
      primitiveFailFor(PrimErrCallbackError);
      return 0;
    }
    destPPW = 32 / destDepth;
    destBits = (destPitch = 0);
  } else {
    if (!(isWordsOrBytes(destBits))) {
      return 0;
    }
    destPPW = 32 / destDepth;
    destPitch = ((destWidth + (destPPW - 1)) / destPPW) * 4;
    destBitsSize = byteSizeOf(destBits);
    if (!(destBitsSize >= (destPitch * destHeight))) {
      return 0;
    }

    /* Skip header since external bits don't have one */
    destBits = oopForPointer(firstIndexableField(destBits));
  }

  /* Query for actual surface dimensions */
  destX = fetchIntOrFloatofObjectifNil(BBDestXIndex, bitBltOop, 0);
  destY = fetchIntOrFloatofObjectifNil(BBDestYIndex, bitBltOop, 0);
  width = fetchIntOrFloatofObjectifNil(BBWidthIndex, bitBltOop, destWidth);
  height = fetchIntOrFloatofObjectifNil(BBHeightIndex, bitBltOop, destHeight);
  if (failed()) {
    return 0;
  }
  if (noSource) {
    sourceX = (sourceY = 0);
  } else {
    /* begin loadBitBltSourceForm */
    if (!((isPointers(sourceForm)) && ((slotSizeOf(sourceForm)) >= 4))) {
      return 0;
    }
    sourceBits = fetchPointerofObject(FormBitsIndex, sourceForm);

    /* begin fetchIntOrFloat:ofObject: */
    fieldOop = fetchPointerofObject(FormWidthIndex, sourceForm);
    if (isIntegerObject(fieldOop)) {
      sourceWidth = integerValueOf(fieldOop);
      goto l1;
    }
    floatValue = floatValueOf(fieldOop);
    if (!((-2.147483648e9 <= floatValue) && (floatValue <= 2.147483647e9))) {
      primitiveFail();
      sourceWidth = 0;
      goto l1;
    }
    sourceWidth = ((sqInt)floatValue);
    /* end fetchIntOrFloat:ofObject: */
  l1:

    /* begin fetchIntOrFloat:ofObject: */
    fieldOop = fetchPointerofObject(FormHeightIndex, sourceForm);
    if (isIntegerObject(fieldOop)) {
      sourceHeight = integerValueOf(fieldOop);
      goto l2;
    }
    floatValue = floatValueOf(fieldOop);
    if (!((-2.147483648e9 <= floatValue) && (floatValue <= 2.147483647e9))) {
      primitiveFail();
      sourceHeight = 0;
      goto l2;
    }
    sourceHeight = ((sqInt)floatValue);
    /* end fetchIntOrFloat:ofObject: */
  l2:
    if (!((sourceWidth >= 0) && (sourceHeight >= 0))) {
      return 0;
    }
    sourceDepth = fetchIntegerofObject(FormDepthIndex, sourceForm);
    if (!((sourceMSB = sourceDepth > 0))) {
      sourceDepth = 0 - sourceDepth;
    }

    /* Ignore an integer bits handle for Display in which case
       the appropriate values will be obtained by calling ioLockSurfaceBits().
     */
    if (isIntegerObject(sourceBits)) {
      if (!querySurfaceFn) {
        if (!(loadSurfacePlugin())) {
          return 0;
        }
      }
      if (!(querySurfaceFn(integerValueOf(sourceBits), (&sourceWidth),
                           (&sourceHeight), (&sourceDepth), (&sourceMSB)))) {
        primitiveFailFor(PrimErrCallbackError);
        return 0;
      }
      sourcePPW = 32 / sourceDepth;
      sourceBits = (sourcePitch = 0);
    } else {
      if (!(isWordsOrBytes(sourceBits))) {
        return 0;
      }
      sourcePPW = 32 / sourceDepth;
      sourcePitch = ((sourceWidth + (sourcePPW - 1)) / sourcePPW) * 4;
      sourceBitsSize = byteSizeOf(sourceBits);
      if (!(sourceBitsSize >= (sourcePitch * sourceHeight))) {
        return 0;
      }

      /* Skip header since external bits don't have one */
      sourceBits = oopForPointer(firstIndexableField(sourceBits));
    }

    /* Query for actual surface dimensions */

    /* begin loadColorMap */
    cmFlags = (cmMask = (cmBitsPerColor = 0));
    cmShiftTable = null;
    cmMaskTable = null;
    cmLookupTable = null;
    cmOop = fetchPointerofObject(BBColorMapIndex, bitBltOop);
    if (cmOop == (nilObject())) {
      goto l8;
    }

    /* even if identity or somesuch - may be cleared later */
    cmFlags = ColorMapPresent;
    if (isWords(cmOop)) {
      oldStyle = 1;
      cmSize = slotSizeOf(cmOop);
      cmLookupTable = firstIndexableField(cmOop);
    } else {
      oldStyle = 0;
      if (!((isPointers(cmOop)) && ((slotSizeOf(cmOop)) >= 3))) {
        return 0;
      }
      mapOop = fetchPointerofObject(0, cmOop);

      /* begin loadColorMapShiftOrMaskFrom:type: */
      if (mapOop == (nilObject())) {
        cmShiftTable = null;
        goto l6;
      }
      if (!((isWords(mapOop)) && ((slotSizeOf(mapOop)) == 4))) {
        primitiveFail();
        cmShiftTable = null;
        goto l6;
      }
      cmShiftTable = firstIndexableField(mapOop);
      /* end loadColorMapShiftOrMaskFrom:type: */
    l6:
      mapOop = fetchPointerofObject(1, cmOop);

      /* begin loadColorMapShiftOrMaskFrom:type: */
      if (mapOop == (nilObject())) {
        cmMaskTable = null;
        goto l7;
      }
      if (!((isWords(mapOop)) && ((slotSizeOf(mapOop)) == 4))) {
        primitiveFail();
        cmMaskTable = null;
        goto l7;
      }
      cmMaskTable = firstIndexableField(mapOop);
      /* end loadColorMapShiftOrMaskFrom:type: */
    l7:
      oop = fetchPointerofObject(2, cmOop);
      if (oop == (nilObject())) {
        cmSize = 0;
      } else {
        if (!(isWords(oop))) {
          return 0;
        }
        cmSize = slotSizeOf(oop);
        cmLookupTable = firstIndexableField(oop);
      }
      cmFlags = cmFlags | ColorMapNewStyle;
    }

    /* This is an old-style color map (indexed only, with implicit RGBA
       conversion) A new-style color map (fully qualified) */
    if (cmSize & (cmSize - 1)) {
      return 0;
    }
    cmMask = cmSize - 1;
    cmBitsPerColor = 0;
    if (cmSize) {
      cmFlags = cmFlags | ColorMapIndexedPart;
      if (cmSize == 0x200) {
        cmBitsPerColor = 3;
      }
      if (cmSize == 0x1000) {
        cmBitsPerColor = 4;
      }
      if (cmSize == 0x8000) {
        cmBitsPerColor = 5;
      }
    } else {
      cmLookupTable = null;
      cmMask = 0;
    }
    if (oldStyle) {
      setupColorMasks();
    }

    /* needs implicit conversion
       Check if colorMap is just identity mapping for RGBA parts */

    /* begin isIdentityMap:with: */
    if ((cmShiftTable == null) || (cmMaskTable == null)) {
      goto l3;
    }
    if (((cmShiftTable[RedIndex]) == 0) &&
        (((cmShiftTable[GreenIndex]) == 0) &&
         (((cmShiftTable[BlueIndex]) == 0) &&
          (((cmShiftTable[AlphaIndex]) == 0) &&
           (((cmMaskTable[RedIndex]) == 0xFF0000) &&
            (((cmMaskTable[GreenIndex]) == 0xFF00) &&
             (((cmMaskTable[BlueIndex]) == 0xFF) &&
              ((cmMaskTable[AlphaIndex]) == 0xFF000000U)))))))) {
      goto l3;
    }
    goto l5;
  l3:
    cmMaskTable = null;
    cmShiftTable = null;
    goto l4;
  l5:
    cmFlags = cmFlags | ColorMapFixedPart;
  l4:
    goto l8;
  l8:

    /* Need the implicit setup here in case of 16<->32 bit conversions */
    if (!(cmFlags & ColorMapNewStyle)) {
      setupColorMasks();
    }
    sourceX = fetchIntOrFloatofObjectifNil(BBSourceXIndex, bitBltOop, 0);
    sourceY = fetchIntOrFloatofObjectifNil(BBSourceYIndex, bitBltOop, 0);
  }

  /* begin loadHalftoneForm */
  if (noHalftone) {
    halftoneBase = null;
    goto l9;
  }
  if ((isPointers(halftoneForm)) && ((slotSizeOf(halftoneForm)) >= 4)) {
    halftoneBits = fetchPointerofObject(FormBitsIndex, halftoneForm);
    halftoneHeight = fetchIntegerofObject(FormHeightIndex, halftoneForm);
    if (!(isWords(halftoneBits))) {
      noHalftone = 1;
    }
  } else {
    if (!(isWords(halftoneForm))) {
      return 0;
    }
    halftoneBits = halftoneForm;
    halftoneHeight = slotSizeOf(halftoneBits);
  }

  /* Old-style 32xN monochrome halftone Forms
     New spec accepts, basically, a word array */
  halftoneBase = oopForPointer(firstIndexableField(halftoneBits));
  goto l9;
l9:
  clipX = fetchIntOrFloatofObjectifNil(BBClipXIndex, bitBltOop, 0);
  clipY = fetchIntOrFloatofObjectifNil(BBClipYIndex, bitBltOop, 0);
  clipWidth =
      fetchIntOrFloatofObjectifNil(BBClipWidthIndex, bitBltOop, destWidth);
  clipHeight =
      fetchIntOrFloatofObjectifNil(BBClipHeightIndex, bitBltOop, destHeight);
  if (failed()) {
    return 0;
  }
  if (clipX < 0) {
    clipWidth += clipX;
    clipX = 0;
  }
  if (clipY < 0) {
    clipHeight += clipY;
    clipY = 0;
  }
  if ((clipX + clipWidth) > destWidth) {
    clipWidth = destWidth - clipX;
  }
  if ((clipY + clipHeight) > destHeight) {
    clipHeight = destHeight - clipY;
  }
  if (numGCsOnInvocation != (statNumGCs())) {
    primitiveFailFor(PrimErrObjectMoved);
    return 0;
  }

  /* querySurface could be a callback in loadSourceFor: and loadDestForm: */
  return 1;
}