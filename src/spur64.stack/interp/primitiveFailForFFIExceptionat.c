/* Extracted from interp.c:15252 (function primitiveFailForFFIExceptionat). */

/*	Set PrimErrFFIException primitive failure and associated exceptionCode
	(a.k.a. 
	secondaryErrorCode) and exceptionPC. Under control of the
	ffiExceptionResponse flag,
	if in a primitive with an error code and ffiCalloutVMHandle indicates
	we're in an FFI call,
	then fail the primitive.
	ffiExceptionResponse < 0 never fail
	ffiExceptionResponse = 0 fail if method has a primitive error code
	(default) ffiExceptionResponse > 0 always fail */

	/* InterpreterPrimitives>>#primitiveFailForFFIException:at: */

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

	if ((!ffiCalloutVMHandle)
	 || (ffiExceptionResponse < 0)) {
		return 0;
	}

	/* i.e. not in an FFI call
	   i.e. never fail */
	secondaryErrorCode = ((sqLong) exceptionCode);
	exceptionPC = pc;
	primFailCode = PrimErrFFIException;
	if ((ffiExceptionResponse > 0)
	 || ((/* isOopCompiledMethod: */
		((!(newMethod & (tagMask()))))
	 && (((byteAt((void *)(newMethod + (formatFieldByteOffset())))) & (formatMask())) >= (firstCompiledMethodFormat())))
	 && (methodUsesPrimitiveErrorCode(newMethod)))) {
		ownVM(ffiCalloutVMHandle);

		/* begin activateFailingPrimitiveMethod */
		assert(primFailCode != 0);
		assert(addressCouldBeObj(newMethod));
		assert(isCompiledMethod(newMethod));
		assert((primitiveIndexOf(newMethod)) != 0);

		/* begin justActivateNewMethod: */
		/* begin methodHeaderOf: */
		assert(isCompiledMethod(newMethod));
		methodHeader = longAt((void *)((newMethod + BaseHeaderSize) + ((((usqInt)(HeaderIndex) << (shiftForWord()))))));
		numTemps = (((usqInt)(methodHeader)) >> MethodHeaderTempCountShift) & 0x3F;
		numArgs = (((usqInt)(methodHeader)) >> MethodHeaderArgCountShift) & 15;

		/* could new rcvr be set at point of send? */
		rcvr = longAt(stackPointer + (numArgs * BytesPerWord));
		assert(!(isOopForwarded(rcvr)));

		/* begin push: */
		longAtput((sp = stackPointer - BytesPerWord),instructionPointer);
		stackPointer = sp;

		/* begin push: */
		longAtput((sp = stackPointer - BytesPerWord),((usqInt)framePointer));
		stackPointer = sp;
		framePointer = stackPointer;

		/* begin push: */
		longAtput((sp = stackPointer - BytesPerWord),newMethod);
		stackPointer = sp;

		/* begin setMethod:methodHeader: */
		method = newMethod;
		assert(isOopCompiledMethod(method));
		assert((methodHeaderOf(method)) == methodHeader);
		bytecodeSetSelector = ((((sqLong) methodHeader)) < 0
					? 0x100
					: 0);
		object = /* encodeFrameFieldHasContext:isBlock:numArgs: */
				(VMBIGENDIAN
					? ((1 + ((numArgs << ((BytesPerWord * 8) - 8)))))
					: ((1 + ((numArgs << 8)))));

		/* begin push: */
		longAtput((sp = stackPointer - BytesPerWord),object);
		stackPointer = sp;

		/* begin push: */
		longAtput((sp = stackPointer - BytesPerWord),nilObj);
		stackPointer = sp;

		/* begin push: */
		longAtput((sp = stackPointer - BytesPerWord),rcvr);
		stackPointer = sp;

		/* clear remaining temps to nil */
		for (i = (numArgs + 1); i <= numTemps; i += 1) {
			/* begin push: */
			longAtput((sp = stackPointer - BytesPerWord),nilObj);
			stackPointer = sp;
		}
		instructionPointer = (((((usqInt)(pointerForOop(newMethod)))) + ((LiteralStart + ((/* begin literalCountOfMethodHeader: */
	assert((((methodHeader) & 7) == 1)),
/* literalCountOfAlternateHeader: */
	((methodHeader >> 3)) & AlternateHeaderNumLiteralsMask))) * BytesPerOop)) + BaseHeaderSize) - 1;
		if (((methodHeader & AlternateHeaderHasPrimFlag) != 0)) {
			instructionPointer += 3 /* sizeOfCallPrimitiveBytecode: */;
			if (primFailCode) {
				reapAndResetErrorCodeToheader(stackPointer, methodHeader);
			}
		}

		/* Skip the CallPrimitive bytecode, if it's there, and store the error code if the method starts
		   with a long store temp.  Strictly no need to skip the store because it's effectively a noop. */
		_longjmp(reenterInterpreter, ReturnToInterpreter);

		/* NOTREACHED */
	}
	return 0;
}