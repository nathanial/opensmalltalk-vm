/* Extracted from interp.c:65439 (function updateDisplayLeftTopRightBottom). */

/*	Repaint the portion of the Smalltalk screen bounded by the affected
        rectangle. Used to synchronize the screen after a Bitblt to the
   Smalltalk Display object. */

/* StackInterpreter>>#updateDisplayLeft:Top:Right:Bottom: */

static sqInt updateDisplayLeftTopRightBottom(sqInt l, sqInt t, sqInt r,
                                             sqInt b) {
  sqInt bottom;
  sqInt left;
  sqInt right;
  sqInt surfaceHandle;
  sqInt top;

  left = ((l < 0) ? 0 : l);
  right = ((r < displayWidth) ? r : displayWidth);
  top = ((t < 0) ? 0 : t);
  bottom = ((b < displayHeight) ? b : displayHeight);
  if (!((left <= right) && (top <= bottom))) {
    return null;
  }
  if (((((((sqInt)displayBits))) & 7) == 1)) {
    surfaceHandle = ((((sqInt)displayBits)) >> 3);
    if (!showSurfaceFn) {
      showSurfaceFn = ioLoadFunctionFrom("ioShowSurface", "SurfacePlugin");
      if (!showSurfaceFn) {
        return (primFailCode = PrimErrNotFound);
      }
    }
    showSurfaceFn(surfaceHandle, left, top, right - left, bottom - top);
  } else {
    assert(isNonImmediate(((sqInt)displayBits)));
    ioShowDisplay(((sqInt)displayBits), displayWidth, displayHeight,
                  displayDepth, left, right, top, bottom);
  }
  return 0;
}