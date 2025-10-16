/* Extracted from BitBltPlugin.c:4408 (function primitiveDrawLoop). */
/* BitBltSimulation>>#primitiveDrawLoop */
/*	returns the single pixel at x@y.
        It does not handle LSB bitmaps right now.
        If x or y are < 0, return 0 to indicate transparent (cf
        BitBlt>bitPeekerFromForm: usage).
        Likewise if x>width or y>depth.
        Fail if the rcvr doesn't seem to be a Form, or x|y seem wrong
 */

EXPORT(sqInt)
primitiveDrawLoop(void) {
  sqInt affB;
  sqInt affL;
  sqInt affR;
  sqInt affT;
  sqInt dx1;
  sqInt dy1;
  sqInt i;
  sqInt P;
  sqInt px;
  sqInt py;
  sqInt rcvr;
  sqInt xDelta;
  sqInt yDelta;

  rcvr = stackValue(2);
  xDelta = stackIntegerValue(1);
  yDelta = stackIntegerValue(0);
  if (!(loadBitBltFromwarping(rcvr, 0))) {
    return primitiveFailFor(PrimErrBadReceiver);
  }
  if (!(failed())) {
    /* begin drawLoopX:Y: */
    if (xDelta > 0) {
      dx1 = 1;
    } else {
      if (xDelta) {
        dx1 = -1;
      } else {
        dx1 = 0;
      }
    }
    if (yDelta > 0) {
      dy1 = 1;
    } else {
      if (yDelta) {
        dy1 = -1;
      } else {
        dy1 = 0;
      }
    }
    px = SQABS(yDelta);
    py = SQABS(xDelta);

    /* init null rectangle */
    affL = (affT = 9999);
    affR = (affB = -9999);
    if (py > px) {
      P = py / 2;
      for (i = 1; i <= py; i += 1) {
        destX += dx1;
        if (((P -= px)) < 0) {
          destY += dy1;
          P += py;
        }
        if (i < py) {
          copyBits();
          if (failed()) {
            goto l1;
          }
          if ((affectedL < affectedR) && (affectedT < affectedB)) {
            affL = ((affL < affectedL) ? affL : affectedL);
            affR = ((affR < affectedR) ? affectedR : affR);
            affT = ((affT < affectedT) ? affT : affectedT);
            affB = ((affB < affectedB) ? affectedB : affB);
            if (((affR - affL) * (affB - affT)) > 4000) {
              affectedL = affL;
              affectedR = affR;
              affectedT = affT;
              affectedB = affB;

              /* begin showDisplayBits */
              /* begin ensureDestAndSourceFormsAreValid */
              if (numGCsOnInvocation != (statNumGCs())) {
                reloadDestAndSourceForms();
              }
              showDisplayBitsLeftTopRightBottom(destForm, affectedL, affectedT,
                                                affectedR, affectedB);

              /* init null rectangle */
              affL = (affT = 9999);
              affR = (affB = -9999);
            }
          }
        }
      }
    } else {
      P = px / 2;
      for (i = 1; i <= px; i += 1) {
        destY += dy1;
        if (((P -= py)) < 0) {
          destX += dx1;
          P += px;
        }
        if (i < px) {
          copyBits();
          if (failed()) {
            goto l1;
          }
          if ((affectedL < affectedR) && (affectedT < affectedB)) {
            affL = ((affL < affectedL) ? affL : affectedL);
            affR = ((affR < affectedR) ? affectedR : affR);
            affT = ((affT < affectedT) ? affT : affectedT);
            affB = ((affB < affectedB) ? affectedB : affB);
            if (((affR - affL) * (affB - affT)) > 4000) {
              affectedL = affL;
              affectedR = affR;
              affectedT = affT;
              affectedB = affB;

              /* begin showDisplayBits */
              /* begin ensureDestAndSourceFormsAreValid */
              if (numGCsOnInvocation != (statNumGCs())) {
                reloadDestAndSourceForms();
              }
              showDisplayBitsLeftTopRightBottom(destForm, affectedL, affectedT,
                                                affectedR, affectedB);

              /* init null rectangle */
              affL = (affT = 9999);
              affR = (affB = -9999);
            }
          }
        }
      }
    }

    /* more horizontal
       more vertical
       Remaining affected rect */
    affectedL = affL;
    affectedR = affR;
    affectedT = affT;
    affectedB = affB;

    /* store destX, Y back */
    storeIntegerofObjectwithValue(BBDestXIndex, bitBltOop, destX);
    storeIntegerofObjectwithValue(BBDestYIndex, bitBltOop, destY);
    /* end drawLoopX:Y: */
  l1:

    /* begin showDisplayBits */
    /* begin ensureDestAndSourceFormsAreValid */
    if (numGCsOnInvocation != (statNumGCs())) {
      reloadDestAndSourceForms();
    }
    showDisplayBitsLeftTopRightBottom(destForm, affectedL, affectedT, affectedR,
                                      affectedB);
    if (!(failed())) {
      pop(2);
    }
  }
  return 0;
}
