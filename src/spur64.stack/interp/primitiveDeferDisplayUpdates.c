/* Extracted from interp.c:67350 (function primitiveDeferDisplayUpdates). */

static void
primitiveDeferDisplayUpdates(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt flag;

	if (cannotDeferDisplayUpdates) {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
		return;
	}
	flag = longAt(GIV(stackPointer));
	if (flag == GIV(trueObj)) {
		deferDisplayUpdates = 1;
	}
	else {
		if (flag == GIV(falseObj)) {
			deferDisplayUpdates = 0;
		}
		else {
			/* begin primitiveFail */
			if (!GIV(primFailCode)) {
				GIV(primFailCode) = 1;
			}
			return;
		}
	}

	/* begin pop: */
	GIV(stackPointer) += 1 * BytesPerWord;
}