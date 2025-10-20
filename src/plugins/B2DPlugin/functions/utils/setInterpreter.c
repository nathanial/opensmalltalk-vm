/* Extracted from B2DPlugin.c:9635 (function setInterpreter). */

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
    booleanValueOf = interpreterProxy->booleanValueOf;
    byteSizeOf = interpreterProxy->byteSizeOf;
    classBitmap = interpreterProxy->classBitmap;
    classPoint = interpreterProxy->classPoint;
    failed = interpreterProxy->failed;
    fetchClassOf = interpreterProxy->fetchClassOf;
    fetchIntegerofObject = interpreterProxy->fetchIntegerofObject;
    fetchPointerofObject = interpreterProxy->fetchPointerofObject;
    firstIndexableField = interpreterProxy->firstIndexableField;
    floatValueOf = interpreterProxy->floatValueOf;
#if !defined(integerValueOf)
    integerValueOf = interpreterProxy->integerValueOf;
#endif
    ioLoadFunctionFrom = interpreterProxy->ioLoadFunctionFrom;
    ioMicroMSecs = interpreterProxy->ioMicroMSecs;
    isArray = interpreterProxy->isArray;
    isBytes = interpreterProxy->isBytes;
    isFloatObject = interpreterProxy->isFloatObject;
#if !defined(isImmediate)
#if VM_PROXY_MAJOR > 1 || (VM_PROXY_MAJOR == 1 && VM_PROXY_MINOR >= 13)
    isImmediate = interpreterProxy->isImmediate;
#else
#if !defined(isImmediate)
    isImmediate = 0;
#endif
#endif
#endif /* !defined(isImmediate) */
#if !defined(isIntegerObject)
    isIntegerObject = interpreterProxy->isIntegerObject;
#endif
    isPointers = interpreterProxy->isPointers;
    isWords = interpreterProxy->isWords;
    makePointwithxValueyValue = interpreterProxy->makePointwithxValueyValue;
    methodArgumentCount = interpreterProxy->methodArgumentCount;
    methodReturnInteger = interpreterProxy->methodReturnInteger;
    methodReturnReceiver = interpreterProxy->methodReturnReceiver;
    nilObject = interpreterProxy->nilObject;
    pop = interpreterProxy->pop;
    popthenPush = interpreterProxy->popthenPush;
    popRemappableOop = interpreterProxy->popRemappableOop;
    positive32BitIntegerFor = interpreterProxy->positive32BitIntegerFor;
    positive32BitValueOf = interpreterProxy->positive32BitValueOf;
    primitiveFail = interpreterProxy->primitiveFail;
    primitiveFailFor = interpreterProxy->primitiveFailFor;
    pushBool = interpreterProxy->pushBool;
    pushInteger = interpreterProxy->pushInteger;
    pushRemappableOop = interpreterProxy->pushRemappableOop;
    slotSizeOf = interpreterProxy->slotSizeOf;
    stackIntegerValue = interpreterProxy->stackIntegerValue;
    stackObjectValue = interpreterProxy->stackObjectValue;
    stackValue = interpreterProxy->stackValue;
    storeIntegerofObjectwithValue =
        interpreterProxy->storeIntegerofObjectwithValue;
    storePointerofObjectwithValue =
        interpreterProxy->storePointerofObjectwithValue;
    topRemappableOop = interpreterProxy->topRemappableOop;
#endif /* !defined(SQUEAK_BUILTIN_PLUGIN) */
  }
  return ok;
}