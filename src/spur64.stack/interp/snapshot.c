/* Extracted from interp.c:63601 (function snapshot). */

static NoDbgRegParms sqInt
snapshot(sqInt embedded)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt activeContext;
    sqInt activeProc;
    usqInt i;
    sqInt methodHeader;
    usqInt numArgs;
    usqInt numTemps;
    sqInt object;
    sqInt objOop;
    sqInt oop;
    sqInt rcvr;
    sqInt rcvrSqInt;
    sqInt savedTenuringThreshold;
    void *setMacType;
    char *sp;
    sqInt stackIndex;


	/* For now the stack munging below doesn't deal with more than one argument.
	   It can, and should. */
	rcvr = 0;
	if (GIV(argumentCount)) {
		return (GIV(primFailCode) = PrimErrBadNumArgs);
	}

	/* Need to convert all frames into contexts since the snapshot file only holds objects. */

	/* begin push: */
	longAtput((sp = GIV(stackPointer) - BytesPerWord),GIV(instructionPointer));
	GIV(stackPointer) = sp;
	activeContext = voidVMStateForSnapshotFlushingExternalPrimitivesIf(1);

	/* update state of active process */

	/* begin activeProcess */
	objOop = longAt((void *)(((longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(SchedulerAssociation) << (shiftForWord()))))))) + BaseHeaderSize) + ((((usqInt)(ValueIndex) << (shiftForWord()))))));
	activeProc = longAt((void *)((objOop + BaseHeaderSize) + ((((usqInt)(ActiveProcessIndex) << (shiftForWord()))))));

	/* begin storePointer:ofObject:withValue: */
	assert(validStorePointerArgs(SuspendedContextIndex, activeProc, activeContext));
	assert(isNonImmediate(activeProc));
	if (oopisGreaterThanOrEqualTo(activeProc, GIV(oldSpaceStart))) {
		if (/* isYoung: */
			((!(activeContext & (tagMask()))))
		 && (oopisLessThan(activeContext, GIV(oldSpaceStart)))) {
			/* begin possibleRootStoreInto: */
			if (!((byteAt((void *)(activeProc + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
				remember(activeProc);
			}
		}
	}

	/* most stores into young objects */
	longAtput((void *)((activeProc + BaseHeaderSize) + ((((usqInt)(SuspendedContextIndex) << (shiftForWord()))))),activeContext);
	GIV(tempOop) = activeContext;

	/* begin garbageCollectForSnapshot */
	/* begin flushNewSpace */
	savedTenuringThreshold = GIV(tenureThreshold);
	GIV(tenureThreshold) = GIV(oldSpaceStart);
	scavengingGCTenuringIf(TenureByAge);

	/* begin setRawTenuringThreshold: */
	GIV(tenureThreshold) = savedTenuringThreshold;
	assert((GIV(rememberedSetSize)) == 0);
	assert(GIV(pastSpaceStart) == (((GIV(pastSpace)).start)));
	assert(GIV(freeStart) == (((GIV(eden)).start)));

	/* begin biasForSnapshot */
	GIV(biasForGC) = 0;
	fullGC();

	/* begin biasForGC */
	GIV(biasForGC) = 1;
	unfireQueuedEphemeronsForSnapshot();
	prepareForSnapshot();

	/* begin checkFreeSpace: */
	assert(bitsSetInFreeSpaceMaskForAllFreeLists());
	assert(GIV(totalFreeOldSpace) == (totalFreeListBytes()));
	if (((checkForLeaks & (GCCheckFreeSpace | GCModeFull)) == (GCCheckFreeSpace | GCModeFull))) {
		runLeakCheckerForFreeSpaceignoring(GCCheckFreeSpace, null);
	}

	/* Nothing moves from here on so it is safe to grab the activeContext again. */
	activeContext = GIV(tempOop);
	GIV(tempOop) = 0;
	if (!GIV(primFailCode)) {
		/* begin quickFetchInteger:ofObject: */
		oop = longAt((void *)((activeContext + BaseHeaderSize) + ((((usqInt)(StackPointerIndex) << (shiftForWord()))))));
		assert((((oop) & 7) == 1));
		stackIndex = (oop >> 3);
		rcvr = longAt((void *)((activeContext + BaseHeaderSize) + ((((usqInt)(((stackIndex + CtxtTempFrameStart) - 1)) << (shiftForWord()))))));

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(activeContext))
		 && (!(isForwarded(activeContext))));
		assert(validStorePointerUncheckedArgs((stackIndex + CtxtTempFrameStart) - 1, activeContext, GIV(trueObj)));
		longAtput((void *)((activeContext + BaseHeaderSize) + ((((usqInt)(((stackIndex + CtxtTempFrameStart) - 1)) << (shiftForWord()))))),GIV(trueObj));

		/* now attempt to write the snapshot file */
		writeImageFileIO();
		if ((!GIV(primFailCode))
		 && (!embedded)) {
			setMacType = ioLoadFunctionFrom("setMacFileTypeAndCreator", "FilePlugin");
			if (setMacType) {
				((sqInt (*)(char *, char *, char *))setMacType)(imageName, "STim", "FAST");
			}
		}

		/* set Mac file type and creator; this is a noop on other platforms
		   Without contexts or stacks simulate
		   self pop: 1 */

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(activeContext))
		 && (!(isForwarded(activeContext))));
		assert(validStorePointerUncheckedArgs(StackPointerIndex, activeContext, (((usqInt)(stackIndex - 1) << 3) | 1)));
		longAtput((void *)((activeContext + BaseHeaderSize) + ((((usqInt)(StackPointerIndex) << (shiftForWord()))))),(((usqInt)(stackIndex - 1) << 3) | 1));
	}

	/* Without contexts or stacks simulate
	   rcvr := self popStack. ''pop rcvr''
	   self push: trueObj.
	   to arrange that the snapshot resumes with true.  N.B. stackIndex is one-relative. */

	/* begin postSnapshot */
	restorePostSnapshot();
	refireQueuedEphemeronsPostSnapshot();
	marryContextInNewStackPageAndInitializeInterpreterRegisters(activeContext);
	if (GIV(primFailCode)) {
		/* begin push: */
		longAtput((sp = GIV(stackPointer) - BytesPerWord),rcvr);
		GIV(stackPointer) = sp;

		/* begin justActivateNewMethod: */
		/* begin methodHeaderOf: */
		assert(isCompiledMethod(GIV(newMethod)));
		methodHeader = longAt((void *)((GIV(newMethod) + BaseHeaderSize) + ((((usqInt)(HeaderIndex) << (shiftForWord()))))));
		numTemps = (((usqInt)(methodHeader)) >> MethodHeaderTempCountShift) & 0x3F;
		numArgs = (((usqInt)(methodHeader)) >> MethodHeaderArgCountShift) & 15;

		/* could new rcvr be set at point of send? */
		rcvrSqInt = longAt(GIV(stackPointer) + (numArgs * BytesPerWord));
		assert(!(isOopForwarded(rcvrSqInt)));

		/* begin push: */
		longAtput((sp = GIV(stackPointer) - BytesPerWord),GIV(instructionPointer));
		GIV(stackPointer) = sp;

		/* begin push: */
		longAtput((sp = GIV(stackPointer) - BytesPerWord),((usqInt)GIV(framePointer)));
		GIV(stackPointer) = sp;
		GIV(framePointer) = GIV(stackPointer);

		/* begin push: */
		longAtput((sp = GIV(stackPointer) - BytesPerWord),GIV(newMethod));
		GIV(stackPointer) = sp;

		/* begin setMethod:methodHeader: */
		GIV(method) = GIV(newMethod);
		assert(isOopCompiledMethod(GIV(method)));
		assert((methodHeaderOf(GIV(method))) == methodHeader);
		GIV(bytecodeSetSelector) = ((((sqLong) methodHeader)) < 0
					? 0x100
					: 0);
		object = /* encodeFrameFieldHasContext:isBlock:numArgs: */
				(VMBIGENDIAN
					? ((1 + ((numArgs << ((BytesPerWord * 8) - 8)))))
					: ((1 + ((numArgs << 8)))));

		/* begin push: */
		longAtput((sp = GIV(stackPointer) - BytesPerWord),object);
		GIV(stackPointer) = sp;

		/* begin push: */
		longAtput((sp = GIV(stackPointer) - BytesPerWord),GIV(nilObj));
		GIV(stackPointer) = sp;

		/* begin push: */
		longAtput((sp = GIV(stackPointer) - BytesPerWord),rcvrSqInt);
		GIV(stackPointer) = sp;

		/* clear remaining temps to nil */
		for (i = (numArgs + 1); i <= numTemps; i += 1) {
			/* begin push: */
			longAtput((sp = GIV(stackPointer) - BytesPerWord),GIV(nilObj));
			GIV(stackPointer) = sp;
		}
		GIV(instructionPointer) = (((((usqInt)(pointerForOop(GIV(newMethod))))) + ((LiteralStart + ((/* begin literalCountOfMethodHeader: */
	assert((((methodHeader) & 7) == 1)),
/* literalCountOfAlternateHeader: */
	((methodHeader >> 3)) & AlternateHeaderNumLiteralsMask))) * BytesPerOop)) + BaseHeaderSize) - 1;
		if (((methodHeader & AlternateHeaderHasPrimFlag) != 0)) {
			GIV(instructionPointer) += 3 /* sizeOfCallPrimitiveBytecode: */;
			if (GIV(primFailCode)) {
				reapAndResetErrorCodeToheader(GIV(stackPointer), methodHeader);
			}
		}

		/* Skip the CallPrimitive bytecode, if it's there, and store the error code if the method starts
		   with a long store temp.  Strictly no need to skip the store because it's effectively a noop. */
	}
	else {
		/* begin push: */
		longAtput((sp = GIV(stackPointer) - BytesPerWord),GIV(falseObj));
		GIV(stackPointer) = sp;
	}
	return 0;
}