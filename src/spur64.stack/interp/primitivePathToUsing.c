/* Extracted from interp.c:20095 (function primitivePathToUsing). */

EXPORT(sqInt)
primitivePathToUsing(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt contextSize;
    sqInt current;
    sqInt err;
    sqInt fmt;
    usqInt freeStartAtStart;
    sqInt header;
    sqInt i;
    sqInt index;
    sqInt iSqInt;
    sqInt next;
    usqInt numLiterals;
    usqInt numSlots;
    usqInt numSlotsUsqInt;
    sqInt path;
    char *sp;
    sqInt spSqInt;
    sqInt stack;
    sqInt stackp;
    sqInt stackSize;
    sqInt toDoLimit;
    sqInt toDoLimitSqInt;

	err = 0;

	/* begin externalWriteBackHeadFramePointers */
	assert((GIV(framePointer) - GIV(stackPointer)) < (LargeContextSlots * BytesPerOop));
	assert(GIV(stackPage) == (GIV(mostRecentlyUsedPage)));
	assert(!((isFree(GIV(stackPage)))));

	/* begin setHeadFP:andSP:inPage: */
	assert(GIV(stackPointer) < GIV(framePointer));
	assert((GIV(stackPointer) < ((GIV(stackPage)->baseAddress)))
	 && (GIV(stackPointer) > (((GIV(stackPage)->realStackLimit)) - (LargeContextSlots * BytesPerOop))));
	assert((GIV(framePointer) < ((GIV(stackPage)->baseAddress)))
	 && (GIV(framePointer) > (((GIV(stackPage)->realStackLimit)) - ((LargeContextSlots * BytesPerOop) / 2))));
	(GIV(stackPage)->headFP = GIV(framePointer));
	(GIV(stackPage)->headSP = GIV(stackPointer));
	assert(pageListIsWellFormed());
	if (!(GIV(argumentCount) >= 2)) {
		return (GIV(primFailCode) = PrimErrBadNumArgs);
	}
	if (!(((longAt(GIV(stackPointer))) == GIV(trueObj))
		 || ((longAt(GIV(stackPointer))) == GIV(falseObj)))) {
		return (GIV(primFailCode) = PrimErrBadArgument);
	}
	stack = longAt(GIV(stackPointer) + (1 * BytesPerWord));

	/* begin pathTo:using:followWeak: */
	if (!(/* isArray: */
			((!(stack & (tagMask()))))
		 && (((byteAt((void *)(stack + (formatFieldByteOffset())))) & (formatMask())) == (arrayFormat())))) {
		err = PrimErrBadArgument;
		goto l5;
	}
	assert(allObjectsUnmarked());

	/* check no allocations during search */
	freeStartAtStart = GIV(freeStart);
	beRootIfOld(stack);

	/* begin lengthOf: */
	fmt = (byteAt((void *)(stack + (formatFieldByteOffset())))) & (formatMask());
	numSlotsUsqInt = byteAt((void *)(stack + (numSlotsFieldByteOffset())));
	numSlots = (numSlotsUsqInt == (numSlotsMask())
				? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(stack - BaseHeaderSize)))) << 8)))))) >> 8
				: numSlotsUsqInt);
	if (fmt <= (ephemeronFormat())) {
		stackSize = numSlots;
		goto l1;
	}
	if (fmt >= (firstByteFormat())) {
		stackSize = ((numSlots << (shiftForWord()))) - (fmt & 7);
		goto l1;
	}

	/* bytes, including CompiledMethod */
	if (fmt >= (firstShortFormat())) {
		stackSize = ((numSlots << ((shiftForWord()) - 1))) - (fmt & 3);
		goto l1;
	}
	if (fmt >= (firstLongFormat())) {
		stackSize = ((numSlots << ((shiftForWord()) - 2))) - (fmt & 1);
		goto l1;
	}
	if (fmt == (sixtyFourBitIndexableFormat())) {
		stackSize = numSlots;
		goto l1;
	}

	/* fmt = self forwardedFormat */
	stackSize = 0;
	/* end lengthOf: */
