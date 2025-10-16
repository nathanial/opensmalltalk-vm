/* Extracted from BitBltPlugin.c:3411 (function lockSurfaces). */
/* BitBltSimulation>>#lockSurfaces */
/*	The module with the given name was just unloaded.
        Make sure we have no dangling references. */

static sqInt lockSurfaces(void) {
  sqInt b;
  sqInt destHandle;
  sqInt l;
  sqInt r;
  sqInt sourceHandle;
  sqInt t;

  assert(numGCsOnInvocation == (statNumGCs()));
  hasSurfaceLock = 0;
  if (!destBits) {
    if (!lockSurfaceFn) {
      if (!(loadSurfacePlugin())) {
        return 0;
      }
    }
    destHandle = fetchIntegerofObject(FormBitsIndex, destForm);
    if (!((sourceBits != 0) || (noSource))) {
      sourceHandle = fetchIntegerofObject(FormBitsIndex, sourceForm);

      /* Handle the special case of equal source and dest handles */
      if (sourceHandle == destHandle) {
        if (isWarping) {
          l = ((sx < dx) ? sx : dx);
          r = (((sx < dx) ? dx : sx)) + bbW;
          t = ((sy < dy) ? sy : dy);
          b = (((sy < dy) ? dy : sy)) + bbH;
          sourceBits =
              lockSurfaceFn(sourceHandle, (&sourcePitch), l, t, r - l, b - t);
        } else {
          sourceBits = lockSurfaceFn(sourceHandle, (&sourcePitch), 0, 0,
                                     sourceWidth, sourceHeight);
        }

        /* When warping we always need the entire surface for the source
           Otherwise use overlapping area */
        destBits = sourceBits;
        destPitch = sourcePitch;
        hasSurfaceLock = 1;
        if (numGCsOnInvocation != (statNumGCs())) {
          unlockSurfaces();
          primitiveFailFor(PrimErrObjectMoved);
          return 0;
        }
        if (!destBits) {
          unlockSurfaces();
          primitiveFailFor(PrimErrCallbackError);
          return 0;
        }
        endOfDestination =
            (endOfSource = sourceBits + (sourcePitch * sourceHeight));
        return 1;
      }
    }

    /* Fall through - if not equal it'll be handled below */
    destBits = lockSurfaceFn(destHandle, (&destPitch), dx, dy, bbW, bbH);
    hasSurfaceLock = 1;
    if (numGCsOnInvocation != (statNumGCs())) {
      unlockSurfaces();
      primitiveFailFor(PrimErrObjectMoved);
      return 0;
    }
    if (!destBits) {
      primitiveFailFor(PrimErrCallbackError);
    }
  }

  /* Blitting *to* OS surface */
  if (!((sourceBits != 0) || (noSource))) {
    sourceHandle = fetchIntegerofObject(FormBitsIndex, sourceForm);
    if (failed()) {
      return 0;
    }
    if (!lockSurfaceFn) {
      if (!(loadSurfacePlugin())) {
        return 0;
      }
    }

    /* Warping requiring the entire surface */
    if (isWarping) {
      sourceBits = lockSurfaceFn(sourceHandle, (&sourcePitch), 0, 0,
                                 sourceWidth, sourceHeight);
    } else {
      sourceBits =
          lockSurfaceFn(sourceHandle, (&sourcePitch), sx, sy, bbW, bbH);
    }
    hasSurfaceLock = 1;
    if (numGCsOnInvocation != (statNumGCs())) {
      unlockSurfaces();
      primitiveFailFor(PrimErrObjectMoved);
      return 0;
    }
    if (!sourceBits) {
      primitiveFailFor(PrimErrCallbackError);
    }
  }

  /* Blitting *from* OS surface */
  endOfSource = (noSource || (sourceBits == 0)
                     ? 0
                     : sourceBits + (sourcePitch * sourceHeight));
  endOfDestination = destBits + (destPitch * destHeight);
  return (destBits != 0) && ((sourceBits != 0) || (noSource));
}
