/* Extracted from interp.c:14476 (function primitiveControlVMProfiling). */

static void
primitiveControlVMProfiling(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt bufferSize;
    sqInt numSamples;
    sqInt onOffBar;
    char *sp;

	if (GIV(argumentCount) != 2) {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
		return;
	}
	if (((onOffBar = longAt(GIV(stackPointer) + (1 * BytesPerWord)))) == GIV(trueObj)) {
		onOffBar = 1;
	}
	else {
		if (onOffBar == GIV(falseObj)) {
			onOffBar = 0;
		}
		else {
			/* begin primitiveFail */
			if (!GIV(primFailCode)) {
				GIV(primFailCode) = 1;
			}
			return;
		}
	}
	if (((bufferSize = longAt(GIV(stackPointer)))) == GIV(nilObj)) {
		bufferSize = 0;
	}
	else {
		if (!(((((bufferSize) & 7) == 1))
			 && (((bufferSize = (bufferSize >> 3))) > 0))) {
			/* begin primitiveFail */
			if (!GIV(primFailCode)) {
				GIV(primFailCode) = 1;
			}
			return;
		}
	}
	numSamples = ioControlNewProfile(onOffBar,bufferSize);

	/* begin pop:thenPushInteger: */
	longAtput((sp = GIV(stackPointer) + (2 * BytesPerWord)),(((usqInt)numSamples << 3) | 1));
	GIV(stackPointer) = sp;
}