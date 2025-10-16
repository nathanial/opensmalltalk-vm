/* Extracted from BitBltPlugin.c:4218 (function primitiveCopyBits). */
/* BitBltSimulation>>#primitiveCopyBits */

EXPORT(sqInt)
primitiveCopyBits(void) {
  sqInt rcvr;

  rcvr = stackValue(methodArgumentCount());
  if (!(loadBitBltFromwarping(rcvr, 0))) {
    return primitiveFailFor(PrimErrBadReceiver);
  }
  copyBits();
  if (failed()) {
    return null;
  }

  /* begin showDisplayBits */
  /* begin ensureDestAndSourceFormsAreValid */
  if (numGCsOnInvocation != (statNumGCs())) {
    reloadDestAndSourceForms();
  }
  showDisplayBitsLeftTopRightBottom(destForm, affectedL, affectedT, affectedR,
                                    affectedB);
  if (failed()) {
    return null;
  }
  if ((combinationRule == 22) || (combinationRule == 32)) {
    methodReturnInteger(bitCount);
  } else {
    methodReturnReceiver();
  }
  return 0;
}
