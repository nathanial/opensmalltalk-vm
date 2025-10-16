/* Extracted from interp.c:65439 (function updateDisplayLeftTopRightBottom). */

static NoDbgRegParms sqInt
updateDisplayLeftTopRightBottom(sqInt l, sqInt t, sqInt r, sqInt b)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt bottom;
    sqInt left;
    sqInt right;
    sqInt surfaceHandle;
    sqInt top;

	left = ((l < 0) ? 0 : l);
	right = ((r < displayWidth) ? r : displayWidth);
	top = ((t < 0) ? 0 : t);
	bottom = ((b < displayHeight) ? b : displayHeight);
	if (!((left <= right)
		 && (top <= bottom))) {
		return null;
	}
	if (((((((sqInt)displayBits))) & 7) == 1)) {
		surfaceHandle = ((((sqInt)displayBits)) >> 3);
		if (!showSurfaceFn) {
			showSurfaceFn = ioLoadFunctionFrom("ioShowSurface", "SurfacePlugin");
			if (!showSurfaceFn) {
				return (GIV(primFailCode) = PrimErrNotFound);
			}
		}
		showSurfaceFn(surfaceHandle, left, top, right - left, bottom - top);
	}
	else {
		assert(isNonImmediate(((sqInt)displayBits)));
		ioShowDisplay(((sqInt)displayBits), displayWidth, displayHeight, displayDepth, left, right, top, bottom);
	}
	return 0;
}