/* Extracted from BitBltPlugin.c:1419 (function copyBitsFastPathSpecialised). */

static sqInt copyBitsFastPathSpecialised(void) {

#if ENABLE_FAST_BLT
  affectedL = (affectedR = (affectedT = (affectedB = 0)));
  if (!((combinationRule != 41) || (copyBitsRule41Test()))) {
    return primitiveFail();
  }

  /* we skip the tryCopyingBitsQuickly and leave that to falback code */
  if ((combinationRule == 30) || (combinationRule == 0x1F)) {
    if ((methodArgumentCount()) == 1) {
      sourceAlpha = stackIntegerValue(0);
      if (!((!(failed())) && ((sourceAlpha >= 0) && (sourceAlpha <= 0xFF)))) {
        return primitiveFail();
      }
    } else {
      return primitiveFail();
    }
  }

  /* Check and fetch source alpha parameter for alpha blend
     we don't worry about bitCount
     bitCount := 0.
     We don't  do - Choose and perform the actual copy loop.
     self performCopyLoop.
     this is done inversely to plain copyBitsLockedAndClipped */
  if ((combinationRule != 22) && (combinationRule != 32)) {
    affectedL = dx;
    affectedR = dx + bbW;
    affectedT = dy;
    affectedB = dy + bbH;
  }

  /* zero width and height; return the count
     Now we fill the 'operation' structure and pass it to the sneaky ARM code */

  // fill the operation structure
  operation_t op;
  op.combinationRule = combinationRule;
  op.noSource = noSource;
  op.src.bits = (void *)sourceBits;
  op.src.pitch = sourcePitch;
  op.src.depth = sourceDepth;
  op.src.msb = sourceMSB;
  op.src.x = sx;
  op.src.y = sy;
  op.dest.bits = (void *)destBits;
  op.dest.pitch = destPitch;
  op.dest.depth = destDepth;
  op.dest.msb = destMSB;
  op.dest.x = dx;
  op.dest.y = dy;
  op.width = bbW;
  op.height = bbH;
  op.cmFlags = cmFlags;
  op.cmShiftTable = (void *)cmShiftTable;
  op.cmMaskTable = (void *)cmMaskTable;
  op.cmMask = cmMask;
  op.cmLookupTable = (void *)cmLookupTable;
  op.noHalftone = noHalftone;
  op.halftoneHeight = halftoneHeight;
  op.halftoneBase = (void *)halftoneBase;
  if (combinationRule == 30 || combinationRule == 31) {
    op.opt.sourceAlpha = sourceAlpha;
  }
  if (combinationRule == 41) {
    op.opt.componentAlpha.componentAlphaModeColor = componentAlphaModeColor;
    op.opt.componentAlpha.componentAlphaModeAlpha = componentAlphaModeAlpha;
    op.opt.componentAlpha.gammaLookupTable = (void *)gammaLookupTable;
    op.opt.componentAlpha.ungammaLookupTable = (void *)ungammaLookupTable;
  }
  // call the sneaky code
  copyBitsDispatch(&op);
#endif // ENABLE_FAST_BLT

  return 0;
}