/* Extracted from interp.c:17292 (function primitiveGetNextEvent). */

/*	Primitive. Return the next input event from the VM event queue. */

	/* InterpreterPrimitives>>#primitiveGetNextEvent */

static void
primitiveGetNextEvent(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt arg;
    sqIntptr_t eventTypeIs;
    static char * eventTypeNames[] = {
		"None", "Mouse", "Keyboard", "DragDropFiles", "Menu", "Window", "Complex", "MouseWheel",
		"Plugin"
	};
    sqIntptr_t evtBuf[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    sqInt i;
    sqInt integerValue;
    sqIntptr_t value;

	arg = longAt(stackPointer);
	if (!((/* isArray: */
			((!(arg & (tagMask()))))
		 && (((byteAt((void *)(arg + (formatFieldByteOffset())))) & (formatMask())) == (arrayFormat())))
		 && ((slotSizeOf(arg)) == 8))) {
		/* primitiveFailFor: */
		primFailCode = PrimErrBadArgument;
		return;
	}
	ioGetNextEvent(((sqInputEvent*) evtBuf));
	if (primFailCode) {
		return;
	}
	if ((eventTraceMask != 0)
	 && (((eventTraceMask & ((((evtBuf[0]) < 0) ? (((usqInt)(1)) >> (-(evtBuf[0]))) : (1ULL << (evtBuf[0]))))) != 0))) {
		fprintf(stderr,
				"Event%s/%" PRIdSQINT " @ %" PRIuSQINT "\t\t%" PRIdSQINT "/%" PRIxSQINT " %" PRIdSQINT "/%" PRIxSQINT "\n\t%" PRIdSQINT "/%" PRIxSQINT " %" PRIdSQINT "/%" PRIxSQINT "\t %" PRIdSQINT "/%" PRIxSQINT " %p\n",
				((((evtBuf[0]) >= 0) && ((evtBuf[0]) <= 8))
				? eventTypeNames[evtBuf[0]]
				: "?"),
				evtBuf[0],
				evtBuf[1],
				evtBuf[2],
				evtBuf[2],
				evtBuf[3],
				evtBuf[3],
				evtBuf[4],
				evtBuf[4],
				evtBuf[5],
				evtBuf[5],
				evtBuf[6],
				evtBuf[6],
				((void *)(evtBuf[7])));
	}

	/* Event type */
	eventTypeIs = evtBuf[0];
	integerValue = evtBuf[0];

	/* begin storeInteger:ofObject:withValue: */
	if ((((((usqInt)(integerValue)) >> 60) + 1) & 15) <= 1) {
		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(arg))
		 && (!(isForwarded(arg))));
		assert(validStorePointerUncheckedArgs(0, arg, (((usqInt)integerValue << 3) | 1)));
		longAtput((void *)((arg + BaseHeaderSize) + (0U << (shiftForWord()))),(((usqInt)integerValue << 3) | 1));
	}
	else {
		/* begin primitiveFail */
		if (!primFailCode) {
			primFailCode = 1;
		}
	}
	if (primFailCode) {
		return;
	}
	if (eventTypeIs == 6) {
		for (i = 1; i <= 7; i += 1) {
			value = evtBuf[i];

			/* begin storePointer:ofObject:withValue: */
			assert(validStorePointerArgs(i, arg, value));
			assert(isNonImmediate(arg));
			if (oopisGreaterThanOrEqualTo(arg, oldSpaceStart)) {
				if (/* isYoung: */
					((!(value & (tagMask()))))
				 && (oopisLessThan(value, oldSpaceStart))) {
					/* begin possibleRootStoreInto: */
					if (!((byteAt((void *)(arg + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
						remember(arg);
					}
				}
			}

			/* most stores into young objects */
			longAtput((void *)((arg + BaseHeaderSize) + ((((usqInt)(i) << (shiftForWord()))))),value);
		}
	}
	else {
		integerValue = (evtBuf[1]) & MillisecondClockMask;

		/* begin storeInteger:ofObject:withValue: */
		if ((((((usqInt)(integerValue)) >> 60) + 1) & 15) <= 1) {
			/* begin storePointerUnchecked:ofObject:withValue: */
			assert((isNonImmediate(arg))
			 && (!(isForwarded(arg))));
			assert(validStorePointerUncheckedArgs(1, arg, (((usqInt)integerValue << 3) | 1)));
			longAtput((void *)((arg + BaseHeaderSize) + (1U << (shiftForWord()))),(((usqInt)integerValue << 3) | 1));
		}
		else {
			/* begin primitiveFail */
			if (!primFailCode) {
				primFailCode = 1;
			}
		}
		if (primFailCode) {
			return;
		}
		for (i = 2; i <= 7; i += 1) {
			value = evtBuf[i];
			if ((((((usqIntptr_t)(value)) >> 60) + 1) & 15) <= 1) {
				/* begin storeInteger:ofObject:withValue: */
				if ((((((usqIntptr_t)(value)) >> 60) + 1) & 15) <= 1) {
					/* begin storePointerUnchecked:ofObject:withValue: */
					assert((isNonImmediate(arg))
					 && (!(isForwarded(arg))));
					assert(validStorePointerUncheckedArgs(i, arg, (((usqInt)value << 3) | 1)));
					longAtput((void *)((arg + BaseHeaderSize) + ((((usqInt)(i) << (shiftForWord()))))),(((usqInt)value << 3) | 1));
				}
				else {
					/* begin primitiveFail */
					if (!primFailCode) {
						primFailCode = 1;
					}
				}
			}
			else {
				value = positive64BitIntegerFor(value);

				/* begin storePointer:ofObject:withValue: */
				assert(validStorePointerArgs(i, arg, value));
				assert(isNonImmediate(arg));
				if (oopisGreaterThanOrEqualTo(arg, oldSpaceStart)) {
					if (/* isYoung: */
						((!(value & (tagMask()))))
					 && (oopisLessThan(value, oldSpaceStart))) {
						/* begin possibleRootStoreInto: */
						if (!((byteAt((void *)(arg + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
							remember(arg);
						}
					}
				}

				/* most stores into young objects */
				longAtput((void *)((arg + BaseHeaderSize) + ((((usqInt)(i) << (shiftForWord()))))),value);
			}
		}
	}

	/* Event is Complex, assume evtBuf is populated correctly and return
	   Event time stamp */
	if (!primFailCode) {
		/* begin pop: */
		stackPointer += 1 * BytesPerWord;
	}
}