l1:

	/* begin mark: */
	/* begin setIsMarkedOf:to: */
	assert(!(isFreeObject(stack)));
	byteAtput((void *)(stack + (markBitsByteOffset())),(byteAt((void *)(stack + (markBitsByteOffset())))) | (1U << (markedBitByteShift())));

	/* no need. the current context is not reachable from the active process (suspendedContext is nil)
	   objectMemory mark: self activeProcess. */
	current = GIV(specialObjectsOop);

	/* begin mark: */
	/* begin setIsMarkedOf:to: */
	assert(!(isFreeObject(current)));
	byteAtput((void *)(current + (markBitsByteOffset())),(byteAt((void *)(current + (markBitsByteOffset())))) | (1U << (markedBitByteShift())));

	/* begin lengthOf: */
	fmt = (byteAt((void *)(current + (formatFieldByteOffset())))) & (formatMask());
	numSlotsUsqInt = byteAt((void *)(current + (numSlotsFieldByteOffset())));
	numSlots = (numSlotsUsqInt == (numSlotsMask())
				? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(current - BaseHeaderSize)))) << 8)))))) >> 8
				: numSlotsUsqInt);
	if (fmt <= (ephemeronFormat())) {
		index = numSlots;
		goto l2;
	}
	if (fmt >= (firstByteFormat())) {
		index = ((numSlots << (shiftForWord()))) - (fmt & 7);
		goto l2;
	}

	/* bytes, including CompiledMethod */
	if (fmt >= (firstShortFormat())) {
		index = ((numSlots << ((shiftForWord()) - 1))) - (fmt & 3);
		goto l2;
	}
	if (fmt >= (firstLongFormat())) {
		index = ((numSlots << ((shiftForWord()) - 2))) - (fmt & 1);
		goto l2;
	}
	if (fmt == (sixtyFourBitIndexableFormat())) {
		index = numSlots;
		goto l2;
	}

	/* fmt = self forwardedFormat */
	index = 0;
	/* end lengthOf: */
