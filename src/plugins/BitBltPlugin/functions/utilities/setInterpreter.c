/* Extracted from BitBltPlugin.c:5776 (function setInterpreter). */
/* InterpreterPlugin>>#setInterpreter: */
/*	WARNING: For WarpBlt w/ smoothing the source depth is wrong here! */

EXPORT(sqInt)
setInterpreter(struct VirtualMachine *anInterpreter) {
  sqInt ok;

  interpreterProxy = anInterpreter;

  /* This may seem tautological, but in a real plugin it checks that the VM
     provides the version the plugin was compiled against which is the version
     the plugin expects. */
  ok = ((interpreterProxy->majorVersion()) == (VM_PROXY_MAJOR)) &&
       ((interpreterProxy->minorVersion()) >= (VM_PROXY_MINOR));
  if (ok) {

#if !defined(SQUEAK_BUILTIN_PLUGIN)
    byteSizeOf = interpreterProxy->byteSizeOf;
    failed = interpreterProxy->failed;
    fetchIntegerofObject = interpreterProxy->fetchIntegerofObject;
#if VM_PROXY_MAJOR > 1 || (VM_PROXY_MAJOR == 1 && VM_PROXY_MINOR >= 6)
    fetchLong32ofObject = interpreterProxy->fetchLong32ofObject;
#else
#if !defined(fetchLong32ofObject)
    fetchLong32ofObject = 0;
#endif
#endif
    fetchPointerofObject = interpreterProxy->fetchPointerofObject;
    firstIndexableField = interpreterProxy->firstIndexableField;
    floatValueOf = interpreterProxy->floatValueOf;
#if !defined(integerObjectOf)
    integerObjectOf = interpreterProxy->integerObjectOf;
#endif
#if !defined(integerValueOf)
    integerValueOf = interpreterProxy->integerValueOf;
#endif
    ioLoadFunctionFrom = interpreterProxy->ioLoadFunctionFrom;
    isArray = interpreterProxy->isArray;
    isBytes = interpreterProxy->isBytes;
#if !defined(isIntegerObject)
    isIntegerObject = interpreterProxy->isIntegerObject;
#endif
    isPointers = interpreterProxy->isPointers;
#if VM_PROXY_MAJOR > 1 || (VM_PROXY_MAJOR == 1 && VM_PROXY_MINOR >= 15)
    isPositiveMachineIntegerObject =
        interpreterProxy->isPositiveMachineIntegerObject;
#else
#if !defined(isPositiveMachineIntegerObject)
    isPositiveMachineIntegerObject = 0;
#endif
#endif
    isWords = interpreterProxy->isWords;
    isWordsOrBytes = interpreterProxy->isWordsOrBytes;
    methodArgumentCount = interpreterProxy->methodArgumentCount;
    methodReturnInteger = interpreterProxy->methodReturnInteger;
    methodReturnReceiver = interpreterProxy->methodReturnReceiver;
    methodReturnValue = interpreterProxy->methodReturnValue;
    nilObject = interpreterProxy->nilObject;
    pop = interpreterProxy->pop;
    positive32BitIntegerFor = interpreterProxy->positive32BitIntegerFor;
    positive32BitValueOf = interpreterProxy->positive32BitValueOf;
    positive64BitValueOf = interpreterProxy->positive64BitValueOf;
    primitiveFail = interpreterProxy->primitiveFail;
    primitiveFailFor = interpreterProxy->primitiveFailFor;
    showDisplayBitsLeftTopRightBottom =
        interpreterProxy->showDisplayBitsLeftTopRightBottom;
    slotSizeOf = interpreterProxy->slotSizeOf;
    stackIntegerValue = interpreterProxy->stackIntegerValue;
    stackObjectValue = interpreterProxy->stackObjectValue;
    stackValue = interpreterProxy->stackValue;
#if VM_PROXY_MAJOR > 1 || (VM_PROXY_MAJOR == 1 && VM_PROXY_MINOR >= 14)
    statNumGCs = interpreterProxy->statNumGCs;
#else
#if !defined(statNumGCs)
    statNumGCs = 0;
#endif
#endif
    storeIntegerofObjectwithValue =
        interpreterProxy->storeIntegerofObjectwithValue;
#if VM_PROXY_MAJOR > 1 || (VM_PROXY_MAJOR == 1 && VM_PROXY_MINOR >= 6)
    storeLong32ofObjectwithValue =
        interpreterProxy->storeLong32ofObjectwithValue;
#else
#if !defined(storeLong32ofObjectwithValue)
    storeLong32ofObjectwithValue = 0;
#endif
#endif
#endif /* !defined(SQUEAK_BUILTIN_PLUGIN) */
  }
  return ok;
}
