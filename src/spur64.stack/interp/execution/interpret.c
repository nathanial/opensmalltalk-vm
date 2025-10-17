/* Extracted from interp.c:2547 (function interpret). */

/*** Methods ***/

/*	This is the main interpreter loop. It normally loops forever, fetching
   and executing bytecodes. When running in the context of a browser plugin VM,
        however, it must return control to the browser periodically. This should
        done only when the state of the currently running Squeak thread is
   safely stored in the object heap. Since this is the case at the moment that a
        check for interrupts is performed, that is when we return to the browser
        if it is time to do so. Interrupt checks happen quite frequently.
 */
/*	If stacklimit is zero then the stack pages have not been initialized. */

/* StackInterpreter>>#interpret */

sqInt interpret(void) {
  sqInt currentBytecode;
  sqInt extA;
  sqInt extB;
  sqInt lkupClassTag;
  char *localFP;
  char *localIP;
  sqInt localReturnValue;
  char *localSP;
  sqInt methodHeader;
  sqInt numExtB;
  sqInt stackPageBytes;
  usqIntptr_t stackZoneBytes;
  void *theStackMemory;

  if (!stackLimit) {
    /* begin initStackPagesAndInterpret */
    /* begin ensureInitializeStackZone */
    if (!pages) {
      stackPageBytes = stackPageByteSize();
      stackZoneBytes =
          (numStackPages * ((sizeof(CogStackPage)) + (stackPageByteSize()))) +
          BytesPerWord;
      theStackMemory = alloca(stackZoneBytes);
      memset(theStackMemory, 0, stackZoneBytes);
      initializeStacknumSlotspageSize(theStackMemory,
                                      stackZoneBytes / BytesPerWord,
                                      stackPageBytes / BytesPerWord);
    }

    /* Once the stack pages are initialized we can continue to bootstrap the
     * system. */
    loadInitialContext();

    /* We're ready for the heartbeat (poll interrupt) */
    ioInitHeartbeat();
    initialEnterSmalltalkExecutive();
    return null;
  }

  /* record entry time when running as a browser plug-in */
  browserPluginInitialiseIfNeeded();

  /* begin setMethod: */
  method = longAt(framePointer + FoxMethod);
  assert(isOopCompiledMethod(method));

  /* begin methodUsesAlternateBytecodeSet: */
  /* begin methodHeaderOf: */
  assert(isCompiledMethod(method));
  methodHeader = fetchPointerofObject(HeaderIndex, method);
  if ((((sqLong)methodHeader)) < 0) {
    bytecodeSetSelector = 0x100;
  } else {
    bytecodeSetSelector = 0;
  }
  assertValidExecutionPointersimbarline(instructionPointer, framePointer,
                                        stackPointer, 1, __LINE__);

  /* begin internalizeIPandSP */
  localIP = ((char *)instructionPointer);
  localSP = stackPointer;
  localFP = framePointer;

  /* begin initExtensions */
  extA = (numExtB = (extB = 0));
  currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
  while (1) {
    /* begin dispatchBytecode */
    bytecodeDispatchDebugHook();

    VM_LABEL(bytecodeDispatch);
    switch (currentBytecode) {
    case 0:   // pushReceiverVariableBytecode
    case 256: // 0	pushReceiverVariableBytecode
    {
      VM_LABEL(pushReceiverVariableBytecode);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      longAtput(
          (localSP -= BytesPerOop),
          longAt((void *)((longAt(localFP + FoxReceiver)) + BaseHeaderSize)));
    } break;
    case 1:   // pushReceiverVariableBytecode
    case 257: // 1	pushReceiverVariableBytecode
    {
      VM_LABEL(pushReceiverVariableBytecode1);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      longAtput((localSP -= BytesPerOop),
                fetchPointerofObject(1, longAt(localFP + FoxReceiver)));
    } break;
    case 2:   // pushReceiverVariableBytecode
    case 258: // 2	pushReceiverVariableBytecode
    {
      VM_LABEL(pushReceiverVariableBytecode2);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      longAtput((localSP -= BytesPerOop),
                fetchPointerofObject(2, longAt(localFP + FoxReceiver)));
    } break;
    case 3:   // pushReceiverVariableBytecode
    case 259: // 3	pushReceiverVariableBytecode
    {
      VM_LABEL(pushReceiverVariableBytecode3);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      longAtput((localSP -= BytesPerOop),
                fetchPointerofObject(3, longAt(localFP + FoxReceiver)));
    } break;
    case 4:   // pushReceiverVariableBytecode
    case 260: // 4	pushReceiverVariableBytecode
    {
      VM_LABEL(pushReceiverVariableBytecode4);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      longAtput((localSP -= BytesPerOop),
                fetchPointerofObject(4, longAt(localFP + FoxReceiver)));
    } break;
    case 5:   // pushReceiverVariableBytecode
    case 261: // 5	pushReceiverVariableBytecode
    {
      VM_LABEL(pushReceiverVariableBytecode5);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      longAtput((localSP -= BytesPerOop),
                fetchPointerofObject(5, longAt(localFP + FoxReceiver)));
    } break;
    case 6:   // pushReceiverVariableBytecode
    case 262: // 6	pushReceiverVariableBytecode
    {
      VM_LABEL(pushReceiverVariableBytecode6);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      longAtput((localSP -= BytesPerOop),
                fetchPointerofObject(6, longAt(localFP + FoxReceiver)));
    } break;
    case 7:   // pushReceiverVariableBytecode
    case 263: // 7	pushReceiverVariableBytecode
    {
      VM_LABEL(pushReceiverVariableBytecode7);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      longAtput((localSP -= BytesPerOop),
                fetchPointerofObject(7, longAt(localFP + FoxReceiver)));
    } break;
    case 8:   // pushReceiverVariableBytecode
    case 264: // 8	pushReceiverVariableBytecode
    {
      VM_LABEL(pushReceiverVariableBytecode8);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      longAtput((localSP -= BytesPerOop),
                fetchPointerofObject(8, longAt(localFP + FoxReceiver)));
    } break;
    case 9:   // pushReceiverVariableBytecode
    case 265: // 9	pushReceiverVariableBytecode
    {
      VM_LABEL(pushReceiverVariableBytecode9);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      longAtput((localSP -= BytesPerOop),
                fetchPointerofObject(9, longAt(localFP + FoxReceiver)));
    } break;
    case 10:  // pushReceiverVariableBytecode
    case 266: // 10	pushReceiverVariableBytecode
    {
      VM_LABEL(pushReceiverVariableBytecode10);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      longAtput((localSP -= BytesPerOop),
                fetchPointerofObject(10, longAt(localFP + FoxReceiver)));
    } break;
    case 11:  // pushReceiverVariableBytecode
    case 267: // 11	pushReceiverVariableBytecode
    {
      VM_LABEL(pushReceiverVariableBytecode11);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      longAtput((localSP -= BytesPerOop),
                fetchPointerofObject(11, longAt(localFP + FoxReceiver)));
    } break;
    case 12:  // pushReceiverVariableBytecode
    case 268: // 12	pushReceiverVariableBytecode
    {
      VM_LABEL(pushReceiverVariableBytecode12);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      longAtput((localSP -= BytesPerOop),
                fetchPointerofObject(12, longAt(localFP + FoxReceiver)));
    } break;
    case 13:  // pushReceiverVariableBytecode
    case 269: // 13	pushReceiverVariableBytecode
    {
      VM_LABEL(pushReceiverVariableBytecode13);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      longAtput((localSP -= BytesPerOop),
                fetchPointerofObject(13, longAt(localFP + FoxReceiver)));
    } break;
    case 14:  // pushReceiverVariableBytecode
    case 270: // 14	pushReceiverVariableBytecode
    {
      VM_LABEL(pushReceiverVariableBytecode14);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      longAtput((localSP -= BytesPerOop),
                fetchPointerofObject(14, longAt(localFP + FoxReceiver)));
    } break;
    case 15:  // pushReceiverVariableBytecode
    case 271: // 15	pushReceiverVariableBytecode
    {
      VM_LABEL(pushReceiverVariableBytecode15);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      longAtput((localSP -= BytesPerOop),
                fetchPointerofObject(15, longAt(localFP + FoxReceiver)));
    } break;
    case 16:  // pushTemporaryVariableBytecode
    case 320: // 64	pushTemporaryVariableBytecode
    {
      usqInt frameNumArgs;
      sqInt object;

      VM_LABEL(pushTemporaryVariableBytecode);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      object = /* temporary:in: */
          (0 < ((frameNumArgs = byteAt((localFP + FoxFrameFlags) + 1)))
               ? longAt((localFP + FoxCallerSavedIP) +
                        (frameNumArgs * BytesPerWord))
               : longAt(((localFP + FoxReceiver) - BytesPerWord) +
                        (frameNumArgs * BytesPerWord)));
      longAtput((localSP -= BytesPerOop), object);
    } break;
    case 17:  // pushTemporaryVariableBytecode
    case 321: // 65	pushTemporaryVariableBytecode
    {
      usqInt frameNumArgs;
      sqInt object;

      VM_LABEL(pushTemporaryVariableBytecode1);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      object = /* temporary:in: */
          (1 < ((frameNumArgs = byteAt((localFP + FoxFrameFlags) + 1)))
               ? longAt((localFP + FoxCallerSavedIP) +
                        ((frameNumArgs - 1) * BytesPerWord))
               : longAt(((localFP + FoxReceiver) - BytesPerWord) +
                        ((frameNumArgs - 1) * BytesPerWord)));
      longAtput((localSP -= BytesPerOop), object);
    } break;
    case 18:  // pushTemporaryVariableBytecode
    case 322: // 66	pushTemporaryVariableBytecode
    {
      usqInt frameNumArgs;
      sqInt object;

      VM_LABEL(pushTemporaryVariableBytecode2);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      object = /* temporary:in: */
          (2 < ((frameNumArgs = byteAt((localFP + FoxFrameFlags) + 1)))
               ? longAt((localFP + FoxCallerSavedIP) +
                        ((frameNumArgs - 2) * BytesPerWord))
               : longAt(((localFP + FoxReceiver) - BytesPerWord) +
                        ((frameNumArgs - 2) * BytesPerWord)));
      longAtput((localSP -= BytesPerOop), object);
    } break;
    case 19:  // pushTemporaryVariableBytecode
    case 323: // 67	pushTemporaryVariableBytecode
    {
      usqInt frameNumArgs;
      sqInt object;

      VM_LABEL(pushTemporaryVariableBytecode3);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      object = /* temporary:in: */
          (3 < ((frameNumArgs = byteAt((localFP + FoxFrameFlags) + 1)))
               ? longAt((localFP + FoxCallerSavedIP) +
                        ((frameNumArgs - 3) * BytesPerWord))
               : longAt(((localFP + FoxReceiver) - BytesPerWord) +
                        ((frameNumArgs - 3) * BytesPerWord)));
      longAtput((localSP -= BytesPerOop), object);
    } break;
    case 20:  // pushTemporaryVariableBytecode
    case 324: // 68	pushTemporaryVariableBytecode
    {
      usqInt frameNumArgs;
      sqInt object;

      VM_LABEL(pushTemporaryVariableBytecode4);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      object = /* temporary:in: */
          (4 < ((frameNumArgs = byteAt((localFP + FoxFrameFlags) + 1)))
               ? longAt((localFP + FoxCallerSavedIP) +
                        ((frameNumArgs - 4) * BytesPerWord))
               : longAt(((localFP + FoxReceiver) - BytesPerWord) +
                        ((frameNumArgs - 4) * BytesPerWord)));
      longAtput((localSP -= BytesPerOop), object);
    } break;
    case 21:  // pushTemporaryVariableBytecode
    case 325: // 69	pushTemporaryVariableBytecode
    {
      usqInt frameNumArgs;
      sqInt object;

      VM_LABEL(pushTemporaryVariableBytecode5);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      object = /* temporary:in: */
          (5 < ((frameNumArgs = byteAt((localFP + FoxFrameFlags) + 1)))
               ? longAt((localFP + FoxCallerSavedIP) +
                        ((frameNumArgs - 5) * BytesPerWord))
               : longAt(((localFP + FoxReceiver) - BytesPerWord) +
                        ((frameNumArgs - 5) * BytesPerWord)));
      longAtput((localSP -= BytesPerOop), object);
    } break;
    case 22:  // pushTemporaryVariableBytecode
    case 326: // 70	pushTemporaryVariableBytecode
    {
      usqInt frameNumArgs;
      sqInt object;

      VM_LABEL(pushTemporaryVariableBytecode6);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      object = /* temporary:in: */
          (6 < ((frameNumArgs = byteAt((localFP + FoxFrameFlags) + 1)))
               ? longAt((localFP + FoxCallerSavedIP) +
                        ((frameNumArgs - 6) * BytesPerWord))
               : longAt(((localFP + FoxReceiver) - BytesPerWord) +
                        ((frameNumArgs - 6) * BytesPerWord)));
      longAtput((localSP -= BytesPerOop), object);
    } break;
    case 23:  // pushTemporaryVariableBytecode
    case 327: // 71	pushTemporaryVariableBytecode
    {
      usqInt frameNumArgs;
      sqInt object;

      VM_LABEL(pushTemporaryVariableBytecode7);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      object = /* temporary:in: */
          (7 < ((frameNumArgs = byteAt((localFP + FoxFrameFlags) + 1)))
               ? longAt((localFP + FoxCallerSavedIP) +
                        ((frameNumArgs - 7) * BytesPerWord))
               : longAt(((localFP + FoxReceiver) - BytesPerWord) +
                        ((frameNumArgs - 7) * BytesPerWord)));
      longAtput((localSP -= BytesPerOop), object);
    } break;
    case 24:  // pushTemporaryVariableBytecode
    case 328: // 72	pushTemporaryVariableBytecode
    {
      usqInt frameNumArgs;
      sqInt object;

      VM_LABEL(pushTemporaryVariableBytecode8);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      object = /* temporary:in: */
          (8 < ((frameNumArgs = byteAt((localFP + FoxFrameFlags) + 1)))
               ? longAt((localFP + FoxCallerSavedIP) +
                        ((frameNumArgs - 8) * BytesPerWord))
               : longAt(((localFP + FoxReceiver) - BytesPerWord) +
                        ((frameNumArgs - 8) * BytesPerWord)));
      longAtput((localSP -= BytesPerOop), object);
    } break;
    case 25:  // pushTemporaryVariableBytecode
    case 329: // 73	pushTemporaryVariableBytecode
    {
      usqInt frameNumArgs;
      sqInt object;

      VM_LABEL(pushTemporaryVariableBytecode9);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      object = /* temporary:in: */
          (9 < ((frameNumArgs = byteAt((localFP + FoxFrameFlags) + 1)))
               ? longAt((localFP + FoxCallerSavedIP) +
                        ((frameNumArgs - 9) * BytesPerWord))
               : longAt(((localFP + FoxReceiver) - BytesPerWord) +
                        ((frameNumArgs - 9) * BytesPerWord)));
      longAtput((localSP -= BytesPerOop), object);
    } break;
    case 26:  // pushTemporaryVariableBytecode
    case 330: // 74	pushTemporaryVariableBytecode
    {
      usqInt frameNumArgs;
      sqInt object;

      VM_LABEL(pushTemporaryVariableBytecode10);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      object = /* temporary:in: */
          (10 < ((frameNumArgs = byteAt((localFP + FoxFrameFlags) + 1)))
               ? longAt((localFP + FoxCallerSavedIP) +
                        ((frameNumArgs - 10) * BytesPerWord))
               : longAt(((localFP + FoxReceiver) - BytesPerWord) +
                        ((frameNumArgs - 10) * BytesPerWord)));
      longAtput((localSP -= BytesPerOop), object);
    } break;
    case 27:  // pushTemporaryVariableBytecode
    case 331: // 75	pushTemporaryVariableBytecode
    {
      usqInt frameNumArgs;
      sqInt object;

      VM_LABEL(pushTemporaryVariableBytecode11);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      object = /* temporary:in: */
          (11 < ((frameNumArgs = byteAt((localFP + FoxFrameFlags) + 1)))
               ? longAt((localFP + FoxCallerSavedIP) +
                        ((frameNumArgs - 11) * BytesPerWord))
               : longAt(((localFP + FoxReceiver) - BytesPerWord) +
                        ((frameNumArgs - 11) * BytesPerWord)));
      longAtput((localSP -= BytesPerOop), object);
    } break;
    case 28: // pushTemporaryVariableBytecode
    {
      usqInt frameNumArgs;
      sqInt object;

      VM_LABEL(pushTemporaryVariableBytecode12);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      object = /* temporary:in: */
          (12 < ((frameNumArgs = byteAt((localFP + FoxFrameFlags) + 1)))
               ? longAt((localFP + FoxCallerSavedIP) +
                        ((frameNumArgs - 12) * BytesPerWord))
               : longAt(((localFP + FoxReceiver) - BytesPerWord) +
                        ((frameNumArgs - 12) * BytesPerWord)));
      longAtput((localSP -= BytesPerOop), object);
    } break;
    case 29: // pushTemporaryVariableBytecode
    {
      usqInt frameNumArgs;
      sqInt object;

      VM_LABEL(pushTemporaryVariableBytecode13);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      object = /* temporary:in: */
          (13 < ((frameNumArgs = byteAt((localFP + FoxFrameFlags) + 1)))
               ? longAt((localFP + FoxCallerSavedIP) +
                        ((frameNumArgs - 13) * BytesPerWord))
               : longAt(((localFP + FoxReceiver) - BytesPerWord) +
                        ((frameNumArgs - 13) * BytesPerWord)));
      longAtput((localSP -= BytesPerOop), object);
    } break;
    case 30: // pushTemporaryVariableBytecode
    {
      usqInt frameNumArgs;
      sqInt object;

      VM_LABEL(pushTemporaryVariableBytecode14);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      object = /* temporary:in: */
          (14 < ((frameNumArgs = byteAt((localFP + FoxFrameFlags) + 1)))
               ? longAt((localFP + FoxCallerSavedIP) +
                        ((frameNumArgs - 14) * BytesPerWord))
               : longAt(((localFP + FoxReceiver) - BytesPerWord) +
                        ((frameNumArgs - 14) * BytesPerWord)));
      longAtput((localSP -= BytesPerOop), object);
    } break;
    case 31: // pushTemporaryVariableBytecode
    {
      usqInt frameNumArgs;
      sqInt object;

      VM_LABEL(pushTemporaryVariableBytecode15);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      object = /* temporary:in: */
          (15 < ((frameNumArgs = byteAt((localFP + FoxFrameFlags) + 1)))
               ? longAt((localFP + FoxCallerSavedIP) +
                        ((frameNumArgs - 15) * BytesPerWord))
               : longAt(((localFP + FoxReceiver) - BytesPerWord) +
                        ((frameNumArgs - 15) * BytesPerWord)));
      longAtput((localSP -= BytesPerOop), object);
    } break;
    case 32:  // pushLiteralConstantBytecode
    case 288: // 32	pushLiteralConstantBytecode
    {
      sqInt object;

      VM_LABEL(pushLiteralConstantBytecode);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      assert(method == (iframeMethod(localFP)));
      object = fetchPointerofObject(0 + LiteralStart, method);

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop), object);
    } break;
    case 33:  // pushLiteralConstantBytecode
    case 289: // 33	pushLiteralConstantBytecode
    {
      sqInt object;

      VM_LABEL(pushLiteralConstantBytecode1);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      assert(method == (iframeMethod(localFP)));
      object = fetchPointerofObject(1 + LiteralStart, method);

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop), object);
    } break;
    case 34:  // pushLiteralConstantBytecode
    case 290: // 34	pushLiteralConstantBytecode
    {
      sqInt object;

      VM_LABEL(pushLiteralConstantBytecode2);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      assert(method == (iframeMethod(localFP)));
      object = fetchPointerofObject(2 + LiteralStart, method);

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop), object);
    } break;
    case 35:  // pushLiteralConstantBytecode
    case 291: // 35	pushLiteralConstantBytecode
    {
      sqInt object;

      VM_LABEL(pushLiteralConstantBytecode3);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      assert(method == (iframeMethod(localFP)));
      object = fetchPointerofObject(3 + LiteralStart, method);

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop), object);
    } break;
    case 36:  // pushLiteralConstantBytecode
    case 292: // 36	pushLiteralConstantBytecode
    {
      sqInt object;

      VM_LABEL(pushLiteralConstantBytecode4);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      assert(method == (iframeMethod(localFP)));
      object = fetchPointerofObject(4 + LiteralStart, method);

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop), object);
    } break;
    case 37:  // pushLiteralConstantBytecode
    case 293: // 37	pushLiteralConstantBytecode
    {
      sqInt object;

      VM_LABEL(pushLiteralConstantBytecode5);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      assert(method == (iframeMethod(localFP)));
      object = fetchPointerofObject(5 + LiteralStart, method);

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop), object);
    } break;
    case 38:  // pushLiteralConstantBytecode
    case 294: // 38	pushLiteralConstantBytecode
    {
      sqInt object;

      VM_LABEL(pushLiteralConstantBytecode6);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      assert(method == (iframeMethod(localFP)));
      object = fetchPointerofObject(6 + LiteralStart, method);

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop), object);
    } break;
    case 39:  // pushLiteralConstantBytecode
    case 295: // 39	pushLiteralConstantBytecode
    {
      sqInt object;

      VM_LABEL(pushLiteralConstantBytecode7);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      assert(method == (iframeMethod(localFP)));
      object = fetchPointerofObject(7 + LiteralStart, method);

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop), object);
    } break;
    case 40:  // pushLiteralConstantBytecode
    case 296: // 40	pushLiteralConstantBytecode
    {
      sqInt object;

      VM_LABEL(pushLiteralConstantBytecode8);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      assert(method == (iframeMethod(localFP)));
      object = fetchPointerofObject(8 + LiteralStart, method);

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop), object);
    } break;
    case 41:  // pushLiteralConstantBytecode
    case 297: // 41	pushLiteralConstantBytecode
    {
      sqInt object;

      VM_LABEL(pushLiteralConstantBytecode9);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      assert(method == (iframeMethod(localFP)));
      object = fetchPointerofObject(9 + LiteralStart, method);

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop), object);
    } break;
    case 42:  // pushLiteralConstantBytecode
    case 298: // 42	pushLiteralConstantBytecode
    {
      sqInt object;

      VM_LABEL(pushLiteralConstantBytecode10);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      assert(method == (iframeMethod(localFP)));
      object = fetchPointerofObject(10 + LiteralStart, method);

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop), object);
    } break;
    case 43:  // pushLiteralConstantBytecode
    case 299: // 43	pushLiteralConstantBytecode
    {
      sqInt object;

      VM_LABEL(pushLiteralConstantBytecode11);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      assert(method == (iframeMethod(localFP)));
      object = fetchPointerofObject(11 + LiteralStart, method);

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop), object);
    } break;
    case 44:  // pushLiteralConstantBytecode
    case 300: // 44	pushLiteralConstantBytecode
    {
      sqInt object;

      VM_LABEL(pushLiteralConstantBytecode12);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      assert(method == (iframeMethod(localFP)));
      object = fetchPointerofObject(12 + LiteralStart, method);

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop), object);
    } break;
    case 45:  // pushLiteralConstantBytecode
    case 301: // 45	pushLiteralConstantBytecode
    {
      sqInt object;

      VM_LABEL(pushLiteralConstantBytecode13);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      assert(method == (iframeMethod(localFP)));
      object = fetchPointerofObject(13 + LiteralStart, method);

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop), object);
    } break;
    case 46:  // pushLiteralConstantBytecode
    case 302: // 46	pushLiteralConstantBytecode
    {
      sqInt object;

      VM_LABEL(pushLiteralConstantBytecode14);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      assert(method == (iframeMethod(localFP)));
      object = fetchPointerofObject(14 + LiteralStart, method);

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop), object);
    } break;
    case 47:  // pushLiteralConstantBytecode
    case 303: // 47	pushLiteralConstantBytecode
    {
      sqInt object;

      VM_LABEL(pushLiteralConstantBytecode15);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      assert(method == (iframeMethod(localFP)));
      object = fetchPointerofObject(15 + LiteralStart, method);

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop), object);
    } break;
    case 48:  // pushLiteralConstantBytecode
    case 304: // 48	pushLiteralConstantBytecode
    {
      sqInt object;

      VM_LABEL(pushLiteralConstantBytecode16);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      assert(method == (iframeMethod(localFP)));
      object = fetchPointerofObject(16 + LiteralStart, method);

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop), object);
    } break;
    case 49:  // pushLiteralConstantBytecode
    case 305: // 49	pushLiteralConstantBytecode
    {
      sqInt object;

      VM_LABEL(pushLiteralConstantBytecode17);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      assert(method == (iframeMethod(localFP)));
      object = fetchPointerofObject(17 + LiteralStart, method);

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop), object);
    } break;
    case 50:  // pushLiteralConstantBytecode
    case 306: // 50	pushLiteralConstantBytecode
    {
      sqInt object;

      VM_LABEL(pushLiteralConstantBytecode18);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      assert(method == (iframeMethod(localFP)));
      object = fetchPointerofObject(18 + LiteralStart, method);

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop), object);
    } break;
    case 51:  // pushLiteralConstantBytecode
    case 307: // 51	pushLiteralConstantBytecode
    {
      sqInt object;

      VM_LABEL(pushLiteralConstantBytecode19);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      assert(method == (iframeMethod(localFP)));
      object = fetchPointerofObject(19 + LiteralStart, method);

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop), object);
    } break;
    case 52:  // pushLiteralConstantBytecode
    case 308: // 52	pushLiteralConstantBytecode
    {
      sqInt object;

      VM_LABEL(pushLiteralConstantBytecode20);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      assert(method == (iframeMethod(localFP)));
      object = fetchPointerofObject(20 + LiteralStart, method);

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop), object);
    } break;
    case 53:  // pushLiteralConstantBytecode
    case 309: // 53	pushLiteralConstantBytecode
    {
      sqInt object;

      VM_LABEL(pushLiteralConstantBytecode21);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      assert(method == (iframeMethod(localFP)));
      object = fetchPointerofObject(21 + LiteralStart, method);

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop), object);
    } break;
    case 54:  // pushLiteralConstantBytecode
    case 310: // 54	pushLiteralConstantBytecode
    {
      sqInt object;

      VM_LABEL(pushLiteralConstantBytecode22);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      assert(method == (iframeMethod(localFP)));
      object = fetchPointerofObject(22 + LiteralStart, method);

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop), object);
    } break;
    case 55:  // pushLiteralConstantBytecode
    case 311: // 55	pushLiteralConstantBytecode
    {
      sqInt object;

      VM_LABEL(pushLiteralConstantBytecode23);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      assert(method == (iframeMethod(localFP)));
      object = fetchPointerofObject(23 + LiteralStart, method);

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop), object);
    } break;
    case 56:  // pushLiteralConstantBytecode
    case 312: // 56	pushLiteralConstantBytecode
    {
      sqInt object;

      VM_LABEL(pushLiteralConstantBytecode24);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      assert(method == (iframeMethod(localFP)));
      object = fetchPointerofObject(24 + LiteralStart, method);

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop), object);
    } break;
    case 57:  // pushLiteralConstantBytecode
    case 313: // 57	pushLiteralConstantBytecode
    {
      sqInt object;

      VM_LABEL(pushLiteralConstantBytecode25);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      assert(method == (iframeMethod(localFP)));
      object = fetchPointerofObject(25 + LiteralStart, method);

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop), object);
    } break;
    case 58:  // pushLiteralConstantBytecode
    case 314: // 58	pushLiteralConstantBytecode
    {
      sqInt object;

      VM_LABEL(pushLiteralConstantBytecode26);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      assert(method == (iframeMethod(localFP)));
      object = fetchPointerofObject(26 + LiteralStart, method);

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop), object);
    } break;
    case 59:  // pushLiteralConstantBytecode
    case 315: // 59	pushLiteralConstantBytecode
    {
      sqInt object;

      VM_LABEL(pushLiteralConstantBytecode27);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      assert(method == (iframeMethod(localFP)));
      object = fetchPointerofObject(27 + LiteralStart, method);

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop), object);
    } break;
    case 60:  // pushLiteralConstantBytecode
    case 316: // 60	pushLiteralConstantBytecode
    {
      sqInt object;

      VM_LABEL(pushLiteralConstantBytecode28);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      assert(method == (iframeMethod(localFP)));
      object = fetchPointerofObject(28 + LiteralStart, method);

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop), object);
    } break;
    case 61:  // pushLiteralConstantBytecode
    case 317: // 61	pushLiteralConstantBytecode
    {
      sqInt object;

      VM_LABEL(pushLiteralConstantBytecode29);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      assert(method == (iframeMethod(localFP)));
      object = fetchPointerofObject(29 + LiteralStart, method);

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop), object);
    } break;
    case 62:  // pushLiteralConstantBytecode
    case 318: // 62	pushLiteralConstantBytecode
    {
      sqInt object;

      VM_LABEL(pushLiteralConstantBytecode30);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      assert(method == (iframeMethod(localFP)));
      object = fetchPointerofObject(30 + LiteralStart, method);

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop), object);
    } break;
    case 63:  // pushLiteralConstantBytecode
    case 319: // 63	pushLiteralConstantBytecode
    {
      sqInt object;

      VM_LABEL(pushLiteralConstantBytecode31);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      assert(method == (iframeMethod(localFP)));
      object = fetchPointerofObject(31 + LiteralStart, method);

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop), object);
    } break;
    case 64:  // pushLiteralVariableBytecode
    case 272: // 16	pushLiteralVariable16CasesBytecode
    {
      sqInt litVar;

      VM_LABEL(pushLiteralVariableBytecode);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      litVar = fetchPointerofObject(0 + LiteralStart, method);
      assert(isNonImmediate(litVar));
      if ((!((longAt((void *)(litVar))) &
             ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
        litVar = fixFollowedFieldofObjectwithInitialValue(
            1 /* (0 + LiteralStart) */, method, litVar);
      }

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop),
                fetchPointerofObject(ValueIndex, litVar));
    } break;
    case 65:  // pushLiteralVariableBytecode
    case 273: // 17	pushLiteralVariable16CasesBytecode
    {
      sqInt litVar;

      VM_LABEL(pushLiteralVariableBytecode1);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      litVar = fetchPointerofObject(1 + LiteralStart, method);
      assert(isNonImmediate(litVar));
      if ((!((longAt((void *)(litVar))) &
             ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
        litVar = fixFollowedFieldofObjectwithInitialValue(
            2 /* (1 + LiteralStart) */, method, litVar);
      }

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop),
                fetchPointerofObject(ValueIndex, litVar));
    } break;
    case 66:  // pushLiteralVariableBytecode
    case 274: // 18	pushLiteralVariable16CasesBytecode
    {
      sqInt litVar;

      VM_LABEL(pushLiteralVariableBytecode2);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      litVar = fetchPointerofObject(2 + LiteralStart, method);
      assert(isNonImmediate(litVar));
      if ((!((longAt((void *)(litVar))) &
             ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
        litVar = fixFollowedFieldofObjectwithInitialValue(
            3 /* (2 + LiteralStart) */, method, litVar);
      }

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop),
                fetchPointerofObject(ValueIndex, litVar));
    } break;
    case 67:  // pushLiteralVariableBytecode
    case 275: // 19	pushLiteralVariable16CasesBytecode
    {
      sqInt litVar;

      VM_LABEL(pushLiteralVariableBytecode3);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      litVar = fetchPointerofObject(3 + LiteralStart, method);
      assert(isNonImmediate(litVar));
      if ((!((longAt((void *)(litVar))) &
             ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
        litVar = fixFollowedFieldofObjectwithInitialValue(
            4 /* (3 + LiteralStart) */, method, litVar);
      }

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop),
                fetchPointerofObject(ValueIndex, litVar));
    } break;
    case 68:  // pushLiteralVariableBytecode
    case 276: // 20	pushLiteralVariable16CasesBytecode
    {
      sqInt litVar;

      VM_LABEL(pushLiteralVariableBytecode4);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      litVar = fetchPointerofObject(4 + LiteralStart, method);
      assert(isNonImmediate(litVar));
      if ((!((longAt((void *)(litVar))) &
             ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
        litVar = fixFollowedFieldofObjectwithInitialValue(
            5 /* (4 + LiteralStart) */, method, litVar);
      }

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop),
                fetchPointerofObject(ValueIndex, litVar));
    } break;
    case 69:  // pushLiteralVariableBytecode
    case 277: // 21	pushLiteralVariable16CasesBytecode
    {
      sqInt litVar;

      VM_LABEL(pushLiteralVariableBytecode5);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      litVar = fetchPointerofObject(5 + LiteralStart, method);
      assert(isNonImmediate(litVar));
      if ((!((longAt((void *)(litVar))) &
             ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
        litVar = fixFollowedFieldofObjectwithInitialValue(
            6 /* (5 + LiteralStart) */, method, litVar);
      }

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop),
                fetchPointerofObject(ValueIndex, litVar));
    } break;
    case 70:  // pushLiteralVariableBytecode
    case 278: // 22	pushLiteralVariable16CasesBytecode
    {
      sqInt litVar;

      VM_LABEL(pushLiteralVariableBytecode6);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      litVar = fetchPointerofObject(6 + LiteralStart, method);
      assert(isNonImmediate(litVar));
      if ((!((longAt((void *)(litVar))) &
             ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
        litVar = fixFollowedFieldofObjectwithInitialValue(
            7 /* (6 + LiteralStart) */, method, litVar);
      }

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop),
                fetchPointerofObject(ValueIndex, litVar));
    } break;
    case 71:  // pushLiteralVariableBytecode
    case 279: // 23	pushLiteralVariable16CasesBytecode
    {
      sqInt litVar;

      VM_LABEL(pushLiteralVariableBytecode7);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      litVar = fetchPointerofObject(7 + LiteralStart, method);
      assert(isNonImmediate(litVar));
      if ((!((longAt((void *)(litVar))) &
             ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
        litVar = fixFollowedFieldofObjectwithInitialValue(
            8 /* (7 + LiteralStart) */, method, litVar);
      }

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop),
                fetchPointerofObject(ValueIndex, litVar));
    } break;
    case 72:  // pushLiteralVariableBytecode
    case 280: // 24	pushLiteralVariable16CasesBytecode
    {
      sqInt litVar;

      VM_LABEL(pushLiteralVariableBytecode8);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      litVar = fetchPointerofObject(8 + LiteralStart, method);
      assert(isNonImmediate(litVar));
      if ((!((longAt((void *)(litVar))) &
             ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
        litVar = fixFollowedFieldofObjectwithInitialValue(
            9 /* (8 + LiteralStart) */, method, litVar);
      }

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop),
                fetchPointerofObject(ValueIndex, litVar));
    } break;
    case 73:  // pushLiteralVariableBytecode
    case 281: // 25	pushLiteralVariable16CasesBytecode
    {
      sqInt litVar;

      VM_LABEL(pushLiteralVariableBytecode9);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      litVar = fetchPointerofObject(9 + LiteralStart, method);
      assert(isNonImmediate(litVar));
      if ((!((longAt((void *)(litVar))) &
             ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
        litVar = fixFollowedFieldofObjectwithInitialValue(
            10 /* (9 + LiteralStart) */, method, litVar);
      }

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop),
                fetchPointerofObject(ValueIndex, litVar));
    } break;
    case 74:  // pushLiteralVariableBytecode
    case 282: // 26	pushLiteralVariable16CasesBytecode
    {
      sqInt litVar;

      VM_LABEL(pushLiteralVariableBytecode10);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      litVar = fetchPointerofObject(10 + LiteralStart, method);
      assert(isNonImmediate(litVar));
      if ((!((longAt((void *)(litVar))) &
             ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
        litVar = fixFollowedFieldofObjectwithInitialValue(
            11 /* (10 + LiteralStart) */, method, litVar);
      }

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop),
                fetchPointerofObject(ValueIndex, litVar));
    } break;
    case 75:  // pushLiteralVariableBytecode
    case 283: // 27	pushLiteralVariable16CasesBytecode
    {
      sqInt litVar;

      VM_LABEL(pushLiteralVariableBytecode11);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      litVar = fetchPointerofObject(11 + LiteralStart, method);
      assert(isNonImmediate(litVar));
      if ((!((longAt((void *)(litVar))) &
             ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
        litVar = fixFollowedFieldofObjectwithInitialValue(
            12 /* (11 + LiteralStart) */, method, litVar);
      }

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop),
                fetchPointerofObject(ValueIndex, litVar));
    } break;
    case 76:  // pushLiteralVariableBytecode
    case 284: // 28	pushLiteralVariable16CasesBytecode
    {
      sqInt litVar;

      VM_LABEL(pushLiteralVariableBytecode12);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      litVar = fetchPointerofObject(12 + LiteralStart, method);
      assert(isNonImmediate(litVar));
      if ((!((longAt((void *)(litVar))) &
             ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
        litVar = fixFollowedFieldofObjectwithInitialValue(
            13 /* (12 + LiteralStart) */, method, litVar);
      }

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop),
                fetchPointerofObject(ValueIndex, litVar));
    } break;
    case 77:  // pushLiteralVariableBytecode
    case 285: // 29	pushLiteralVariable16CasesBytecode
    {
      sqInt litVar;

      VM_LABEL(pushLiteralVariableBytecode13);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      litVar = fetchPointerofObject(13 + LiteralStart, method);
      assert(isNonImmediate(litVar));
      if ((!((longAt((void *)(litVar))) &
             ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
        litVar = fixFollowedFieldofObjectwithInitialValue(
            14 /* (13 + LiteralStart) */, method, litVar);
      }

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop),
                fetchPointerofObject(ValueIndex, litVar));
    } break;
    case 78:  // pushLiteralVariableBytecode
    case 286: // 30	pushLiteralVariable16CasesBytecode
    {
      sqInt litVar;

      VM_LABEL(pushLiteralVariableBytecode14);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      litVar = fetchPointerofObject(14 + LiteralStart, method);
      assert(isNonImmediate(litVar));
      if ((!((longAt((void *)(litVar))) &
             ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
        litVar = fixFollowedFieldofObjectwithInitialValue(
            15 /* (14 + LiteralStart) */, method, litVar);
      }

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop),
                fetchPointerofObject(ValueIndex, litVar));
    } break;
    case 79:  // pushLiteralVariableBytecode
    case 287: // 31	pushLiteralVariable16CasesBytecode
    {
      sqInt litVar;

      VM_LABEL(pushLiteralVariableBytecode15);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      litVar = fetchPointerofObject(15 + LiteralStart, method);
      assert(isNonImmediate(litVar));
      if ((!((longAt((void *)(litVar))) &
             ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
        litVar = fixFollowedFieldofObjectwithInitialValue(
            16 /* (15 + LiteralStart) */, method, litVar);
      }

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop),
                fetchPointerofObject(ValueIndex, litVar));
    } break;
    case 80: // pushLiteralVariableBytecode
    {
      sqInt litVar;

      VM_LABEL(pushLiteralVariableBytecode16);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      litVar = fetchPointerofObject(16 + LiteralStart, method);
      assert(isNonImmediate(litVar));
      if ((!((longAt((void *)(litVar))) &
             ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
        litVar = fixFollowedFieldofObjectwithInitialValue(
            17 /* (16 + LiteralStart) */, method, litVar);
      }

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop),
                fetchPointerofObject(ValueIndex, litVar));
    } break;
    case 81: // pushLiteralVariableBytecode
    {
      sqInt litVar;

      VM_LABEL(pushLiteralVariableBytecode17);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      litVar = fetchPointerofObject(17 + LiteralStart, method);
      assert(isNonImmediate(litVar));
      if ((!((longAt((void *)(litVar))) &
             ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
        litVar = fixFollowedFieldofObjectwithInitialValue(
            18 /* (17 + LiteralStart) */, method, litVar);
      }

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop),
                fetchPointerofObject(ValueIndex, litVar));
    } break;
    case 82: // pushLiteralVariableBytecode
    {
      sqInt litVar;

      VM_LABEL(pushLiteralVariableBytecode18);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      litVar = fetchPointerofObject(18 + LiteralStart, method);
      assert(isNonImmediate(litVar));
      if ((!((longAt((void *)(litVar))) &
             ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
        litVar = fixFollowedFieldofObjectwithInitialValue(
            19 /* (18 + LiteralStart) */, method, litVar);
      }

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop),
                fetchPointerofObject(ValueIndex, litVar));
    } break;
    case 83: // pushLiteralVariableBytecode
    {
      sqInt litVar;

      VM_LABEL(pushLiteralVariableBytecode19);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      litVar = fetchPointerofObject(19 + LiteralStart, method);
      assert(isNonImmediate(litVar));
      if ((!((longAt((void *)(litVar))) &
             ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
        litVar = fixFollowedFieldofObjectwithInitialValue(
            20 /* (19 + LiteralStart) */, method, litVar);
      }

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop),
                fetchPointerofObject(ValueIndex, litVar));
    } break;
    case 84: // pushLiteralVariableBytecode
    {
      sqInt litVar;

      VM_LABEL(pushLiteralVariableBytecode20);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      litVar = fetchPointerofObject(20 + LiteralStart, method);
      assert(isNonImmediate(litVar));
      if ((!((longAt((void *)(litVar))) &
             ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
        litVar = fixFollowedFieldofObjectwithInitialValue(
            21 /* (20 + LiteralStart) */, method, litVar);
      }

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop),
                fetchPointerofObject(ValueIndex, litVar));
    } break;
    case 85: // pushLiteralVariableBytecode
    {
      sqInt litVar;

      VM_LABEL(pushLiteralVariableBytecode21);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      litVar = fetchPointerofObject(21 + LiteralStart, method);
      assert(isNonImmediate(litVar));
      if ((!((longAt((void *)(litVar))) &
             ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
        litVar = fixFollowedFieldofObjectwithInitialValue(
            22 /* (21 + LiteralStart) */, method, litVar);
      }

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop),
                fetchPointerofObject(ValueIndex, litVar));
    } break;
    case 86: // pushLiteralVariableBytecode
    {
      sqInt litVar;

      VM_LABEL(pushLiteralVariableBytecode22);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      litVar = fetchPointerofObject(22 + LiteralStart, method);
      assert(isNonImmediate(litVar));
      if ((!((longAt((void *)(litVar))) &
             ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
        litVar = fixFollowedFieldofObjectwithInitialValue(
            23 /* (22 + LiteralStart) */, method, litVar);
      }

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop),
                fetchPointerofObject(ValueIndex, litVar));
    } break;
    case 87: // pushLiteralVariableBytecode
    {
      sqInt litVar;

      VM_LABEL(pushLiteralVariableBytecode23);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      litVar = fetchPointerofObject(23 + LiteralStart, method);
      assert(isNonImmediate(litVar));
      if ((!((longAt((void *)(litVar))) &
             ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
        litVar = fixFollowedFieldofObjectwithInitialValue(
            24 /* (23 + LiteralStart) */, method, litVar);
      }

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop),
                fetchPointerofObject(ValueIndex, litVar));
    } break;
    case 88: // pushLiteralVariableBytecode
    {
      sqInt litVar;

      VM_LABEL(pushLiteralVariableBytecode24);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      litVar = fetchPointerofObject(24 + LiteralStart, method);
      assert(isNonImmediate(litVar));
      if ((!((longAt((void *)(litVar))) &
             ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
        litVar = fixFollowedFieldofObjectwithInitialValue(
            25 /* (24 + LiteralStart) */, method, litVar);
      }

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop),
                fetchPointerofObject(ValueIndex, litVar));
    } break;
    case 89: // pushLiteralVariableBytecode
    {
      sqInt litVar;

      VM_LABEL(pushLiteralVariableBytecode25);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      litVar = fetchPointerofObject(25 + LiteralStart, method);
      assert(isNonImmediate(litVar));
      if ((!((longAt((void *)(litVar))) &
             ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
        litVar = fixFollowedFieldofObjectwithInitialValue(
            26 /* (25 + LiteralStart) */, method, litVar);
      }

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop),
                fetchPointerofObject(ValueIndex, litVar));
    } break;
    case 90: // pushLiteralVariableBytecode
    {
      sqInt litVar;

      VM_LABEL(pushLiteralVariableBytecode26);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      litVar = fetchPointerofObject(26 + LiteralStart, method);
      assert(isNonImmediate(litVar));
      if ((!((longAt((void *)(litVar))) &
             ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
        litVar = fixFollowedFieldofObjectwithInitialValue(
            27 /* (26 + LiteralStart) */, method, litVar);
      }

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop),
                fetchPointerofObject(ValueIndex, litVar));
    } break;
    case 91: // pushLiteralVariableBytecode
    {
      sqInt litVar;

      VM_LABEL(pushLiteralVariableBytecode27);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      litVar = fetchPointerofObject(27 + LiteralStart, method);
      assert(isNonImmediate(litVar));
      if ((!((longAt((void *)(litVar))) &
             ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
        litVar = fixFollowedFieldofObjectwithInitialValue(
            28 /* (27 + LiteralStart) */, method, litVar);
      }

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop),
                fetchPointerofObject(ValueIndex, litVar));
    } break;
    case 92: // pushLiteralVariableBytecode
    {
      sqInt litVar;

      VM_LABEL(pushLiteralVariableBytecode28);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      litVar = fetchPointerofObject(28 + LiteralStart, method);
      assert(isNonImmediate(litVar));
      if ((!((longAt((void *)(litVar))) &
             ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
        litVar = fixFollowedFieldofObjectwithInitialValue(
            29 /* (28 + LiteralStart) */, method, litVar);
      }

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop),
                fetchPointerofObject(ValueIndex, litVar));
    } break;
    case 93: // pushLiteralVariableBytecode
    {
      sqInt litVar;

      VM_LABEL(pushLiteralVariableBytecode29);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      litVar = fetchPointerofObject(29 + LiteralStart, method);
      assert(isNonImmediate(litVar));
      if ((!((longAt((void *)(litVar))) &
             ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
        litVar = fixFollowedFieldofObjectwithInitialValue(
            30 /* (29 + LiteralStart) */, method, litVar);
      }

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop),
                fetchPointerofObject(ValueIndex, litVar));
    } break;
    case 94: // pushLiteralVariableBytecode
    {
      sqInt litVar;

      VM_LABEL(pushLiteralVariableBytecode30);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      litVar = fetchPointerofObject(30 + LiteralStart, method);
      assert(isNonImmediate(litVar));
      if ((!((longAt((void *)(litVar))) &
             ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
        litVar = fixFollowedFieldofObjectwithInitialValue(
            0x1F /* (30 + LiteralStart) */, method, litVar);
      }

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop),
                fetchPointerofObject(ValueIndex, litVar));
    } break;
    case 95: // pushLiteralVariableBytecode
    {
      sqInt litVar;

      VM_LABEL(pushLiteralVariableBytecode31);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      litVar = fetchPointerofObject(31 + LiteralStart, method);
      assert(isNonImmediate(litVar));
      if ((!((longAt((void *)(litVar))) &
             ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
        litVar = fixFollowedFieldofObjectwithInitialValue(
            32 /* (31 + LiteralStart) */, method, litVar);
      }

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop),
                fetchPointerofObject(ValueIndex, litVar));
    } break;
    case 96:  // storeAndPopReceiverVariableBytecode
    case 97:  // storeAndPopReceiverVariableBytecode
    case 98:  // storeAndPopReceiverVariableBytecode
    case 99:  // storeAndPopReceiverVariableBytecode
    case 100: // storeAndPopReceiverVariableBytecode
    case 101: // storeAndPopReceiverVariableBytecode
    case 102: // storeAndPopReceiverVariableBytecode
    case 103: // storeAndPopReceiverVariableBytecode
    case 456: // 200	storeAndPopReceiverVariableBytecode
    case 457: // 201	storeAndPopReceiverVariableBytecode
    case 458: // 202	storeAndPopReceiverVariableBytecode
    case 459: // 203	storeAndPopReceiverVariableBytecode
    case 460: // 204	storeAndPopReceiverVariableBytecode
    case 461: // 205	storeAndPopReceiverVariableBytecode
    case 462: // 206	storeAndPopReceiverVariableBytecode
    case 463: // 207	storeAndPopReceiverVariableBytecode
    {
      usqInt instVarIndex;
      sqInt rcvr;
      sqInt top;

      VM_LABEL(storeAndPopReceiverVariableBytecode);
      rcvr = longAt(localFP + FoxReceiver);
      top = longAt(localSP);
      instVarIndex = currentBytecode & 7;

      /* begin internalPop: */
      localSP += 1 * BytesPerOop;
#if IMMUTABILITY
      assert(!((isImmediate(rcvr))));
      if ((((usqInt)((
               byteAt((void *)(rcvr + (immutableExtraBitsByteOffset())))))) >>
           (immutableBitByteShift())) &
          1) {
        /* begin cannotAssign:to:withIndex: */
        /* begin internalPush: */
        longAtput((localSP -= BytesPerOop), rcvr);
        longAtput((localSP -= BytesPerOop), top);
        longAtput((localSP -= BytesPerOop), (((instVarIndex + 1) << 3) | 1));
        messageSelector =
            fetchPointerofObject(SelectorAttemptToAssign, specialObjectsOop);
        argumentCount = 2;
        goto normalSend;
        goto l1;
      }
#endif // IMMUTABILITY

      /* begin storePointer:ofObject:withValue: */
      assert(validStorePointerArgs(instVarIndex, rcvr, top));
      assert(isNonImmediate(rcvr));
      if (oopisGreaterThanOrEqualTo(rcvr, oldSpaceStart)) {
        if (/* isYoung: */
            ((!(top & (tagMask())))) && (oopisLessThan(top, oldSpaceStart))) {
          /* begin possibleRootStoreInto: */
          if (!((byteAt((void *)(rcvr + (formatFieldByteOffset())))) &
                (1U << (rememberedBitByteShift())))) {
            remember(rcvr);
          }
        }
      }

      /* most stores into young objects */
      longAtput((void *)((rcvr + BaseHeaderSize) +
                         ((instVarIndex << (shiftForWord())))),
                top);
      /* end storePointerImmutabilityCheck:ofObject:withValue: */
    l1:

      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
    } break;
    case 104: // storeAndPopTemporaryVariableBytecode
    case 464: // 208	storeAndPopTemporaryVariableBytecode
    {
      usqInt frameNumArgs;

      VM_LABEL(storeAndPopTemporaryVariableBytecode);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;

      /* temporary:in:put: */
      if (0 < ((frameNumArgs = byteAt((localFP + FoxFrameFlags) + 1)))) {
        longAtput((localFP + FoxCallerSavedIP) + (frameNumArgs * BytesPerWord),
                  longAt(localSP));
      } else {
        longAtput(((localFP + FoxReceiver) - BytesPerWord) +
                      (frameNumArgs * BytesPerWord),
                  longAt(localSP));
      }

      /* begin internalPop: */
      localSP += 1 * BytesPerOop;
    } break;
    case 105: // storeAndPopTemporaryVariableBytecode
    case 465: // 209	storeAndPopTemporaryVariableBytecode
    {
      usqInt frameNumArgs;

      VM_LABEL(storeAndPopTemporaryVariableBytecode1);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;

      /* temporary:in:put: */
      if (1 < ((frameNumArgs = byteAt((localFP + FoxFrameFlags) + 1)))) {
        longAtput((localFP + FoxCallerSavedIP) +
                      ((frameNumArgs - 1) * BytesPerWord),
                  longAt(localSP));
      } else {
        longAtput(((localFP + FoxReceiver) - BytesPerWord) +
                      ((frameNumArgs - 1) * BytesPerWord),
                  longAt(localSP));
      }

      /* begin internalPop: */
      localSP += 1 * BytesPerOop;
    } break;
    case 106: // storeAndPopTemporaryVariableBytecode
    case 466: // 210	storeAndPopTemporaryVariableBytecode
    {
      usqInt frameNumArgs;

      VM_LABEL(storeAndPopTemporaryVariableBytecode2);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;

      /* temporary:in:put: */
      if (2 < ((frameNumArgs = byteAt((localFP + FoxFrameFlags) + 1)))) {
        longAtput((localFP + FoxCallerSavedIP) +
                      ((frameNumArgs - 2) * BytesPerWord),
                  longAt(localSP));
      } else {
        longAtput(((localFP + FoxReceiver) - BytesPerWord) +
                      ((frameNumArgs - 2) * BytesPerWord),
                  longAt(localSP));
      }

      /* begin internalPop: */
      localSP += 1 * BytesPerOop;
    } break;
    case 107: // storeAndPopTemporaryVariableBytecode
    case 467: // 211	storeAndPopTemporaryVariableBytecode
    {
      usqInt frameNumArgs;

      VM_LABEL(storeAndPopTemporaryVariableBytecode3);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;

      /* temporary:in:put: */
      if (3 < ((frameNumArgs = byteAt((localFP + FoxFrameFlags) + 1)))) {
        longAtput((localFP + FoxCallerSavedIP) +
                      ((frameNumArgs - 3) * BytesPerWord),
                  longAt(localSP));
      } else {
        longAtput(((localFP + FoxReceiver) - BytesPerWord) +
                      ((frameNumArgs - 3) * BytesPerWord),
                  longAt(localSP));
      }

      /* begin internalPop: */
      localSP += 1 * BytesPerOop;
    } break;
    case 108: // storeAndPopTemporaryVariableBytecode
    case 468: // 212	storeAndPopTemporaryVariableBytecode
    {
      usqInt frameNumArgs;

      VM_LABEL(storeAndPopTemporaryVariableBytecode4);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;

      /* temporary:in:put: */
      if (4 < ((frameNumArgs = byteAt((localFP + FoxFrameFlags) + 1)))) {
        longAtput((localFP + FoxCallerSavedIP) +
                      ((frameNumArgs - 4) * BytesPerWord),
                  longAt(localSP));
      } else {
        longAtput(((localFP + FoxReceiver) - BytesPerWord) +
                      ((frameNumArgs - 4) * BytesPerWord),
                  longAt(localSP));
      }

      /* begin internalPop: */
      localSP += 1 * BytesPerOop;
    } break;
    case 109: // storeAndPopTemporaryVariableBytecode
    case 469: // 213	storeAndPopTemporaryVariableBytecode
    {
      usqInt frameNumArgs;

      VM_LABEL(storeAndPopTemporaryVariableBytecode5);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;

      /* temporary:in:put: */
      if (5 < ((frameNumArgs = byteAt((localFP + FoxFrameFlags) + 1)))) {
        longAtput((localFP + FoxCallerSavedIP) +
                      ((frameNumArgs - 5) * BytesPerWord),
                  longAt(localSP));
      } else {
        longAtput(((localFP + FoxReceiver) - BytesPerWord) +
                      ((frameNumArgs - 5) * BytesPerWord),
                  longAt(localSP));
      }

      /* begin internalPop: */
      localSP += 1 * BytesPerOop;
    } break;
    case 110: // storeAndPopTemporaryVariableBytecode
    case 470: // 214	storeAndPopTemporaryVariableBytecode
    {
      usqInt frameNumArgs;

      VM_LABEL(storeAndPopTemporaryVariableBytecode6);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;

      /* temporary:in:put: */
      if (6 < ((frameNumArgs = byteAt((localFP + FoxFrameFlags) + 1)))) {
        longAtput((localFP + FoxCallerSavedIP) +
                      ((frameNumArgs - 6) * BytesPerWord),
                  longAt(localSP));
      } else {
        longAtput(((localFP + FoxReceiver) - BytesPerWord) +
                      ((frameNumArgs - 6) * BytesPerWord),
                  longAt(localSP));
      }

      /* begin internalPop: */
      localSP += 1 * BytesPerOop;
    } break;
    case 111: // storeAndPopTemporaryVariableBytecode
    case 471: // 215	storeAndPopTemporaryVariableBytecode
    {
      usqInt frameNumArgs;

      VM_LABEL(storeAndPopTemporaryVariableBytecode7);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;

      /* temporary:in:put: */
      if (7 < ((frameNumArgs = byteAt((localFP + FoxFrameFlags) + 1)))) {
        longAtput((localFP + FoxCallerSavedIP) +
                      ((frameNumArgs - 7) * BytesPerWord),
                  longAt(localSP));
      } else {
        longAtput(((localFP + FoxReceiver) - BytesPerWord) +
                      ((frameNumArgs - 7) * BytesPerWord),
                  longAt(localSP));
      }

      /* begin internalPop: */
      localSP += 1 * BytesPerOop;
    } break;
    case 112: // pushReceiverBytecode
    case 332: // 76	pushReceiverBytecode
    {
      VM_LABEL(pushReceiverBytecode);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      longAtput((localSP -= BytesPerOop), longAt(localFP + FoxReceiver));
    } break;
    case 113: // pushConstantTrueBytecode
    case 333: // 77	pushConstantTrueBytecode
    {
      VM_LABEL(pushConstantTrueBytecode);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      longAtput((localSP -= BytesPerOop), trueObj);
    } break;
    case 114: // pushConstantFalseBytecode
    case 334: // 78	pushConstantFalseBytecode
    {
      VM_LABEL(pushConstantFalseBytecode);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      longAtput((localSP -= BytesPerOop), falseObj);
    } break;
    case 115: // pushConstantNilBytecode
    case 335: // 79	pushConstantNilBytecode
    {
      VM_LABEL(pushConstantNilBytecode);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      longAtput((localSP -= BytesPerOop), nilObj);
    } break;
    case 116: // pushConstantMinusOneBytecode
    {
      VM_LABEL(pushConstantMinusOneBytecode);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      longAtput((localSP -= BytesPerOop), ConstMinusOne);
    } break;
    case 117: // pushConstantZeroBytecode
    case 336: // 80	pushConstantZeroBytecode
    {
      VM_LABEL(pushConstantZeroBytecode);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      longAtput((localSP -= BytesPerOop), ConstZero);
    } break;
    case 118: // pushConstantOneBytecode
    case 337: // 81	pushConstantOneBytecode
    {
      VM_LABEL(pushConstantOneBytecode);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      longAtput((localSP -= BytesPerOop), ConstOne);
    } break;
    case 119: // pushConstantTwoBytecode
    {
      VM_LABEL(pushConstantTwoBytecode);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      longAtput((localSP -= BytesPerOop), ConstTwo);
    } break;
    case 120: // returnReceiver
    {
      VM_LABEL(returnReceiver);
      localReturnValue = longAt(localFP + FoxReceiver);

      /* goto commonReturn */
    }

    commonReturn:
      /* commonReturn */
      {
        char *callerFP;
        sqInt closure;
        sqInt contextToReturnTo;
        sqInt ctxtOrNilOrZero;
        sqInt currentCtx;
        char *frameToReturnTo;
        sqInt home;
        sqInt methodHeader;
        StackPage *newPage;
        sqInt nextCntx;
        sqInt onSamePage;
        sqInt ourContext;
        sqInt senderContext;
        sqInt senderOop;
        char *theFP;
        char *theFP1;
        char *theFPAbove;
        sqInt theMethod;
        sqInt theMethodSqInt;
        StackPage *thePage;
        StackPage *thePage1;
        char *theSP;
        sqInt unwindContextOrNilOrZero;

        VM_LABEL(commonReturn);

        /* If this is a method simply return to the  sender/caller. */
        frameToReturnTo = ((char *)0);
        theFP = ((char *)0);
        if (!(byteAt((localFP + FoxFrameFlags) + 3))) {
          goto commonCallerReturn;
          goto l154;
        }

        /* Update the current page's headFrame pointers to enable the search for
           unwind protects below to identify widowed contexts correctly. */

        /* begin writeBackHeadFramePointers */
        assert(stackPage == (mostRecentlyUsedPage));
        assert(localSP < localFP);
        assert((localSP < ((stackPage->baseAddress))) &&
               (localSP > (((stackPage->realStackLimit)) -
                           (LargeContextSlots * BytesPerOop))));
        assert((localFP < ((stackPage->baseAddress))) &&
               (localFP > (((stackPage->realStackLimit)) -
                           ((LargeContextSlots * BytesPerOop) / 2))));
        (stackPage->headFP = localFP);
        (stackPage->headSP = localSP);
        assert(pageListIsWellFormed());

        /* begin externalizeIPandSP */
        instructionPointer = ((usqInt)localIP);
        stackPointer = localSP;
        framePointer = localFP;

        /* Since this is a block activation the closure is on the stack above
         * any args and the frame. */
        closure = longAt(localFP +
                         ((FoxCallerSavedIP + BytesPerWord) +
                          ((((usqInt)((byteAt((localFP + FoxFrameFlags) + 1)))
                             << (shiftForWord()))))));

        /* avoid compiler warning */
        /* Walk the closure's lexical chain to find the context or frame to
           return from (home). If home is missing (Sista closures) then throw
           cannotReturn rather than crash. */
        home = null;
        while (closure != nilObj) {
          /* begin followObjField:ofObject: */
          home = fetchPointerofObject(ClosureOuterContextIndex, closure);
          assert(isNonImmediate(home));
          if ((!((longAt((void *)(home))) &
                 ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
            home = fixFollowedFieldofObjectwithInitialValue(
                ClosureOuterContextIndex, closure, home);
          }
          if (!(/* isContext: */
                ((!(home & (tagMask())))) &&
                (((longAt((void *)(home))) & (classIndexMask())) ==
                 ClassMethodContextCompactIndex))) {
            /* begin internalCannotReturn: */
            /* begin ensureFrameIsMarried:SP: */
            if (byteAt((localFP + FoxFrameFlags) + 2)) {
              assert(isContext(frameContext(localFP)));
              ourContext = longAt(localFP + FoxThisContext);
              goto l158;
            }
            ourContext = marryFrameSP(localFP, localSP);
            /* end ensureFrameIsMarried:SP: */
          l158:

            /* begin internalPush: */
            longAtput((localSP -= BytesPerOop), ourContext);
            longAtput((localSP -= BytesPerOop), localReturnValue);
            messageSelector =
                fetchPointerofObject(SelectorCannotReturn, specialObjectsOop);
            argumentCount = 1;
            goto normalSend;
            goto l154;
          }

          /* begin followObjField:ofObject: */
          closure = fetchPointerofObject(ClosureIndex, home);
          assert(isNonImmediate(closure));
          if ((!((longAt((void *)(closure))) &
                 ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
            closure = fixFollowedFieldofObjectwithInitialValue(ClosureIndex,
                                                               home, closure);
          }
        }

        /* home is to be returned from provided there is no unwind-protect
           activation between this frame and home's sender.  Search for an
           unwind.  findUnwindThroughContext: will answer either the context for
           an unwind-protect activation or nilObj if the sender cannot be found
           or 0 if no unwind is found but the sender is. */

        /* begin findUnwindThroughContext: */
        onSamePage =
            (/* isStillMarriedContext: */
             (((((fetchPointerofObject(SenderIndex, home))) & 7) == 1)) &&
             (!(isWidowedContext(home)))) &&
            ((pageIndexForstackMemorybytesPerPage(framePointer, stackMemory,
                                                  bytesPerPage)) ==
             (pageIndexFor(frameOfMarriedContext(home))));

        /* Since nothing changes we don't need to internalize. */

        /* begin findMethodWithPrimitive:FromFP:UpToContext: */
        theFP1 = framePointer;
        theFPAbove = framePointer;
        do {
          if (((byteAt((theFP1 + FoxFrameFlags) + 2)) != 0) &&
              (home == (longAt(theFP1 + FoxThisContext)))) {
            ctxtOrNilOrZero = 0;
            goto l159;
          }
          if (!(byteAt((theFP1 + FoxFrameFlags) + 3))) {
            theMethodSqInt = longAt(theFP1 + FoxMethod);
            if ((primitiveIndexOf(theMethodSqInt)) == PrimNumberUnwindMarker) {
              if (theFP1 == theFPAbove) {
                theSP = findSPOfon(theFP1, stackPageFor(theFP1));
              } else {
                /* begin frameCallerStackPointer: */
                assert(!(isBaseFrame(theFPAbove)));
                theSP =
                    (theFPAbove +
                     ((FoxCallerSavedIP + BytesPerWord) +
                      ((((usqInt)((byteAt((theFPAbove + FoxFrameFlags) + 1)))
                         << (shiftForWord())))))) +
                    BytesPerWord;
              }

              /* begin ensureFrameIsMarried:SP: */
              if (byteAt((theFP1 + FoxFrameFlags) + 2)) {
                assert(isContext(frameContext(theFP1)));
                ctxtOrNilOrZero = longAt(theFP1 + FoxThisContext);
                goto l159;
              }
              ctxtOrNilOrZero = marryFrameSP(theFP1, theSP);
              goto l159;
            }
          }
          theFPAbove = theFP1;
          theFP1 = ((char *)(longAt(theFP1 + FoxSavedFP)));
        } while (theFP1 != 0);

        /* begin frameCallerContext: */
        assert(isBaseFrame(theFPAbove));
        senderContext = longAt(theFPAbove + FoxCallerContext);
        if (!(/* isContext: */
              ((!(senderContext & (tagMask())))) &&
              (((longAt((void *)(senderContext))) & (classIndexMask())) ==
               ClassMethodContextCompactIndex))) {
          ctxtOrNilOrZero = nilObj;
          goto l159;
        }
        ctxtOrNilOrZero = findMethodWithPrimitiveFromContextUpToContext(
            PrimNumberUnwindMarker, senderContext, home);
        /* end findMethodWithPrimitive:FromFP:UpToContext: */
      l159:
        assert(!((onSamePage && (ctxtOrNilOrZero == (nilObject())))));
        if (!ctxtOrNilOrZero) {
          theMethod = fetchPointerofObject(MethodIndex, home);
          if ((primitiveIndexOf(theMethod)) == PrimNumberUnwindMarker) {
            unwindContextOrNilOrZero = home;
            goto l155;
          }
          unwindContextOrNilOrZero = 0;
          goto l155;
        }

        /* If an unwind was found, can the home context be found also?  No need
           to look if on the same page. No need to look if cannot return
           (ctxtOrNilOrZero = objectMemory nilObject) */
        if (!(onSamePage || (ctxtOrNilOrZero == nilObj))) {
          if ((findMethodWithPrimitiveFromContextUpToContext(0, ctxtOrNilOrZero,
                                                             home)) == nilObj) {
            unwindContextOrNilOrZero = nilObj;
            goto l155;
          }
        }
        unwindContextOrNilOrZero = ctxtOrNilOrZero;
        /* end findUnwindThroughContext: */
      l155:
        if (unwindContextOrNilOrZero == nilObj) {
          /* begin internalCannotReturn: */
          /* begin ensureFrameIsMarried:SP: */
          if (byteAt((localFP + FoxFrameFlags) + 2)) {
            assert(isContext(frameContext(localFP)));
            ourContext = longAt(localFP + FoxThisContext);
            goto l161;
          }
          ourContext = marryFrameSP(localFP, localSP);
          /* end ensureFrameIsMarried:SP: */
        l161:

          /* begin internalPush: */
          longAtput((localSP -= BytesPerOop), ourContext);
          longAtput((localSP -= BytesPerOop), localReturnValue);
          messageSelector =
              fetchPointerofObject(SelectorCannotReturn, specialObjectsOop);
          argumentCount = 1;
          goto normalSend;
          goto l154;
        }

        /* error: can't find home on chain; cannot return */
        if (unwindContextOrNilOrZero) {
          /* begin internalAboutToReturn:through: */
          /* begin ensureFrameIsMarried:SP: */
          if (byteAt((localFP + FoxFrameFlags) + 2)) {
            assert(isContext(frameContext(localFP)));
            ourContext = longAt(localFP + FoxThisContext);
            goto l157;
          }
          ourContext = marryFrameSP(localFP, localSP);
          /* end ensureFrameIsMarried:SP: */
        l157:

          /* begin internalPush: */
          longAtput((localSP -= BytesPerOop), ourContext);
          longAtput((localSP -= BytesPerOop), localReturnValue);
          longAtput((localSP -= BytesPerOop), unwindContextOrNilOrZero);
          messageSelector =
              fetchPointerofObject(SelectorAboutToReturn, specialObjectsOop);
          argumentCount = 2;
          goto normalSend;
          goto l154;
        }

        /* Now we know home is on the sender chain.
           We could be returning to either a context or a frame.  Find out
           which. */
        contextToReturnTo = null;
        if (((((fetchPointerofObject(SenderIndex, home))) & 7) == 1)) {
          assert(checkIsStillMarriedContextcurrentFP(home, localFP));

          /* begin frameOfMarriedContext: */
          senderOop = fetchPointerofObject(SenderIndex, home);
          assert((((senderOop) & 7) == 1));
          theFP = ((char *)(senderOop - (smallIntegerTag())));
          if (longAt(theFP + FoxSavedFP)) {
            frameToReturnTo = ((char *)(longAt(theFP + FoxSavedFP)));
          } else {
            /* begin frameCallerContext: */
            assert(isBaseFrame(theFP));
            contextToReturnTo = longAt(theFP + FoxCallerContext);
          }
        } else {
          contextToReturnTo = fetchPointerofObject(SenderIndex, home);
          if ((/* isContext: */
               ((!(contextToReturnTo & (tagMask())))) &&
               (((longAt((void *)(contextToReturnTo))) & (classIndexMask())) ==
                ClassMethodContextCompactIndex)) &&
              (((((fetchPointerofObject(SenderIndex, contextToReturnTo))) &
                 7) == 1))) {
            assert(checkIsStillMarriedContextcurrentFP(contextToReturnTo,
                                                       localFP));

            /* begin frameOfMarriedContext: */
            senderOop = fetchPointerofObject(SenderIndex, contextToReturnTo);
            assert((((senderOop) & 7) == 1));
            frameToReturnTo = ((char *)(senderOop - (smallIntegerTag())));
            contextToReturnTo = null;
          }
        }

        /* If returning to a context we must make a frame for it unless it is
         * dead. */
        if (contextToReturnTo) {
          /* begin establishFrameForContextToReturnTo: */
          if (!(/* isContext: */
                ((!(contextToReturnTo & (tagMask())))) &&
                (((longAt((void *)(contextToReturnTo))) & (classIndexMask())) ==
                 ClassMethodContextCompactIndex))) {
            frameToReturnTo = 0;
            goto l156;
          }
          if (((((fetchPointerofObject(SenderIndex, contextToReturnTo))) & 7) ==
               1)) {
            if (isWidowedContext(contextToReturnTo)) {
              frameToReturnTo = 0;
              goto l156;
            }

            /* error: home's sender is dead; cannot return */

            /* begin frameOfMarriedContext: */
            senderOop = fetchPointerofObject(SenderIndex, contextToReturnTo);
            assert((((senderOop) & 7) == 1));
            frameToReturnTo = ((char *)(senderOop - (smallIntegerTag())));
            goto l156;
          }
          if (!(((((fetchPointerofObject(InstructionPointerIndex,
                                         contextToReturnTo))) &
                  7) == 1))) {
            frameToReturnTo = 0;
            goto l156;
          }
          thePage1 = makeBaseFrameFor(contextToReturnTo);
          markStackPageMostRecentlyUsed(thePage1);
          frameToReturnTo = (thePage1->baseFP);
          /* end establishFrameForContextToReturnTo: */
        l156:
          if (!frameToReturnTo) {
            /* begin internalCannotReturn: */
            /* begin ensureFrameIsMarried:SP: */
            if (byteAt((localFP + FoxFrameFlags) + 2)) {
              assert(isContext(frameContext(localFP)));
              ourContext = longAt(localFP + FoxThisContext);
              goto l160;
            }
            ourContext = marryFrameSP(localFP, localSP);
            /* end ensureFrameIsMarried:SP: */
          l160:

            /* begin internalPush: */
            longAtput((localSP -= BytesPerOop), ourContext);
            longAtput((localSP -= BytesPerOop), localReturnValue);
            messageSelector =
                fetchPointerofObject(SelectorCannotReturn, specialObjectsOop);
            argumentCount = 1;
            goto normalSend;
            goto l154;
          }
        }

        /* Now we have a frame to return to.  If it is on a different page we
           must free intervening pages and nil out intervening contexts.  We
           must free intervening stack pages because if we leave the pages to be
           divorced then their contexts will be divorced with intact senders and
           instruction pointers.  This code is similar to primitiveTerminateTo.
           We must move any frames on intervening pages above the frame linked
           to because these may be in use, e.g. via co-routining (see
           baseFrameReturn). */
        assert(pageListIsWellFormed());

        /* begin stackPageFor: */
        newPage =
            stackPageAtpages(pageIndexForstackMemorybytesPerPage(
                                 frameToReturnTo, stackMemory, bytesPerPage),
                             pages);
        if (newPage != stackPage) {
          /* begin frameCallerContext: */
          assert(isBaseFrame((stackPage->baseFP)));
          currentCtx = longAt(((stackPage->baseFP)) + FoxCallerContext);
          freeStackPage(stackPage);
          while (1) {
            assert(isContext(currentCtx));
            if ((((((fetchPointerofObject(SenderIndex, currentCtx))) & 7) ==
                  1)) &&
                ((stackPageFor((theFP = frameOfMarriedContext(currentCtx)))) ==
                 newPage))
              break;
            if (((((fetchPointerofObject(SenderIndex, currentCtx))) & 7) ==
                 1)) {
              /* begin stackPageFor: */
              thePage = stackPageAtpages(pageIndexForstackMemorybytesPerPage(
                                             theFP, stackMemory, bytesPerPage),
                                         pages);
              if (theFP != ((thePage->headFP))) {
                moveFramesInthroughtoPage(thePage,
                                          findFrameAboveinPage(theFP, thePage),
                                          newStackPage());
              }

              /* Since we've just deallocated a page we know that newStackPage
               * won't deallocate an existing one. */

              /* begin frameCallerContext: */
              assert(isBaseFrame((thePage->baseFP)));
              currentCtx = longAt(((thePage->baseFP)) + FoxCallerContext);
              freeStackPage(thePage);
            } else {
              nextCntx = fetchPointerofObject(SenderIndex, currentCtx);

              /* begin markContextAsDead: */
              assert(isContext(currentCtx));
              assert((isNonImmediate(currentCtx)) &&
                     (!(isForwarded(currentCtx))));
              assert(validStorePointerUncheckedArgs(SenderIndex, currentCtx,
                                                    nilObj));
              longAtput(
                  (void *)((currentCtx + BaseHeaderSize) +
                           ((((usqInt)(SenderIndex) << (shiftForWord()))))),
                  nilObj);

              /* begin storePointerUnchecked:ofObject:withValue: */
              assert((isNonImmediate(currentCtx)) &&
                     (!(isForwarded(currentCtx))));
              assert(validStorePointerUncheckedArgs(InstructionPointerIndex,
                                                    currentCtx, nilObj));
              longAtput((void *)((currentCtx + BaseHeaderSize) +
                                 ((((usqInt)(InstructionPointerIndex)
                                    << (shiftForWord()))))),
                        nilObj);
              currentCtx = nextCntx;
            }
          }

          /* begin setStackPageAndLimit: */
          assert(newPage);
          stackPage = newPage;
          if (stackLimit != (((char *)(((usqInt)-1))))) {
            stackLimit = (stackPage->stackLimit);
          }
          markStackPageMostRecentlyUsed(newPage);
          localSP = (stackPage->headSP);
          localFP = (stackPage->headFP);
        }

        /* Two cases.  Returning to the top frame on a new page or an interior
           frame on the current page. The top frame has its instruction pointer
           on top of stack. An interior frame has its instruction pointer in the
           caller frame. We need to peel back any frames on the page until we
           get to the correct frame. */
        if (localFP == frameToReturnTo) {
          localIP = ((char *)(longAt(localSP)));
        } else {
          do {
            callerFP = localFP;
            localFP = ((char *)(longAt(localFP + FoxSavedFP)));
          } while (localFP != frameToReturnTo);
          localIP = ((char *)(longAt(callerFP + FoxCallerSavedIP)));
          localSP = ((/* begin frameCallerSP: */
                      assert(!(isBaseFrame(callerFP))),
                      (callerFP +
                       ((FoxCallerSavedIP + BytesPerWord) +
                        ((((usqInt)((byteAt((callerFP + FoxFrameFlags) + 1)))
                           << (shiftForWord())))))) +
                          BytesPerWord)) -
                    BytesPerWord;
        }

        /* pop the saved IP, push the return value and continue. */

        /* begin setMethod: */
        method = longAt(localFP + FoxMethod);
        assert(isOopCompiledMethod(method));

        /* begin methodUsesAlternateBytecodeSet: */
        /* begin methodHeaderOf: */
        assert(isCompiledMethod(method));
        methodHeader = fetchPointerofObject(HeaderIndex, method);
        if ((((sqLong)methodHeader)) < 0) {
          bytecodeSetSelector = 0x100;
        } else {
          bytecodeSetSelector = 0;
        }

        /* begin fetchNextBytecode */
        currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;

        /* internalStackTopPut: */
        longAtput(localSP, localReturnValue);
      }
      /* end case */
    l154:;
      break;
    case 121: // returnTrue
    case 345: // 89	returnTrue
    {
      VM_LABEL(returnTrue);
      localReturnValue = trueObj;
      goto commonReturn;
    } break;
    case 122: // returnFalse
    case 346: // 90	returnFalse
    {
      VM_LABEL(returnFalse);
      localReturnValue = falseObj;
      goto commonReturn;
    } break;
    case 123: // returnNil
    case 347: // 91	returnNil
    {
      VM_LABEL(returnNil);
      localReturnValue = nilObj;
      goto commonReturn;
    } break;
    case 124: // returnTopFromMethod
    case 348: // 92	returnTopFromMethod
    {
      VM_LABEL(returnTopFromMethod);
      localReturnValue = longAt(localSP);
      goto commonReturn;
    } break;
    case 125: // returnTopFromBlock
    {
      VM_LABEL(returnTopFromBlock);
      localReturnValue = longAt(localSP);

      /* goto commonCallerReturn */
    }

    commonCallerReturn:
      /* commonCallerReturn */
      {
        char *callerFP;
        char *callersFPOrNull;
        sqInt contextToReturnTo;
        char *fp;
        char *frameAbove;
        sqInt isAContext;
        sqInt methodHeader;
        sqInt ourContext;
        sqInt senderOop;
        char *theFP;
        StackPage *thePage;
        char *theSP;

        VM_LABEL(commonCallerReturn);

        /* TODO: Store/restore the nativeSP more properly, when it exists */
        callersFPOrNull = ((char *)(longAt(localFP + FoxSavedFP)));
        if (!callersFPOrNull) {
          assert(localFP == ((stackPage->baseFP)));

          /* begin baseFrameReturn */
          /* begin frameCallerContext: */
          assert(isBaseFrame(localFP));
          contextToReturnTo = longAt(localFP + FoxCallerContext);
          isAContext = /* isContext: */
              ((!(contextToReturnTo & (tagMask())))) &&
              (((longAt((void *)(contextToReturnTo))) & (classIndexMask())) ==
               ClassMethodContextCompactIndex);
          if (isAContext &&
              (/* isStillMarriedContext: */
               (((((fetchPointerofObject(SenderIndex, contextToReturnTo))) &
                  7) == 1)) &&
               (!(isWidowedContext(contextToReturnTo))))) {
            /* begin frameOfMarriedContext: */
            senderOop = fetchPointerofObject(SenderIndex, contextToReturnTo);
            assert((((senderOop) & 7) == 1));
            theFP = ((char *)(senderOop - (smallIntegerTag())));

            /* begin stackPageFor: */
            thePage = stackPageAtpages(pageIndexForstackMemorybytesPerPage(
                                           theFP, stackMemory, bytesPerPage),
                                       pages);
            if (theFP == ((thePage->headFP))) {
              theSP = (thePage->headSP);
              freeStackPageNoAssert(stackPage);
            } else {
              /* begin findFrameAbove:inPage: */
              callerFP = ((char *)0);
              fp = (thePage->headFP);
              if (fp == theFP) {
                frameAbove = 0;
                goto l163;
              }
              while (((callerFP = ((char *)(longAt(fp + FoxSavedFP))))) != 0) {
                if (callerFP == theFP) {
                  frameAbove = fp;
                  goto l163;
                }
                fp = callerFP;
              }
              error("did not find theFP in stack page");
              frameAbove = 0;
              /* end findFrameAbove:inPage: */
            l163:

              /* Reuse the page we're exiting, which avoids allocating a new
                 page and manipulating the page list to mark the page we're
                 entering as least recently used (to avoid it being deallocated
                 when allocating a new page). */
              moveFramesInthroughtoPage(thePage, frameAbove, stackPage);
              theFP = (thePage->headFP);
              theSP = (thePage->headSP);
            }
          } else {
            if (!(isAContext &&
                  (((((fetchPointerofObject(InstructionPointerIndex,
                                            contextToReturnTo))) &
                     7) == 1)))) {
              /* begin internalCannotReturn: */
              /* begin ensureFrameIsMarried:SP: */
              if (byteAt((localFP + FoxFrameFlags) + 2)) {
                assert(isContext(frameContext(localFP)));
                ourContext = longAt(localFP + FoxThisContext);
                goto l164;
              }
              ourContext = marryFrameSP(localFP, localSP);
              /* end ensureFrameIsMarried:SP: */
            l164:

              /* begin internalPush: */
              longAtput((localSP -= BytesPerOop), ourContext);
              longAtput((localSP -= BytesPerOop), localReturnValue);
              messageSelector =
                  fetchPointerofObject(SelectorCannotReturn, specialObjectsOop);
              argumentCount = 1;
              goto normalSend;
              goto l162;
            }
            thePage = makeBaseFrameFor(contextToReturnTo);
            theFP = (thePage->headFP);
            theSP = (thePage->headSP);
            freeStackPageNoAssert(stackPage);
          }

          /* begin setStackPageAndLimit: */
          assert(thePage);
          stackPage = thePage;
          if (stackLimit != (((char *)(((usqInt)-1))))) {
            stackLimit = (stackPage->stackLimit);
          }
          markStackPageMostRecentlyUsed(thePage);
          assert((stackPageFor(theFP)) == stackPage);
          localSP = theSP;
          localFP = theFP;

          /* begin setMethod: */
          method = longAt(localFP + FoxMethod);
          assert(isOopCompiledMethod(method));

          /* begin methodUsesAlternateBytecodeSet: */
          /* begin methodHeaderOf: */
          assert(isCompiledMethod(method));
          methodHeader = fetchPointerofObject(HeaderIndex, method);
          if ((((sqLong)methodHeader)) < 0) {
            bytecodeSetSelector = 0x100;
          } else {
            bytecodeSetSelector = 0;
          }
          localIP = ((char *)(longAt(localSP)));

          /* internalStackTopPut: */
          longAtput(localSP, localReturnValue);
          assert(
              checkIsStillMarriedContextcurrentFP(contextToReturnTo, localFP));

          /* begin fetchNextBytecode */
          currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
          goto l162;
        }

        /* baseFrame */
        localIP = ((char *)(longAt(localFP + FoxCallerSavedIP)));
        localSP =
            localFP + ((FoxCallerSavedIP + BytesPerWord) +
                       ((((usqInt)((byteAt((localFP + FoxFrameFlags) + 1)))
                          << (shiftForWord())))));
        localFP = callersFPOrNull;

        /* begin setMethod: */
        method = longAt(localFP + FoxMethod);
        assert(isOopCompiledMethod(method));

        /* begin methodUsesAlternateBytecodeSet: */
        /* begin methodHeaderOf: */
        assert(isCompiledMethod(method));
        methodHeader = fetchPointerofObject(HeaderIndex, method);
        if ((((sqLong)methodHeader)) < 0) {
          bytecodeSetSelector = 0x100;
        } else {
          bytecodeSetSelector = 0;
        }

        /* begin fetchNextBytecode */
        currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;

        /* internalStackTopPut: */
        longAtput(localSP, localReturnValue);
      }
      /* end case */
    l162:;
      break;
    case 126: // unknownBytecode
    case 127: // unknownBytecode

    respondToUnknownBytecode:
      /* respondToUnknownBytecode */
      {
        usqInt numSlots;
        sqInt ourContext;

        VM_LABEL(respondToUnknownBytecode);
        messageSelector = /* maybeSplObj: */
            (SelectorUnknownBytecode <
                     ((assert((classIndexOf(specialObjectsOop)) >
                              (isForwardedObjectClassIndexPun())),
                       numSlotsOf(specialObjectsOop)))
                 ? fetchPointerofObject(SelectorUnknownBytecode,
                                        specialObjectsOop)
                 : 0);
        if ((!messageSelector) || (messageSelector == nilObj)) {
          error("Unknown bytecode");
        }

        /* begin ensureFrameIsMarried:SP: */
        if (byteAt((localFP + FoxFrameFlags) + 2)) {
          assert(isContext(frameContext(localFP)));
          ourContext = longAt(localFP + FoxThisContext);
          goto l165;
        }
        ourContext = marryFrameSP(localFP, localSP);
        /* end ensureFrameIsMarried:SP: */
      l165:

        /* undo fetch of bytecode so that context's pc is pointing to the
         * unknown bytecode. */
        localIP -= 1;

        /* begin internalPush: */
        longAtput((localSP -= BytesPerOop), ourContext);
        argumentCount = 0;
        goto normalSend;
      }
      break;
    case 128: // extendedPushBytecode
    {
      sqInt descriptor;
      usqInt frameNumArgs;
      sqInt litVar;
      sqInt object;
      usqInt variableIndex;
      usqInt variableType;

      VM_LABEL(extendedPushBytecode);
      descriptor = byteAt(++localIP);

      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      variableType = (((usqInt)(descriptor)) >> 6) & 3;
      variableIndex = descriptor & 0x3F;
      if (!variableType) {
        /* begin pushReceiverVariable: */
        /* begin internalPush: */
        longAtput(
            (localSP -= BytesPerOop),
            fetchPointerofObject(variableIndex, longAt(localFP + FoxReceiver)));
        goto l2;
      }
      if (variableType == 1) {
        /* begin pushTemporaryVariable: */
        object = /* temporary:in: */
            (variableIndex <
                     ((frameNumArgs = byteAt((localFP + FoxFrameFlags) + 1)))
                 ? longAt((localFP + FoxCallerSavedIP) +
                          ((frameNumArgs - variableIndex) * BytesPerWord))
                 : longAt(((localFP + FoxReceiver) - BytesPerWord) +
                          ((frameNumArgs - variableIndex) * BytesPerWord)));
        longAtput((localSP -= BytesPerOop), object);
        goto l2;
      }
      if (variableType == 2) {
        /* begin pushLiteralConstant: */
        /* begin literal: */
        assert(method == (iframeMethod(localFP)));
        object = fetchPointerofObject(variableIndex + LiteralStart, method);

        /* begin internalPush: */
        longAtput((localSP -= BytesPerOop), object);
        goto l2;
      }
      if (variableType == 3) {
        /* begin pushLiteralVariable: */
        /* begin followObjLiteral:ofMethod: */
        /* begin followObjField:ofObject: */
        litVar = fetchPointerofObject(variableIndex + LiteralStart, method);
        assert(isNonImmediate(litVar));
        if ((!((longAt((void *)(litVar))) &
               ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
          litVar = fixFollowedFieldofObjectwithInitialValue(
              variableIndex + LiteralStart, method, litVar);
        }

        /* begin internalPush: */
        longAtput((localSP -= BytesPerOop),
                  fetchPointerofObject(ValueIndex, litVar));
        goto l2;
      }
    }
      /* end case */
    l2:;
      break;
    case 129: // extendedStoreBytecode
    {
      sqInt descriptor;
      usqInt frameNumArgs;
      sqInt litVar;
      sqInt objOop;
      sqInt value;
      usqInt variableIndex;

      VM_LABEL(extendedStoreBytecode);
      /* begin extendedStoreBytecodePop: */
      descriptor = byteAt(++localIP);
      variableIndex = descriptor & 0x3F;
      value = longAt(localSP);
      switch ((((usqInt)(descriptor)) >> 6) & 3) {
      case 0:
        objOop = longAt(localFP + FoxReceiver);

        /* begin storePointerImmutabilityCheck:ofObject:withValue: */
#if IMMUTABILITY
        assert(!((isImmediate(objOop))));
        if ((((usqInt)((byteAt(
                 (void *)(objOop + (immutableExtraBitsByteOffset())))))) >>
             (immutableBitByteShift())) &
            1) {
          /* begin cannotAssign:to:withIndex: */
          /* begin internalPush: */
          longAtput((localSP -= BytesPerOop), objOop);
          longAtput((localSP -= BytesPerOop), value);
          longAtput((localSP -= BytesPerOop), (((variableIndex + 1) << 3) | 1));
          messageSelector =
              fetchPointerofObject(SelectorAttemptToAssign, specialObjectsOop);
          argumentCount = 2;
          goto normalSend;
          goto l3;
        }
#endif // IMMUTABILITY

        /* begin storePointer:ofObject:withValue: */
        assert(validStorePointerArgs(variableIndex, objOop, value));
        assert(isNonImmediate(objOop));
        if (oopisGreaterThanOrEqualTo(objOop, oldSpaceStart)) {
          if (/* isYoung: */
              ((!(value & (tagMask())))) &&
              (oopisLessThan(value, oldSpaceStart))) {
            /* begin possibleRootStoreInto: */
            if (!((byteAt((void *)(objOop + (formatFieldByteOffset())))) &
                  (1U << (rememberedBitByteShift())))) {
              remember(objOop);
            }
          }
        }

        /* most stores into young objects */
        longAtput((void *)((objOop + BaseHeaderSize) +
                           ((variableIndex << (shiftForWord())))),
                  value);
        /* end storePointerImmutabilityCheck:ofObject:withValue: */
      l3:;
        break;
      case 1:
        /* temporary:in:put: */
        if (variableIndex <
            ((frameNumArgs = byteAt((localFP + FoxFrameFlags) + 1)))) {
          longAtput((localFP + FoxCallerSavedIP) +
                        ((frameNumArgs - variableIndex) * BytesPerWord),
                    value);
        } else {
          longAtput(((localFP + FoxReceiver) - BytesPerWord) +
                        ((frameNumArgs - variableIndex) * BytesPerWord),
                    value);
        }
        break;
      case 2:
        error("illegal store");
        break;
      case 3:
        /* begin storeLiteralVariable:withValue: */
        /* begin followObjLiteral:ofMethod: */
        /* begin followObjField:ofObject: */
        litVar = fetchPointerofObject(variableIndex + LiteralStart, method);
        assert(isNonImmediate(litVar));
        if ((!((longAt((void *)(litVar))) &
               ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
          litVar = fixFollowedFieldofObjectwithInitialValue(
              variableIndex + LiteralStart, method, litVar);
        }

        /* begin storePointerImmutabilityCheck:ofObject:withValue: */
#if IMMUTABILITY
        assert(!((isImmediate(litVar))));
        if ((((usqInt)((byteAt(
                 (void *)(litVar + (immutableExtraBitsByteOffset())))))) >>
             (immutableBitByteShift())) &
            1) {
          /* begin cannotAssign:to:withIndex: */
          /* begin internalPush: */
          longAtput((localSP -= BytesPerOop), litVar);
          longAtput((localSP -= BytesPerOop), value);
          longAtput((localSP -= BytesPerOop),
                    (((usqInt)2 /* (ValueIndex + 1) */ << 3) | 1));
          messageSelector =
              fetchPointerofObject(SelectorAttemptToAssign, specialObjectsOop);
          argumentCount = 2;
          goto normalSend;
          goto l4;
        }
#endif // IMMUTABILITY

        /* begin storePointer:ofObject:withValue: */
        assert(validStorePointerArgs(ValueIndex, litVar, value));
        assert(isNonImmediate(litVar));
        if (oopisGreaterThanOrEqualTo(litVar, oldSpaceStart)) {
          if (/* isYoung: */
              ((!(value & (tagMask())))) &&
              (oopisLessThan(value, oldSpaceStart))) {
            /* begin possibleRootStoreInto: */
            if (!((byteAt((void *)(litVar + (formatFieldByteOffset())))) &
                  (1U << (rememberedBitByteShift())))) {
              remember(litVar);
            }
          }
        }

        /* most stores into young objects */
        longAtput((void *)((litVar + BaseHeaderSize) +
                           ((((usqInt)(ValueIndex) << (shiftForWord()))))),
                  value);
        /* end storePointerImmutabilityCheck:ofObject:withValue: */
      l4:;
        break;
      default:
        error("Case not found and no otherwise clause");
      }

      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
    } break;
    case 130: // extendedStoreAndPopBytecode
    {
      sqInt descriptor;
      usqInt frameNumArgs;
      sqInt litVar;
      sqInt objOop;
      sqInt value;
      usqInt variableIndex;

      VM_LABEL(extendedStoreAndPopBytecode);
      /* begin extendedStoreBytecodePop: */
      descriptor = byteAt(++localIP);
      variableIndex = descriptor & 0x3F;
      value = longAt(localSP);

      /* begin internalPop: */
      localSP += 1 * BytesPerOop;
      switch ((((usqInt)(descriptor)) >> 6) & 3) {
      case 0:
        objOop = longAt(localFP + FoxReceiver);

        /* begin storePointerImmutabilityCheck:ofObject:withValue: */
#if IMMUTABILITY
        assert(!((isImmediate(objOop))));
        if ((((usqInt)((byteAt(
                 (void *)(objOop + (immutableExtraBitsByteOffset())))))) >>
             (immutableBitByteShift())) &
            1) {
          /* begin cannotAssign:to:withIndex: */
          /* begin internalPush: */
          longAtput((localSP -= BytesPerOop), objOop);
          longAtput((localSP -= BytesPerOop), value);
          longAtput((localSP -= BytesPerOop), (((variableIndex + 1) << 3) | 1));
          messageSelector =
              fetchPointerofObject(SelectorAttemptToAssign, specialObjectsOop);
          argumentCount = 2;
          goto normalSend;
          goto l5;
        }
#endif // IMMUTABILITY

        /* begin storePointer:ofObject:withValue: */
        assert(validStorePointerArgs(variableIndex, objOop, value));
        assert(isNonImmediate(objOop));
        if (oopisGreaterThanOrEqualTo(objOop, oldSpaceStart)) {
          if (/* isYoung: */
              ((!(value & (tagMask())))) &&
              (oopisLessThan(value, oldSpaceStart))) {
            /* begin possibleRootStoreInto: */
            if (!((byteAt((void *)(objOop + (formatFieldByteOffset())))) &
                  (1U << (rememberedBitByteShift())))) {
              remember(objOop);
            }
          }
        }

        /* most stores into young objects */
        longAtput((void *)((objOop + BaseHeaderSize) +
                           ((variableIndex << (shiftForWord())))),
                  value);
        /* end storePointerImmutabilityCheck:ofObject:withValue: */
      l5:;
        break;
      case 1:
        /* temporary:in:put: */
        if (variableIndex <
            ((frameNumArgs = byteAt((localFP + FoxFrameFlags) + 1)))) {
          longAtput((localFP + FoxCallerSavedIP) +
                        ((frameNumArgs - variableIndex) * BytesPerWord),
                    value);
        } else {
          longAtput(((localFP + FoxReceiver) - BytesPerWord) +
                        ((frameNumArgs - variableIndex) * BytesPerWord),
                    value);
        }
        break;
      case 2:
        error("illegal store");
        break;
      case 3:
        /* begin storeLiteralVariable:withValue: */
        /* begin followObjLiteral:ofMethod: */
        /* begin followObjField:ofObject: */
        litVar = fetchPointerofObject(variableIndex + LiteralStart, method);
        assert(isNonImmediate(litVar));
        if ((!((longAt((void *)(litVar))) &
               ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
          litVar = fixFollowedFieldofObjectwithInitialValue(
              variableIndex + LiteralStart, method, litVar);
        }

        /* begin storePointerImmutabilityCheck:ofObject:withValue: */
#if IMMUTABILITY
        assert(!((isImmediate(litVar))));
        if ((((usqInt)((byteAt(
                 (void *)(litVar + (immutableExtraBitsByteOffset())))))) >>
             (immutableBitByteShift())) &
            1) {
          /* begin cannotAssign:to:withIndex: */
          /* begin internalPush: */
          longAtput((localSP -= BytesPerOop), litVar);
          longAtput((localSP -= BytesPerOop), value);
          longAtput((localSP -= BytesPerOop),
                    (((usqInt)2 /* (ValueIndex + 1) */ << 3) | 1));
          messageSelector =
              fetchPointerofObject(SelectorAttemptToAssign, specialObjectsOop);
          argumentCount = 2;
          goto normalSend;
          goto l6;
        }
#endif // IMMUTABILITY

        /* begin storePointer:ofObject:withValue: */
        assert(validStorePointerArgs(ValueIndex, litVar, value));
        assert(isNonImmediate(litVar));
        if (oopisGreaterThanOrEqualTo(litVar, oldSpaceStart)) {
          if (/* isYoung: */
              ((!(value & (tagMask())))) &&
              (oopisLessThan(value, oldSpaceStart))) {
            /* begin possibleRootStoreInto: */
            if (!((byteAt((void *)(litVar + (formatFieldByteOffset())))) &
                  (1U << (rememberedBitByteShift())))) {
              remember(litVar);
            }
          }
        }

        /* most stores into young objects */
        longAtput((void *)((litVar + BaseHeaderSize) +
                           ((((usqInt)(ValueIndex) << (shiftForWord()))))),
                  value);
        /* end storePointerImmutabilityCheck:ofObject:withValue: */
      l6:;
        break;
      default:
        error("Case not found and no otherwise clause");
      }

      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
    } break;
    case 131: // singleExtendedSendBytecode
    {
      sqInt descriptor;

      VM_LABEL(singleExtendedSendBytecode);
      descriptor = byteAt(++localIP);

      /* begin literal: */
      assert(method == (iframeMethod(localFP)));
      messageSelector =
          fetchPointerofObject((descriptor & 0x1F) + LiteralStart, method);
      argumentCount = ((usqInt)(descriptor)) >> 5;

      /* goto normalSend */
    }

    normalSend:
      /* normalSend */
      {
        sqInt rcvr;
        sqInt tagBits;

        VM_LABEL(normalSend);
        rcvr = longAt(localSP + (argumentCount * BytesPerOop));
        lkupClassTag = /* fetchClassTagOf: */
            ((tagBits = rcvr & (tagMask()))
                 ? tagBits
                 : (longAt((void *)(rcvr))) & (classIndexMask()));
        assert(lkupClassTag != (nilObject()));

      commonSendOrdinary:
        /* commonSendOrdinary */
        {
          sqInt classTablePage;
          sqInt fieldIndex;
          sqInt hash;
          sqInt i;
          sqInt localPrimIndex;
          sqInt methodHeader;
          usqInt numTemps;
          sqInt object;
          usqInt probe;
          sqInt rcvr;
          int succeeded;
          sqInt tagBits;

          VM_LABEL(commonSendOrdinary);
          rcvr = longAt(localSP + (argumentCount * BytesPerOop));

          /* begin sendBreakpoint:receiver: */
          sendBreakpointclassTag(
              firstFixedFieldOfMaybeImmediate(messageSelector),
              lengthOfMaybeImmediate(messageSelector), /* fetchClassTagOf: */
              ((tagBits = rcvr & (tagMask()))
                   ? tagBits
                   : (longAt((void *)(rcvr))) & (classIndexMask())));
          if (
#if SEND_PRINTING
              printSends
#else
              0
#endif
          ) {
            printActivationNameForSelectorstartClass(
                messageSelector, classForClassTag(lkupClassTag));
            cr();
          }

          /* begin internalFindNewMethodOrdinary */
          /* begin inlineLookupInMethodCacheSel:classTag: */
          /* shift drops two low-order zeros from addresses */
          hash = messageSelector ^ ((((usqInt)(lkupClassTag) << 2)));

          /* first probe */
          probe = hash & MethodCacheMask;
          if (((methodCache[probe + MethodCacheSelector]) == messageSelector) &&
              ((methodCache[probe + MethodCacheClass]) == lkupClassTag)) {
            newMethod = methodCache[probe + MethodCacheMethod];
            primitiveFunctionPointer =
                ((void (*)())(methodCache[probe + MethodCachePrimFunction]));
            goto l167;
          }

          /* second probe */
          probe = (((usqInt)(hash)) >> 1) & MethodCacheMask;
          if (((methodCache[probe + MethodCacheSelector]) == messageSelector) &&
              ((methodCache[probe + MethodCacheClass]) == lkupClassTag)) {
            newMethod = methodCache[probe + MethodCacheMethod];
            primitiveFunctionPointer =
                ((void (*)())(methodCache[probe + MethodCachePrimFunction]));
            goto l167;
          }
          probe = (((usqInt)(hash)) >> 2) & MethodCacheMask;
          if (((methodCache[probe + MethodCacheSelector]) == messageSelector) &&
              ((methodCache[probe + MethodCacheClass]) == lkupClassTag)) {
            newMethod = methodCache[probe + MethodCacheMethod];
            primitiveFunctionPointer =
                ((void (*)())(methodCache[probe + MethodCachePrimFunction]));
            goto l167;
          }

          /* begin externalizeIPandSP */
          instructionPointer = ((usqInt)localIP);
          stackPointer = localSP;
          framePointer = localFP;
          if ((isOopForwarded(messageSelector)) ||
              (lkupClassTag == (isForwardedObjectClassIndexPun()))) {
            if (isOopForwarded(messageSelector)) {
              messageSelector =
                  handleForwardedSelectorFaultFor(messageSelector);
            }
            if (lkupClassTag == (isForwardedObjectClassIndexPun())) {
              lkupClassTag = handleForwardedSendFaultForTag(lkupClassTag);
            }
            if (lookupInMethodCacheSelclassTag(messageSelector, lkupClassTag)) {
              goto l166;
            }
          }

          /* begin classForClassTag: */
          /* begin classAtIndex: */
          assert((lkupClassTag >= 0) &&
                 ((lkupClassTag <= (tagMask())) ||
                  ((lkupClassTag >= (arrayClassIndexPun())) &&
                   (lkupClassTag <= (classIndexMask())))));
          fieldIndex =
              ((usqInt)(lkupClassTag)) >> (classTableMajorIndexShift());

          classTablePage = fetchPointerofObject(fieldIndex, hiddenRootsObj);
          if (classTablePage == nilObj) {
            lkupClass = null;
            goto l169;
          }
          fieldIndex =
              lkupClassTag & ((1U << (classTableMajorIndexShift())) - 1);
          lkupClass = fetchPointerofObject(fieldIndex, classTablePage);
          /* end classForClassTag: */
        l169:
          lookupMethodInClass(lkupClass);

          /* begin internalizeIPandSP */
          localIP = ((char *)instructionPointer);
          localSP = stackPointer;
          localFP = framePointer;
          addNewMethodToCache(lkupClass);
        l167:
          /* end internalFindNewMethodOrdinary */
        l166:

          /* begin internalExecuteNewMethod */
          if (primitiveFunctionPointer) {
            if ((((usqIntptr_t)primitiveFunctionPointer)) <=
                MaxQuickPrimitiveIndex) {
              /* begin internalQuickPrimitiveResponse */
              assert(isPrimitiveFunctionPointerAnIndex());
              localPrimIndex = ((sqInt)primitiveFunctionPointer);
              assert((localPrimIndex > 0xFF) && (localPrimIndex < 520));

              /* Quick return inst vars */
              if (localPrimIndex >= 264) {
                longAtput(localSP, fetchPointerofObject(localPrimIndex - 264,
                                                        longAt(localSP)));
                goto l168;
              }

              /* Quick return constants */
              if (localPrimIndex == 0x100) {
                goto l168;
              }
              if (localPrimIndex == 0x101) {
                /* internalStackTopPut: */
                longAtput(localSP, trueObj);
                goto l168;
              }
              if (localPrimIndex == 258) {
                /* internalStackTopPut: */
                longAtput(localSP, falseObj);
                goto l168;
              }
              if (localPrimIndex == 259) {
                /* internalStackTopPut: */
                longAtput(localSP, nilObj);
                goto l168;
              }

              /* internalStackTopPut: */
              longAtput(localSP, (((usqInt)(localPrimIndex - 261) << 3) | 1));
              goto l168;
            }

            /* begin externalizeIPandSP */
            instructionPointer = ((usqInt)localIP);
            stackPointer = localSP;
            framePointer = localFP;

            /* begin slowPrimitiveResponse */
            assert(!(isOopForwarded(stackValue(argumentCount))));
            assert((remapBufferCount) == 0);

            /* begin initPrimCall */
            primFailCode = 0;
            if (LOGPRIMITIVES) {
              /* begin fastLogPrim: */
              primTraceLog[primTraceLogIndex] = newMethod;
              primTraceLogIndex(primTraceLogIndex + 1);
            }
            dispatchFunctionPointer(primitiveFunctionPointer);
            assert(maybeLeakCheckExternalPrimCall(newMethod));

            /* begin maybeRetryPrimitiveOnFailure */
            if (primFailCode) {
              retryPrimitiveOnFailure();
            }

            /* Don't fail if primitive has done something radical, e.g. perform:
               If we are profiling, take accurate primitive measures */
            if (nextProfileTick > 0) {
              checkProfileTickPostPrimitive(newMethod);
            }
            succeeded = !primFailCode;

            /* begin internalizeIPandSP */
            localIP = ((char *)instructionPointer);
            localSP = stackPointer;
            localFP = framePointer;
            if (succeeded) {
              browserPluginReturnIfNeeded();
              goto l168;
            }
          }

          /* if not primitive, or primitive failed, activate the method */

          /* begin internalActivateNewMethod */
          /* begin methodHeaderOf: */
          assert(isCompiledMethod(newMethod));
          methodHeader = fetchPointerofObject(HeaderIndex, newMethod);
          numTemps =
              (((usqInt)(methodHeader)) >> MethodHeaderTempCountShift) & 0x3F;
          assert(argumentCount == (argumentCountOfMethodHeader(methodHeader)));

          /* could new rcvr be set at point of send? */
          rcvr = longAt(localSP + (argumentCount * BytesPerOop));
          assert(!(isOopForwarded(rcvr)));

          /* begin internalPush: */
          longAtput((localSP -= BytesPerOop), ((usqInt)localIP));
          longAtput((localSP -= BytesPerOop), ((usqInt)localFP));
          localFP = localSP;

          /* begin internalPush: */
          longAtput((localSP -= BytesPerOop), newMethod);
          method = newMethod;
          assert(isOopCompiledMethod(method));
          assert((methodHeaderOf(method)) == methodHeader);
          bytecodeSetSelector = ((((sqLong)methodHeader)) < 0 ? 0x100 : 0);
          object = /* encodeFrameFieldHasContext:isBlock:numArgs: */
              (VMBIGENDIAN ? ((1 + ((((((usqInt)(methodHeader)) >>
                                       MethodHeaderArgCountShift) &
                                      15)
                                     << ((BytesPerWord * 8) - 8)))))
                           : ((1 + ((((((usqInt)(methodHeader)) >>
                                       MethodHeaderArgCountShift) &
                                      15)
                                     << 8)))));

          /* begin internalPush: */
          longAtput((localSP -= BytesPerOop), object);
          longAtput((localSP -= BytesPerOop), nilObj);
          longAtput((localSP -= BytesPerOop), rcvr);

          /* Initialize temps... */
          for (i = (argumentCount + 1); i <= numTemps; i += 1) {
            /* begin internalPush: */
            longAtput((localSP -= BytesPerOop), nilObj);
          }

          /* -1 to account for pre-increment in fetchNextBytecode */
          localIP = (((char *)(((((usqInt)(pointerForOop(newMethod)))) +
                                ((LiteralStart +
                                  ((/* begin literalCountOfMethodHeader: */
                                    assert((((methodHeader) & 7) == 1)),
                                    /* literalCountOfAlternateHeader: */
                                    ((methodHeader >> 3)) &
                                        AlternateHeaderNumLiteralsMask))) *
                                 BytesPerOop)) +
                               BaseHeaderSize))) -
                    1;
          if (((methodHeader & AlternateHeaderHasPrimFlag) != 0)) {
            localIP += 3 /* sizeOfCallPrimitiveBytecode: */;
            if (primFailCode) {
              reapAndResetErrorCodeToheader(localSP, methodHeader);
            }
          }

          /* Skip the CallPrimitive bytecode, if it's there, and store the error
             code if the method starts with a long store temp.  Strictly no need
             to skip the store because it's effectively a noop. */
          assert((frameNumArgs(localFP)) == argumentCount);
          assert(!(frameIsBlockActivation(localFP)));
          assert(!(frameHasContext(localFP)));

          /* Now check for stack overflow or an event (interrupt, must scavenge,
           * etc). */
          if (localSP < stackLimit) {
            /* begin externalizeIPandSP */
            instructionPointer = ((usqInt)localIP);
            stackPointer = localSP;
            framePointer = localFP;
            handleStackOverflowOrEventAllowContextSwitch(
                canContextSwitchIfActivatingheader(newMethod, methodHeader));

            /* begin internalizeIPandSP */
            localIP = ((char *)instructionPointer);
            localSP = stackPointer;
            localFP = framePointer;
          }
          /* end internalExecuteNewMethod */
        l168:

          /* begin fetchNextBytecode */
          currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
        }
      }
      break;
    case 132: // doubleExtendedDoAnythingBytecode
    {
      sqInt byte2;
      sqInt byte3;
      char *callerFP;
      sqInt litVar;
      sqInt obj;
      sqInt object;
      int onCurrentPage;
      sqInt opType;
      sqInt senderOop;
      char *spouseFP;
      char *theFP;
      StackPage *thePage;
      sqInt top;

      VM_LABEL(doubleExtendedDoAnythingBytecode);
      byte2 = byteAt(++localIP);
      byte3 = byteAt(++localIP);
      opType = ((usqInt)(byte2)) >> 5;
      if (!opType) {
        /* begin literal: */
        assert(method == (iframeMethod(localFP)));
        messageSelector = fetchPointerofObject(byte3 + LiteralStart, method);
        argumentCount = byte2 & 0x1F;
        goto normalSend;
        goto l7;
      }
      if (opType == 1) {
        /* begin literal: */
        assert(method == (iframeMethod(localFP)));
        messageSelector = fetchPointerofObject(byte3 + LiteralStart, method);
        argumentCount = byte2 & 0x1F;
        goto superclassSend;
        goto l7;
      }
      if (opType == 2) {
        /* begin fetchNextBytecode */
        currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
        obj = longAt(localFP + FoxReceiver);
        if ((byte3 <= StackPointerIndex) &&
            (((longAt((void *)(obj))) & (classIndexMask())) ==
             ClassMethodContextCompactIndex)) {
          /* begin instVar:ofContext: */
          assert(byte3 < MethodIndex);
          assert(isContext(obj));
          if (!(((((fetchPointerofObject(SenderIndex, obj))) & 7) == 1))) {
            object = fetchPointerofObject(byte3, obj);
            goto l10;
          }

          /* begin writeBackHeadFramePointers */
          assert(stackPage == (mostRecentlyUsedPage));
          assert(localSP < localFP);
          assert((localSP < ((stackPage->baseAddress))) &&
                 (localSP > (((stackPage->realStackLimit)) -
                             (LargeContextSlots * BytesPerOop))));
          assert((localFP < ((stackPage->baseAddress))) &&
                 (localFP > (((stackPage->realStackLimit)) -
                             ((LargeContextSlots * BytesPerOop) / 2))));
          (stackPage->headFP = localFP);
          (stackPage->headSP = localSP);
          assert(pageListIsWellFormed());
          if (isWidowedContext(obj)) {
            object = fetchPointerofObject(byte3, obj);
            goto l10;
          }

          /* begin frameOfMarriedContext: */
          senderOop = fetchPointerofObject(SenderIndex, obj);
          assert((((senderOop) & 7) == 1));
          spouseFP = ((char *)(senderOop - (smallIntegerTag())));
          if (!byte3) {
            /* begin ensureCallerContext: */
            callerFP = ((char *)(longAt(spouseFP + FoxSavedFP)));
            if (!callerFP) {
              /* begin frameCallerContext: */
              assert(isBaseFrame(spouseFP));
              object = longAt(spouseFP + FoxCallerContext);
              goto l10;
            }

            /* base frame, context in saved ip slot (or base of stack in Cog) */

            /* begin ensureFrameIsMarried:SP: */
            if (byteAt((callerFP + FoxFrameFlags) + 2)) {
              assert(isContext(frameContext(callerFP)));
              object = longAt(callerFP + FoxThisContext);
              goto l10;
            }
            object = marryFrameSP(
                callerFP,
                (/* begin frameCallerStackPointer: */
                 assert(!(isBaseFrame(spouseFP))),
                 (spouseFP +
                  ((FoxCallerSavedIP + BytesPerWord) +
                   ((((usqInt)((byteAt((spouseFP + FoxFrameFlags) + 1)))
                      << (shiftForWord())))))) +
                     BytesPerWord));
            goto l10;
          }
          if (byte3 == StackPointerIndex) {
            assert((ReceiverIndex + (stackPointerIndexForFrame(spouseFP))) <
                   (lengthOf(obj)));
            object = (((stackPointerIndexForFrame(spouseFP)) << 3) | 1);
            goto l10;
          }
          if (byte3 == InstructionPointerIndex) {
            object = instructionPointerForFramecurrentFPcurrentIP(
                spouseFP, localFP, oopForPointer(localIP));
            goto l10;
          }
          error("bad index");
          object = 0;
          /* end instVar:ofContext: */
        l10:

          /* begin internalPush: */
          longAtput((localSP -= BytesPerOop), object);
        } else {
          /* begin internalPush: */
          longAtput((localSP -= BytesPerOop), fetchPointerofObject(byte3, obj));
        }
        goto l7;
      }
      if (opType == 3) {
        /* begin fetchNextBytecode */
        currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
        assert(method == (iframeMethod(localFP)));
        object = fetchPointerofObject(byte3 + LiteralStart, method);

        /* begin internalPush: */
        longAtput((localSP -= BytesPerOop), object);
        goto l7;
      }
      if (opType == 4) {
        /* begin fetchNextBytecode */
        currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
        litVar = fetchPointerofObject(byte3 + LiteralStart, method);
        assert(isNonImmediate(litVar));
        if ((!((longAt((void *)(litVar))) &
               ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
          litVar = fixFollowedFieldofObjectwithInitialValue(
              byte3 + LiteralStart, method, litVar);
        }

        /* begin internalPush: */
        longAtput((localSP -= BytesPerOop),
                  fetchPointerofObject(ValueIndex, litVar));
        goto l7;
      }
      top = longAt(localSP);
      if (opType == 7) {
        /* begin storeLiteralVariable:withValue: */
        /* begin followObjLiteral:ofMethod: */
        /* begin followObjField:ofObject: */
        litVar = fetchPointerofObject(byte3 + LiteralStart, method);
        assert(isNonImmediate(litVar));
        if ((!((longAt((void *)(litVar))) &
               ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
          litVar = fixFollowedFieldofObjectwithInitialValue(
              byte3 + LiteralStart, method, litVar);
        }

        /* begin storePointerImmutabilityCheck:ofObject:withValue: */
#if IMMUTABILITY
        assert(!((isImmediate(litVar))));
        if ((((usqInt)((byteAt(
                 (void *)(litVar + (immutableExtraBitsByteOffset())))))) >>
             (immutableBitByteShift())) &
            1) {
          /* begin cannotAssign:to:withIndex: */
          /* begin internalPush: */
          longAtput((localSP -= BytesPerOop), litVar);
          longAtput((localSP -= BytesPerOop), top);
          longAtput((localSP -= BytesPerOop),
                    (((usqInt)2 /* (ValueIndex + 1) */ << 3) | 1));
          messageSelector =
              fetchPointerofObject(SelectorAttemptToAssign, specialObjectsOop);
          argumentCount = 2;
          goto normalSend;
          goto l9;
        }
#endif // IMMUTABILITY

        /* begin storePointer:ofObject:withValue: */
        assert(validStorePointerArgs(ValueIndex, litVar, top));
        assert(isNonImmediate(litVar));
        if (oopisGreaterThanOrEqualTo(litVar, oldSpaceStart)) {
          if (/* isYoung: */
              ((!(top & (tagMask())))) && (oopisLessThan(top, oldSpaceStart))) {
            /* begin possibleRootStoreInto: */
            if (!((byteAt((void *)(litVar + (formatFieldByteOffset())))) &
                  (1U << (rememberedBitByteShift())))) {
              remember(litVar);
            }
          }
        }

        /* most stores into young objects */
        longAtput((void *)((litVar + BaseHeaderSize) +
                           ((((usqInt)(ValueIndex) << (shiftForWord()))))),
                  top);
        /* end storePointerImmutabilityCheck:ofObject:withValue: */
      l9:

        /* begin fetchNextBytecode */
        currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
        goto l7;
      }

      /* opType = 5 is store; opType = 6 is storePop */
      if (opType == 6) {
        /* begin internalPop: */
        localSP += 1 * BytesPerOop;
      }
      obj = longAt(localFP + FoxReceiver);
      if ((byte3 <= ReceiverIndex) &&
          ((((longAt((void *)(obj))) & (classIndexMask())) ==
            ClassMethodContextCompactIndex) &&
           (((((fetchPointerofObject(SenderIndex, obj))) & 7) == 1)))) {
        /* begin instVar:ofContext:put: */
        assert(isMarriedOrWidowedContext(obj));
        assert(!((isObjImmutable(obj))));

        /* begin writeBackHeadFramePointers */
        assert(stackPage == (mostRecentlyUsedPage));
        assert(localSP < localFP);
        assert((localSP < ((stackPage->baseAddress))) &&
               (localSP > (((stackPage->realStackLimit)) -
                           (LargeContextSlots * BytesPerOop))));
        assert((localFP < ((stackPage->baseAddress))) &&
               (localFP > (((stackPage->realStackLimit)) -
                           ((LargeContextSlots * BytesPerOop) / 2))));
        (stackPage->headFP = localFP);
        (stackPage->headSP = localSP);
        assert(pageListIsWellFormed());
        if (!(/* isStillMarriedContext: */
              (((((fetchPointerofObject(SenderIndex, obj))) & 7) == 1)) &&
              (!(isWidowedContext(obj))))) {
          /* begin storePointer:ofObject:withValue: */
          assert(validStorePointerArgs(byte3, obj, top));
          assert(isNonImmediate(obj));
          if (oopisGreaterThanOrEqualTo(obj, oldSpaceStart)) {
            if (/* isYoung: */
                ((!(top & (tagMask())))) &&
                (oopisLessThan(top, oldSpaceStart))) {
              /* begin possibleRootStoreInto: */
              if (!((byteAt((void *)(obj + (formatFieldByteOffset())))) &
                    (1U << (rememberedBitByteShift())))) {
                remember(obj);
              }
            }
          }

          /* most stores into young objects */
          longAtput((void *)((obj + BaseHeaderSize) +
                             ((((usqInt)(byte3) << (shiftForWord()))))),
                    top);
          goto l8;
        }

        /* begin frameOfMarriedContext: */
        senderOop = fetchPointerofObject(SenderIndex, obj);
        assert((((senderOop) & 7) == 1));
        theFP = ((char *)(senderOop - (smallIntegerTag())));
        if (!byte3) {
          /* begin stackPageFor: */
          thePage = stackPageAtpages(pageIndexForstackMemorybytesPerPage(
                                         theFP, stackMemory, bytesPerPage),
                                     pages);
          assert(stackPage == (mostRecentlyUsedPage));
          onCurrentPage = thePage == stackPage;
          storeSenderOfFramewithValue(theFP, top);
          if (onCurrentPage) {
            localFP = (stackPage->headFP);
            localSP = (stackPage->headSP);
          } else {
            markStackPageMostRecentlyUsed(stackPage);
          }
          goto l8;
        }

        /* begin externalizeIPandSP */
        instructionPointer = ((usqInt)localIP);
        stackPointer = localSP;
        framePointer = localFP;
        externalDivorceFrameandContext(theFP, obj);

        /* begin storePointer:ofObject:withValue: */
        assert(validStorePointerArgs(byte3, obj, top));
        assert(isNonImmediate(obj));
        if (oopisGreaterThanOrEqualTo(obj, oldSpaceStart)) {
          if (/* isYoung: */
              ((!(top & (tagMask())))) && (oopisLessThan(top, oldSpaceStart))) {
            /* begin possibleRootStoreInto: */
            if (!((byteAt((void *)(obj + (formatFieldByteOffset())))) &
                  (1U << (rememberedBitByteShift())))) {
              remember(obj);
            }
          }
        }

        /* most stores into young objects */
        longAtput((void *)((obj + BaseHeaderSize) +
                           ((((usqInt)(byte3) << (shiftForWord()))))),
                  top);

        /* begin internalizeIPandSP */
        localIP = ((char *)instructionPointer);
        localSP = stackPointer;
        localFP = framePointer;

        /* Assigning various fields can force a divorce which can change the
         * stackPage. */
        markStackPageMostRecentlyUsed(stackPage);
        assertValidExecutionPointersimbarline(((usqInt)localIP), localFP,
                                              localSP, 1, __LINE__);
        /* end instVar:ofContext:put: */
      l8:;
      } else {
        /* begin storePointerImmutabilityCheck:ofObject:withValue: */
#if IMMUTABILITY
        assert(!((isImmediate(obj))));
        if ((((usqInt)((
                 byteAt((void *)(obj + (immutableExtraBitsByteOffset())))))) >>
             (immutableBitByteShift())) &
            1) {
          /* begin cannotAssign:to:withIndex: */
          /* begin internalPush: */
          longAtput((localSP -= BytesPerOop), obj);
          longAtput((localSP -= BytesPerOop), top);
          longAtput((localSP -= BytesPerOop), (((usqInt)(byte3 + 1) << 3) | 1));
          messageSelector =
              fetchPointerofObject(SelectorAttemptToAssign, specialObjectsOop);
          argumentCount = 2;
          goto normalSend;
          goto l11;
        }
#endif // IMMUTABILITY

        /* begin storePointer:ofObject:withValue: */
        assert(validStorePointerArgs(byte3, obj, top));
        assert(isNonImmediate(obj));
        if (oopisGreaterThanOrEqualTo(obj, oldSpaceStart)) {
          if (/* isYoung: */
              ((!(top & (tagMask())))) && (oopisLessThan(top, oldSpaceStart))) {
            /* begin possibleRootStoreInto: */
            if (!((byteAt((void *)(obj + (formatFieldByteOffset())))) &
                  (1U << (rememberedBitByteShift())))) {
              remember(obj);
            }
          }
        }

        /* most stores into young objects */
        longAtput((void *)((obj + BaseHeaderSize) +
                           ((((usqInt)(byte3) << (shiftForWord()))))),
                  top);
        /* end storePointerImmutabilityCheck:ofObject:withValue: */
      l11:;
      }

      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
    }
      /* end case */
    l7:;
      break;
    case 133: // singleExtendedSuperBytecode
    {
      sqInt descriptor;

      VM_LABEL(singleExtendedSuperBytecode);
      descriptor = byteAt(++localIP);

      /* begin literal: */
      assert(method == (iframeMethod(localFP)));
      messageSelector =
          fetchPointerofObject((descriptor & 0x1F) + LiteralStart, method);
      argumentCount = ((usqInt)(descriptor)) >> 5;

      /* goto superclassSend */
    }

    superclassSend:
      /* superclassSend */
      {
        sqInt aValue;
        sqInt classPointer;
        sqInt err;
        sqInt hash;
        sqInt literal;
        sqInt objOop;
        sqInt offset;
        sqInt referent;
        sqInt superclass;

        VM_LABEL(superclassSend);
        /* begin methodClassOf: */
        offset = (literalCountOf(method)) - 1;
        literal = fetchPointerofObject(offset + LiteralStart, method);
        if (isOopForwarded(literal)) {
          literal = fixFollowedFieldofObjectwithInitialValue(
              offset + LiteralStart, method, literal);
        }
        classPointer =
            ((literal != nilObj) &&
                     (/* isPointers: */
                      ((!(literal & (tagMask())))) &&
                      (((byteAt(
                            (void *)(literal + (formatFieldByteOffset())))) &
                        (formatMask())) <= 5 /* lastPointerFormat */))
                 ? (assert((numSlotsOf(literal)) > ValueIndex),
                    followFieldofObject(ValueIndex, literal))
                 : nilObj);

        /* begin superclassOf: */
        /* begin followObjField:ofObject: */
        superclass = fetchPointerofObject(SuperclassIndex, classPointer);
        assert(isNonImmediate(superclass));
        if ((!((longAt((void *)(superclass))) &
               ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
          superclass = fixFollowedFieldofObjectwithInitialValue(
              SuperclassIndex, classPointer, superclass);
        }

        /* classTagForClass: uses ensureBehaviorHash: */

        /* begin classTagForClass: */
        /* begin ensureBehaviorHash: */
        assert(addressCouldBeClassObj(superclass));

        /* eem 12/28/2021 the above asserft is too weak (and only an assert) */
        lkupClassTag =
            ((hash = (long32At((void *)(superclass + 4))) &
                     (identityHashHalfWordMask()))
                 ? hash
                 : (objCouldBeClassObj(superclass)
                        ? ((err = enterIntoClassTable(superclass))
                               ? -err
                               : (long32At((void *)(superclass + 4))) &
                                     (identityHashHalfWordMask()))
                        : -PrimErrBadReceiver));

        /* To maintain the invariant that all receivers are unforwarded we need
           an explicit read barrier in the super send cases.  Even though we
           always follow receivers on become  e.g. super doSomethingWith: (self
           become: other) forwards the receiver self pushed on the stack. */

        /* begin ensureReceiverUnforwarded */
        if (isOopForwarded(longAt(localSP + (argumentCount * BytesPerOop)))) {
          objOop = longAt(localSP + (argumentCount * BytesPerOop));

          aValue = followForwarded(objOop);

          /* begin internalStackValue:put: */
          longAtput(localSP + (argumentCount * BytesPerOop), aValue);
        }
        assert(lkupClassTag != (nilObject()));
        goto commonSendOrdinary;
      }
      break;
    case 134: // secondExtendedSendBytecode
    {
      sqInt descriptor;

      VM_LABEL(secondExtendedSendBytecode);
      descriptor = byteAt(++localIP);

      /* begin literal: */
      assert(method == (iframeMethod(localFP)));
      messageSelector =
          fetchPointerofObject((descriptor & 0x3F) + LiteralStart, method);
      argumentCount = ((usqInt)(descriptor)) >> 6;
      goto normalSend;
    } break;
    case 135: // popStackBytecode
    case 472: // 216	popStackBytecode
    {
      VM_LABEL(popStackBytecode);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      localSP += 1 * BytesPerOop;
    } break;
    case 136: // duplicateTopBytecode
    case 339: // 83	duplicateTopBytecode
    {
      sqInt object;

      VM_LABEL(duplicateTopBytecode);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      object = longAt(localSP);

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop), object);
    } break;
    case 137: // pushActiveContextBytecode
    {
      sqInt ourContext;

      VM_LABEL(pushActiveContextBytecode);
      /* begin ensureFrameIsMarried:SP: */
      if (byteAt((localFP + FoxFrameFlags) + 2)) {
        assert(isContext(frameContext(localFP)));
        ourContext = longAt(localFP + FoxThisContext);
        goto l12;
      }
      ourContext = marryFrameSP(localFP, localSP);
      /* end ensureFrameIsMarried:SP: */
    l12:

      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      longAtput((localSP -= BytesPerOop), ourContext);
    } break;
    case 138: // pushNewArrayBytecode
    case 487: // 231	pushNewArrayBytecode
    {
      usqInt array;
      sqInt i;
      usqInt newObj;
      usqInt numBytes;
      int popValues;
      sqInt size;

      VM_LABEL(pushNewArrayBytecode);
      size = byteAt(++localIP);
      if ((popValues = size >= 128)) {
        size -= 128;
      }

      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      instructionPointer = ((usqInt)localIP);
      stackPointer = localSP;
      framePointer = localFP;

      /* begin eeInstantiateSmallClassIndex:format:numSlots: */
      assert((size >= 0) &&
             ((knownClassAtIndex(ClassArrayCompactIndex)) != nilObj));
      assert((arrayFormat()) ==
             (instSpecOfClass(knownClassAtIndex(ClassArrayCompactIndex))));

      /* begin allocateSmallNewSpaceSlots:format:classIndex: */
      assert(size < (numSlotsMask()));
      newObj = freeStart;
      numBytes = BaseHeaderSize + ((size < 1 ? 8 /* allocationUnit */
                                             : size * BytesPerOop));
      assert((numBytes % (allocationUnit())) == 0);
      assert((newObj % (allocationUnit())) == 0);
      if ((freeStart + numBytes) > scavengeThreshold) {
        if (!needGCFlag) {
          /* begin scheduleScavenge */
          needGCFlag = 1;
          forceInterruptCheck();
        }
        if ((freeStart + numBytes) > (((eden).limit))) {
          error("no room in eden for "
                "allocateSmallNewSpaceSlots:format:classIndex:");
          array = 0;
          goto l13;
        }
      }
      long64Atput((void *)(newObj),
                  ((((((usqLong)size)) << (numSlotsFullShift()))) +
                   ((((usqInt)((arrayFormat())) << (formatShift()))))) +
                      ClassArrayCompactIndex);
      freeStart += numBytes;
      array = newObj;
      /* end eeInstantiateSmallClassIndex:format:numSlots: */
    l13:

      /* Can't use
         NewObjectMemory>>#eeInstantiateSmallClassIndex:format:numSlots: cuz it
         assumes <= 252 bytes whereras this may allocate 127 * 4 + headerSize
         bytes. */
      if (popValues) {
        for (i = 0; i < size; i += 1) {
          /* begin storePointerUnchecked:ofObject:withValue: */
          assert((isNonImmediate(array)) && (!(isForwarded(array))));
          assert(validStorePointerUncheckedArgs(
              i, array,
              longAt((void *)(localSP + (((size - i) - 1) * BytesPerOop)))));
          longAtput(
              (void *)((array + BaseHeaderSize) +
                       ((((usqInt)(i) << (shiftForWord()))))),
              longAt((void *)(localSP + (((size - i) - 1) * BytesPerOop))));
        }

        /* Assume: have just allocated a new Array; it must be young. Thus, can
         * use unchecked stores. */

        /* begin internalPop: */
        localSP += size * BytesPerOop;
      } else {
        for (i = 0; i < size; i += 1) {
          /* begin storePointerUnchecked:ofObject:withValue: */
          assert((isNonImmediate(array)) && (!(isForwarded(array))));
          assert(validStorePointerUncheckedArgs(i, array, nilObj));
          longAtput((void *)((array + BaseHeaderSize) +
                             ((((usqInt)(i) << (shiftForWord()))))),
                    nilObj);
        }
      }

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop), array);
    } break;
    case 139: // callPrimitiveBytecode
    case 504: // 248	callPrimitiveBytecode
    {
      sqInt header;

      VM_LABEL(callPrimitiveBytecode);
      /* begin methodHeaderOf: */
      assert(isCompiledMethod(method));
      header = fetchPointerofObject(HeaderIndex, method);
      if ((((header & AlternateHeaderHasPrimFlag) != 0)) &&
          ((((sqInt)localIP)) ==
           (((((usqInt)(pointerForOop(method)))) +
             ((LiteralStart +
               ((/* begin literalCountOfMethodHeader: */
                 assert((((header) & 7) == 1)),
                 /* literalCountOfAlternateHeader: */
                 ((header >> 3)) & AlternateHeaderNumLiteralsMask))) *
              BytesPerOop)) +
            BaseHeaderSize))) {
        localIP = (localIP + 3 /* sizeOfCallPrimitiveBytecode: */) - 1;

        /* begin fetchNextBytecode */
        currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
        goto l14;
      }
      goto respondToUnknownBytecode;
    }
      /* end case */
    l14:;
      break;
    case 140: // pushRemoteTempLongBytecode
    case 507: // 251	pushRemoteTempLongBytecode
    {
      usqInt frameNumArgs;
      sqInt remoteTempIndex;
      sqInt tempVector;
      sqInt tempVectorIndex;

      VM_LABEL(pushRemoteTempLongBytecode);
      remoteTempIndex = byteAt(++localIP);
      tempVectorIndex = byteAt(++localIP);

      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      tempVector = /* temporary:in: */
          (tempVectorIndex <
                   ((frameNumArgs = byteAt((localFP + FoxFrameFlags) + 1)))
               ? longAt((localFP + FoxCallerSavedIP) +
                        ((frameNumArgs - tempVectorIndex) * BytesPerWord))
               : longAt(((localFP + FoxReceiver) - BytesPerWord) +
                        ((frameNumArgs - tempVectorIndex) * BytesPerWord)));

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop),
                fetchPointerofObject(remoteTempIndex, tempVector));
    } break;
    case 141: // storeRemoteTempLongBytecode
    case 508: // 252	storeRemoteTempLongBytecode
    {
      usqInt frameNumArgs;
      sqInt remoteTempIndex;
      sqInt tempVector;
      sqInt tempVectorIndex;
      sqInt valuePointer;

      VM_LABEL(storeRemoteTempLongBytecode);
      remoteTempIndex = byteAt(++localIP);
      tempVectorIndex = byteAt(++localIP);

      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      tempVector = /* temporary:in: */
          (tempVectorIndex <
                   ((frameNumArgs = byteAt((localFP + FoxFrameFlags) + 1)))
               ? longAt((localFP + FoxCallerSavedIP) +
                        ((frameNumArgs - tempVectorIndex) * BytesPerWord))
               : longAt(((localFP + FoxReceiver) - BytesPerWord) +
                        ((frameNumArgs - tempVectorIndex) * BytesPerWord)));
      valuePointer = longAt(localSP);

      /* begin storePointer:ofObject:withValue: */
      assert(validStorePointerArgs(remoteTempIndex, tempVector, valuePointer));
      assert(isNonImmediate(tempVector));
      if (oopisGreaterThanOrEqualTo(tempVector, oldSpaceStart)) {
        if (/* isYoung: */
            ((!(valuePointer & (tagMask())))) &&
            (oopisLessThan(valuePointer, oldSpaceStart))) {
          /* begin possibleRootStoreInto: */
          if (!((byteAt((void *)(tempVector + (formatFieldByteOffset())))) &
                (1U << (rememberedBitByteShift())))) {
            remember(tempVector);
          }
        }
      }

      /* most stores into young objects */
      longAtput((void *)((tempVector + BaseHeaderSize) +
                         ((((usqInt)(remoteTempIndex) << (shiftForWord()))))),
                valuePointer);
    } break;
    case 142: // storeAndPopRemoteTempLongBytecode
    case 509: // 253	storeAndPopRemoteTempLongBytecode
    {
      usqInt frameNumArgs;
      sqInt remoteTempIndex;
      sqInt tempVector;
      sqInt tempVectorIndex;
      sqInt valuePointer;

      VM_LABEL(storeAndPopRemoteTempLongBytecode);
      /* begin storeRemoteTempLongBytecode */
      remoteTempIndex = byteAt(++localIP);
      tempVectorIndex = byteAt(++localIP);

      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      tempVector = /* temporary:in: */
          (tempVectorIndex <
                   ((frameNumArgs = byteAt((localFP + FoxFrameFlags) + 1)))
               ? longAt((localFP + FoxCallerSavedIP) +
                        ((frameNumArgs - tempVectorIndex) * BytesPerWord))
               : longAt(((localFP + FoxReceiver) - BytesPerWord) +
                        ((frameNumArgs - tempVectorIndex) * BytesPerWord)));
      valuePointer = longAt(localSP);

      /* begin storePointer:ofObject:withValue: */
      assert(validStorePointerArgs(remoteTempIndex, tempVector, valuePointer));
      assert(isNonImmediate(tempVector));
      if (oopisGreaterThanOrEqualTo(tempVector, oldSpaceStart)) {
        if (/* isYoung: */
            ((!(valuePointer & (tagMask())))) &&
            (oopisLessThan(valuePointer, oldSpaceStart))) {
          /* begin possibleRootStoreInto: */
          if (!((byteAt((void *)(tempVector + (formatFieldByteOffset())))) &
                (1U << (rememberedBitByteShift())))) {
            remember(tempVector);
          }
        }
      }

      /* most stores into young objects */
      longAtput((void *)((tempVector + BaseHeaderSize) +
                         ((((usqInt)(remoteTempIndex) << (shiftForWord()))))),
                valuePointer);

      /* begin internalPop: */
      localSP += 1 * BytesPerOop;
    } break;
    case 143: // pushClosureCopyCopiedValuesBytecode
    {
      sqInt blockSize;
      sqInt context;
      sqInt i;
      sqInt initialIP;
      usqInt newClosure;
      usqInt newObj;
      usqInt numArgs;
      sqInt numArgsNumCopied;
      usqInt numBytes;
      sqInt numCopied;
      sqInt numSlots;

      VM_LABEL(pushClosureCopyCopiedValuesBytecode);
      numArgsNumCopied = byteAt(++localIP);
      numArgs = numArgsNumCopied & 15;
      numCopied = (((usqInt)(numArgsNumCopied)) >> 4);

      /* Split blockSize := (self fetchByte * 256) + self fetchByte. into two
       * because evaluation order in C is undefined. */
      blockSize = ((sqInt)((usqInt)((byteAt(++localIP))) << 8));
      blockSize += byteAt(++localIP);

      /* begin pushClosureNumArgs:copiedValues:blockSize: */
      /* begin ensureFrameIsMarried:SP: */
      if (byteAt((localFP + FoxFrameFlags) + 2)) {
        assert(isContext(frameContext(localFP)));
        context = longAt(localFP + FoxThisContext);
        goto l15;
      }
      context = marryFrameSP(localFP, localSP + (numCopied * BytesPerOop));
      /* end ensureFrameIsMarried:SP: */
    l15:
      initialIP = ((((usqInt)localIP)) + 2) - (method + BaseHeaderSize);

      /* begin closureIn:numArgs:instructionPointer:numCopiedValues: */
      numSlots = ClosureFirstCopiedValueIndex + numCopied;
      assert((numSlots >= 0) &&
             ((knownClassAtIndex(ClassBlockClosureCompactIndex)) != nilObj));
      assert(
          (indexablePointersFormat()) ==
          (instSpecOfClass(knownClassAtIndex(ClassBlockClosureCompactIndex))));

      /* begin allocateSmallNewSpaceSlots:format:classIndex: */
      assert(numSlots < (numSlotsMask()));
      newObj = freeStart;
      numBytes = BaseHeaderSize + ((numSlots < 1 ? 8 /* allocationUnit */
                                                 : numSlots * BytesPerOop));
      assert((numBytes % (allocationUnit())) == 0);
      assert((newObj % (allocationUnit())) == 0);
      if ((freeStart + numBytes) > scavengeThreshold) {
        if (!needGCFlag) {
          /* begin scheduleScavenge */
          needGCFlag = 1;
          forceInterruptCheck();
        }
        if ((freeStart + numBytes) > (((eden).limit))) {
          error("no room in eden for "
                "allocateSmallNewSpaceSlots:format:classIndex:");
          newClosure = 0;
          goto l16;
        }
      }
      long64Atput(
          (void *)(newObj),
          ((((((usqLong)numSlots)) << (numSlotsFullShift()))) +
           ((((usqInt)((indexablePointersFormat())) << (formatShift()))))) +
              ClassBlockClosureCompactIndex);
      freeStart += numBytes;
      newClosure = newObj;
      /* end eeInstantiateSmallClassIndex:format:numSlots: */
    l16:

      /* Assume: have just allocated a new closure; it must be young. Thus, can
       * use unchecked stores. */

      /* begin storePointerUnchecked:ofObject:withValue: */
      assert((isNonImmediate(newClosure)) && (!(isForwarded(newClosure))));
      assert(validStorePointerUncheckedArgs(ClosureOuterContextIndex,
                                            newClosure, context));
      longAtput((void *)((newClosure + BaseHeaderSize) +
                         ((((usqInt)(ClosureOuterContextIndex)
                            << (shiftForWord()))))),
                context);

      /* begin storePointerUnchecked:ofObject:withValue: */
      assert((isNonImmediate(newClosure)) && (!(isForwarded(newClosure))));
      assert(validStorePointerUncheckedArgs(ClosureStartPCIndex, newClosure,
                                            (((usqInt)initialIP << 3) | 1)));
      longAtput(
          (void *)((newClosure + BaseHeaderSize) +
                   ((((usqInt)(ClosureStartPCIndex) << (shiftForWord()))))),
          (((usqInt)initialIP << 3) | 1));

      /* begin storePointerUnchecked:ofObject:withValue: */
      assert((isNonImmediate(newClosure)) && (!(isForwarded(newClosure))));
      assert(validStorePointerUncheckedArgs(ClosureNumArgsIndex, newClosure,
                                            ((numArgs << 3) | 1)));
      longAtput(
          (void *)((newClosure + BaseHeaderSize) +
                   ((((usqInt)(ClosureNumArgsIndex) << (shiftForWord()))))),
          ((numArgs << 3) | 1));
      if (numCopied > 0) {
        for (i = 0; i < numCopied; i += 1) {
          /* begin storePointerUnchecked:ofObject:withValue: */
          assert((isNonImmediate(newClosure)) && (!(isForwarded(newClosure))));
          assert(validStorePointerUncheckedArgs(
              i + ClosureFirstCopiedValueIndex, newClosure,
              longAt(
                  (void *)(localSP + (((numCopied - i) - 1) * BytesPerOop)))));
          longAtput((void *)((newClosure + BaseHeaderSize) +
                             ((((usqInt)((i + ClosureFirstCopiedValueIndex))
                                << (shiftForWord()))))),
                    longAt((void *)(localSP +
                                    (((numCopied - i) - 1) * BytesPerOop))));
        }

        /* Assume: have just allocated a new BlockClosure; it must be young.
           Thus, can use unchecked stores. */

        /* begin internalPop: */
        localSP += numCopied * BytesPerOop;
      }
      localIP += blockSize;

      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      longAtput((localSP -= BytesPerOop), newClosure);
    } break;
    case 144: // shortUnconditionalJump
    case 432: // 176	shortUnconditionalJump
    {
      sqInt offset;

      VM_LABEL(shortUnconditionalJump);
      offset = 1;

      /* begin jump: */
      localIP = (localIP + offset) + 1;
      currentBytecode = (byteAt(localIP)) + bytecodeSetSelector;
    } break;
    case 145: // shortUnconditionalJump
    case 433: // 177	shortUnconditionalJump
    {
      sqInt offset;

      VM_LABEL(shortUnconditionalJump1);
      offset = 2;

      /* begin jump: */
      localIP = (localIP + offset) + 1;
      currentBytecode = (byteAt(localIP)) + bytecodeSetSelector;
    } break;
    case 146: // shortUnconditionalJump
    case 434: // 178	shortUnconditionalJump
    {
      sqInt offset;

      VM_LABEL(shortUnconditionalJump2);
      offset = 3;

      /* begin jump: */
      localIP = (localIP + offset) + 1;
      currentBytecode = (byteAt(localIP)) + bytecodeSetSelector;
    } break;
    case 147: // shortUnconditionalJump
    case 435: // 179	shortUnconditionalJump
    {
      sqInt offset;

      VM_LABEL(shortUnconditionalJump3);
      offset = 4;

      /* begin jump: */
      localIP = (localIP + offset) + 1;
      currentBytecode = (byteAt(localIP)) + bytecodeSetSelector;
    } break;
    case 148: // shortUnconditionalJump
    case 436: // 180	shortUnconditionalJump
    {
      sqInt offset;

      VM_LABEL(shortUnconditionalJump4);
      offset = 5;

      /* begin jump: */
      localIP = (localIP + offset) + 1;
      currentBytecode = (byteAt(localIP)) + bytecodeSetSelector;
    } break;
    case 149: // shortUnconditionalJump
    case 437: // 181	shortUnconditionalJump
    {
      sqInt offset;

      VM_LABEL(shortUnconditionalJump5);
      offset = 6;

      /* begin jump: */
      localIP = (localIP + offset) + 1;
      currentBytecode = (byteAt(localIP)) + bytecodeSetSelector;
    } break;
    case 150: // shortUnconditionalJump
    case 438: // 182	shortUnconditionalJump
    {
      sqInt offset;

      VM_LABEL(shortUnconditionalJump6);
      offset = 7;

      /* begin jump: */
      localIP = (localIP + offset) + 1;
      currentBytecode = (byteAt(localIP)) + bytecodeSetSelector;
    } break;
    case 151: // shortUnconditionalJump
    case 439: // 183	shortUnconditionalJump
    {
      sqInt offset;

      VM_LABEL(shortUnconditionalJump7);
      offset = 8;

      /* begin jump: */
      localIP = (localIP + offset) + 1;
      currentBytecode = (byteAt(localIP)) + bytecodeSetSelector;
    } break;
    case 152: // shortConditionalJumpFalse
    case 153: // shortConditionalJumpFalse
    case 154: // shortConditionalJumpFalse
    case 155: // shortConditionalJumpFalse
    case 156: // shortConditionalJumpFalse
    case 157: // shortConditionalJumpFalse
    case 158: // shortConditionalJumpFalse
    case 159: // shortConditionalJumpFalse
    case 448: // 192	shortConditionalJumpFalse
    case 449: // 193	shortConditionalJumpFalse
    case 450: // 194	shortConditionalJumpFalse
    case 451: // 195	shortConditionalJumpFalse
    case 452: // 196	shortConditionalJumpFalse
    case 453: // 197	shortConditionalJumpFalse
    case 454: // 198	shortConditionalJumpFalse
    case 455: // 199	shortConditionalJumpFalse
    {
      sqInt boolean;
      sqInt offset;

      VM_LABEL(shortConditionalJumpFalse);
      offset = (currentBytecode & 7) + 1;

      /* begin jumplfFalseBy: */
      boolean = longAt(localSP);
      if (boolean == falseObj) {
        /* begin jump: */
        localIP = (localIP + offset) + 1;
        currentBytecode = (byteAt(localIP)) + bytecodeSetSelector;
      } else {
        if (!(boolean == trueObj)) {
          /* begin internalMustBeBoolean */
          messageSelector =
              fetchPointerofObject(SelectorMustBeBoolean, specialObjectsOop);
          argumentCount = 0;
          goto normalSend;
          goto l17;
        }

        /* begin fetchNextBytecode */
        currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      }
      localSP += 1 * BytesPerOop;
      /* end jumplfFalseBy: */
    l17:;
    } break;
    case 160: // longUnconditionalJump
    case 161: // longUnconditionalJump
    case 162: // longUnconditionalJump
    case 163: // longUnconditionalJump
    case 164: // longUnconditionalJump
    case 165: // longUnconditionalJump
    case 166: // longUnconditionalJump
    case 167: // longUnconditionalJump
    {
      sqInt offset;

      VM_LABEL(longUnconditionalJump);
      offset = (((currentBytecode & 7) - 4) * 0x100) + (byteAt(++localIP));
      localIP += offset;

      /* begin ifBackwardsCheckForEvents: */
      if ((offset < 0) && (localSP < stackLimit)) {
        /* begin externalizeIPandSP */
        instructionPointer = ((usqInt)localIP);
        stackPointer = localSP;
        framePointer = localFP;
        checkForEventsMayContextSwitch(1);
        browserPluginReturnIfNeeded();

        /* begin internalizeIPandSP */
        localIP = ((char *)instructionPointer);
        localSP = stackPointer;
        localFP = framePointer;
      }

      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
    } break;
    case 168: // longJumpIfTrue
    case 169: // longJumpIfTrue
    case 170: // longJumpIfTrue
    case 171: // longJumpIfTrue
    {
      sqInt boolean;
      sqInt offset;

      VM_LABEL(longJumpIfTrue);
      offset = ((currentBytecode & 3) * 0x100) + (byteAt(++localIP));

      /* begin jumplfTrueBy: */
      boolean = longAt(localSP);
      if (boolean == trueObj) {
        /* begin jump: */
        localIP = (localIP + offset) + 1;
        currentBytecode = (byteAt(localIP)) + bytecodeSetSelector;
      } else {
        if (!(boolean == falseObj)) {
          /* begin internalMustBeBoolean */
          messageSelector =
              fetchPointerofObject(SelectorMustBeBoolean, specialObjectsOop);
          argumentCount = 0;
          goto normalSend;
          goto l18;
        }

        /* begin fetchNextBytecode */
        currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      }
      localSP += 1 * BytesPerOop;
      /* end jumplfTrueBy: */
    l18:;
    } break;
    case 172: // longJumpIfFalse
    case 173: // longJumpIfFalse
    case 174: // longJumpIfFalse
    case 175: // longJumpIfFalse
    {
      sqInt boolean;
      sqInt offset;

      VM_LABEL(longJumpIfFalse);
      offset = ((currentBytecode & 3) * 0x100) + (byteAt(++localIP));

      /* begin jumplfFalseBy: */
      boolean = longAt(localSP);
      if (boolean == falseObj) {
        /* begin jump: */
        localIP = (localIP + offset) + 1;
        currentBytecode = (byteAt(localIP)) + bytecodeSetSelector;
      } else {
        if (!(boolean == trueObj)) {
          /* begin internalMustBeBoolean */
          messageSelector =
              fetchPointerofObject(SelectorMustBeBoolean, specialObjectsOop);
          argumentCount = 0;
          goto normalSend;
          goto l19;
        }

        /* begin fetchNextBytecode */
        currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      }
      localSP += 1 * BytesPerOop;
      /* end jumplfFalseBy: */
    l19:;
    } break;
    case 176: // bytecodePrimAdd
    case 352: // 96	bytecodePrimAdd
    {
      sqInt arg;
      double argDouble;
      usqLong bits;
      sqInt rcvr;
      double rcvrDouble;
      sqInt result;
      double resultDouble;
      char *sp;
      sqInt tagBits;
      double value;

      VM_LABEL(bytecodePrimAdd);
      rcvr = longAt(localSP + (1 * BytesPerOop));
      arg = longAt(localSP);
      if ((((rcvr & arg) & (smallIntegerTag())) != 0)) {
        result = ((rcvr >> 3)) + ((arg >> 3));
        if ((((((usqInt)(result)) >> 60) + 1) & 15) <= 1) {
          /* begin internalPop:thenPush: */
          longAtput((localSP += 1 * BytesPerOop), (((usqInt)result << 3) | 1));
          currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
          goto l20;
        }
      } else {
        /* begin initPrimCall */
        primFailCode = 0;
        instructionPointer = ((usqInt)localIP);
        stackPointer = localSP;
        framePointer = localFP;

        /* begin primitiveFloatAdd:toArg: */
        /* begin loadFloatOrIntFrom: */
        if ((tagBits = rcvr & (tagMask()))) {
          if (tagBits == (smallFloatTag())) {
            value = smallFloatValueOf(rcvr);
            rcvrDouble = value;
            goto l21;
          }
          if ((primitiveDoMixedArithmetic) &&
              (tagBits == (smallIntegerTag()))) {
            rcvrDouble = ((double)((rcvr >> 3)));
            goto l21;
          }
        } else {
          if (((longAt((void *)(rcvr))) & (classIndexMask())) ==
              ClassFloatCompactIndex) {
            fetchFloatAtinto(rcvr + BaseHeaderSize, resultDouble);
            rcvrDouble = resultDouble;
            goto l21;
          }
        }

        /* begin primitiveFail */
        if (!primFailCode) {
          primFailCode = 1;
        }
        rcvrDouble = 0.0;
        /* end loadFloatOrIntFrom: */
      l21:

        /* begin loadFloatOrIntFrom: */
        if ((tagBits = arg & (tagMask()))) {
          if (tagBits == (smallFloatTag())) {
            value = smallFloatValueOf(arg);
            argDouble = value;
            goto l22;
          }
          if ((primitiveDoMixedArithmetic) &&
              (tagBits == (smallIntegerTag()))) {
            argDouble = ((double)((arg >> 3)));
            goto l22;
          }
        } else {
          if (((longAt((void *)(arg))) & (classIndexMask())) ==
              ClassFloatCompactIndex) {
            fetchFloatAtinto(arg + BaseHeaderSize, resultDouble);
            argDouble = resultDouble;
            goto l22;
          }
        }

        /* begin primitiveFail */
        if (!primFailCode) {
          primFailCode = 1;
        }
        argDouble = 0.0;
        /* end loadFloatOrIntFrom: */
      l22:
        if (!primFailCode) {
          /* begin pop:thenPushFloat: */
          longAtput((sp = stackPointer + (1 * BytesPerWord)),
                    floatObjectOf(rcvrDouble + argDouble));
          stackPointer = sp;
        }

        /* begin internalizeIPandSP */
        localIP = ((char *)instructionPointer);
        localSP = stackPointer;
        localFP = framePointer;
        if (!primFailCode) {
          /* begin fetchNextBytecode */
          currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
          goto l20;
        }
      }
      messageSelector = fetchPointerofObject(
          0U, fetchPointerofObject(SpecialSelectors, specialObjectsOop));
      argumentCount = 1;
      goto normalSend;
    }
      /* end case */
    l20:;
      break;
    case 177: // bytecodePrimSubtract
    case 353: // 97	bytecodePrimSubtract
    {
      sqInt arg;
      double argDouble;
      usqLong bits;
      sqInt rcvr;
      double rcvrDouble;
      sqInt result;
      double resultDouble;
      char *sp;
      sqInt tagBits;
      double value;

      VM_LABEL(bytecodePrimSubtract);
      rcvr = longAt(localSP + (1 * BytesPerOop));
      arg = longAt(localSP);
      if ((((rcvr & arg) & (smallIntegerTag())) != 0)) {
        result = ((rcvr >> 3)) - ((arg >> 3));
        if ((((((usqInt)(result)) >> 60) + 1) & 15) <= 1) {
          /* begin internalPop:thenPush: */
          longAtput((localSP += 1 * BytesPerOop), (((usqInt)result << 3) | 1));
          currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
          goto l23;
        }
      } else {
        /* begin initPrimCall */
        primFailCode = 0;
        instructionPointer = ((usqInt)localIP);
        stackPointer = localSP;
        framePointer = localFP;

        /* begin primitiveFloatSubtract:fromArg: */
        /* begin loadFloatOrIntFrom: */
        if ((tagBits = rcvr & (tagMask()))) {
          if (tagBits == (smallFloatTag())) {
            value = smallFloatValueOf(rcvr);
            rcvrDouble = value;
            goto l24;
          }
          if ((primitiveDoMixedArithmetic) &&
              (tagBits == (smallIntegerTag()))) {
            rcvrDouble = ((double)((rcvr >> 3)));
            goto l24;
          }
        } else {
          if (((longAt((void *)(rcvr))) & (classIndexMask())) ==
              ClassFloatCompactIndex) {
            fetchFloatAtinto(rcvr + BaseHeaderSize, resultDouble);
            rcvrDouble = resultDouble;
            goto l24;
          }
        }

        /* begin primitiveFail */
        if (!primFailCode) {
          primFailCode = 1;
        }
        rcvrDouble = 0.0;
        /* end loadFloatOrIntFrom: */
      l24:

        /* begin loadFloatOrIntFrom: */
        if ((tagBits = arg & (tagMask()))) {
          if (tagBits == (smallFloatTag())) {
            value = smallFloatValueOf(arg);
            argDouble = value;
            goto l25;
          }
          if ((primitiveDoMixedArithmetic) &&
              (tagBits == (smallIntegerTag()))) {
            argDouble = ((double)((arg >> 3)));
            goto l25;
          }
        } else {
          if (((longAt((void *)(arg))) & (classIndexMask())) ==
              ClassFloatCompactIndex) {
            fetchFloatAtinto(arg + BaseHeaderSize, resultDouble);
            argDouble = resultDouble;
            goto l25;
          }
        }

        /* begin primitiveFail */
        if (!primFailCode) {
          primFailCode = 1;
        }
        argDouble = 0.0;
        /* end loadFloatOrIntFrom: */
      l25:
        if (!primFailCode) {
          /* begin pop:thenPushFloat: */
          longAtput((sp = stackPointer + (1 * BytesPerWord)),
                    floatObjectOf(rcvrDouble - argDouble));
          stackPointer = sp;
        }

        /* begin internalizeIPandSP */
        localIP = ((char *)instructionPointer);
        localSP = stackPointer;
        localFP = framePointer;
        if (!primFailCode) {
          /* begin fetchNextBytecode */
          currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
          goto l23;
        }
      }
      messageSelector = fetchPointerofObject(
          2U, fetchPointerofObject(SpecialSelectors, specialObjectsOop));
      argumentCount = 1;
      goto normalSend;
    }
      /* end case */
    l23:;
      break;
    case 178: // bytecodePrimLessThan
    {
      int aBool;
      sqInt arg;
      sqInt rcvr;

      VM_LABEL(bytecodePrimLessThan);
      rcvr = longAt(localSP + (1 * BytesPerOop));
      arg = longAt(localSP);
      if ((((rcvr & arg) & (smallIntegerTag())) != 0)) {
        /* begin booleanCheat: */
        if (rcvr < arg) {
          /* goto booleanCheatTrue */
        } else {
          goto booleanCheatFalse;
        }
        goto l26;
      }

      /* The C code can avoid detagging since tagged integers are still signed.
         But this means the simulator must override to do detagging. */

      /* begin initPrimCall */
      primFailCode = 0;
      aBool = primitiveFloatLessthanArg(rcvr, arg);
      if (!primFailCode) {
        /* begin booleanCheat: */
        if (aBool) {
          goto booleanCheatTrue;
        } else {
          goto booleanCheatFalse;
        }
        goto l26;
      }
      messageSelector = fetchPointerofObject(
          4U, fetchPointerofObject(SpecialSelectors, specialObjectsOop));
      argumentCount = 1;
      goto normalSend;
    }
      /* end case */
    l26:

    booleanCheatTrue:
      /* booleanCheatTrue */
      {
        sqInt bytecode;
        sqInt offset;

        VM_LABEL(booleanCheatTrue);
        /* assume next bytecode is jumpIfFalse (99%) */
        bytecode = byteAt(++localIP);

        /* begin internalPop: */
        localSP += 2 * BytesPerOop;
        if ((bytecode < 173) && (bytecode > 151)) {
          if (bytecode < 160) {
            /* begin fetchNextBytecode */
            currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
            goto l170;
          }

          /* short jumpIfFalse 152 - 159 */
          if (bytecode == 172) {
            /* fetchByte */
            byteAt(++localIP);

            /* begin fetchNextBytecode */
            currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
            goto l170;
          }

          /* long jumpIfFalse */
          if (bytecode > 167) {
            offset =
                ((((usqInt)((bytecode - 168)) << 8))) + (byteAt(++localIP));

            /* begin jump: */
            localIP = (localIP + offset) + 1;
            currentBytecode = (byteAt(localIP)) + bytecodeSetSelector;
            goto l170;
          }
        }

        /* not followed by a jumpIfFalse; (un)do instruction fetch and push
         * boolean result */
        currentBytecode = bytecode + bytecodeSetSelector;

        /* begin internalPush: */
        longAtput((localSP -= BytesPerOop), trueObj);
      }
      /* end case */
    l170:;
      break;
    case 179: // bytecodePrimGreaterThan
    {
      int aBool;
      sqInt arg;
      sqInt rcvr;

      VM_LABEL(bytecodePrimGreaterThan);
      rcvr = longAt(localSP + (1 * BytesPerOop));
      arg = longAt(localSP);
      if ((((rcvr & arg) & (smallIntegerTag())) != 0)) {
        /* begin booleanCheat: */
        if (rcvr > arg) {
          goto booleanCheatTrue;
        } else {
          /* goto booleanCheatFalse */
        }
        goto l27;
      }

      /* The C code can avoid detagging since tagged integers are still signed.
         But this means the simulator must override to do detagging. */

      /* begin initPrimCall */
      primFailCode = 0;
      aBool = primitiveFloatGreaterthanArg(rcvr, arg);
      if (!primFailCode) {
        /* begin booleanCheat: */
        if (aBool) {
          goto booleanCheatTrue;
        } else {
          goto booleanCheatFalse;
        }
        goto l27;
      }
      messageSelector = fetchPointerofObject(
          6U, fetchPointerofObject(SpecialSelectors, specialObjectsOop));
      argumentCount = 1;
      goto normalSend;
    }
      /* end case */
    l27:

    booleanCheatFalse:
      /* booleanCheatFalse */
      {
        sqInt bytecode;
        sqInt offset;

        VM_LABEL(booleanCheatFalse);
        /* assume next bytecode is jumpIfFalse (99%) */
        bytecode = byteAt(++localIP);

        /* begin internalPop: */
        localSP += 2 * BytesPerOop;
        if ((bytecode < 160) && (bytecode > 151)) {
          /* begin jump: */
          localIP = (localIP + (bytecode - 151)) + 1;
          currentBytecode = (byteAt(localIP)) + bytecodeSetSelector;
          goto l171;
        }

        /* short jumpIfFalse */
        if (bytecode == 172) {
          offset = byteAt(++localIP);

          /* begin jump: */
          localIP = (localIP + offset) + 1;
          currentBytecode = (byteAt(localIP)) + bytecodeSetSelector;
          goto l171;
        }

        /* long jumpIfFalse
           not followed by a jumpIfFalse; (un)do instruction fetch and push
           boolean result */
        currentBytecode = bytecode + bytecodeSetSelector;

        /* begin internalPush: */
        longAtput((localSP -= BytesPerOop), falseObj);
      }
      /* end case */
    l171:;
      break;
    case 180: // bytecodePrimLessOrEqual
    {
      int aBool;
      sqInt arg;
      sqInt rcvr;

      VM_LABEL(bytecodePrimLessOrEqual);
      rcvr = longAt(localSP + (1 * BytesPerOop));
      arg = longAt(localSP);
      if ((((rcvr & arg) & (smallIntegerTag())) != 0)) {
        /* begin booleanCheat: */
        if (rcvr <= arg) {
          goto booleanCheatTrue;
        } else {
          goto booleanCheatFalse;
        }
        goto l28;
      }

      /* The C code can avoid detagging since tagged integers are still signed.
         But this means the simulator must override to do detagging. */

      /* begin initPrimCall */
      primFailCode = 0;
      aBool = primitiveFloatLessOrEqualtoArg(rcvr, arg);
      if (!primFailCode) {
        /* begin booleanCheat: */
        if (aBool) {
          goto booleanCheatTrue;
        } else {
          goto booleanCheatFalse;
        }
        goto l28;
      }
      messageSelector = fetchPointerofObject(
          8U, fetchPointerofObject(SpecialSelectors, specialObjectsOop));
      argumentCount = 1;
      goto normalSend;
    }
      /* end case */
    l28:;
      break;
    case 181: // bytecodePrimGreaterOrEqual
    {
      int aBool;
      sqInt arg;
      sqInt rcvr;

      VM_LABEL(bytecodePrimGreaterOrEqual);
      rcvr = longAt(localSP + (1 * BytesPerOop));
      arg = longAt(localSP);
      if ((((rcvr & arg) & (smallIntegerTag())) != 0)) {
        /* begin booleanCheat: */
        if (rcvr >= arg) {
          goto booleanCheatTrue;
        } else {
          goto booleanCheatFalse;
        }
        goto l29;
      }

      /* The C code can avoid detagging since tagged integers are still signed.
         But this means the simulator must override to do detagging. */

      /* begin initPrimCall */
      primFailCode = 0;
      aBool = primitiveFloatGreaterOrEqualtoArg(rcvr, arg);
      if (!primFailCode) {
        /* begin booleanCheat: */
        if (aBool) {
          goto booleanCheatTrue;
        } else {
          goto booleanCheatFalse;
        }
        goto l29;
      }
      messageSelector = fetchPointerofObject(
          10U, fetchPointerofObject(SpecialSelectors, specialObjectsOop));
      argumentCount = 1;
      goto normalSend;
    }
      /* end case */
    l29:;
      break;
    case 182: // bytecodePrimEqual
    {
      int aBool;
      sqInt arg;
      sqInt rcvr;

      VM_LABEL(bytecodePrimEqual);
      rcvr = longAt(localSP + (1 * BytesPerOop));
      arg = longAt(localSP);
      if ((((rcvr & arg) & (smallIntegerTag())) != 0)) {
        /* begin booleanCheat: */
        if (rcvr == arg) {
          goto booleanCheatTrue;
        } else {
          goto booleanCheatFalse;
        }
        goto l30;
      }

      /* begin initPrimCall */
      primFailCode = 0;
      aBool = primitiveFloatEqualtoArg(rcvr, arg);
      if (!primFailCode) {
        /* begin booleanCheat: */
        if (aBool) {
          goto booleanCheatTrue;
        } else {
          goto booleanCheatFalse;
        }
        goto l30;
      }
      messageSelector = fetchPointerofObject(
          12U, fetchPointerofObject(SpecialSelectors, specialObjectsOop));
      argumentCount = 1;
      goto normalSend;
    }
      /* end case */
    l30:;
      break;
    case 183: // bytecodePrimNotEqual
    {
      int aBool;
      sqInt arg;
      sqInt rcvr;

      VM_LABEL(bytecodePrimNotEqual);
      rcvr = longAt(localSP + (1 * BytesPerOop));
      arg = longAt(localSP);
      if ((((rcvr & arg) & (smallIntegerTag())) != 0)) {
        /* begin booleanCheat: */
        if (rcvr != arg) {
          goto booleanCheatTrue;
        } else {
          goto booleanCheatFalse;
        }
        goto l31;
      }

      /* begin initPrimCall */
      primFailCode = 0;
      aBool = primitiveFloatEqualtoArg(rcvr, arg);
      if (!primFailCode) {
        /* begin booleanCheat: */
        if (aBool) {
          goto booleanCheatFalse;
        } else {
          goto booleanCheatTrue;
        }
        goto l31;
      }
      messageSelector = fetchPointerofObject(
          14U, fetchPointerofObject(SpecialSelectors, specialObjectsOop));
      argumentCount = 1;
      goto normalSend;
    }
      /* end case */
    l31:;
      break;
    case 184: // bytecodePrimMultiply
    case 360: // 104	bytecodePrimMultiply
    {
      sqInt arg;
      double argDouble;
      usqLong bits;
      sqInt oop;
      sqInt overflow;
      sqInt rcvr;
      double rcvrDouble;
      sqInt result;
      double resultDouble;
      char *sp;
      sqInt tagBits;
      double value;

      VM_LABEL(bytecodePrimMultiply);
      rcvr = longAt(localSP + (1 * BytesPerOop));
      arg = longAt(localSP);
      if ((((rcvr & arg) & (smallIntegerTag())) != 0)) {
        rcvr = (rcvr >> 3);
        arg = (arg >> 3);
        overflow =
            (rcvr > 0 ? (arg > 0 ? rcvr > ((MaxSmallInteger) / arg)
                                 : arg < ((MinSmallInteger) / rcvr))
                      : (arg > 0 ? rcvr < ((MinSmallInteger) / arg)
                                 : (rcvr < 0) &&
                                       (arg < ((MaxSmallInteger) / rcvr))));
        if (!overflow) {
          result = rcvr * arg;
          oop = (((usqInt)result << 3) | 1);

          /* begin internalPop:thenPush: */
          longAtput((localSP += 1 * BytesPerOop), oop);
          currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
          goto l32;
        }
      } else {
        /* begin initPrimCall */
        primFailCode = 0;
        instructionPointer = ((usqInt)localIP);
        stackPointer = localSP;
        framePointer = localFP;

        /* begin primitiveFloatMultiply:byArg: */
        /* begin loadFloatOrIntFrom: */
        if ((tagBits = rcvr & (tagMask()))) {
          if (tagBits == (smallFloatTag())) {
            value = smallFloatValueOf(rcvr);
            rcvrDouble = value;
            goto l33;
          }
          if ((primitiveDoMixedArithmetic) &&
              (tagBits == (smallIntegerTag()))) {
            rcvrDouble = ((double)((rcvr >> 3)));
            goto l33;
          }
        } else {
          if (((longAt((void *)(rcvr))) & (classIndexMask())) ==
              ClassFloatCompactIndex) {
            fetchFloatAtinto(rcvr + BaseHeaderSize, resultDouble);
            rcvrDouble = resultDouble;
            goto l33;
          }
        }

        /* begin primitiveFail */
        if (!primFailCode) {
          primFailCode = 1;
        }
        rcvrDouble = 0.0;
        /* end loadFloatOrIntFrom: */
      l33:

        /* begin loadFloatOrIntFrom: */
        if ((tagBits = arg & (tagMask()))) {
          if (tagBits == (smallFloatTag())) {
            value = smallFloatValueOf(arg);
            argDouble = value;
            goto l34;
          }
          if ((primitiveDoMixedArithmetic) &&
              (tagBits == (smallIntegerTag()))) {
            argDouble = ((double)((arg >> 3)));
            goto l34;
          }
        } else {
          if (((longAt((void *)(arg))) & (classIndexMask())) ==
              ClassFloatCompactIndex) {
            fetchFloatAtinto(arg + BaseHeaderSize, resultDouble);
            argDouble = resultDouble;
            goto l34;
          }
        }

        /* begin primitiveFail */
        if (!primFailCode) {
          primFailCode = 1;
        }
        argDouble = 0.0;
        /* end loadFloatOrIntFrom: */
      l34:
        if (!primFailCode) {
          /* begin pop:thenPushFloat: */
          longAtput((sp = stackPointer + (1 * BytesPerWord)),
                    floatObjectOf(rcvrDouble * argDouble));
          stackPointer = sp;
        }

        /* begin internalizeIPandSP */
        localIP = ((char *)instructionPointer);
        localSP = stackPointer;
        localFP = framePointer;
        if (!primFailCode) {
          /* begin fetchNextBytecode */
          currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
          goto l32;
        }
      }
      messageSelector = fetchPointerofObject(
          16U, fetchPointerofObject(SpecialSelectors, specialObjectsOop));
      argumentCount = 1;
      goto normalSend;
    }
      /* end case */
    l32:;
      break;
    case 185: // bytecodePrimDivide
    case 361: // 105	bytecodePrimDivide
    {
      sqInt arg;
      double argDouble;
      usqLong bits;
      sqInt rcvr;
      double rcvrDouble;
      sqInt result;
      double resultDouble;
      char *sp;
      sqInt tagBits;
      double value;

      VM_LABEL(bytecodePrimDivide);
      rcvr = longAt(localSP + (1 * BytesPerOop));
      arg = longAt(localSP);
      if ((((rcvr & arg) & (smallIntegerTag())) != 0)) {
        rcvr = (rcvr >> 3);
        arg = (arg >> 3);
        if ((arg != 0) && ((rcvr % arg) == 0)) {
          result = rcvr / arg;

          /* generates C / operation */
          if ((((((usqInt)(result)) >> 60) + 1) & 15) <= 1) {
            /* begin internalPop:thenPush: */
            longAtput((localSP += 1 * BytesPerOop),
                      (((usqInt)result << 3) | 1));
            currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
            goto l35;
          }
        }
      } else {
        /* begin initPrimCall */
        primFailCode = 0;
        instructionPointer = ((usqInt)localIP);
        stackPointer = localSP;
        framePointer = localFP;

        /* begin primitiveFloatDivide:byArg: */
        /* begin loadFloatOrIntFrom: */
        if ((tagBits = rcvr & (tagMask()))) {
          if (tagBits == (smallFloatTag())) {
            value = smallFloatValueOf(rcvr);
            rcvrDouble = value;
            goto l36;
          }
          if ((primitiveDoMixedArithmetic) &&
              (tagBits == (smallIntegerTag()))) {
            rcvrDouble = ((double)((rcvr >> 3)));
            goto l36;
          }
        } else {
          if (((longAt((void *)(rcvr))) & (classIndexMask())) ==
              ClassFloatCompactIndex) {
            fetchFloatAtinto(rcvr + BaseHeaderSize, resultDouble);
            rcvrDouble = resultDouble;
            goto l36;
          }
        }

        /* begin primitiveFail */
        if (!primFailCode) {
          primFailCode = 1;
        }
        rcvrDouble = 0.0;
        /* end loadFloatOrIntFrom: */
      l36:

        /* begin loadFloatOrIntFrom: */
        if ((tagBits = arg & (tagMask()))) {
          if (tagBits == (smallFloatTag())) {
            value = smallFloatValueOf(arg);
            argDouble = value;
            goto l37;
          }
          if ((primitiveDoMixedArithmetic) &&
              (tagBits == (smallIntegerTag()))) {
            argDouble = ((double)((arg >> 3)));
            goto l37;
          }
        } else {
          if (((longAt((void *)(arg))) & (classIndexMask())) ==
              ClassFloatCompactIndex) {
            fetchFloatAtinto(arg + BaseHeaderSize, resultDouble);
            argDouble = resultDouble;
            goto l37;
          }
        }

        /* begin primitiveFail */
        if (!primFailCode) {
          primFailCode = 1;
        }
        argDouble = 0.0;
        /* end loadFloatOrIntFrom: */
      l37:

        /* begin success: */
        if (!(argDouble != 0.0)) {
          if (!primFailCode) {
            primFailCode = 1;
          }
        }
        if (!primFailCode) {
          /* begin pop:thenPushFloat: */
          longAtput((sp = stackPointer + (1 * BytesPerWord)),
                    floatObjectOf(rcvrDouble / argDouble));
          stackPointer = sp;
        }

        /* begin internalizeIPandSP */
        localIP = ((char *)instructionPointer);
        localSP = stackPointer;
        localFP = framePointer;
        if (!primFailCode) {
          /* begin fetchNextBytecode */
          currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
          goto l35;
        }
      }
      messageSelector = fetchPointerofObject(
          18U, fetchPointerofObject(SpecialSelectors, specialObjectsOop));
      argumentCount = 1;
      goto normalSend;
    }
      /* end case */
    l35:;
      break;
    case 186: // bytecodePrimMod
    case 362: // 106	bytecodePrimMod
    {
      sqInt mod;

      VM_LABEL(bytecodePrimMod);
      /* begin initPrimCall */
      primFailCode = 0;
      mod = doPrimitiveModby(longAt(localSP + (1 * BytesPerOop)),
                             longAt(localSP));
      if (!primFailCode) {
        /* begin internalPop:thenPush: */
        longAtput((localSP += 1 * BytesPerOop), (((usqInt)mod << 3) | 1));
        currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
        goto l38;
      }
      messageSelector = fetchPointerofObject(
          20U, fetchPointerofObject(SpecialSelectors, specialObjectsOop));
      argumentCount = 1;
      goto normalSend;
    }
      /* end case */
    l38:;
      break;
    case 187: // bytecodePrimMakePoint
    case 363: // 107	bytecodePrimMakePoint
    {
      sqInt arg;
      usqInt newObj;
      usqInt numBytes;
      usqInt pt;
      sqInt rcvr;

      VM_LABEL(bytecodePrimMakePoint);
      rcvr = longAt(localSP + (1 * BytesPerOop));
      arg = longAt(localSP);
      if ((/* isFloatOrInt: */
           (((rcvr & (tagMask())) != 0)
                ? !(((rcvr & (characterTag())) != 0))
                : ((longAt((void *)(rcvr))) & (classIndexMask())) ==
                      ClassFloatCompactIndex)) &&
          (/* isFloatOrInt: */
           (((arg & (tagMask())) != 0)
                ? !(((arg & (characterTag())) != 0))
                : ((longAt((void *)(arg))) & (classIndexMask())) ==
                      ClassFloatCompactIndex))) {
        /* begin eeInstantiatePoint */
        /* begin eeInstantiateSmallClassIndex:format:numSlots: */
        assert(((YIndex + 1) >= 0) &&
               ((knownClassAtIndex(ClassPointCompactIndex)) != nilObj));
        assert((nonIndexablePointerFormat()) ==
               (instSpecOfClass(knownClassAtIndex(ClassPointCompactIndex))));

        /* begin allocateSmallNewSpaceSlots:format:classIndex: */
        assert((YIndex + 1) < (numSlotsMask()));
        newObj = freeStart;
        numBytes =
            BaseHeaderSize + (((YIndex + 1) < 1 ? 8 /* allocationUnit */
                                                : (YIndex + 1) * BytesPerOop));
        assert((numBytes % (allocationUnit())) == 0);
        assert((newObj % (allocationUnit())) == 0);
        if ((freeStart + numBytes) > scavengeThreshold) {
          if (!needGCFlag) {
            /* begin scheduleScavenge */
            needGCFlag = 1;
            forceInterruptCheck();
          }
          if ((freeStart + numBytes) > (((eden).limit))) {
            error("no room in eden for "
                  "allocateSmallNewSpaceSlots:format:classIndex:");
            pt = 0;
            goto l40;
          }
        }
        long64Atput(
            (void *)(newObj),
            ((((((usqLong)(YIndex + 1))) << (numSlotsFullShift()))) +
             ((((usqInt)((nonIndexablePointerFormat())) << (formatShift()))))) +
                ClassPointCompactIndex);
        freeStart += numBytes;
        pt = newObj;
        /* end eeInstantiatePoint */
      l40:

        /* No need to check since new object is always new. */

        /* begin storePointerUnchecked:ofObject:withValue: */
        assert((isNonImmediate(pt)) && (!(isForwarded(pt))));
        assert(validStorePointerUncheckedArgs(XIndex, pt, rcvr));
        longAtput((void *)((pt + BaseHeaderSize) +
                           ((((usqInt)(XIndex) << (shiftForWord()))))),
                  rcvr);

        /* begin storePointerUnchecked:ofObject:withValue: */
        assert((isNonImmediate(pt)) && (!(isForwarded(pt))));
        assert(validStorePointerUncheckedArgs(YIndex, pt, arg));
        longAtput((void *)((pt + BaseHeaderSize) +
                           ((((usqInt)(YIndex) << (shiftForWord()))))),
                  arg);

        /* begin internalPop:thenPush: */
        longAtput((localSP += 1 * BytesPerOop), pt);
        currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
        goto l39;
      }
      messageSelector = fetchPointerofObject(
          22U, fetchPointerofObject(SpecialSelectors, specialObjectsOop));
      argumentCount = 1;
      goto normalSend;
    }
      /* end case */
    l39:;
      break;
    case 188: // bytecodePrimBitShift
    case 364: // 108	bytecodePrimBitShift
    {
      sqInt integerArgument;
      sqInt integerReceiver;
      sqInt shifted;
      char *sp;

      VM_LABEL(bytecodePrimBitShift);
      /* begin initPrimCall */
      primFailCode = 0;
      instructionPointer = ((usqInt)localIP);
      stackPointer = localSP;
      framePointer = localFP;

      /* begin primitiveBitShift */
      integerArgument = longAt(stackPointer);
      if (!((((integerArgument) & 7) == 1))) {
        /* begin primitiveFail */
        if (!primFailCode) {
          primFailCode = 1;
        }
        goto l42;
      }
      integerReceiver = longAt(stackPointer + (1 * BytesPerWord));
      integerReceiver = signed64BitValueOf(integerReceiver);
      if (!primFailCode) {
        if (((integerArgument = (integerArgument >> 3))) >= 0) {
          if (!(integerArgument <= 61 /* numSmallIntegerBits */)) {
            /* begin primitiveFail */
            if (!primFailCode) {
              primFailCode = 1;
            }
            goto l42;
          }
          shifted = ((sqInt)((usqInt)(integerReceiver) << integerArgument));
          if (!(integerReceiver == ((shifted) >> integerArgument))) {
            /* begin primitiveFail */
            if (!primFailCode) {
              primFailCode = 1;
            }
            goto l42;
          }
        } else {
          if (!(integerArgument >= (-61 /* numSmallIntegerBits */))) {
            /* begin primitiveFail */
            if (!primFailCode) {
              primFailCode = 1;
            }
            goto l42;
          }
          shifted = (integerReceiver) >> (0 - integerArgument);
        }

        /* Left shift -- must fail bits would be lost
           Right shift -- OK to lose bits */
        shifted = ((((((usqInt)(shifted)) >> 60) + 1) & 15) <= 1
                       ? (((usqInt)shifted << 3) | 1)
                       : signed64BitIntegerFor(shifted));

        /* begin pop:thenPush: */
        longAtput((sp = stackPointer + (1 * BytesPerWord)), shifted);
        stackPointer = sp;
      }
      /* end primitiveBitShift */
    l42:

      /* begin internalizeIPandSP */
      localIP = ((char *)instructionPointer);
      localSP = stackPointer;
      localFP = framePointer;
      if (!primFailCode) {
        /* begin fetchNextBytecode */
        currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
        goto l41;
      }
      messageSelector = fetchPointerofObject(
          24U, fetchPointerofObject(SpecialSelectors, specialObjectsOop));
      argumentCount = 1;
      goto normalSend;
    }
      /* end case */
    l41:;
      break;
    case 189: // bytecodePrimDiv
    case 365: // 109	bytecodePrimDiv
    {
      sqInt quotient;

      VM_LABEL(bytecodePrimDiv);
      /* begin initPrimCall */
      primFailCode = 0;
      quotient = doPrimitiveDivby(longAt(localSP + (1 * BytesPerOop)),
                                  longAt(localSP));
      if (!primFailCode) {
        /* begin internalPop:thenPush: */
        longAtput((localSP += 1 * BytesPerOop), (((usqInt)quotient << 3) | 1));
        currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
        goto l43;
      }
      messageSelector = fetchPointerofObject(
          26U, fetchPointerofObject(SpecialSelectors, specialObjectsOop));
      argumentCount = 1;
      goto normalSend;
    }
      /* end case */
    l43:;
      break;
    case 190: // bytecodePrimBitAnd
    case 366: // 110	bytecodePrimBitAnd
    {
      sqInt arg;
      sqInt rcvr;

      VM_LABEL(bytecodePrimBitAnd);
      arg = longAt(localSP);
      rcvr = longAt(localSP + (1 * BytesPerOop));
      if (((((arg) & 7) == 1)) && ((((rcvr) & 7) == 1))) {
        /* begin internalPop:thenPush: */
        longAtput((localSP += 1 * BytesPerOop), arg & rcvr);
        currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
        goto l44;
      }

      /* begin initPrimCall */
      primFailCode = 0;
      instructionPointer = ((usqInt)localIP);
      stackPointer = localSP;
      framePointer = localFP;
      primitiveBitAnd();

      /* begin internalizeIPandSP */
      localIP = ((char *)instructionPointer);
      localSP = stackPointer;
      localFP = framePointer;
      if (!primFailCode) {
        /* begin fetchNextBytecode */
        currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
        goto l44;
      }
      messageSelector = fetchPointerofObject(
          28U, fetchPointerofObject(SpecialSelectors, specialObjectsOop));
      argumentCount = 1;
      goto normalSend;
    }
      /* end case */
    l44:;
      break;
    case 191: // bytecodePrimBitOr
    case 367: // 111	bytecodePrimBitOr
    {
      sqInt arg;
      sqInt rcvr;

      VM_LABEL(bytecodePrimBitOr);
      arg = longAt(localSP);
      rcvr = longAt(localSP + (1 * BytesPerOop));
      if (((((arg) & 7) == 1)) && ((((rcvr) & 7) == 1))) {
        /* begin internalPop:thenPush: */
        longAtput((localSP += 1 * BytesPerOop), arg | rcvr);
        currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
        goto l45;
      }

      /* begin initPrimCall */
      primFailCode = 0;
      instructionPointer = ((usqInt)localIP);
      stackPointer = localSP;
      framePointer = localFP;
      primitiveBitOr();

      /* begin internalizeIPandSP */
      localIP = ((char *)instructionPointer);
      localSP = stackPointer;
      localFP = framePointer;
      if (!primFailCode) {
        /* begin fetchNextBytecode */
        currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
        goto l45;
      }
      messageSelector = fetchPointerofObject(
          30U, fetchPointerofObject(SpecialSelectors, specialObjectsOop));
      argumentCount = 1;
      goto normalSend;
    }
      /* end case */
    l45:;
      break;
    case 192: // bytecodePrimAt
    case 368: // 112	bytecodePrimAt
    {
      usqInt atIx;
      sqInt class;
      sqInt fixedFields;
      usqLong fmt;
      sqInt fmtSqInt;
      sqLong hdr;
      sqInt index;
      sqInt indexSqInt;
      usqInt numSlots;
      sqInt rcvr;
      sqInt reasonCode;
      sqInt result;
      sqInt resultSqInt;
      sqInt stSize;
      sqInt totalLength;

      VM_LABEL(bytecodePrimAt);
      result = 0;
      index = longAt(localSP);
      rcvr = longAt(localSP + (1 * BytesPerOop));
      if (((!(rcvr & (tagMask())))) && ((((index) & 7) == 1))) {
        /* Index into atCache = 4N, for N = 0 ... 7 */
        atIx = rcvr & AtCacheMask;
        if ((atCache[atIx + AtCacheOop]) != rcvr) {
          lkupClassTag = (longAt((void *)(rcvr))) & (classIndexMask());
          messageSelector = fetchPointerofObject(
              32U, fetchPointerofObject(SpecialSelectors, specialObjectsOop));
          if (!(lookupInMethodCacheSelclassTag(messageSelector,
                                               lkupClassTag))) {
            argumentCount = 1;
            goto commonSendOrdinary;
            goto l46;
          }
          if (primitiveFunctionPointer == primitiveAt) {
            /* begin install:inAtCache:at:string: */
            assert(!(isContext(rcvr)));
            hdr = long64At((void *)(rcvr));
            fmt = (((usqLong)(hdr)) >> (formatShift())) & (formatMask());

            /* For now the AtCache code doesn't support 16-bit indexability. */
            if (((fmt >= (firstShortFormat())) &&
                 (fmt <= ((firstByteFormat()) - 1)))) {
              /* begin primitiveFail */
              if (!primFailCode) {
                primFailCode = 1;
              }
              goto l55;
            }
            if ((fmt == (indexablePointersFormat())) &&
                ((hdr & (classIndexMask())) ==
                 ClassMethodContextCompactIndex)) {
              /* primitiveFailFor: */
              primFailCode = PrimErrBadReceiver;
              goto l55;
            }

            /* Contexts must not be put in the atCache, since their size is not
             * constant */

            /* begin lengthOf:baseHeader:format: */
            /* begin lengthOf:format: */
            numSlots = numSlotsOfAny(rcvr);
            if (fmt <= (ephemeronFormat())) {
              totalLength = numSlots;
              goto l51;
            }
            if (fmt >= (firstByteFormat())) {
              totalLength = ((numSlots << (shiftForWord()))) - (fmt & 7);
              goto l51;
            }

            /* bytes, including CompiledMethod */
            if (fmt >= (firstShortFormat())) {
              totalLength = ((numSlots << ((shiftForWord()) - 1))) - (fmt & 3);
              goto l51;
            }
            if (fmt >= (firstLongFormat())) {
              totalLength = ((numSlots << ((shiftForWord()) - 2))) - (fmt & 1);
              goto l51;
            }
            if (fmt == (sixtyFourBitIndexableFormat())) {
              totalLength = numSlots;
              goto l51;
            }

            /* fmt = self forwardedFormat */
            totalLength = 0;
            /* end lengthOf:baseHeader:format: */
          l51:

            /* begin fixedFieldsOf:format:length: */
            if ((fmt >= (sixtyFourBitIndexableFormat())) ||
                (fmt == (arrayFormat()))) {
              fixedFields = 0;
              goto l54;
            }
            if (fmt < (arrayFormat())) {
              fixedFields = totalLength;
              goto l54;
            }
            class = fetchClassOfNonImm(rcvr);
            fixedFields =
                (((fetchPointerofObject(InstanceSpecificationIndex, class)) >>
                  3)) &
                ((1U << (fixedFieldsFieldWidth())) - 1);
            /* end fixedFieldsOf:format:length: */
          l54:
            atCache[atIx + AtCacheOop] = rcvr;
            atCache[atIx + AtCacheFmt] = fmt;
            atCache[atIx + AtCacheFixedFields] = fixedFields;
            atCache[atIx + AtCacheSize] = (totalLength - fixedFields);
            /* end install:inAtCache:at:string: */
          l55:;
          } else {
            if (primitiveFunctionPointer == primitiveStringAt) {
              /* begin install:inAtCache:at:string: */
              assert(!(isContext(rcvr)));
              hdr = long64At((void *)(rcvr));
              fmt = (((usqLong)(hdr)) >> (formatShift())) & (formatMask());

              /* For now the AtCache code doesn't support 16-bit indexability.
               */
              if (((fmt >= (firstShortFormat())) &&
                   (fmt <= ((firstByteFormat()) - 1)))) {
                /* begin primitiveFail */
                if (!primFailCode) {
                  primFailCode = 1;
                }
                goto l50;
              }

              /* begin lengthOf:baseHeader:format: */
              /* begin lengthOf:format: */
              numSlots = numSlotsOfAny(rcvr);
              if (fmt <= (ephemeronFormat())) {
                totalLength = numSlots;
                goto l47;
              }
              if (fmt >= (firstByteFormat())) {
                totalLength = ((numSlots << (shiftForWord()))) - (fmt & 7);
                goto l47;
              }

              /* bytes, including CompiledMethod */
              if (fmt >= (firstShortFormat())) {
                totalLength =
                    ((numSlots << ((shiftForWord()) - 1))) - (fmt & 3);
                goto l47;
              }
              if (fmt >= (firstLongFormat())) {
                totalLength =
                    ((numSlots << ((shiftForWord()) - 2))) - (fmt & 1);
                goto l47;
              }
              if (fmt == (sixtyFourBitIndexableFormat())) {
                totalLength = numSlots;
                goto l47;
              }

              /* fmt = self forwardedFormat */
              totalLength = 0;
              /* end lengthOf:baseHeader:format: */
            l47:
              fixedFields = 0;

              /* special flag for strings */
              fmt += 32 /* firstStringyFakeFormat */;
              atCache[atIx + AtCacheOop] = rcvr;
              atCache[atIx + AtCacheFmt] = fmt;
              atCache[atIx + AtCacheFixedFields] = fixedFields;
              atCache[atIx + AtCacheSize] = (totalLength - fixedFields);
              /* end install:inAtCache:at:string: */
            l50:;
            } else {
              argumentCount = 1;
              goto commonSendOrdinary;
              goto l46;
            }
          }
        }
        if (!primFailCode) {
          indexSqInt = (index >> 3);

          /* begin commonVariable:at:cacheIndex: */
          stSize = atCache[atIx + AtCacheSize];
          if ((oopisGreaterThanOrEqualTo(indexSqInt, 1)) &&
              (oopisLessThanOrEqualTo(indexSqInt, stSize))) {
            fmtSqInt = atCache[atIx + AtCacheFmt];
            if (fmtSqInt <= (weakArrayFormat())) {
              assert(!(isContextNonImm(rcvr)));
              fixedFields = atCache[atIx + AtCacheFixedFields];
              result =
                  fetchPointerofObject((indexSqInt + fixedFields) - 1, rcvr);
              goto l53;
            }
            if (fmtSqInt < (firstByteFormat())) {
              if (fmtSqInt >= (firstShortFormat())) {
                result =
                    (((usqInt)(((unsigned short)(shortAt(
                          (void *)((rcvr + BaseHeaderSize) +
                                   ((((usqInt)((indexSqInt - 1)) << 1))))))))
                      << 3) |
                     1);
                goto l53;
              }
              if (fmtSqInt == (sixtyFourBitIndexableFormat())) {
                result = positive64BitIntegerFor(
                    long64At((void *)((rcvr + BaseHeaderSize) +
                                      ((((usqInt)((indexSqInt - 1)) << 3))))));
                goto l53;
              }
              resultSqInt =
                  long32At((void *)((rcvr + BaseHeaderSize) +
                                    ((((usqInt)((indexSqInt - 1)) << 2)))));
              result = ((((((usqInt)resultSqInt)) & 0xFFFFFFFFU) << 3) | 1);
              goto l53;
            }

            /* 64, 32, & 16 bits */
            if (fmtSqInt >= 32 /* firstStringyFakeFormat */) {
              resultSqInt =
                  (fmtSqInt < ((firstByteFormat()) +
                               32 /* firstStringyFakeFormat */)
                       ? (fmtSqInt < ((firstShortFormat()) +
                                      32 /* firstStringyFakeFormat */)
                              ? long32At((void *)((rcvr + BaseHeaderSize) +
                                                  ((((usqInt)((indexSqInt - 1))
                                                     << 2)))))
                              : ((unsigned short)(shortAt(
                                    (void *)((rcvr + BaseHeaderSize) +
                                             ((((usqInt)((indexSqInt - 1))
                                                << 1))))))))
                       : byteAt((void *)((rcvr + BaseHeaderSize) +
                                         (indexSqInt - 1))));
              result = ((((usqInt)(resultSqInt) << (numTagBits())))) +
                       (characterTag());
              goto l53;
            } else {
              if ((fmtSqInt < (firstCompiledMethodFormat())) ||
                  (indexSqInt >=
                   ((((literalCountOf(rcvr)) + LiteralStart) * BytesPerOop) +
                    1))) {
                result = (((usqInt)(byteAt((void *)((rcvr + BaseHeaderSize) +
                                                    (indexSqInt - 1))))
                           << 3) |
                          1);
                goto l53;
              }
            }
          }
          reasonCode =
              (isIndexable(rcvr) ? PrimErrBadIndex : PrimErrBadReceiver);
          result = (primFailCode = reasonCode);
          /* end commonVariable:at:cacheIndex: */
        l53:;
        }
        if (!primFailCode) {
          /* begin fetchNextBytecode */
          currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
          longAtput((localSP += 1 * BytesPerOop), result);
          goto l46;
        }

        /* begin initPrimCall */
        primFailCode = 0;
      }
      messageSelector = fetchPointerofObject(
          32U, fetchPointerofObject(SpecialSelectors, specialObjectsOop));
      argumentCount = 1;
      goto normalSend;
    }
      /* end case */
    l46:;
      break;
    case 193: // bytecodePrimAtPut
    case 369: // 113	bytecodePrimAtPut
    {
      usqInt atIx;
      sqInt class;
      sqInt correctRcvr;
      sqInt fieldIndex;
      sqInt fixedFields;
      usqLong fmt;
      sqInt fmtSqInt;
      sqLong hdr;
      sqInt index;
      sqInt indexSqInt;
      int isCharacter;
      usqInt numSlots;
      sqInt rcvr;
      sqInt reasonCode;
      sqInt stSize;
      sqInt totalLength;
      usqLong val64ToPut;
      sqInt valToPut;
      sqInt value;

      VM_LABEL(bytecodePrimAtPut);
      value = longAt(localSP);
      index = longAt(localSP + (1 * BytesPerOop));
      rcvr = longAt(localSP + (2 * BytesPerOop));
#if IMMUTABILITY
      correctRcvr = /* isOopMutable: */
          ((!(rcvr & (tagMask())))) &&
          (!(((((usqInt)((byteAt(
                   (void *)(rcvr + (immutableExtraBitsByteOffset())))))) >>
               (immutableBitByteShift())) &
              1) != 0));
#else // IMMUTABILITY
      correctRcvr = (!(rcvr & (tagMask())));
#endif

      if (correctRcvr && ((((index) & 7) == 1))) {
        /* Index into atPutCache */
        atIx = (rcvr & AtCacheMask) + AtPutBase;
        if ((atCache[atIx + AtCacheOop]) != rcvr) {
          lkupClassTag = (longAt((void *)(rcvr))) & (classIndexMask());
          messageSelector = fetchPointerofObject(
              34U, fetchPointerofObject(SpecialSelectors, specialObjectsOop));
          if (!(lookupInMethodCacheSelclassTag(messageSelector,
                                               lkupClassTag))) {
            argumentCount = 2;
            goto commonSendOrdinary;
            goto l56;
          }
          if (primitiveFunctionPointer == primitiveAtPut) {
            /* begin install:inAtCache:at:string: */
            assert(!(isContext(rcvr)));
            hdr = long64At((void *)(rcvr));
            fmt = (((usqLong)(hdr)) >> (formatShift())) & (formatMask());

            /* For now the AtCache code doesn't support 16-bit indexability. */
            if (((fmt >= (firstShortFormat())) &&
                 (fmt <= ((firstByteFormat()) - 1)))) {
              /* begin primitiveFail */
              if (!primFailCode) {
                primFailCode = 1;
              }
              goto l65;
            }
            if ((fmt == (indexablePointersFormat())) &&
                ((hdr & (classIndexMask())) ==
                 ClassMethodContextCompactIndex)) {
              /* primitiveFailFor: */
              primFailCode = PrimErrBadReceiver;
              goto l65;
            }

            /* Contexts must not be put in the atCache, since their size is not
             * constant */

            /* begin lengthOf:baseHeader:format: */
            /* begin lengthOf:format: */
            numSlots = numSlotsOfAny(rcvr);
            if (fmt <= (ephemeronFormat())) {
              totalLength = numSlots;
              goto l61;
            }
            if (fmt >= (firstByteFormat())) {
              totalLength = ((numSlots << (shiftForWord()))) - (fmt & 7);
              goto l61;
            }

            /* bytes, including CompiledMethod */
            if (fmt >= (firstShortFormat())) {
              totalLength = ((numSlots << ((shiftForWord()) - 1))) - (fmt & 3);
              goto l61;
            }
            if (fmt >= (firstLongFormat())) {
              totalLength = ((numSlots << ((shiftForWord()) - 2))) - (fmt & 1);
              goto l61;
            }
            if (fmt == (sixtyFourBitIndexableFormat())) {
              totalLength = numSlots;
              goto l61;
            }

            /* fmt = self forwardedFormat */
            totalLength = 0;
            /* end lengthOf:baseHeader:format: */
          l61:

            /* begin fixedFieldsOf:format:length: */
            if ((fmt >= (sixtyFourBitIndexableFormat())) ||
                (fmt == (arrayFormat()))) {
              fixedFields = 0;
              goto l64;
            }
            if (fmt < (arrayFormat())) {
              fixedFields = totalLength;
              goto l64;
            }
            class = fetchClassOfNonImm(rcvr);
            fixedFields =
                (((fetchPointerofObject(InstanceSpecificationIndex, class)) >>
                  3)) &
                ((1U << (fixedFieldsFieldWidth())) - 1);
            /* end fixedFieldsOf:format:length: */
          l64:
            atCache[atIx + AtCacheOop] = rcvr;
            atCache[atIx + AtCacheFmt] = fmt;
            atCache[atIx + AtCacheFixedFields] = fixedFields;
            atCache[atIx + AtCacheSize] = (totalLength - fixedFields);
            /* end install:inAtCache:at:string: */
          l65:;
          } else {
            if (primitiveFunctionPointer == primitiveStringAtPut) {
              /* begin install:inAtCache:at:string: */
              assert(!(isContext(rcvr)));
              hdr = long64At((void *)(rcvr));
              fmt = (((usqLong)(hdr)) >> (formatShift())) & (formatMask());

              /* For now the AtCache code doesn't support 16-bit indexability.
               */
              if (((fmt >= (firstShortFormat())) &&
                   (fmt <= ((firstByteFormat()) - 1)))) {
                /* begin primitiveFail */
                if (!primFailCode) {
                  primFailCode = 1;
                }
                goto l60;
              }

              /* begin lengthOf:baseHeader:format: */
              /* begin lengthOf:format: */
              numSlots = numSlotsOfAny(rcvr);
              if (fmt <= (ephemeronFormat())) {
                totalLength = numSlots;
                goto l57;
              }
              if (fmt >= (firstByteFormat())) {
                totalLength = ((numSlots << (shiftForWord()))) - (fmt & 7);
                goto l57;
              }

              /* bytes, including CompiledMethod */
              if (fmt >= (firstShortFormat())) {
                totalLength =
                    ((numSlots << ((shiftForWord()) - 1))) - (fmt & 3);
                goto l57;
              }
              if (fmt >= (firstLongFormat())) {
                totalLength =
                    ((numSlots << ((shiftForWord()) - 2))) - (fmt & 1);
                goto l57;
              }
              if (fmt == (sixtyFourBitIndexableFormat())) {
                totalLength = numSlots;
                goto l57;
              }

              /* fmt = self forwardedFormat */
              totalLength = 0;
              /* end lengthOf:baseHeader:format: */
            l57:
              fixedFields = 0;

              /* special flag for strings */
              fmt += 32 /* firstStringyFakeFormat */;
              atCache[atIx + AtCacheOop] = rcvr;
              atCache[atIx + AtCacheFmt] = fmt;
              atCache[atIx + AtCacheFixedFields] = fixedFields;
              atCache[atIx + AtCacheSize] = (totalLength - fixedFields);
              /* end install:inAtCache:at:string: */
            l60:;
            } else {
              argumentCount = 2;
              goto commonSendOrdinary;
              goto l56;
            }
          }
        }
        if (!primFailCode) {
          indexSqInt = (index >> 3);

          /* begin commonVariable:at:put:cacheIndex: */
          stSize = atCache[atIx + AtCacheSize];
          if ((oopisGreaterThanOrEqualTo(indexSqInt, 1)) &&
              (oopisLessThanOrEqualTo(indexSqInt, stSize))) {
            fmtSqInt = atCache[atIx + AtCacheFmt];
            if (fmtSqInt <= (weakArrayFormat())) {
              assert(!(isContextNonImm(rcvr)));
              fixedFields = atCache[atIx + AtCacheFixedFields];
              fieldIndex = (indexSqInt + fixedFields) - 1;

              /* begin storePointer:ofObject:withValue: */
              assert(validStorePointerArgs(fieldIndex, rcvr, value));
              assert(isNonImmediate(rcvr));
              if (oopisGreaterThanOrEqualTo(rcvr, oldSpaceStart)) {
                if (/* isYoung: */
                    ((!(value & (tagMask())))) &&
                    (oopisLessThan(value, oldSpaceStart))) {
                  /* begin possibleRootStoreInto: */
                  if (!((byteAt((void *)(rcvr + (formatFieldByteOffset())))) &
                        (1U << (rememberedBitByteShift())))) {
                    remember(rcvr);
                  }
                }
              }

              /* most stores into young objects */
              ((void)(longAtput(
                  (void *)((rcvr + BaseHeaderSize) +
                           ((((usqInt)(fieldIndex) << (shiftForWord()))))),
                  value)));
              goto l63;
            }
            if (fmtSqInt < (firstByteFormat())) {
              if (fmtSqInt >= (firstShortFormat())) {
                valToPut = ((((value) & 7) == 1) ? (value >> 3) : -1);
                if (!((valToPut >= 0) && (valToPut <= 0xFFFF))) {
                  /* primitiveFailFor: */
                  primFailCode = PrimErrBadArgument;
                  goto l63;
                }

                /* storeShort16:ofObject:withValue: */
                shortAtput((void *)((rcvr + BaseHeaderSize) +
                                    ((((usqInt)((indexSqInt - 1)) << 1)))),
                           valToPut);
                goto l63;
              }
              if (fmtSqInt == (sixtyFourBitIndexableFormat())) {
                val64ToPut = positive64BitValueOf(value);
                if (!primFailCode) {
                  /* storeLong64:ofObject:withValue: */
                  long64Atput((void *)((rcvr + BaseHeaderSize) +
                                       ((((usqInt)((indexSqInt - 1)) << 3)))),
                              val64ToPut);
                  goto l63;
                }

                /* primitiveFailFor: */
                primFailCode = PrimErrBadArgument;
                goto l63;
              }
              valToPut = positive32BitValueOf(value);
              if (!primFailCode) {
                /* storeLong32:ofObject:withValue: */
                long32Atput((void *)((rcvr + BaseHeaderSize) +
                                     ((((usqInt)((indexSqInt - 1)) << 2)))),
                            valToPut);
                goto l63;
              }

              /* primitiveFailFor: */
              primFailCode = PrimErrBadArgument;
              goto l63;
            }

            /* 64, 32, & 16 bits */
            if (fmtSqInt >= 32 /* firstStringyFakeFormat */) {
              isCharacter = ((value & (characterTag())) != 0);
              if (!isCharacter) {
                /* primitiveFailFor: */
                primFailCode = PrimErrBadArgument;
                goto l63;
              }
              valToPut = ((((usqInt)value))) >> (numTagBits());
              if (fmtSqInt <
                  ((firstByteFormat()) + 32 /* firstStringyFakeFormat */)) {
                if (fmtSqInt <
                    ((firstShortFormat()) + 32 /* firstStringyFakeFormat */)) {
                  assert(fmtSqInt != ((sixtyFourBitIndexableFormat()) +
                                      (firstStringyFakeFormat())));

                  /* storeLong32:ofObject:withValue: */
                  long32Atput((void *)((rcvr + BaseHeaderSize) +
                                       ((((usqInt)((indexSqInt - 1)) << 2)))),
                              valToPut);
                  goto l63;
                } else {
                  if (!((valToPut >= 0) && (valToPut <= 0xFFFF))) {
                    /* primitiveFailFor: */
                    primFailCode = PrimErrBadArgument;
                    goto l63;
                  }

                  /* storeShort16:ofObject:withValue: */
                  shortAtput((void *)((rcvr + BaseHeaderSize) +
                                      ((((usqInt)((indexSqInt - 1)) << 1)))),
                             valToPut);
                  goto l63;
                }
              }
            } else {
              if ((fmtSqInt >= (firstCompiledMethodFormat())) &&
                  (indexSqInt <
                   ((((literalCountOf(rcvr)) + LiteralStart) * BytesPerOop) +
                    1))) {
                /* primitiveFailFor: */
                primFailCode = PrimErrBadIndex;
                goto l63;
              }
              valToPut = ((((value) & 7) == 1) ? (value >> 3) : -1);
            }

            /* Note fmt >= firstStringyFormat is an artificial flag for strings
             */
            if (!((valToPut >= 0) && (valToPut <= 0xFF))) {
              /* primitiveFailFor: */
              primFailCode = PrimErrBadArgument;
              goto l63;
            }

            /* storeByte:ofObject:withValue: */
            byteAtput((void *)((rcvr + BaseHeaderSize) + (indexSqInt - 1)),
                      valToPut);
            goto l63;
          }
          reasonCode =
              (isIndexable(rcvr) ? PrimErrBadIndex : PrimErrBadReceiver);
          primFailCode = reasonCode;
          /* end commonVariable:at:put:cacheIndex: */
        l63:;
        }
        if (!primFailCode) {
          /* begin fetchNextBytecode */
          currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
          longAtput((localSP += 2 * BytesPerOop), value);
          goto l56;
        }

        /* begin initPrimCall */
        primFailCode = 0;
      }
      messageSelector = fetchPointerofObject(
          34U, fetchPointerofObject(SpecialSelectors, specialObjectsOop));
      argumentCount = 2;
      goto normalSend;
    }
      /* end case */
    l56:;
      break;
    case 194: // bytecodePrimSize
    case 370: // 114	bytecodePrimSize
    {
      sqInt ccIndex;
      sqInt fmt;
      sqInt isArray;
      sqInt isString;
      usqInt numSlots;
      sqInt rcvr;
      sqInt sz;

      VM_LABEL(bytecodePrimSize);
      /* begin initPrimCall */
      primFailCode = 0;
      rcvr = longAt(localSP);

      /* Shortcut the mega-lookup for ByteString and Array, the two big
         consumers of cycles here. Both of these have compact class indices and
         neither has any added fields. */

      /* begin isInstanceOfClassByteString: */
      /* begin is:instanceOf:compactClassIndex: */
      if (((rcvr & (tagMask())) != 0)) {
        isString = 0;
        goto l67;
      }

      /* begin isClassOfNonImm:equalTo:compactClassIndex: */
      assert(!(isImmediate(rcvr)));
      ccIndex = (longAt((void *)(rcvr))) & (classIndexMask());
      isString = ClassByteStringCompactIndex == ccIndex;
      /* end isInstanceOfClassByteString: */
    l67:
      if (isString) {
        /* begin lengthOf: */
        fmt = (byteAt((void *)(rcvr + (formatFieldByteOffset())))) &
              (formatMask());
        numSlots = numSlotsOfAny(rcvr);
        if (fmt <= (ephemeronFormat())) {
          sz = numSlots;
          goto l68;
        }
        if (fmt >= (firstByteFormat())) {
          sz = ((numSlots << (shiftForWord()))) - (fmt & 7);
          goto l68;
        }

        /* bytes, including CompiledMethod */
        if (fmt >= (firstShortFormat())) {
          sz = ((numSlots << ((shiftForWord()) - 1))) - (fmt & 3);
          goto l68;
        }
        if (fmt >= (firstLongFormat())) {
          sz = ((numSlots << ((shiftForWord()) - 2))) - (fmt & 1);
          goto l68;
        }
        if (fmt == (sixtyFourBitIndexableFormat())) {
          sz = numSlots;
          goto l68;
        }

        /* fmt = self forwardedFormat */
        sz = 0;
        /* end lengthOf: */
      l68:

        /* internalStackTopPut: */
        longAtput(localSP, (((usqInt)sz << 3) | 1));

        /* begin fetchNextBytecode */
        currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
        goto l66;
      }

      /* begin isInstanceOfClassArray: */
      /* begin is:instanceOf:compactClassIndex: */
      if (((rcvr & (tagMask())) != 0)) {
        isArray = 0;
        goto l69;
      }

      /* begin isClassOfNonImm:equalTo:compactClassIndex: */
      assert(!(isImmediate(rcvr)));
      ccIndex = (longAt((void *)(rcvr))) & (classIndexMask());
      isArray = ClassArrayCompactIndex == ccIndex;
      /* end isInstanceOfClassArray: */
    l69:
      if (isArray) {
        /* begin lengthOf: */
        fmt = (byteAt((void *)(rcvr + (formatFieldByteOffset())))) &
              (formatMask());
        numSlots = numSlotsOfAny(rcvr);
        if (fmt <= (ephemeronFormat())) {
          sz = numSlots;
          goto l70;
        }
        if (fmt >= (firstByteFormat())) {
          sz = ((numSlots << (shiftForWord()))) - (fmt & 7);
          goto l70;
        }

        /* bytes, including CompiledMethod */
        if (fmt >= (firstShortFormat())) {
          sz = ((numSlots << ((shiftForWord()) - 1))) - (fmt & 3);
          goto l70;
        }
        if (fmt >= (firstLongFormat())) {
          sz = ((numSlots << ((shiftForWord()) - 2))) - (fmt & 1);
          goto l70;
        }
        if (fmt == (sixtyFourBitIndexableFormat())) {
          sz = numSlots;
          goto l70;
        }

        /* fmt = self forwardedFormat */
        sz = 0;
        /* end lengthOf: */
      l70:

        /* internalStackTopPut: */
        longAtput(localSP, (((usqInt)sz << 3) | 1));

        /* begin fetchNextBytecode */
        currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
        goto l66;
      }
      messageSelector = fetchPointerofObject(
          36U, fetchPointerofObject(SpecialSelectors, specialObjectsOop));
      argumentCount = 0;
      goto normalSend;
    }
      /* end case */
    l66:;
      break;
    case 195: // bytecodePrimNext
    case 371: // 115	bytecodePrimNext
    {
      VM_LABEL(bytecodePrimNext);
      messageSelector = fetchPointerofObject(
          38U, fetchPointerofObject(SpecialSelectors, specialObjectsOop));
      argumentCount = ((fetchPointerofObject(
                           39, fetchPointerofObject(SpecialSelectors,
                                                    specialObjectsOop))) >>
                       3);
      goto normalSend;
    } break;
    case 196: // bytecodePrimNextPut
    case 372: // 116	bytecodePrimNextPut
    {
      VM_LABEL(bytecodePrimNextPut);
      messageSelector = fetchPointerofObject(
          40U, fetchPointerofObject(SpecialSelectors, specialObjectsOop));
      argumentCount = ((fetchPointerofObject(
                           41, fetchPointerofObject(SpecialSelectors,
                                                    specialObjectsOop))) >>
                       3);
      goto normalSend;
    } break;
    case 197: // bytecodePrimAtEnd
    case 373: // 117	bytecodePrimAtEnd
    {
      VM_LABEL(bytecodePrimAtEnd);
      messageSelector = fetchPointerofObject(
          42U, fetchPointerofObject(SpecialSelectors, specialObjectsOop));
      argumentCount = ((fetchPointerofObject(
                           43, fetchPointerofObject(SpecialSelectors,
                                                    specialObjectsOop))) >>
                       3);
      goto normalSend;
    } break;
    case 198: // bytecodePrimIdentical
    {
      sqInt arg;
      sqInt rcvr;

      VM_LABEL(bytecodePrimIdentical);
      rcvr = longAt(localSP + (1 * BytesPerOop));
      if (isOopForwarded(rcvr)) {
        rcvr = handleSpecialSelectorSendFaultForfpsp(rcvr, localFP, localSP);
      }
      arg = longAt(localSP);
      if (isOopForwarded(arg)) {
        arg = handleSpecialSelectorSendFaultForfpsp(arg, localFP, localSP);
      }

      /* begin booleanCheat: */
      if (rcvr == arg) {
        goto booleanCheatTrue;
      } else {
        goto booleanCheatFalse;
      }
    } break;
    case 199: // bytecodePrimClass
    case 375: // 119	bytecodePrimClass
    {
      sqInt aValue;
      sqInt rcvr;
      sqInt tagBits;

      VM_LABEL(bytecodePrimClass);
      rcvr = longAt(localSP);
      if (isOopForwarded(rcvr)) {
        rcvr = handleSpecialSelectorSendFaultForfpsp(rcvr, localFP, localSP);
      }
      aValue = /* fetchClassOf: */
          ((tagBits = rcvr & (tagMask()))
               ? fetchPointerofObject(tagBits, classTableFirstPage)
               : fetchClassOfNonImm(rcvr));

      /* begin internalStackTopPut: */
      longAtput(localSP, aValue);
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
    } break;
    case 200: // bytecodePrimNotIdentical
    {
      sqInt arg;
      sqInt rcvr;

      VM_LABEL(bytecodePrimNotIdentical);
      rcvr = longAt(localSP + (1 * BytesPerOop));
      if (isOopForwarded(rcvr)) {
        rcvr = handleSpecialSelectorSendFaultForfpsp(rcvr, localFP, localSP);
      }
      arg = longAt(localSP);
      if (isOopForwarded(arg)) {
        arg = handleSpecialSelectorSendFaultForfpsp(arg, localFP, localSP);
      }

      /* begin booleanCheat: */
      if (rcvr != arg) {
        goto booleanCheatTrue;
      } else {
        goto booleanCheatFalse;
      }
    } break;
    case 201: // bytecodePrimValue
    case 377: // 121	bytecodePrimValue
    {
      sqInt ccIndex;
      sqInt isBlock;
      sqInt rcvr;

      VM_LABEL(bytecodePrimValue);
      rcvr = longAt(localSP);
      argumentCount = 0;

      /* begin isInstanceOfClassBlockClosure: */
      /* begin is:instanceOf:compactClassIndex: */
      if (((rcvr & (tagMask())) != 0)) {
        isBlock = 0;
        goto l72;
      }

      /* begin isClassOfNonImm:equalTo:compactClassIndex: */
      assert(!(isImmediate(rcvr)));
      ccIndex = (longAt((void *)(rcvr))) & (classIndexMask());
      isBlock = ClassBlockClosureCompactIndex == ccIndex;
      /* end isInstanceOfClassBlockClosure: */
    l72:
      if (isBlock) {
        /* begin externalizeIPandSP */
        instructionPointer = ((usqInt)localIP);
        stackPointer = localSP;
        framePointer = localFP;

        /* begin initPrimCall */
        primFailCode = 0;
        primitiveClosureValue();

        /* begin internalizeIPandSP */
        localIP = ((char *)instructionPointer);
        localSP = stackPointer;
        localFP = framePointer;
        if (!primFailCode) {
          /* begin fetchNextBytecode */
          currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
          goto l71;
        }
        primFailCode = 0;
      }
      messageSelector = fetchPointerofObject(
          50U, fetchPointerofObject(SpecialSelectors, specialObjectsOop));
      goto normalSend;
    }
      /* end case */
    l71:;
      break;
    case 202: // bytecodePrimValueWithArg
    case 378: // 122	bytecodePrimValueWithArg
    {
      sqInt ccIndex;
      sqInt isBlock;
      sqInt rcvr;

      VM_LABEL(bytecodePrimValueWithArg);
      rcvr = longAt(localSP + (1 * BytesPerOop));
      argumentCount = 1;

      /* begin isInstanceOfClassBlockClosure: */
      /* begin is:instanceOf:compactClassIndex: */
      if (((rcvr & (tagMask())) != 0)) {
        isBlock = 0;
        goto l74;
      }

      /* begin isClassOfNonImm:equalTo:compactClassIndex: */
      assert(!(isImmediate(rcvr)));
      ccIndex = (longAt((void *)(rcvr))) & (classIndexMask());
      isBlock = ClassBlockClosureCompactIndex == ccIndex;
      /* end isInstanceOfClassBlockClosure: */
    l74:
      if (isBlock) {
        /* begin externalizeIPandSP */
        instructionPointer = ((usqInt)localIP);
        stackPointer = localSP;
        framePointer = localFP;

        /* begin initPrimCall */
        primFailCode = 0;
        primitiveClosureValue();

        /* begin internalizeIPandSP */
        localIP = ((char *)instructionPointer);
        localSP = stackPointer;
        localFP = framePointer;
        if (!primFailCode) {
          /* begin fetchNextBytecode */
          currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
          goto l73;
        }
        primFailCode = 0;
      }
      messageSelector = fetchPointerofObject(
          52U, fetchPointerofObject(SpecialSelectors, specialObjectsOop));
      goto normalSend;
    }
      /* end case */
    l73:;
      break;
    case 203: // bytecodePrimDo
    case 379: // 123	bytecodePrimDo
    {
      VM_LABEL(bytecodePrimDo);
      messageSelector = fetchPointerofObject(
          54U, fetchPointerofObject(SpecialSelectors, specialObjectsOop));
      argumentCount = ((fetchPointerofObject(
                           55, fetchPointerofObject(SpecialSelectors,
                                                    specialObjectsOop))) >>
                       3);
      goto normalSend;
    } break;
    case 204: // bytecodePrimNew
    case 380: // 124	bytecodePrimNew
    {
      VM_LABEL(bytecodePrimNew);
      messageSelector = fetchPointerofObject(
          56U, fetchPointerofObject(SpecialSelectors, specialObjectsOop));
      argumentCount = 0;
      goto normalSend;
    } break;
    case 205: // bytecodePrimNewWithArg
    case 381: // 125	bytecodePrimNewWithArg
    {
      VM_LABEL(bytecodePrimNewWithArg);
      messageSelector = fetchPointerofObject(
          58U, fetchPointerofObject(SpecialSelectors, specialObjectsOop));
      argumentCount = 1;
      goto normalSend;
    } break;
    case 206: // bytecodePrimPointX
    case 382: // 126	bytecodePrimPointX
    {
      sqInt ccIndex;
      sqInt isClassOfNonImmequalTocompactClassIndexRV;
      sqInt rcvr;

      VM_LABEL(bytecodePrimPointX);
      /* begin initPrimCall */
      primFailCode = 0;
      rcvr = longAt(localSP);
      if ((!(rcvr & (tagMask())))) {
        /* begin isClassOfNonImm:equalTo:compactClassIndex: */
        assert(!(isImmediate(rcvr)));
        ccIndex = (longAt((void *)(rcvr))) & (classIndexMask());
        isClassOfNonImmequalTocompactClassIndexRV =
            ClassPointCompactIndex == ccIndex;
        if (isClassOfNonImmequalTocompactClassIndexRV) {
          longAtput(localSP, fetchPointerofObject(XIndex, rcvr));

          /* begin fetchNextBytecode */
          currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
          goto l75;
        }
      }
      messageSelector = fetchPointerofObject(
          60U, fetchPointerofObject(SpecialSelectors, specialObjectsOop));
      argumentCount = 0;
      goto normalSend;
    }
      /* end case */
    l75:;
      break;
    case 207: // bytecodePrimPointY
    case 383: // 127	bytecodePrimPointY
    {
      sqInt ccIndex;
      sqInt isClassOfNonImmequalTocompactClassIndexRV;
      sqInt rcvr;

      VM_LABEL(bytecodePrimPointY);
      /* begin initPrimCall */
      primFailCode = 0;
      rcvr = longAt(localSP);
      if ((!(rcvr & (tagMask())))) {
        /* begin isClassOfNonImm:equalTo:compactClassIndex: */
        assert(!(isImmediate(rcvr)));
        ccIndex = (longAt((void *)(rcvr))) & (classIndexMask());
        isClassOfNonImmequalTocompactClassIndexRV =
            ClassPointCompactIndex == ccIndex;
        if (isClassOfNonImmequalTocompactClassIndexRV) {
          longAtput(localSP, fetchPointerofObject(YIndex, rcvr));

          /* begin fetchNextBytecode */
          currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
          goto l76;
        }
      }
      messageSelector = fetchPointerofObject(
          0x3EU, fetchPointerofObject(SpecialSelectors, specialObjectsOop));
      argumentCount = 0;
      goto normalSend;
    }
      /* end case */
    l76:;
      break;
    case 208: // sendLiteralSelector0ArgsBytecode
    case 209: // sendLiteralSelector0ArgsBytecode
    case 210: // sendLiteralSelector0ArgsBytecode
    case 211: // sendLiteralSelector0ArgsBytecode
    case 212: // sendLiteralSelector0ArgsBytecode
    case 213: // sendLiteralSelector0ArgsBytecode
    case 214: // sendLiteralSelector0ArgsBytecode
    case 215: // sendLiteralSelector0ArgsBytecode
    case 216: // sendLiteralSelector0ArgsBytecode
    case 217: // sendLiteralSelector0ArgsBytecode
    case 218: // sendLiteralSelector0ArgsBytecode
    case 219: // sendLiteralSelector0ArgsBytecode
    case 220: // sendLiteralSelector0ArgsBytecode
    case 221: // sendLiteralSelector0ArgsBytecode
    case 222: // sendLiteralSelector0ArgsBytecode
    case 223: // sendLiteralSelector0ArgsBytecode
    case 384: // 128	sendLiteralSelector0ArgsBytecode
    case 385: // 129	sendLiteralSelector0ArgsBytecode
    case 386: // 130	sendLiteralSelector0ArgsBytecode
    case 387: // 131	sendLiteralSelector0ArgsBytecode
    case 388: // 132	sendLiteralSelector0ArgsBytecode
    case 389: // 133	sendLiteralSelector0ArgsBytecode
    case 390: // 134	sendLiteralSelector0ArgsBytecode
    case 391: // 135	sendLiteralSelector0ArgsBytecode
    case 392: // 136	sendLiteralSelector0ArgsBytecode
    case 393: // 137	sendLiteralSelector0ArgsBytecode
    case 394: // 138	sendLiteralSelector0ArgsBytecode
    case 395: // 139	sendLiteralSelector0ArgsBytecode
    case 396: // 140	sendLiteralSelector0ArgsBytecode
    case 397: // 141	sendLiteralSelector0ArgsBytecode
    case 398: // 142	sendLiteralSelector0ArgsBytecode
    case 399: // 143	sendLiteralSelector0ArgsBytecode
    {
      sqInt rcvr;
      sqInt tagBits;

      VM_LABEL(sendLiteralSelector0ArgsBytecode);
      /* begin literal: */
      assert(method == (iframeMethod(localFP)));
      messageSelector =
          fetchPointerofObject((currentBytecode & 15) + LiteralStart, method);
      argumentCount = 0;
      rcvr = longAt(localSP);
      lkupClassTag = /* fetchClassTagOf: */
          ((tagBits = rcvr & (tagMask()))
               ? tagBits
               : (longAt((void *)(rcvr))) & (classIndexMask()));
      assert(lkupClassTag != (nilObject()));
      goto commonSendOrdinary;
    } break;
    case 224: // sendLiteralSelector1ArgBytecode
    case 225: // sendLiteralSelector1ArgBytecode
    case 226: // sendLiteralSelector1ArgBytecode
    case 227: // sendLiteralSelector1ArgBytecode
    case 228: // sendLiteralSelector1ArgBytecode
    case 229: // sendLiteralSelector1ArgBytecode
    case 230: // sendLiteralSelector1ArgBytecode
    case 231: // sendLiteralSelector1ArgBytecode
    case 232: // sendLiteralSelector1ArgBytecode
    case 233: // sendLiteralSelector1ArgBytecode
    case 234: // sendLiteralSelector1ArgBytecode
    case 235: // sendLiteralSelector1ArgBytecode
    case 236: // sendLiteralSelector1ArgBytecode
    case 237: // sendLiteralSelector1ArgBytecode
    case 238: // sendLiteralSelector1ArgBytecode
    case 239: // sendLiteralSelector1ArgBytecode
    case 400: // 144	sendLiteralSelector1ArgBytecode
    case 401: // 145	sendLiteralSelector1ArgBytecode
    case 402: // 146	sendLiteralSelector1ArgBytecode
    case 403: // 147	sendLiteralSelector1ArgBytecode
    case 404: // 148	sendLiteralSelector1ArgBytecode
    case 405: // 149	sendLiteralSelector1ArgBytecode
    case 406: // 150	sendLiteralSelector1ArgBytecode
    case 407: // 151	sendLiteralSelector1ArgBytecode
    case 408: // 152	sendLiteralSelector1ArgBytecode
    case 409: // 153	sendLiteralSelector1ArgBytecode
    case 410: // 154	sendLiteralSelector1ArgBytecode
    case 411: // 155	sendLiteralSelector1ArgBytecode
    case 412: // 156	sendLiteralSelector1ArgBytecode
    case 413: // 157	sendLiteralSelector1ArgBytecode
    case 414: // 158	sendLiteralSelector1ArgBytecode
    case 415: // 159	sendLiteralSelector1ArgBytecode
    {
      sqInt rcvr;
      sqInt tagBits;

      VM_LABEL(sendLiteralSelector1ArgBytecode);
      /* begin literal: */
      assert(method == (iframeMethod(localFP)));
      messageSelector =
          fetchPointerofObject((currentBytecode & 15) + LiteralStart, method);
      argumentCount = 1;
      rcvr = longAt(localSP + (1 * BytesPerOop));
      lkupClassTag = /* fetchClassTagOf: */
          ((tagBits = rcvr & (tagMask()))
               ? tagBits
               : (longAt((void *)(rcvr))) & (classIndexMask()));
      assert(lkupClassTag != (nilObject()));
      goto commonSendOrdinary;
    } break;
    case 240: // sendLiteralSelector2ArgsBytecode
    case 241: // sendLiteralSelector2ArgsBytecode
    case 242: // sendLiteralSelector2ArgsBytecode
    case 243: // sendLiteralSelector2ArgsBytecode
    case 244: // sendLiteralSelector2ArgsBytecode
    case 245: // sendLiteralSelector2ArgsBytecode
    case 246: // sendLiteralSelector2ArgsBytecode
    case 247: // sendLiteralSelector2ArgsBytecode
    case 248: // sendLiteralSelector2ArgsBytecode
    case 249: // sendLiteralSelector2ArgsBytecode
    case 250: // sendLiteralSelector2ArgsBytecode
    case 251: // sendLiteralSelector2ArgsBytecode
    case 252: // sendLiteralSelector2ArgsBytecode
    case 253: // sendLiteralSelector2ArgsBytecode
    case 254: // sendLiteralSelector2ArgsBytecode
    case 255: // sendLiteralSelector2ArgsBytecode
    case 416: // 160	sendLiteralSelector2ArgsBytecode
    case 417: // 161	sendLiteralSelector2ArgsBytecode
    case 418: // 162	sendLiteralSelector2ArgsBytecode
    case 419: // 163	sendLiteralSelector2ArgsBytecode
    case 420: // 164	sendLiteralSelector2ArgsBytecode
    case 421: // 165	sendLiteralSelector2ArgsBytecode
    case 422: // 166	sendLiteralSelector2ArgsBytecode
    case 423: // 167	sendLiteralSelector2ArgsBytecode
    case 424: // 168	sendLiteralSelector2ArgsBytecode
    case 425: // 169	sendLiteralSelector2ArgsBytecode
    case 426: // 170	sendLiteralSelector2ArgsBytecode
    case 427: // 171	sendLiteralSelector2ArgsBytecode
    case 428: // 172	sendLiteralSelector2ArgsBytecode
    case 429: // 173	sendLiteralSelector2ArgsBytecode
    case 430: // 174	sendLiteralSelector2ArgsBytecode
    case 431: // 175	sendLiteralSelector2ArgsBytecode
    {
      sqInt rcvr;
      sqInt tagBits;

      VM_LABEL(sendLiteralSelector2ArgsBytecode);
      /* begin literal: */
      assert(method == (iframeMethod(localFP)));
      messageSelector =
          fetchPointerofObject((currentBytecode & 15) + LiteralStart, method);
      argumentCount = 2;
      rcvr = longAt(localSP + (2 * BytesPerOop));
      lkupClassTag = /* fetchClassTagOf: */
          ((tagBits = rcvr & (tagMask()))
               ? tagBits
               : (longAt((void *)(rcvr))) & (classIndexMask()));
      assert(lkupClassTag != (nilObject()));
      goto commonSendOrdinary;
    } break;
    case 338: // 82	extPushPseudoVariable
    {
      sqInt objOop;
      sqInt theThingToPush;

      VM_LABEL(extPushPseudoVariable);
      switch (extB) {
      case 0:
        /* begin ensureFrameIsMarried:SP: */
        if (byteAt((localFP + FoxFrameFlags) + 2)) {
          assert(isContext(frameContext(localFP)));
          theThingToPush = longAt(localFP + FoxThisContext);
          goto l77;
        }
        theThingToPush = marryFrameSP(localFP, localSP);
        /* end ensureFrameIsMarried:SP: */
      l77:;
        break;
      case 1:
        /* begin activeProcess */
        objOop = fetchPointerofObject(
            ValueIndex,
            fetchPointerofObject(SchedulerAssociation, specialObjectsOop));
        theThingToPush = fetchPointerofObject(ActiveProcessIndex, objOop);
        break;
      default:
        goto respondToUnknownBytecode;
      }

      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      longAtput((localSP -= BytesPerOop), theThingToPush);
      extB = 0;
      numExtB = 0;
    } break;
    case 340: // 84	unknownBytecode
    case 341: // 85	unknownBytecode
    case 342: // 86	unknownBytecode
    case 343: // 87	unknownBytecode
    case 473: // 217	unconditionalTrapBytecode
    case 474: // 218	unknownBytecode
    case 475: // 219	unknownBytecode
    case 476: // 220	unknownBytecode
    case 477: // 221	unknownBytecode
    case 478: // 222	unknownBytecode
    case 479: // 223	unknownBytecode
    case 486: // 230	unknownBytecode
    case 502: // 246	unknownBytecode
    case 503: // 247	unknownBytecode
    case 510: // 254	unknownBytecode
    case 511: // 255	unknownBytecode
    {
      VM_LABEL(unknownBytecode);
      goto respondToUnknownBytecode;
    } break;
    case 344: // 88	returnReceiver
    {
      VM_LABEL(returnReceiver1);
      localReturnValue = longAt(localFP + FoxReceiver);
      goto commonReturn;
    } break;
    case 349: // 93	returnNilFromBlock
    {
      VM_LABEL(returnNilFromBlock);
      localReturnValue = nilObj;
      goto commonCallerReturn;
    } break;
    case 350: // 94	returnTopFromBlock
    {
      VM_LABEL(returnTopFromBlock1);
      localReturnValue = longAt(localSP);
      goto commonCallerReturn;
    } break;
    case 351: // 95	extNopBytecode
    {
      VM_LABEL(extNopBytecode);
      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      numExtB = (extA = (extB = 0));
    } break;
    case 354: // 98	bytecodePrimLessThanSistaV1
    {
      int aBool;
      sqInt arg;
      sqInt rcvr;

      VM_LABEL(bytecodePrimLessThanSistaV1);
      rcvr = longAt(localSP + (1 * BytesPerOop));
      arg = longAt(localSP);
      if ((((rcvr & arg) & (smallIntegerTag())) != 0)) {
        /* begin booleanCheatSistaV1: */
        if (rcvr < arg) {
          /* goto booleanCheatTrueSistaV1 */
        } else {
          goto booleanCheatFalseSistaV1;
        }
        goto l84;
      }

      /* The C code can avoid detagging since tagged integers are still signed.
         But this means the simulator must override to do detagging. */

      /* begin initPrimCall */
      primFailCode = 0;
      aBool = primitiveFloatLessthanArg(rcvr, arg);
      if (!primFailCode) {
        /* begin booleanCheatSistaV1: */
        if (aBool) {
          goto booleanCheatTrueSistaV1;
        } else {
          goto booleanCheatFalseSistaV1;
        }
        goto l84;
      }
      messageSelector = fetchPointerofObject(
          4U, fetchPointerofObject(SpecialSelectors, specialObjectsOop));
      argumentCount = 1;
      goto normalSend;
    }
      /* end case */
    l84:

    booleanCheatTrueSistaV1:
      /* booleanCheatTrueSistaV1 */
      {
        sqInt bytecode;
        sqInt offset;

        VM_LABEL(booleanCheatTrueSistaV1);
        /* assume next bytecode is jumpIfFalse (99%) */
        bytecode = byteAt(++localIP);

        /* begin internalPop: */
        localSP += 2 * BytesPerOop;
        if (bytecode >= 192) {
          if (bytecode <= 199) {
            /* begin fetchNextBytecode */
            currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
            goto l172;
          }

          /* short jumpIfFalse 192 - 199 */
          if (bytecode == 239) {
            /* fetchByte */
            byteAt(++localIP);

            /* begin fetchNextBytecode */
            currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
            goto l172;
          }

          /* long jumpIfFalse */
          if (bytecode == 238) {
            offset = byteAt(++localIP);

            /* begin jump: */
            localIP = (localIP + offset) + 1;
            currentBytecode = (byteAt(localIP)) + bytecodeSetSelector;
            goto l172;
          }
        }

        /* not followed by a jumpIfFalse; (un)do instruction fetch and push
         * boolean result */
        currentBytecode = bytecode + bytecodeSetSelector;

        /* begin internalPush: */
        longAtput((localSP -= BytesPerOop), trueObj);
      }
      /* end case */
    l172:;
      break;
    case 355: // 99	bytecodePrimGreaterThanSistaV1
    {
      int aBool;
      sqInt arg;
      sqInt rcvr;

      VM_LABEL(bytecodePrimGreaterThanSistaV1);
      rcvr = longAt(localSP + (1 * BytesPerOop));
      arg = longAt(localSP);
      if ((((rcvr & arg) & (smallIntegerTag())) != 0)) {
        /* begin booleanCheatSistaV1: */
        if (rcvr > arg) {
          goto booleanCheatTrueSistaV1;
        } else {
          /* goto booleanCheatFalseSistaV1 */
        }
        goto l85;
      }

      /* The C code can avoid detagging since tagged integers are still signed.
         But this means the simulator must override to do detagging. */

      /* begin initPrimCall */
      primFailCode = 0;
      aBool = primitiveFloatGreaterthanArg(rcvr, arg);
      if (!primFailCode) {
        /* begin booleanCheatSistaV1: */
        if (aBool) {
          goto booleanCheatTrueSistaV1;
        } else {
          goto booleanCheatFalseSistaV1;
        }
        goto l85;
      }
      messageSelector = fetchPointerofObject(
          6U, fetchPointerofObject(SpecialSelectors, specialObjectsOop));
      argumentCount = 1;
      goto normalSend;
    }
      /* end case */
    l85:

    booleanCheatFalseSistaV1:
      /* booleanCheatFalseSistaV1 */
      {
        sqInt bytecode;
        sqInt offset;

        VM_LABEL(booleanCheatFalseSistaV1);
        /* assume next bytecode is jumpIfFalse (99%) */
        bytecode = byteAt(++localIP);

        /* begin internalPop: */
        localSP += 2 * BytesPerOop;
        if ((bytecode < 199) && (bytecode > 191)) {
          /* begin jump: */
          localIP = (localIP + (bytecode - 191)) + 1;
          currentBytecode = (byteAt(localIP)) + bytecodeSetSelector;
          goto l173;
        }

        /* short jumpIfFalse */
        if (bytecode == 239) {
          offset = byteAt(++localIP);

          /* begin jump: */
          localIP = (localIP + offset) + 1;
          currentBytecode = (byteAt(localIP)) + bytecodeSetSelector;
          goto l173;
        }

        /* long jumpIfFalse
           not followed by a jumpIfFalse; (un)do instruction fetch and push
           boolean result */
        currentBytecode = bytecode + bytecodeSetSelector;

        /* begin internalPush: */
        longAtput((localSP -= BytesPerOop), falseObj);
      }
      /* end case */
    l173:;
      break;
    case 356: // 100	bytecodePrimLessOrEqualSistaV1
    {
      int aBool;
      sqInt arg;
      sqInt rcvr;

      VM_LABEL(bytecodePrimLessOrEqualSistaV1);
      rcvr = longAt(localSP + (1 * BytesPerOop));
      arg = longAt(localSP);
      if ((((rcvr & arg) & (smallIntegerTag())) != 0)) {
        /* begin booleanCheatSistaV1: */
        if (rcvr <= arg) {
          goto booleanCheatTrueSistaV1;
        } else {
          goto booleanCheatFalseSistaV1;
        }
        goto l86;
      }

      /* The C code can avoid detagging since tagged integers are still signed.
         But this means the simulator must override to do detagging. */

      /* begin initPrimCall */
      primFailCode = 0;
      aBool = primitiveFloatLessOrEqualtoArg(rcvr, arg);
      if (!primFailCode) {
        /* begin booleanCheatSistaV1: */
        if (aBool) {
          goto booleanCheatTrueSistaV1;
        } else {
          goto booleanCheatFalseSistaV1;
        }
        goto l86;
      }
      messageSelector = fetchPointerofObject(
          8U, fetchPointerofObject(SpecialSelectors, specialObjectsOop));
      argumentCount = 1;
      goto normalSend;
    }
      /* end case */
    l86:;
      break;
    case 357: // 101	bytecodePrimGreaterOrEqualSistaV1
    {
      int aBool;
      sqInt arg;
      sqInt rcvr;

      VM_LABEL(bytecodePrimGreaterOrEqualSistaV1);
      rcvr = longAt(localSP + (1 * BytesPerOop));
      arg = longAt(localSP);
      if ((((rcvr & arg) & (smallIntegerTag())) != 0)) {
        /* begin booleanCheatSistaV1: */
        if (rcvr >= arg) {
          goto booleanCheatTrueSistaV1;
        } else {
          goto booleanCheatFalseSistaV1;
        }
        goto l87;
      }

      /* The C code can avoid detagging since tagged integers are still signed.
         But this means the simulator must override to do detagging. */

      /* begin initPrimCall */
      primFailCode = 0;
      aBool = primitiveFloatGreaterOrEqualtoArg(rcvr, arg);
      if (!primFailCode) {
        /* begin booleanCheatSistaV1: */
        if (aBool) {
          goto booleanCheatTrueSistaV1;
        } else {
          goto booleanCheatFalseSistaV1;
        }
        goto l87;
      }
      messageSelector = fetchPointerofObject(
          10U, fetchPointerofObject(SpecialSelectors, specialObjectsOop));
      argumentCount = 1;
      goto normalSend;
    }
      /* end case */
    l87:;
      break;
    case 358: // 102	bytecodePrimEqualSistaV1
    {
      int aBool;
      sqInt arg;
      sqInt rcvr;

      VM_LABEL(bytecodePrimEqualSistaV1);
      rcvr = longAt(localSP + (1 * BytesPerOop));
      arg = longAt(localSP);
      if ((((rcvr & arg) & (smallIntegerTag())) != 0)) {
        /* begin booleanCheatSistaV1: */
        if (rcvr == arg) {
          goto booleanCheatTrueSistaV1;
        } else {
          goto booleanCheatFalseSistaV1;
        }
        goto l88;
      }

      /* begin initPrimCall */
      primFailCode = 0;
      aBool = primitiveFloatEqualtoArg(rcvr, arg);
      if (!primFailCode) {
        /* begin booleanCheatSistaV1: */
        if (aBool) {
          goto booleanCheatTrueSistaV1;
        } else {
          goto booleanCheatFalseSistaV1;
        }
        goto l88;
      }
      messageSelector = fetchPointerofObject(
          12U, fetchPointerofObject(SpecialSelectors, specialObjectsOop));
      argumentCount = 1;
      goto normalSend;
    }
      /* end case */
    l88:;
      break;
    case 359: // 103	bytecodePrimNotEqualSistaV1
    {
      int aBool;
      sqInt arg;
      sqInt rcvr;

      VM_LABEL(bytecodePrimNotEqualSistaV1);
      rcvr = longAt(localSP + (1 * BytesPerOop));
      arg = longAt(localSP);
      if ((((rcvr & arg) & (smallIntegerTag())) != 0)) {
        /* begin booleanCheatSistaV1: */
        if (rcvr != arg) {
          goto booleanCheatTrueSistaV1;
        } else {
          goto booleanCheatFalseSistaV1;
        }
        goto l89;
      }

      /* begin initPrimCall */
      primFailCode = 0;
      aBool = primitiveFloatEqualtoArg(rcvr, arg);
      if (!primFailCode) {
        /* begin booleanCheatSistaV1: */
        if (aBool) {
          goto booleanCheatFalseSistaV1;
        } else {
          goto booleanCheatTrueSistaV1;
        }
        goto l89;
      }
      messageSelector = fetchPointerofObject(
          14U, fetchPointerofObject(SpecialSelectors, specialObjectsOop));
      argumentCount = 1;
      goto normalSend;
    }
      /* end case */
    l89:;
      break;
    case 374: // 118	bytecodePrimIdenticalSistaV1
    {
      sqInt arg;
      sqInt rcvr;

      VM_LABEL(bytecodePrimIdenticalSistaV1);
      rcvr = longAt(localSP + (1 * BytesPerOop));
      arg = longAt(localSP);
      if (isOopForwarded(rcvr)) {
        rcvr = handleSpecialSelectorSendFaultForfpsp(rcvr, localFP, localSP);
      }
      if (isOopForwarded(arg)) {
        arg = handleSpecialSelectorSendFaultForfpsp(arg, localFP, localSP);
      }

      /* begin booleanCheatSistaV1: */
      if (rcvr == arg) {
        goto booleanCheatTrueSistaV1;
      } else {
        goto booleanCheatFalseSistaV1;
      }
    } break;
    case 376: // 120	bytecodePrimNotIdenticalSistaV1
    {
      sqInt arg;
      sqInt rcvr;

      VM_LABEL(bytecodePrimNotIdenticalSistaV1);
      rcvr = longAt(localSP + (1 * BytesPerOop));
      arg = longAt(localSP);
      if (isOopForwarded(rcvr)) {
        rcvr = handleSpecialSelectorSendFaultForfpsp(rcvr, localFP, localSP);
      }
      if (isOopForwarded(arg)) {
        arg = handleSpecialSelectorSendFaultForfpsp(arg, localFP, localSP);
      }

      /* begin booleanCheatSistaV1: */
      if (rcvr != arg) {
        goto booleanCheatTrueSistaV1;
      } else {
        goto booleanCheatFalseSistaV1;
      }
    } break;
    case 440: // 184	shortConditionalJumpTrue
    case 441: // 185	shortConditionalJumpTrue
    case 442: // 186	shortConditionalJumpTrue
    case 443: // 187	shortConditionalJumpTrue
    case 444: // 188	shortConditionalJumpTrue
    case 445: // 189	shortConditionalJumpTrue
    case 446: // 190	shortConditionalJumpTrue
    case 447: // 191	shortConditionalJumpTrue
    {
      sqInt boolean;
      sqInt offset;

      VM_LABEL(shortConditionalJumpTrue);
      offset = (currentBytecode & 7) + 1;

      /* begin jumplfTrueBy: */
      boolean = longAt(localSP);
      if (boolean == trueObj) {
        /* begin jump: */
        localIP = (localIP + offset) + 1;
        currentBytecode = (byteAt(localIP)) + bytecodeSetSelector;
      } else {
        if (!(boolean == falseObj)) {
          /* begin internalMustBeBoolean */
          messageSelector =
              fetchPointerofObject(SelectorMustBeBoolean, specialObjectsOop);
          argumentCount = 0;
          goto normalSend;
          goto l135;
        }

        /* begin fetchNextBytecode */
        currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      }
      localSP += 1 * BytesPerOop;
      /* end jumplfTrueBy: */
    l135:;
    } break;
    case 480: // 224	extABytecode
    {
      VM_LABEL(extABytecode);
      extA = (((((usqInt)(extA) << 8)))) + (byteAt(++localIP));

      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
    } break;
    case 481: // 225	extBBytecode
    {
      sqInt byte;

      VM_LABEL(extBBytecode);
      byte = byteAt(++localIP);

      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      extB = ((numExtB == 0) && (byte > 0x7F)
                  ? byte - 0x100
                  : (((((usqInt)(extB) << 8)))) + byte);
      numExtB += 1;
    } break;
    case 482: // 226	extPushReceiverVariableBytecode
    {
      char *callerFP;
      sqInt index;
      sqInt obj;
      sqInt object;
      sqInt senderOop;
      char *spouseFP;

      VM_LABEL(extPushReceiverVariableBytecode);
      index = (byteAt(++localIP)) + ((((usqInt)(extA) << 8)));

      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      extA = 0;

      /* begin pushMaybeContextReceiverVariable: */
      obj = longAt(localFP + FoxReceiver);
      if ((index <= StackPointerIndex) &&
          (((longAt((void *)(obj))) & (classIndexMask())) ==
           ClassMethodContextCompactIndex)) {
        /* begin instVar:ofContext: */
        assert(index < MethodIndex);
        assert(isContext(obj));
        if (!(((((fetchPointerofObject(SenderIndex, obj))) & 7) == 1))) {
          object = fetchPointerofObject(index, obj);
          goto l138;
        }

        /* begin writeBackHeadFramePointers */
        assert(stackPage == (mostRecentlyUsedPage));
        assert(localSP < localFP);
        assert((localSP < ((stackPage->baseAddress))) &&
               (localSP > (((stackPage->realStackLimit)) -
                           (LargeContextSlots * BytesPerOop))));
        assert((localFP < ((stackPage->baseAddress))) &&
               (localFP > (((stackPage->realStackLimit)) -
                           ((LargeContextSlots * BytesPerOop) / 2))));
        (stackPage->headFP = localFP);
        (stackPage->headSP = localSP);
        assert(pageListIsWellFormed());
        if (isWidowedContext(obj)) {
          object = fetchPointerofObject(index, obj);
          goto l138;
        }

        /* begin frameOfMarriedContext: */
        senderOop = fetchPointerofObject(SenderIndex, obj);
        assert((((senderOop) & 7) == 1));
        spouseFP = ((char *)(senderOop - (smallIntegerTag())));
        if (!index) {
          /* begin ensureCallerContext: */
          callerFP = ((char *)(longAt(spouseFP + FoxSavedFP)));
          if (!callerFP) {
            /* begin frameCallerContext: */
            assert(isBaseFrame(spouseFP));
            object = longAt(spouseFP + FoxCallerContext);
            goto l138;
          }

          /* base frame, context in saved ip slot (or base of stack in Cog) */

          /* begin ensureFrameIsMarried:SP: */
          if (byteAt((callerFP + FoxFrameFlags) + 2)) {
            assert(isContext(frameContext(callerFP)));
            object = longAt(callerFP + FoxThisContext);
            goto l138;
          }
          object = marryFrameSP(
              callerFP, (/* begin frameCallerStackPointer: */
                         assert(!(isBaseFrame(spouseFP))),
                         (spouseFP +
                          ((FoxCallerSavedIP + BytesPerWord) +
                           ((((usqInt)((byteAt((spouseFP + FoxFrameFlags) + 1)))
                              << (shiftForWord())))))) +
                             BytesPerWord));
          goto l138;
        }
        if (index == StackPointerIndex) {
          assert((ReceiverIndex + (stackPointerIndexForFrame(spouseFP))) <
                 (lengthOf(obj)));
          object = (((stackPointerIndexForFrame(spouseFP)) << 3) | 1);
          goto l138;
        }
        if (index == InstructionPointerIndex) {
          object = instructionPointerForFramecurrentFPcurrentIP(
              spouseFP, localFP, oopForPointer(localIP));
          goto l138;
        }
        error("bad index");
        object = 0;
        /* end instVar:ofContext: */
      l138:

        /* begin internalPush: */
        longAtput((localSP -= BytesPerOop), object);
      } else {
        /* begin internalPush: */
        longAtput((localSP -= BytesPerOop), fetchPointerofObject(index, obj));
      }
    } break;
    case 483: // 227	extPushLiteralVariableBytecode
    {
      sqInt index;
      sqInt litVar;

      VM_LABEL(extPushLiteralVariableBytecode);
      index = (byteAt(++localIP)) + ((((usqInt)(extA) << 8)));

      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      extA = 0;

      /* begin pushLiteralVariable: */
      /* begin followObjLiteral:ofMethod: */
      /* begin followObjField:ofObject: */
      litVar = fetchPointerofObject(index + LiteralStart, method);
      assert(isNonImmediate(litVar));
      if ((!((longAt((void *)(litVar))) &
             ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
        litVar = fixFollowedFieldofObjectwithInitialValue(index + LiteralStart,
                                                          method, litVar);
      }

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop),
                fetchPointerofObject(ValueIndex, litVar));
    } break;
    case 484: // 228	extPushLiteralBytecode
    {
      sqInt index;
      sqInt object;

      VM_LABEL(extPushLiteralBytecode);
      index = (byteAt(++localIP)) + ((((usqInt)(extA) << 8)));

      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      extA = 0;

      /* begin pushLiteralConstant: */
      /* begin literal: */
      assert(method == (iframeMethod(localFP)));
      object = fetchPointerofObject(index + LiteralStart, method);

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop), object);
    } break;
    case 485: // 229	longPushTemporaryVariableBytecode
    {
      usqInt frameNumArgs;
      sqInt index;

      VM_LABEL(longPushTemporaryVariableBytecode);
      index = byteAt(++localIP);

      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      longAtput(
          (localSP -= BytesPerOop), /* temporary:in: */
          (index < ((frameNumArgs = byteAt((localFP + FoxFrameFlags) + 1)))
               ? longAt((localFP + FoxCallerSavedIP) +
                        ((frameNumArgs - index) * BytesPerWord))
               : longAt(((localFP + FoxReceiver) - BytesPerWord) +
                        ((frameNumArgs - index) * BytesPerWord))));
    } break;
    case 488: // 232	extPushIntegerBytecode
    {
      sqInt value;

      VM_LABEL(extPushIntegerBytecode);
      value = (byteAt(++localIP)) + ((((usqInt)(extB) << 8)));

      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      extB = 0;
      numExtB = 0;

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop), (((usqInt)value << 3) | 1));
    } break;
    case 489: // 233	extPushCharacterBytecode
    {
      sqInt object;
      sqInt value;

      VM_LABEL(extPushCharacterBytecode);
      value = (byteAt(++localIP)) + ((((usqInt)(extA) << 8)));

      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      object = ((((usqInt)(value) << (numTagBits())))) + (characterTag());

      /* begin internalPush: */
      longAtput((localSP -= BytesPerOop), object);
      extA = 0;
    } break;
    case 490: // 234	extSendBytecode
    {
      sqInt byte;
      sqInt rcvr;
      sqInt tagBits;

      VM_LABEL(extSendBytecode);
      byte = byteAt(++localIP);

      /* begin literal: */
      assert(method == (iframeMethod(localFP)));
      messageSelector = fetchPointerofObject(
          ((((usqInt)(byte)) >> 3) + ((((usqInt)(extA) << 5)))) + LiteralStart,
          method);
      extA = 0;
      argumentCount = (byte & 7) + ((((usqInt)(extB) << 3)));
      extB = 0;
      numExtB = 0;
      rcvr = longAt(localSP + (argumentCount * BytesPerOop));
      lkupClassTag = /* fetchClassTagOf: */
          ((tagBits = rcvr & (tagMask()))
               ? tagBits
               : (longAt((void *)(rcvr))) & (classIndexMask()));
      goto commonSendOrdinary;
    } break;
    case 491: // 235	extSendSuperBytecode
    {
      sqInt byte;

      VM_LABEL(extSendSuperBytecode);
      byte = byteAt(++localIP);

      /* begin literal: */
      assert(method == (iframeMethod(localFP)));
      messageSelector = fetchPointerofObject(
          ((((usqInt)(byte)) >> 3) + ((((usqInt)(extA) << 5)))) + LiteralStart,
          method);
      extA = 0;
      if (extB >= 64) {
        argumentCount = (byte & 7) + ((((usqInt)((extB - 64)) << 3)));
        extB = 0;
        numExtB = 0;

        /* goto directedSuperclassSend */
        goto l140;
      }
      argumentCount = (byte & 7) + ((((usqInt)(extB) << 3)));
      extB = 0;
      numExtB = 0;
      goto superclassSend;
    }
      /* end case */
    l140:

    directedSuperclassSend:
      /* directedSuperclassSend */
      {
        sqInt aValue;
        sqInt class;
        sqInt err;
        sqInt hash;
        sqInt objOop;
        sqInt referent;
        sqInt superclass;

        VM_LABEL(directedSuperclassSend);
        /* begin internalPopStack */
        class = longAt(localSP);
        localSP += BytesPerOop;
        if ((!((longAt((void *)(class))) &
               ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
          class = followForwarded(class);
        }

        /* begin superclassOf: */
        /* begin followObjField:ofObject: */
        superclass = fetchPointerofObject(SuperclassIndex, class);
        assert(isNonImmediate(superclass));
        if ((!((longAt((void *)(superclass))) &
               ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
          superclass = fixFollowedFieldofObjectwithInitialValue(
              SuperclassIndex, class, superclass);
        }

        /* classTagForClass: uses ensureBehaviorHash: */

        /* begin classTagForClass: */
        /* begin ensureBehaviorHash: */
        assert(addressCouldBeClassObj(superclass));

        /* eem 12/28/2021 the above asserft is too weak (and only an assert) */
        lkupClassTag =
            ((hash = (long32At((void *)(superclass + 4))) &
                     (identityHashHalfWordMask()))
                 ? hash
                 : (objCouldBeClassObj(superclass)
                        ? ((err = enterIntoClassTable(superclass))
                               ? -err
                               : (long32At((void *)(superclass + 4))) &
                                     (identityHashHalfWordMask()))
                        : -PrimErrBadReceiver));

        /* To maintain the invariant that all receivers are unforwarded we need
           an explicit read barrier in the super send cases.  Even though we
           always follow receivers on become  e.g. super doSomethingWith: (self
           become: other) forwards the receiver self pushed on the stack. */

        /* begin ensureReceiverUnforwarded */
        if (isOopForwarded(longAt(localSP + (argumentCount * BytesPerOop)))) {
          objOop = longAt(localSP + (argumentCount * BytesPerOop));

          aValue = followForwarded(objOop);

          /* begin internalStackValue:put: */
          longAtput(localSP + (argumentCount * BytesPerOop), aValue);
        }
        assert(lkupClassTag != (nilObject()));
        goto commonSendOrdinary;
      }
      break;
    case 492: // 236	callMappedInlinedPrimitive
    {
      VM_LABEL(callMappedInlinedPrimitive);
      localIP -= 2;
      goto respondToUnknownBytecode;
    } break;
    case 493: // 237	extUnconditionalJump
    {
      sqInt bcpcDelta;
      sqInt byte;
      sqInt offset;

      VM_LABEL(extUnconditionalJump);
      byte = byteAt(++localIP);
      offset = byte + ((((usqInt)(extB) << 8)));
      bcpcDelta = (offset < 0 ? numExtB * 2 : 0);
      extB = 0;
      numExtB = 0;
      localIP += offset;

      /* begin ifBackwardsCheckForEvents: */
      if (((offset + bcpcDelta) < 0) && (localSP < stackLimit)) {
        /* begin externalizeIPandSP */
        instructionPointer = ((usqInt)localIP);
        stackPointer = localSP;
        framePointer = localFP;
        checkForEventsMayContextSwitch(1);
        browserPluginReturnIfNeeded();

        /* begin internalizeIPandSP */
        localIP = ((char *)instructionPointer);
        localSP = stackPointer;
        localFP = framePointer;
      }

      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
    } break;
    case 494: // 238	extJumpIfTrue
    {
      sqInt boolean;
      sqInt byte;
      sqInt offset;

      VM_LABEL(extJumpIfTrue);
      byte = byteAt(++localIP);
      offset = byte + ((((usqInt)(extB) << 8)));
      numExtB = (extB = (extA = 0));

      /* begin jumplfTrueBy: */
      boolean = longAt(localSP);
      if (boolean == trueObj) {
        /* begin jump: */
        localIP = (localIP + offset) + 1;
        currentBytecode = (byteAt(localIP)) + bytecodeSetSelector;
      } else {
        if (!(boolean == falseObj)) {
          /* begin internalMustBeBoolean */
          messageSelector =
              fetchPointerofObject(SelectorMustBeBoolean, specialObjectsOop);
          argumentCount = 0;
          goto normalSend;
          goto l141;
        }

        /* begin fetchNextBytecode */
        currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      }
      localSP += 1 * BytesPerOop;
      /* end jumplfTrueBy: */
    l141:;
    } break;
    case 495: // 239	extJumpIfFalse
    {
      sqInt boolean;
      sqInt byte;
      sqInt offset;

      VM_LABEL(extJumpIfFalse);
      byte = byteAt(++localIP);
      offset = byte + ((((usqInt)(extB) << 8)));
      numExtB = (extB = (extA = 0));

      /* begin jumplfFalseBy: */
      boolean = longAt(localSP);
      if (boolean == falseObj) {
        /* begin jump: */
        localIP = (localIP + offset) + 1;
        currentBytecode = (byteAt(localIP)) + bytecodeSetSelector;
      } else {
        if (!(boolean == trueObj)) {
          /* begin internalMustBeBoolean */
          messageSelector =
              fetchPointerofObject(SelectorMustBeBoolean, specialObjectsOop);
          argumentCount = 0;
          goto normalSend;
          goto l142;
        }

        /* begin fetchNextBytecode */
        currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      }
      localSP += 1 * BytesPerOop;
      /* end jumplfFalseBy: */
    l142:;
    } break;
    case 496: // 240	extStoreAndPopReceiverVariableBytecode
    {
      sqInt obj;
      int onCurrentPage;
      sqInt senderOop;
      char *theFP;
      StackPage *thePage;
      sqInt value;
      sqInt variableIndex;

      VM_LABEL(extStoreAndPopReceiverVariableBytecode);
      variableIndex = (byteAt(++localIP)) + ((((usqInt)(extA) << 8)));
      extA = 0;
      value = longAt(localSP);

      /* begin internalPop: */
      localSP += 1 * BytesPerOop;
      obj = longAt(localFP + FoxReceiver);
      if ((variableIndex <= ReceiverIndex) &&
          ((((longAt((void *)(obj))) & (classIndexMask())) ==
            ClassMethodContextCompactIndex) &&
           (((((fetchPointerofObject(SenderIndex, obj))) & 7) == 1)))) {
        /* begin instVar:ofContext:put: */
        assert(isMarriedOrWidowedContext(obj));
        assert(!((isObjImmutable(obj))));

        /* begin writeBackHeadFramePointers */
        assert(stackPage == (mostRecentlyUsedPage));
        assert(localSP < localFP);
        assert((localSP < ((stackPage->baseAddress))) &&
               (localSP > (((stackPage->realStackLimit)) -
                           (LargeContextSlots * BytesPerOop))));
        assert((localFP < ((stackPage->baseAddress))) &&
               (localFP > (((stackPage->realStackLimit)) -
                           ((LargeContextSlots * BytesPerOop) / 2))));
        (stackPage->headFP = localFP);
        (stackPage->headSP = localSP);
        assert(pageListIsWellFormed());
        if (!(/* isStillMarriedContext: */
              (((((fetchPointerofObject(SenderIndex, obj))) & 7) == 1)) &&
              (!(isWidowedContext(obj))))) {
          /* begin storePointer:ofObject:withValue: */
          assert(validStorePointerArgs(variableIndex, obj, value));
          assert(isNonImmediate(obj));
          if (oopisGreaterThanOrEqualTo(obj, oldSpaceStart)) {
            if (/* isYoung: */
                ((!(value & (tagMask())))) &&
                (oopisLessThan(value, oldSpaceStart))) {
              /* begin possibleRootStoreInto: */
              if (!((byteAt((void *)(obj + (formatFieldByteOffset())))) &
                    (1U << (rememberedBitByteShift())))) {
                remember(obj);
              }
            }
          }

          /* most stores into young objects */
          longAtput((void *)((obj + BaseHeaderSize) +
                             ((((usqInt)(variableIndex) << (shiftForWord()))))),
                    value);
          goto l143;
        }

        /* begin frameOfMarriedContext: */
        senderOop = fetchPointerofObject(SenderIndex, obj);
        assert((((senderOop) & 7) == 1));
        theFP = ((char *)(senderOop - (smallIntegerTag())));
        if (!variableIndex) {
          /* begin stackPageFor: */
          thePage = stackPageAtpages(pageIndexForstackMemorybytesPerPage(
                                         theFP, stackMemory, bytesPerPage),
                                     pages);
          assert(stackPage == (mostRecentlyUsedPage));
          onCurrentPage = thePage == stackPage;
          storeSenderOfFramewithValue(theFP, value);
          if (onCurrentPage) {
            localFP = (stackPage->headFP);
            localSP = (stackPage->headSP);
          } else {
            markStackPageMostRecentlyUsed(stackPage);
          }
          goto l143;
        }

        /* begin externalizeIPandSP */
        instructionPointer = ((usqInt)localIP);
        stackPointer = localSP;
        framePointer = localFP;
        externalDivorceFrameandContext(theFP, obj);

        /* begin storePointer:ofObject:withValue: */
        assert(validStorePointerArgs(variableIndex, obj, value));
        assert(isNonImmediate(obj));
        if (oopisGreaterThanOrEqualTo(obj, oldSpaceStart)) {
          if (/* isYoung: */
              ((!(value & (tagMask())))) &&
              (oopisLessThan(value, oldSpaceStart))) {
            /* begin possibleRootStoreInto: */
            if (!((byteAt((void *)(obj + (formatFieldByteOffset())))) &
                  (1U << (rememberedBitByteShift())))) {
              remember(obj);
            }
          }
        }

        /* most stores into young objects */
        longAtput((void *)((obj + BaseHeaderSize) +
                           ((((usqInt)(variableIndex) << (shiftForWord()))))),
                  value);

        /* begin internalizeIPandSP */
        localIP = ((char *)instructionPointer);
        localSP = stackPointer;
        localFP = framePointer;

        /* Assigning various fields can force a divorce which can change the
         * stackPage. */
        markStackPageMostRecentlyUsed(stackPage);
        assertValidExecutionPointersimbarline(((usqInt)localIP), localFP,
                                              localSP, 1, __LINE__);
        /* end instVar:ofContext:put: */
      l143:;
      } else {
        /* begin storePointerImmutabilityCheck:ofObject:withValue: */
#if IMMUTABILITY
        assert(!((isImmediate(obj))));
        if ((((usqInt)((
                 byteAt((void *)(obj + (immutableExtraBitsByteOffset())))))) >>
             (immutableBitByteShift())) &
            1) {
          /* begin cannotAssign:to:withIndex: */
          /* begin internalPush: */
          longAtput((localSP -= BytesPerOop), obj);
          longAtput((localSP -= BytesPerOop), value);
          longAtput((localSP -= BytesPerOop),
                    (((usqInt)(variableIndex + 1) << 3) | 1));
          messageSelector =
              fetchPointerofObject(SelectorAttemptToAssign, specialObjectsOop);
          argumentCount = 2;
          goto normalSend;
          goto l144;
        }
#endif // IMMUTABILITY

        /* begin storePointer:ofObject:withValue: */
        assert(validStorePointerArgs(variableIndex, obj, value));
        assert(isNonImmediate(obj));
        if (oopisGreaterThanOrEqualTo(obj, oldSpaceStart)) {
          if (/* isYoung: */
              ((!(value & (tagMask())))) &&
              (oopisLessThan(value, oldSpaceStart))) {
            /* begin possibleRootStoreInto: */
            if (!((byteAt((void *)(obj + (formatFieldByteOffset())))) &
                  (1U << (rememberedBitByteShift())))) {
              remember(obj);
            }
          }
        }

        /* most stores into young objects */
        longAtput((void *)((obj + BaseHeaderSize) +
                           ((((usqInt)(variableIndex) << (shiftForWord()))))),
                  value);
        /* end storePointerImmutabilityCheck:ofObject:withValue: */
      l144:;
      }

      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
    } break;
    case 497: // 241	extStoreAndPopLiteralVariableBytecode
    {
      sqInt litVar;
      sqInt value;
      sqInt variableIndex;

      VM_LABEL(extStoreAndPopLiteralVariableBytecode);
      variableIndex = (byteAt(++localIP)) + ((((usqInt)(extA) << 8)));
      value = longAt(localSP);

      /* begin internalPop: */
      localSP += 1 * BytesPerOop;
      extA = 0;

      /* begin storeLiteralVariable:withValue: */
      /* begin followObjLiteral:ofMethod: */
      /* begin followObjField:ofObject: */
      litVar = fetchPointerofObject(variableIndex + LiteralStart, method);
      assert(isNonImmediate(litVar));
      if ((!((longAt((void *)(litVar))) &
             ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
        litVar = fixFollowedFieldofObjectwithInitialValue(
            variableIndex + LiteralStart, method, litVar);
      }

      /* begin storePointerImmutabilityCheck:ofObject:withValue: */
#if IMMUTABILITY
      assert(!((isImmediate(litVar))));
      if ((((usqInt)((
               byteAt((void *)(litVar + (immutableExtraBitsByteOffset())))))) >>
           (immutableBitByteShift())) &
          1) {
        /* begin cannotAssign:to:withIndex: */
        /* begin internalPush: */
        longAtput((localSP -= BytesPerOop), litVar);
        longAtput((localSP -= BytesPerOop), value);
        longAtput((localSP -= BytesPerOop),
                  (((usqInt)2 /* (ValueIndex + 1) */ << 3) | 1));
        messageSelector =
            fetchPointerofObject(SelectorAttemptToAssign, specialObjectsOop);
        argumentCount = 2;
        goto normalSend;
        goto l145;
      }
#endif // IMMUTABILITY

      /* begin storePointer:ofObject:withValue: */
      assert(validStorePointerArgs(ValueIndex, litVar, value));
      assert(isNonImmediate(litVar));
      if (oopisGreaterThanOrEqualTo(litVar, oldSpaceStart)) {
        if (/* isYoung: */
            ((!(value & (tagMask())))) &&
            (oopisLessThan(value, oldSpaceStart))) {
          /* begin possibleRootStoreInto: */
          if (!((byteAt((void *)(litVar + (formatFieldByteOffset())))) &
                (1U << (rememberedBitByteShift())))) {
            remember(litVar);
          }
        }
      }

      /* most stores into young objects */
      longAtput((void *)((litVar + BaseHeaderSize) +
                         ((((usqInt)(ValueIndex) << (shiftForWord()))))),
                value);
      /* end storePointerImmutabilityCheck:ofObject:withValue: */
    l145:

      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
    } break;
    case 498: // 242	longStoreAndPopTemporaryVariableBytecode
    {
      usqInt frameNumArgs;
      sqInt index;

      VM_LABEL(longStoreAndPopTemporaryVariableBytecode);
      /* begin longStoreTemporaryVariableBytecode */
      index = byteAt(++localIP);
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;

      /* temporary:in:put: */
      if (index < ((frameNumArgs = byteAt((localFP + FoxFrameFlags) + 1)))) {
        longAtput((localFP + FoxCallerSavedIP) +
                      ((frameNumArgs - index) * BytesPerWord),
                  longAt(localSP));
      } else {
        longAtput(((localFP + FoxReceiver) - BytesPerWord) +
                      ((frameNumArgs - index) * BytesPerWord),
                  longAt(localSP));
      }

      /* begin internalPop: */
      localSP += 1 * BytesPerOop;
    } break;
    case 499: // 243	extStoreReceiverVariableBytecode
    {
      sqInt anObject;
      sqInt obj;
      int onCurrentPage;
      sqInt senderOop;
      char *theFP;
      StackPage *thePage;
      sqInt variableIndex;

      VM_LABEL(extStoreReceiverVariableBytecode);
      variableIndex = (byteAt(++localIP)) + ((((usqInt)(extA) << 8)));
      extA = 0;
      anObject = longAt(localSP);

      /* begin storeMaybeContextReceiverVariable:withValue: */
      obj = longAt(localFP + FoxReceiver);
      if ((variableIndex <= ReceiverIndex) &&
          ((((longAt((void *)(obj))) & (classIndexMask())) ==
            ClassMethodContextCompactIndex) &&
           (((((fetchPointerofObject(SenderIndex, obj))) & 7) == 1)))) {
        /* begin instVar:ofContext:put: */
        assert(isMarriedOrWidowedContext(obj));
        assert(!((isObjImmutable(obj))));

        /* begin writeBackHeadFramePointers */
        assert(stackPage == (mostRecentlyUsedPage));
        assert(localSP < localFP);
        assert((localSP < ((stackPage->baseAddress))) &&
               (localSP > (((stackPage->realStackLimit)) -
                           (LargeContextSlots * BytesPerOop))));
        assert((localFP < ((stackPage->baseAddress))) &&
               (localFP > (((stackPage->realStackLimit)) -
                           ((LargeContextSlots * BytesPerOop) / 2))));
        (stackPage->headFP = localFP);
        (stackPage->headSP = localSP);
        assert(pageListIsWellFormed());
        if (!(/* isStillMarriedContext: */
              (((((fetchPointerofObject(SenderIndex, obj))) & 7) == 1)) &&
              (!(isWidowedContext(obj))))) {
          /* begin storePointer:ofObject:withValue: */
          assert(validStorePointerArgs(variableIndex, obj, anObject));
          assert(isNonImmediate(obj));
          if (oopisGreaterThanOrEqualTo(obj, oldSpaceStart)) {
            if (/* isYoung: */
                ((!(anObject & (tagMask())))) &&
                (oopisLessThan(anObject, oldSpaceStart))) {
              /* begin possibleRootStoreInto: */
              if (!((byteAt((void *)(obj + (formatFieldByteOffset())))) &
                    (1U << (rememberedBitByteShift())))) {
                remember(obj);
              }
            }
          }

          /* most stores into young objects */
          longAtput((void *)((obj + BaseHeaderSize) +
                             ((((usqInt)(variableIndex) << (shiftForWord()))))),
                    anObject);
          goto l146;
        }

        /* begin frameOfMarriedContext: */
        senderOop = fetchPointerofObject(SenderIndex, obj);
        assert((((senderOop) & 7) == 1));
        theFP = ((char *)(senderOop - (smallIntegerTag())));
        if (!variableIndex) {
          /* begin stackPageFor: */
          thePage = stackPageAtpages(pageIndexForstackMemorybytesPerPage(
                                         theFP, stackMemory, bytesPerPage),
                                     pages);
          assert(stackPage == (mostRecentlyUsedPage));
          onCurrentPage = thePage == stackPage;
          storeSenderOfFramewithValue(theFP, anObject);
          if (onCurrentPage) {
            localFP = (stackPage->headFP);
            localSP = (stackPage->headSP);
          } else {
            markStackPageMostRecentlyUsed(stackPage);
          }
          goto l146;
        }

        /* begin externalizeIPandSP */
        instructionPointer = ((usqInt)localIP);
        stackPointer = localSP;
        framePointer = localFP;
        externalDivorceFrameandContext(theFP, obj);

        /* begin storePointer:ofObject:withValue: */
        assert(validStorePointerArgs(variableIndex, obj, anObject));
        assert(isNonImmediate(obj));
        if (oopisGreaterThanOrEqualTo(obj, oldSpaceStart)) {
          if (/* isYoung: */
              ((!(anObject & (tagMask())))) &&
              (oopisLessThan(anObject, oldSpaceStart))) {
            /* begin possibleRootStoreInto: */
            if (!((byteAt((void *)(obj + (formatFieldByteOffset())))) &
                  (1U << (rememberedBitByteShift())))) {
              remember(obj);
            }
          }
        }

        /* most stores into young objects */
        longAtput((void *)((obj + BaseHeaderSize) +
                           ((((usqInt)(variableIndex) << (shiftForWord()))))),
                  anObject);

        /* begin internalizeIPandSP */
        localIP = ((char *)instructionPointer);
        localSP = stackPointer;
        localFP = framePointer;

        /* Assigning various fields can force a divorce which can change the
         * stackPage. */
        markStackPageMostRecentlyUsed(stackPage);
        assertValidExecutionPointersimbarline(((usqInt)localIP), localFP,
                                              localSP, 1, __LINE__);
        /* end instVar:ofContext:put: */
      l146:;
      } else {
        /* begin storePointerImmutabilityCheck:ofObject:withValue: */
#if IMMUTABILITY
        assert(!((isImmediate(obj))));
        if ((((usqInt)((
                 byteAt((void *)(obj + (immutableExtraBitsByteOffset())))))) >>
             (immutableBitByteShift())) &
            1) {
          /* begin cannotAssign:to:withIndex: */
          /* begin internalPush: */
          longAtput((localSP -= BytesPerOop), obj);
          longAtput((localSP -= BytesPerOop), anObject);
          longAtput((localSP -= BytesPerOop),
                    (((usqInt)(variableIndex + 1) << 3) | 1));
          messageSelector =
              fetchPointerofObject(SelectorAttemptToAssign, specialObjectsOop);
          argumentCount = 2;
          goto normalSend;
          goto l147;
        }
#endif // IMMUTABILITY

        /* begin storePointer:ofObject:withValue: */
        assert(validStorePointerArgs(variableIndex, obj, anObject));
        assert(isNonImmediate(obj));
        if (oopisGreaterThanOrEqualTo(obj, oldSpaceStart)) {
          if (/* isYoung: */
              ((!(anObject & (tagMask())))) &&
              (oopisLessThan(anObject, oldSpaceStart))) {
            /* begin possibleRootStoreInto: */
            if (!((byteAt((void *)(obj + (formatFieldByteOffset())))) &
                  (1U << (rememberedBitByteShift())))) {
              remember(obj);
            }
          }
        }

        /* most stores into young objects */
        longAtput((void *)((obj + BaseHeaderSize) +
                           ((((usqInt)(variableIndex) << (shiftForWord()))))),
                  anObject);
        /* end storePointerImmutabilityCheck:ofObject:withValue: */
      l147:;
      }

      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
    } break;
    case 500: // 244	extStoreLiteralVariableBytecode
    {
      sqInt anObject;
      sqInt litVar;
      sqInt variableIndex;

      VM_LABEL(extStoreLiteralVariableBytecode);
      variableIndex = (byteAt(++localIP)) + ((((usqInt)(extA) << 8)));
      extA = 0;
      anObject = longAt(localSP);

      /* begin storeLiteralVariable:withValue: */
      /* begin followObjLiteral:ofMethod: */
      /* begin followObjField:ofObject: */
      litVar = fetchPointerofObject(variableIndex + LiteralStart, method);
      assert(isNonImmediate(litVar));
      if ((!((longAt((void *)(litVar))) &
             ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
        litVar = fixFollowedFieldofObjectwithInitialValue(
            variableIndex + LiteralStart, method, litVar);
      }

      /* begin storePointerImmutabilityCheck:ofObject:withValue: */
#if IMMUTABILITY
      assert(!((isImmediate(litVar))));
      if ((((usqInt)((
               byteAt((void *)(litVar + (immutableExtraBitsByteOffset())))))) >>
           (immutableBitByteShift())) &
          1) {
        /* begin cannotAssign:to:withIndex: */
        /* begin internalPush: */
        longAtput((localSP -= BytesPerOop), litVar);
        longAtput((localSP -= BytesPerOop), anObject);
        longAtput((localSP -= BytesPerOop),
                  (((usqInt)2 /* (ValueIndex + 1) */ << 3) | 1));
        messageSelector =
            fetchPointerofObject(SelectorAttemptToAssign, specialObjectsOop);
        argumentCount = 2;
        goto normalSend;
        goto l148;
      }
#endif // IMMUTABILITY

      /* begin storePointer:ofObject:withValue: */
      assert(validStorePointerArgs(ValueIndex, litVar, anObject));
      assert(isNonImmediate(litVar));
      if (oopisGreaterThanOrEqualTo(litVar, oldSpaceStart)) {
        if (/* isYoung: */
            ((!(anObject & (tagMask())))) &&
            (oopisLessThan(anObject, oldSpaceStart))) {
          /* begin possibleRootStoreInto: */
          if (!((byteAt((void *)(litVar + (formatFieldByteOffset())))) &
                (1U << (rememberedBitByteShift())))) {
            remember(litVar);
          }
        }
      }

      /* most stores into young objects */
      longAtput((void *)((litVar + BaseHeaderSize) +
                         ((((usqInt)(ValueIndex) << (shiftForWord()))))),
                anObject);
      /* end storePointerImmutabilityCheck:ofObject:withValue: */
    l148:

      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
    } break;
    case 501: // 245	longStoreTemporaryVariableBytecode
    {
      usqInt frameNumArgs;
      sqInt index;

      VM_LABEL(longStoreTemporaryVariableBytecode);
      index = byteAt(++localIP);

      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;

      /* temporary:in:put: */
      if (index < ((frameNumArgs = byteAt((localFP + FoxFrameFlags) + 1)))) {
        longAtput((localFP + FoxCallerSavedIP) +
                      ((frameNumArgs - index) * BytesPerWord),
                  longAt(localSP));
      } else {
        longAtput(((localFP + FoxReceiver) - BytesPerWord) +
                      ((frameNumArgs - index) * BytesPerWord),
                  longAt(localSP));
      }
    } break;
    case 505: // 249	extPushFullClosureBytecode
    {
      sqInt byte;
      sqInt compiledBlock;
      sqInt compiledBlockLiteralIndex;
      sqInt context;
      sqInt header;
      sqInt i;
      int ignoreContext;
      usqInt newClosure;
      usqInt newObj;
      usqInt numArgs;
      usqInt numBytes;
      sqInt numCopied;
      sqInt numCopiedSqInt;
      sqInt numSlots;
      int receiverIsOnStack;
      sqInt startIndex;

      VM_LABEL(extPushFullClosureBytecode);
      compiledBlockLiteralIndex =
          (byteAt(++localIP)) + ((((usqInt)(extA) << 8)));
      extA = 0;

      /* begin followObjLiteral:ofMethod: */
      /* begin followObjField:ofObject: */
      compiledBlock = fetchPointerofObject(
          compiledBlockLiteralIndex + LiteralStart, method);
      assert(isNonImmediate(compiledBlock));
      if ((!((longAt((void *)(compiledBlock))) &
             ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
        compiledBlock = fixFollowedFieldofObjectwithInitialValue(
            compiledBlockLiteralIndex + LiteralStart, method, compiledBlock);
      }
      assert(isOopCompiledMethod(compiledBlock));

      /* begin argumentCountOf: */
      /* begin methodHeaderOf: */
      assert(isCompiledMethod(compiledBlock));
      header = fetchPointerofObject(HeaderIndex, compiledBlock);
      numArgs = (((usqInt)(header)) >> MethodHeaderArgCountShift) & 15;
      byte = byteAt(++localIP);
      numCopied = byte & (0x3F);
      receiverIsOnStack = ((byte & (128)) != 0);
      ignoreContext = ((byte & (64)) != 0);

      /* begin
       * pushFullClosureNumArgs:copiedValues:compiledBlock:receiverIsOnStack:ignoreContext:
       */
      if (ignoreContext) {
        context = nilObj;
      } else {
        /* begin ensureFrameIsMarried:SP: */
        if (byteAt((localFP + FoxFrameFlags) + 2)) {
          assert(isContext(frameContext(localFP)));
          context = longAt(localFP + FoxThisContext);
          goto l150;
        }
        context = marryFrameSP(localFP, localSP + (numCopied * BytesPerOop));
        /* end ensureFrameIsMarried:SP: */
      l150:;
      }

      /* begin fullClosureIn:numArgs:numCopiedValues:compiledBlock: */
      numSlots = FullClosureFirstCopiedValueIndex + numCopied;
      assert(
          (numSlots >= 0) &&
          ((knownClassAtIndex(ClassFullBlockClosureCompactIndex)) != nilObj));
      assert((indexablePointersFormat()) ==
             (instSpecOfClass(
                 knownClassAtIndex(ClassFullBlockClosureCompactIndex))));

      /* begin allocateSmallNewSpaceSlots:format:classIndex: */
      assert(numSlots < (numSlotsMask()));
      newObj = freeStart;
      numBytes = BaseHeaderSize + ((numSlots < 1 ? 8 /* allocationUnit */
                                                 : numSlots * BytesPerOop));
      assert((numBytes % (allocationUnit())) == 0);
      assert((newObj % (allocationUnit())) == 0);
      if ((freeStart + numBytes) > scavengeThreshold) {
        if (!needGCFlag) {
          /* begin scheduleScavenge */
          needGCFlag = 1;
          forceInterruptCheck();
        }
        if ((freeStart + numBytes) > (((eden).limit))) {
          error("no room in eden for "
                "allocateSmallNewSpaceSlots:format:classIndex:");
          newClosure = 0;
          goto l151;
        }
      }
      long64Atput(
          (void *)(newObj),
          ((((((usqLong)numSlots)) << (numSlotsFullShift()))) +
           ((((usqInt)((indexablePointersFormat())) << (formatShift()))))) +
              ClassFullBlockClosureCompactIndex);
      freeStart += numBytes;
      newClosure = newObj;
      /* end eeInstantiateSmallClassIndex:format:numSlots: */
    l151:

      /* Assume: have just allocated a new closure; it must be young. Thus, can
       * use unchecked stores. */

      /* begin storePointerUnchecked:ofObject:withValue: */
      assert((isNonImmediate(newClosure)) && (!(isForwarded(newClosure))));
      assert(validStorePointerUncheckedArgs(ClosureOuterContextIndex,
                                            newClosure, context));
      longAtput((void *)((newClosure + BaseHeaderSize) +
                         ((((usqInt)(ClosureOuterContextIndex)
                            << (shiftForWord()))))),
                context);

      /* begin storePointerUnchecked:ofObject:withValue: */
      assert((isNonImmediate(newClosure)) && (!(isForwarded(newClosure))));
      assert(validStorePointerUncheckedArgs(FullClosureCompiledBlockIndex,
                                            newClosure, compiledBlock));
      longAtput((void *)((newClosure + BaseHeaderSize) +
                         ((((usqInt)(FullClosureCompiledBlockIndex)
                            << (shiftForWord()))))),
                compiledBlock);

      /* begin storePointerUnchecked:ofObject:withValue: */
      assert((isNonImmediate(newClosure)) && (!(isForwarded(newClosure))));
      assert(validStorePointerUncheckedArgs(ClosureNumArgsIndex, newClosure,
                                            ((numArgs << 3) | 1)));
      longAtput(
          (void *)((newClosure + BaseHeaderSize) +
                   ((((usqInt)(ClosureNumArgsIndex) << (shiftForWord()))))),
          ((numArgs << 3) | 1));
      if (receiverIsOnStack) {
        startIndex = FullClosureReceiverIndex;
        numCopiedSqInt = numCopied + 1;
      } else {
        startIndex = FullClosureFirstCopiedValueIndex;

        /* begin storePointerUnchecked:ofObject:withValue: */
        assert((isNonImmediate(newClosure)) && (!(isForwarded(newClosure))));
        assert(validStorePointerUncheckedArgs(FullClosureReceiverIndex,
                                              newClosure,
                                              longAt(localFP + FoxReceiver)));
        longAtput((void *)((newClosure + BaseHeaderSize) +
                           ((((usqInt)(FullClosureReceiverIndex)
                              << (shiftForWord()))))),
                  longAt(localFP + FoxReceiver));
        numCopiedSqInt = numCopied;
      }
      if (numCopiedSqInt > 0) {
        for (i = 0; i < numCopiedSqInt; i += 1) {
          /* begin storePointerUnchecked:ofObject:withValue: */
          assert((isNonImmediate(newClosure)) && (!(isForwarded(newClosure))));
          assert(validStorePointerUncheckedArgs(
              i + startIndex, newClosure,
              longAt((void *)(localSP +
                              (((numCopiedSqInt - i) - 1) * BytesPerOop)))));
          longAtput(
              (void *)((newClosure + BaseHeaderSize) +
                       ((((usqInt)((i + startIndex)) << (shiftForWord()))))),
              longAt((void *)(localSP +
                              (((numCopiedSqInt - i) - 1) * BytesPerOop))));
        }

        /* Assume: have just allocated a new BlockClosure; it must be young.
           Thus, can use unchecked stores. */

        /* begin internalPop: */
        localSP += numCopiedSqInt * BytesPerOop;
      }
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      longAtput((localSP -= BytesPerOop), newClosure);
    } break;
    case 506: // 250	extPushClosureBytecode
    {
      sqInt blockSize;
      sqInt byte;
      sqInt context;
      sqInt i;
      sqInt initialIP;
      usqInt newClosure;
      usqInt newObj;
      usqInt numArgs;
      usqInt numBytes;
      usqInt numCopied;
      sqInt numSlots;

      VM_LABEL(extPushClosureBytecode);
      byte = byteAt(++localIP);
      numArgs = (byte & 7) + ((extA % 16) * 8);
      numCopied = ((((usqInt)(byte)) >> 3) & 7) + ((extA / 16) * 8);
      extA = 0;
      blockSize = (byteAt(++localIP)) + ((((usqInt)(extB) << 8)));
      numExtB = (extB = 0);

      /* begin pushClosureNumArgs:copiedValues:blockSize: */
      /* begin ensureFrameIsMarried:SP: */
      if (byteAt((localFP + FoxFrameFlags) + 2)) {
        assert(isContext(frameContext(localFP)));
        context = longAt(localFP + FoxThisContext);
        goto l152;
      }
      context = marryFrameSP(localFP, localSP + (numCopied * BytesPerOop));
      /* end ensureFrameIsMarried:SP: */
    l152:
      initialIP = ((((usqInt)localIP)) + 2) - (method + BaseHeaderSize);

      /* begin closureIn:numArgs:instructionPointer:numCopiedValues: */
      numSlots = ClosureFirstCopiedValueIndex + numCopied;
      assert((numSlots >= 0) &&
             ((knownClassAtIndex(ClassBlockClosureCompactIndex)) != nilObj));
      assert(
          (indexablePointersFormat()) ==
          (instSpecOfClass(knownClassAtIndex(ClassBlockClosureCompactIndex))));

      /* begin allocateSmallNewSpaceSlots:format:classIndex: */
      assert(numSlots < (numSlotsMask()));
      newObj = freeStart;
      numBytes = BaseHeaderSize + ((numSlots < 1 ? 8 /* allocationUnit */
                                                 : numSlots * BytesPerOop));
      assert((numBytes % (allocationUnit())) == 0);
      assert((newObj % (allocationUnit())) == 0);
      if ((freeStart + numBytes) > scavengeThreshold) {
        if (!needGCFlag) {
          /* begin scheduleScavenge */
          needGCFlag = 1;
          forceInterruptCheck();
        }
        if ((freeStart + numBytes) > (((eden).limit))) {
          error("no room in eden for "
                "allocateSmallNewSpaceSlots:format:classIndex:");
          newClosure = 0;
          goto l153;
        }
      }
      long64Atput(
          (void *)(newObj),
          ((((((usqLong)numSlots)) << (numSlotsFullShift()))) +
           ((((usqInt)((indexablePointersFormat())) << (formatShift()))))) +
              ClassBlockClosureCompactIndex);
      freeStart += numBytes;
      newClosure = newObj;
      /* end eeInstantiateSmallClassIndex:format:numSlots: */
    l153:

      /* Assume: have just allocated a new closure; it must be young. Thus, can
       * use unchecked stores. */

      /* begin storePointerUnchecked:ofObject:withValue: */
      assert((isNonImmediate(newClosure)) && (!(isForwarded(newClosure))));
      assert(validStorePointerUncheckedArgs(ClosureOuterContextIndex,
                                            newClosure, context));
      longAtput((void *)((newClosure + BaseHeaderSize) +
                         ((((usqInt)(ClosureOuterContextIndex)
                            << (shiftForWord()))))),
                context);

      /* begin storePointerUnchecked:ofObject:withValue: */
      assert((isNonImmediate(newClosure)) && (!(isForwarded(newClosure))));
      assert(validStorePointerUncheckedArgs(ClosureStartPCIndex, newClosure,
                                            (((usqInt)initialIP << 3) | 1)));
      longAtput(
          (void *)((newClosure + BaseHeaderSize) +
                   ((((usqInt)(ClosureStartPCIndex) << (shiftForWord()))))),
          (((usqInt)initialIP << 3) | 1));

      /* begin storePointerUnchecked:ofObject:withValue: */
      assert((isNonImmediate(newClosure)) && (!(isForwarded(newClosure))));
      assert(validStorePointerUncheckedArgs(ClosureNumArgsIndex, newClosure,
                                            ((numArgs << 3) | 1)));
      longAtput(
          (void *)((newClosure + BaseHeaderSize) +
                   ((((usqInt)(ClosureNumArgsIndex) << (shiftForWord()))))),
          ((numArgs << 3) | 1));
      if (numCopied > 0) {
        for (i = 0; i < numCopied; i += 1) {
          /* begin storePointerUnchecked:ofObject:withValue: */
          assert((isNonImmediate(newClosure)) && (!(isForwarded(newClosure))));
          assert(validStorePointerUncheckedArgs(
              i + ClosureFirstCopiedValueIndex, newClosure,
              longAt(
                  (void *)(localSP + (((numCopied - i) - 1) * BytesPerOop)))));
          longAtput((void *)((newClosure + BaseHeaderSize) +
                             ((((usqInt)((i + ClosureFirstCopiedValueIndex))
                                << (shiftForWord()))))),
                    longAt((void *)(localSP +
                                    (((numCopied - i) - 1) * BytesPerOop))));
        }

        /* Assume: have just allocated a new BlockClosure; it must be young.
           Thus, can use unchecked stores. */

        /* begin internalPop: */
        localSP += numCopied * BytesPerOop;
      }
      localIP += blockSize;

      /* begin fetchNextBytecode */
      currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;
      longAtput((localSP -= BytesPerOop), newClosure);
    } break;
    }
  }

  /* undo the pre-increment of IP (in fetchByte in fetchNextBytecode) before
   * returning */
  localIP -= 1;
  instructionPointer = ((usqInt)localIP);
  stackPointer = localSP;
  framePointer = localFP;
  return null;
}