l2:
	stackp = 0;
	while (1) {
		while (((index -= 1)) >= -1) {
			if (/* couldBeFramePointer: */
				(((((usqInt)current)) & (BytesPerWord - 1)) == 0)
			 && ((((((usqInt)current)) >= (((usqInt)GIV(stackMemory)))) && ((((usqInt)current)) <= (((usqInt)GIV(pages))))))) {
				next = (index >= 0
							? fieldofFrame(index, ((char *) current))
							: GIV(nilObj));
			}
			else {
				if (index >= 0) {
					next = (((longAt((void *)(current))) & (classIndexMask())) == ClassMethodContextCompactIndex
								? fieldOrSenderFPofContext(index, current)
								: longAt((void *)((current + BaseHeaderSize) + ((((usqInt)(index) << (shiftForWord())))))));
				}
				else {
					next = fetchClassOfNonImm(current);
				}
			}
			if (/* couldBeFramePointer: */
				(((((usqInt)next)) & (BytesPerWord - 1)) == 0)
			 && ((((((usqInt)next)) >= (((usqInt)GIV(stackMemory)))) && ((((usqInt)next)) <= (((usqInt)GIV(pages))))))) {
				assert(isFrameonPage(((char *) next), stackPageFor(((char *) next))));
			}
			else {
				assert(checkOkayOop(next));
			}
			if (next == (longAt(GIV(stackPointer) + (2 * BytesPerWord)))) {
				assert(freeStartAtStart == (GIV(freeStart)));
				unmarkAfterPathTo();

				/* begin storePointer:ofObject:withValue: */
				assert(validStorePointerArgs(stackp, stack, current));
				assert(isNonImmediate(stack));
				if (oopisGreaterThanOrEqualTo(stack, GIV(oldSpaceStart))) {
					if (/* isYoung: */
						((!(current & (tagMask()))))
					 && (oopisLessThan(current, GIV(oldSpaceStart)))) {
						/* begin possibleRootStoreInto: */
						if (!((byteAt((void *)(stack + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
							remember(stack);
						}
					}
				}

				/* most stores into young objects */
				longAtput((void *)((stack + BaseHeaderSize) + ((((usqInt)(stackp) << (shiftForWord()))))),current);
				pruneStackstackp(stack, stackp);
				err = 0;
				goto l5;
			}
			if (((!(next & (smallIntegerTag()))))
			 && ((/* couldBeFramePointer: */
				(((((usqInt)next)) & (BytesPerWord - 1)) == 0)
			 && ((((((usqInt)next)) >= (((usqInt)GIV(stackMemory)))) && ((((usqInt)next)) <= (((usqInt)GIV(pages))))))
					? !(((longAt((void *)(next + FoxFrameFlags))) & 2) != 0)
					: (!(((byteAt((void *)(next + (markBitsByteOffset())))) & (1U << (markedBitByteShift()))) != 0))
					 && (((/* isPointers: */
						((!(next & (tagMask()))))
					 && (((byteAt((void *)(next + (formatFieldByteOffset())))) & (formatMask())) <= 5 /* lastPointerFormat */))
					 || (((byteAt((void *)(next + (formatFieldByteOffset())))) & (formatMask())) >= (firstCompiledMethodFormat())))
					 && (((longAt(GIV(stackPointer))) == GIV(trueObj))
					 || (!(isWeakNonImm(next)))))))) {
				if ((stackp + 2) > stackSize) {
					assert(freeStartAtStart == (GIV(freeStart)));
					unmarkAfterPathTo();

					/* begin nilFieldsOf: */
					toDoLimit = ((assert((classIndexOf(stack)) > (isForwardedObjectClassIndexPun())),
(((numSlots = byteAt((void *)(stack + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
			? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(stack - BaseHeaderSize)))) << 8)))))) >> 8
			: numSlots))) - 1;
					for (i = 0; i <= toDoLimit; i += 1) {
						/* begin storePointerUnchecked:ofObject:withValue: */
						assert((isNonImmediate(stack))
						 && (!(isForwarded(stack))));
						assert(validStorePointerUncheckedArgs(i, stack, GIV(nilObj)));
						longAtput((void *)((stack + BaseHeaderSize) + ((((usqInt)(i) << (shiftForWord()))))),GIV(nilObj));
					}
					err = PrimErrBadIndex;
					goto l5;
				}

				/* begin storePointerUnchecked:ofObject:withValue: */
				assert((isNonImmediate(stack))
				 && (!(isForwarded(stack))));
				assert(validStorePointerUncheckedArgs(stackp, stack, current));
				longAtput((void *)((stack + BaseHeaderSize) + ((((usqInt)(stackp) << (shiftForWord()))))),current);

				/* begin storePointerUnchecked:ofObject:withValue: */
				assert((isNonImmediate(stack))
				 && (!(isForwarded(stack))));
				assert(validStorePointerUncheckedArgs(stackp + 1, stack, (((usqInt)index << 3) | 1)));
				longAtput((void *)((stack + BaseHeaderSize) + ((((usqInt)((stackp + 1)) << (shiftForWord()))))),(((usqInt)index << 3) | 1));
				stackp += 2;
				if (/* couldBeFramePointer: */
					(((((usqInt)(((char *) next)))) & (BytesPerWord - 1)) == 0)
				 && ((((((usqInt)(((char *) next)))) >= (((usqInt)GIV(stackMemory)))) && ((((usqInt)(((char *) next)))) <= (((usqInt)GIV(pages))))))) {
					/* begin markFrame: */
					longAtput((void *)(next + FoxFrameFlags),(longAt((void *)(next + FoxFrameFlags))) | 2);
					index = CtxtTempFrameStart + (stackPointerIndexForFrame(((char *) next)));
				}
				else {
					/* begin mark: */
					/* begin setIsMarkedOf:to: */
					assert(!(isFreeObject(next)));
					byteAtput((void *)(next + (markBitsByteOffset())),(byteAt((void *)(next + (markBitsByteOffset())))) | (1U << (markedBitByteShift())));

					/* begin numPointerSlotsOf: */
					fmt = (byteAt((void *)(next + (formatFieldByteOffset())))) & (formatMask());
					if (fmt <= 5 /* lastPointerFormat */) {
						if ((fmt == (indexablePointersFormat()))
						 && (((longAt((void *)(next))) & (classIndexMask())) == ClassMethodContextCompactIndex)) {
							/* begin fetchStackPointerOf: */
							spSqInt = longAt((void *)((next + BaseHeaderSize) + ((((usqInt)(StackPointerIndex) << (shiftForWord()))))));
							if (!((((spSqInt) & 7) == 1))) {
								contextSize = 0;
								goto l3;
							}
							assert((ReceiverIndex + ((spSqInt >> 3))) < (lengthOf(next)));
							contextSize = (spSqInt >> 3);
							/* end fetchStackPointerOf: */
l3:
							index = ((usqInt) (CtxtTempFrameStart + contextSize));
							goto l4;
						}

						/* contexts end at the stack pointer */

						/* begin numSlotsOf: */
						assert((classIndexOf(next)) > (isForwardedObjectClassIndexPun()));
						index = (((numSlotsUsqInt = byteAt((void *)(next + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
									? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(next - BaseHeaderSize)))) << 8)))))) >> 8
									: numSlotsUsqInt);
						goto l4;
					}
					if (fmt == (forwardedFormat())) {
						index = 1;
						goto l4;
					}
					if (fmt < (firstCompiledMethodFormat())) {
						index = 0;
						goto l4;
					}

					/* begin methodHeaderOf: */
					assert(isCompiledMethod(next));
					header = longAt((void *)((next + BaseHeaderSize) + ((((usqInt)(HeaderIndex) << (shiftForWord()))))));

					/* begin literalCountOfMethodHeader: */
					assert((((header) & 7) == 1));
					numLiterals = ((header >> 3)) & AlternateHeaderNumLiteralsMask;
					index = numLiterals + LiteralStart;
					/* end numPointerSlotsOf: */
