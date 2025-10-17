/* Extracted from interp.c:63356 (function showDisplayBitsLeftTopRightBottom).
 */

/*	Repaint the portion of the Smalltalk screen bounded by the affected
        rectangle. Used to synchronize the screen after a Bitblt to the
   Smalltalk Display object.
 */

/* StackInterpreter>>#showDisplayBits:Left:Top:Right:Bottom: */

sqInt showDisplayBitsLeftTopRightBottom(sqInt aForm, sqInt l, sqInt t, sqInt r,
                                        sqInt b) {
  if (deferDisplayUpdates ||
      (aForm !=
       (longAt((void *)((specialObjectsOop + BaseHeaderSize) +
                        ((((usqInt)(TheDisplay) << (shiftForWord()))))))))) {
    return null;
  }
  updateDisplayLeftTopRightBottom(l, t, r, b);
  return 0;
}