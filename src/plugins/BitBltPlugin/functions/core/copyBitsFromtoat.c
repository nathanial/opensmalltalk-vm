/* Extracted from BitBltPlugin.c:1500 (function copyBitsFromtoat). */
/* BitBltSimulation>>#copyBitsFrom:to:at: */
/*	Perform the actual copyBits operation.
        Assume: Surfaces have been locked and clipping was performed. */

EXPORT(sqInt)
copyBitsFromtoat(sqInt startX, sqInt stopX, sqInt yValue) {
  destX = startX;
  destY = yValue;
  sourceX = startX;
  width = stopX - startX;
  copyBits();

  /* begin showDisplayBits */
  /* begin ensureDestAndSourceFormsAreValid */
  if (numGCsOnInvocation != (statNumGCs())) {
    reloadDestAndSourceForms();
  }
  showDisplayBitsLeftTopRightBottom(destForm, affectedL, affectedT, affectedR,
                                    affectedB);
  return 0;
}
