/* Extracted from BitBltPlugin.c:1500 (function copyBitsFromtoat). */

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