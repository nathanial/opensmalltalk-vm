/* Extracted from BitBltPlugin.c:1808 (function copyBitsFallback). */
/* BitBltSimulation>>#copyBits:Fallback: */
/*	This version of the inner loop assumes noSource = false. */

void copyBitsFallback(operation_t *op, unsigned int flags) {
  sqInt dxLowBits;
  sqInt endBits;
  unsigned int m1;
  sqInt pixPerM1;
  sqInt startBits;
  sqInt sxLowBits;
  sqInt t;

#if ENABLE_FAST_BLT

  combinationRule = op->combinationRule;
  noSource = op->noSource;
  sourceBits = (sqInt)op->src.bits;
  sourcePitch = op->src.pitch;
  sourceDepth = op->src.depth;
  sourceMSB = op->src.msb;
  sx = op->src.x;
  sy = op->src.y;
  destBits = (sqInt)op->dest.bits;
  destPitch = op->dest.pitch;
  destDepth = op->dest.depth;
  destMSB = op->dest.msb;
  dx = op->dest.x;
  dy = op->dest.y;
  bbW = op->width;
  bbH = op->height;
  cmFlags = op->cmFlags;
  cmShiftTable = (void *)op->cmShiftTable;
  cmMaskTable = (void *)op->cmMaskTable;
  cmMask = op->cmMask;
  cmLookupTable = (void *)op->cmLookupTable;
  noHalftone = op->noHalftone;
  halftoneHeight = op->halftoneHeight;
  halftoneBase = (sqInt)op->halftoneBase;
  if (combinationRule == 30 || combinationRule == 31) {
    sourceAlpha = op->opt.sourceAlpha;
  }
  if (combinationRule == 41) {
    componentAlphaModeColor = op->opt.componentAlpha.componentAlphaModeColor;
    componentAlphaModeAlpha = op->opt.componentAlpha.componentAlphaModeAlpha;
    gammaLookupTable = (void *)op->opt.componentAlpha.gammaLookupTable;
    ungammaLookupTable = (void *)op->opt.componentAlpha.ungammaLookupTable;
  }
  destPPW = 32 / destDepth;
  sourcePPW = 32 / sourceDepth;
  cmBitsPerColor = 0;
  if (cmMask == 0x1FF) {
    cmBitsPerColor = 3;
  }
  if (cmMask == 0xFFF) {
    cmBitsPerColor = 4;
  }
  if (cmMask == 0x7FFF) {
    cmBitsPerColor = 5;
  }

  /* In some places, sourceForm and destForm are compared in order to detect
     whether we're reading and writing the same image. However, these have
     not always been initialised by the time we get here, so substitute
     sourceBits and destBits if so. */
  if ((sourceForm == 0) && (destForm == 0)) {
    sourceForm = sourceBits;
    destForm = destBits;
  }

  /* Try a shortcut for stuff that should be run as quickly as possible */

  /* begin tryCopyingBitsQuickly */
  if (noSource) {
    goto l1;
  }

  /* We handle only combinationRule 34 and combinationRule 41 */
  if (!((combinationRule == 34) || (combinationRule == 41))) {
    goto l1;
  }

  /* We handle only sourceDepth 32 */
  if (!(sourceDepth == 32)) {
    goto l1;
  }

  /* We don't handle overlaps */
  if (sourceForm == destForm) {
    goto l1;
  }
  if (combinationRule == 41) {
    if (destDepth == 32) {
      rgbComponentAlpha32();
      affectedL = dx;
      affectedR = dx + bbW;
      affectedT = dy;
      affectedB = dy + bbH;
      return;
    }
    if (destDepth == 16) {
      rgbComponentAlpha16();
      affectedL = dx;
      affectedR = dx + bbW;
      affectedT = dy;
      affectedB = dy + bbH;
      return;
    }
    if (destDepth == 8) {
      rgbComponentAlpha8();
      affectedL = dx;
      affectedR = dx + bbW;
      affectedT = dy;
      affectedB = dy + bbH;
      return;
    }
    goto l1;
  }

  /* We need at least 8bit deep dest forms */
  if (destDepth < 8) {
    goto l1;
  }

  /* If 8bit, then we want a color map */
  if ((destDepth == 8) && ((cmFlags & ColorMapPresent) == 0)) {
    goto l1;
  }
  if (destDepth == 32) {
    alphaSourceBlendBits32();
  }
  if (destDepth == 16) {
    alphaSourceBlendBits16();
  }
  if (destDepth == 8) {
    alphaSourceBlendBits8();
  }
  affectedL = dx;
  affectedR = dx + bbW;
  affectedT = dy;
  affectedB = dy + bbH;
  return;
  return;
l1:
  bitCount = 0;

  /* Choose and perform the actual copy loop. */

  /* begin performCopyLoop */
  /* begin destMaskAndPointerInit */
  /* A mask, assuming power of two */
  /* how many pixels in first word */
  pixPerM1 = destPPW - 1;
  startBits = destPPW - (dx & pixPerM1);

  /* how many pixels in last word */
  endBits = (((dx + bbW) - 1) & pixPerM1) + 1;
  if (destMSB) {
    mask1 = ((usqInt)(AllOnes)) >> (32 - (startBits * destDepth));
    mask2 = ((((usqInt)(AllOnes) << (32 - (endBits * destDepth))))) & AllOnes;
  } else {
    mask1 = ((((usqInt)(AllOnes) << (32 - (startBits * destDepth))))) & AllOnes;
    mask2 = ((usqInt)(AllOnes)) >> (32 - (endBits * destDepth));
  }

  /* determine number of words stored per line; merge masks if only 1 */
  if (bbW <= startBits) {
    mask1 = mask1 & mask2;
    mask2 = 0;
    nWords = 1;
  } else {
    nWords = (((bbW - startBits) + pixPerM1) / destPPW) + 1;
  }

  /* defaults for no overlap with source */
  /* calculate byte addr and delta, based on first word of data */
  /* Note pitch is bytes and nWords is longs, not bytes */
  hDir = (vDir = 1);
  destIndex = (destBits + (dy * destPitch)) + ((dx / destPPW) * 4);

  /* byte addr delta */
  destDelta = (destPitch * vDir) - (4 * (nWords * hDir));
  if (noSource) {
    copyLoopNoSource();
  } else {
    /* begin checkSourceOverlap */
    if ((sourceForm == destForm) && (dy >= sy)) {
      if (dy > sy) {
        vDir = -1;
        sy = (sy + bbH) - 1;
        dy = (dy + bbH) - 1;
      } else {
        if ((dy == sy) && (dx > sx)) {
          hDir = -1;
          sx = (sx + bbW) - 1;

          /* start at right */
          dx = (dx + bbW) - 1;

          /* and fix up masks */
          if (nWords > 1) {
            t = mask1;
            mask1 = mask2;
            mask2 = t;
          }
        }
      }

      /* have to start at bottom
         Dest inits may be affected by this change */
      destIndex = (destBits + (dy * destPitch)) + ((dx / destPPW) * 4);
      destDelta = (destPitch * vDir) - (4 * (nWords * hDir));
    }
    if ((sourceDepth != destDepth) ||
        ((cmFlags != 0) || (sourceMSB != destMSB))) {
      copyLoopPixMap();
    } else {
      /* begin sourceSkewAndPointerInit */
      assert((destPPW == sourcePPW) &&
             ((destMSB == sourceMSB) && (destDepth == sourceDepth)));

      /* A mask, assuming power of two */
      pixPerM1 = destPPW - 1;
      sxLowBits = sx & pixPerM1;
      dxLowBits = dx & pixPerM1;

      /* how many pixels in first word */
      startBits =
          (hDir > 0 ? sourcePPW - (sx & pixPerM1) : (sx & pixPerM1) + 1);
      m1 = (destMSB == (hDir > 0)
                ? ((usqInt)(AllOnes)) >> (32 - (startBits * destDepth))
                : ((((usqInt)(AllOnes) << (32 - (startBits * destDepth))))) &
                      AllOnes);

      /* i.e. there are some missing bits */
      /* calculate right-shift skew from source to dest */
      preload = (bbW > startBits) && ((m1 & mask1) != mask1);

      /* -32..32 */
      skew = destDepth *
             ((sourceMSB ? sxLowBits - dxLowBits : dxLowBits - sxLowBits));
      if (preload) {
        skew = (skew < 0 ? skew + 32 : skew - 32);
      }

      /* Calc byte addr and delta from longWord info */
      sourceIndex = (sourceBits + (sy * sourcePitch)) + ((sx / sourcePPW) * 4);

      /* calculate increments from end of 1 line to start of next */
      sourceDelta = (sourcePitch * vDir) - (4 * (nWords * hDir));
      if (preload) {
        sourceDelta -= 4 * hDir;
      }

      /* Compensate for extra source word fetched */
      assert(!((preload && (skew == 0))));
      assert(((skew >= -31) && (skew <= 0x1F)));
      copyLoop();
    }
  }
#endif // ENABLE_FAST_BLT
}