l4:;
				}
				current = next;
			}
		}
		if (current == GIV(specialObjectsOop)) {
			assert(freeStartAtStart == (GIV(freeStart)));
			unmarkAfterPathTo();

			/* begin nilFieldsOf: */
			toDoLimitSqInt = ((assert((classIndexOf(stack)) > (isForwardedObjectClassIndexPun())),
(((numSlotsUsqInt = byteAt((void *)(stack + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
			? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(stack - BaseHeaderSize)))) << 8)))))) >> 8
			: numSlotsUsqInt))) - 1;
			for (iSqInt = 0; iSqInt <= toDoLimitSqInt; iSqInt += 1) {
				/* begin storePointerUnchecked:ofObject:withValue: */
				assert((isNonImmediate(stack))
				 && (!(isForwarded(stack))));
				assert(validStorePointerUncheckedArgs(iSqInt, stack, GIV(nilObj)));
				longAtput((void *)((stack + BaseHeaderSize) + ((((usqInt)(iSqInt) << (shiftForWord()))))),GIV(nilObj));
			}
			err = PrimErrNotFound;
			goto l5;
		}
		index = ((longAt((void *)((stack + BaseHeaderSize) + ((((usqInt)((stackp - 1)) << (shiftForWord()))))))) >> 3);
		current = longAt((void *)((stack + BaseHeaderSize) + ((((usqInt)((stackp - 2)) << (shiftForWord()))))));
		stackp -= 2;
	}
	/* end pathTo:using:followWeak: */
l5:
	if (err) {
		return (GIV(primFailCode) = err);
	}
	path = longAt(GIV(stackPointer) + (1 * BytesPerWord));

	/* begin pop:thenPush: */
	longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),path);
	GIV(stackPointer) = sp;
	return 0;
}