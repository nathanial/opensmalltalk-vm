/* Extracted from interp.c:15252 (function primitiveFailForFFIExceptionat). */

sqInt
primitiveFailForFFIExceptionat(usqLong exceptionCode, usqInt pc)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    usqInt i;
    sqInt methodHeader;
    usqInt numArgs;
    usqInt numTemps;
    sqInt object;
    sqInt rcvr;
    char *sp;

	if ((!GIV(ffiCalloutVMHandle))
	 || (ffiExceptionResponse < 0)) {
		return 0;
	}

	/* i.e. not in an FFI call
	   i.e. never fail */
	GIV(secondaryErrorCode) = ((sqLong) exceptionCode);
	GIV(exceptionPC) = pc;
	GIV(primFailCode) = PrimErrFFIException;
	if ((ffiExceptionResponse > 0)
	 || ((/* isOopCompiledMethod: */
		((!(GIV(newMethod) & (tagMask()))))
	 && (((byteAt((void *)(GIV(newMethod) + (formatFieldByteOffset())))) & (formatMask())) >= (firstCompiledMethodFormat())))
	 && (methodUsesPrimitiveErrorCode(GIV(newMethod))))) {
		ownVM(GIV(ffiCalloutVMHandle));

		/* begin activateFailingPrimitiveMethod */
		assert(GIV(primFailCode) != 0);
		assert(addressCouldBeObj(GIV(newMethod)));
		assert(isCompiledMethod(GIV(newMethod)));
		assert((primitiveIndexOf(GIV(newMethod))) != 0);

		/* begin justActivateNewMethod: */
		/* begin methodHeaderOf: */
		assert(isCompiledMethod(GIV(newMethod)));
		methodHeader = longAt((void *)((GIV(newMethod) + BaseHeaderSize) + ((((usqInt)(HeaderIndex) << (shiftForWord()))))));
		numTemps = (((usqInt)(methodHeader)) >> MethodHeaderTempCountShift) & 0x3F;
		numArgs = (((usqInt)(methodHeader)) >> MethodHeaderArgCountShift) & 15;

		/* could new rcvr be set at point of send? */
		rcvr = longAt(GIV(stackPointer) + (numArgs * BytesPerWord));
		assert(!(isOopForwarded(rcvr)));

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
		longAtput((sp = GIV(stackPointer) - BytesPerWord),rcvr);
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
		_longjmp(GIV(reenterInterpreter), ReturnToInterpreter);

		/* NOTREACHED */
	}
	return 0;
}