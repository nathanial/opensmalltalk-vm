/* Extracted from interp.c:63846 (function spurPostBecomeAction). */

/*	Insulate the stack zone from the effects of a become.
        All receivers must be unfollowed for two reasons:
        1. inst var access is direct with no read barrier
        2. super sends (always to the receiver) have no class check and so don't
        trap for forwarded receivers. This is an issue for primitives that
   assume their receiver is valid and don't validate. Super sends require an
   explicit check to ensure receivers in super sends are unforwarded. e.g. super
   doSomethingWith: (self become: other) forwards the receiver self pushed on
   the stack. So we could avoid following non-pointer receivers. But this is too
        tricky, Instead, we
        always follow receivers.
        Methods must be unfollowed since bytecode access is direct with no read
        barrier. But this only needs to be done if the becomeEffectsFlags
   indicate that a CompiledMethod was becommed. The scheduler state must be
   followed, but only if the becomeEffectsFlags indicate that a pointer object
   was becommed. */
/*	For VM profiling */

/* StackInterpreter>>#spurPostBecomeAction: */

static void spurPostBecomeAction(sqInt theBecomeEffectsFlags) {
  sqIntptr_t c;
  sqInt first;
  int i;
  sqInt ipdelta;
  sqInt iSqInt;
  sqInt last;
  sqInt list;
  sqIntptr_t m;
  sqInt next;
  usqInt numSlots;
  sqInt obj;
  sqInt procLists;
  sqInt referent;
  sqInt result;
  sqIntptr_t s;
  sqInt sched;
  sqInt schedAssoc;
  sqInt toDoLimit;
  sqInt xArray;

  /* begin flushAtCache */
  memset(atCache, 0, AtCacheTotalSize * (sizeof(atCache[0])));
  if (theBecomeEffectsFlags) {
    if (((theBecomeEffectsFlags & BecameActiveClassFlag) != 0)) {
      /* begin flushBecommedClassesInMethodCache */
      for (i = 0; i < MethodCacheSize; i += MethodCacheEntrySize) {
        c = methodCache[i + MethodCacheClass];
        s = methodCache[i + MethodCacheSelector];
        if ((c != 0) && ((s != 0) && (isForwarded(classOrNilAtIndex(c))))) {
          methodCache[i + MethodCacheClass] = 0;
          methodCache[i + MethodCacheSelector] = 0;
        }
      }
    }
    if (((theBecomeEffectsFlags & BecamePointerObjectFlag) != 0)) {
      /* begin followForwardingPointersInScheduler */
      schedAssoc =
          fetchPointerofObject(SchedulerAssociation, specialObjectsOop);

      /* the GC follows pointers in the special objects array for us. */
      assert(!(isForwarded(schedAssoc)));

      /* Make sure the active process has been followed. */
      followForwardedObjectFieldstoDepth(schedAssoc, 1);
      sched = fetchPointerofObject(ValueIndex, schedAssoc);

      procLists = followObjFieldofObject(ProcessListsIndex, sched);

      /* Follow all links in the process list to ensure the lists are valid. */
      toDoLimit = ((assert((classIndexOf(procLists)) >
                           (isForwardedObjectClassIndexPun())),
                    numSlotsOf(procLists))) -
                  1;
      for (iSqInt = 0; iSqInt <= toDoLimit; iSqInt += 1) {
        list = followObjFieldofObject(iSqInt, procLists);

        first = followObjFieldofObject(FirstLinkIndex, list);

        last = followObjFieldofObject(LastLinkIndex, list);
        while (first != last) {
          next = followObjFieldofObject(NextLinkIndex, first);
          first = next;
        }
      }

      /* begin followForwardingPointersInSpecialObjectsArray */
      /* begin followSemaphoreIn:at: */
      obj = fetchPointerofObject(TheLowSpaceSemaphore, specialObjectsOop);
      if ((!((longAt((void *)(obj))) &
             ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
        obj = followForwarded(obj);

        storePointerofObjectwithValue(TheLowSpaceSemaphore, specialObjectsOop,
                                      obj);
      }

      /* begin followSemaphoreIn:at: */
      obj = fetchPointerofObject(TheInterruptSemaphore, specialObjectsOop);
      if ((!((longAt((void *)(obj))) &
             ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
        obj = followForwarded(obj);

        storePointerofObjectwithValue(TheInterruptSemaphore, specialObjectsOop,
                                      obj);
      }

      /* begin followSemaphoreIn:at: */
      obj = fetchPointerofObject(TheTimerSemaphore, specialObjectsOop);
      if ((!((longAt((void *)(obj))) &
             ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
        obj = followForwarded(obj);

        storePointerofObjectwithValue(TheTimerSemaphore, specialObjectsOop,
                                      obj);
      }

      /* begin followSemaphoreIn:at: */
      obj = fetchPointerofObject(TheFinalizationSemaphore, specialObjectsOop);
      if ((!((longAt((void *)(obj))) &
             ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
        obj = followForwarded(obj);

        storePointerofObjectwithValue(TheFinalizationSemaphore,
                                      specialObjectsOop, obj);
      }
      xArray = fetchPointerofObject(ExternalObjectsArray, specialObjectsOop);
      if ((!((longAt((void *)(xArray))) &
             ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
        xArray = followForwarded(xArray);

        /* begin splObj:put: */
        storePointerofObjectwithValue(ExternalObjectsArray, specialObjectsOop,
                                      xArray);
      }
      toDoLimit = ((numSlotsOf(xArray))) - 1;
      for (ipdelta = 0; ipdelta <= toDoLimit; ipdelta += 1) {
        /* begin followSemaphoreIn:at: */
        obj = fetchPointerofObject(ipdelta, xArray);
        if ((!((longAt((void *)(obj))) &
               ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
          obj = followForwarded(obj);

          storePointerofObjectwithValue(ipdelta, xArray, obj);
        }
      }
    }
    if (((theBecomeEffectsFlags &
          (BecamePointerObjectFlag + BecameCompiledMethodFlag)) != 0)) {
      /* begin followForwardingPointersInProfileState */
      /* begin profileStateDoUpdating: */
      if (profileProcess) {
        if ((result =
                 ((!((longAt((void *)(profileProcess))) &
                     ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))
                      ? followForwarded(profileProcess)
                      : 0))) {
          profileProcess = result;
        }
      }
      if (profileMethod) {
        if ((result =
                 ((!((longAt((void *)(profileMethod))) &
                     ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))
                      ? followForwarded(profileMethod)
                      : 0))) {
          profileMethod = result;
        }
      }
      if (profileSemaphore) {
        if ((result =
                 ((!((longAt((void *)(profileSemaphore))) &
                     ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))
                      ? followForwarded(profileSemaphore)
                      : 0))) {
          profileSemaphore = result;
        }
      }
      if (((theBecomeEffectsFlags & BecameCompiledMethodFlag) != 0)) {
        /* begin followForwardedMethodsInMethodCache */
        for (i = 0; i < MethodCacheSize; i += MethodCacheEntrySize) {
          c = methodCache[i + MethodCacheClass];
          s = methodCache[i + MethodCacheSelector];
          m = methodCache[i + MethodCacheMethod];
          if ((c != 0) && ((s != 0) && ((m != 0) && (isOopForwarded(m))))) {
            m = followForwarded(m);
            methodCache[i + MethodCacheMethod] = m;
          }
        }

        /* begin followMethodNewMethodAndInstructionPointer */
        if ((!((longAt((void *)(method))) &
               ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
          ipdelta =
              (/* method:includesAddress: */
               (instructionPointer > method) &&
                       (instructionPointer <
                        (method +
                         ((((numSlotsOfAny(method)) << (shiftForWord()))) +
                          BaseHeaderSize)))
                   ? instructionPointer - method
                   : 0);

          method = followForwarded(method);
          if (ipdelta) {
            instructionPointer = method + ipdelta;
          }
        }
        if (isOopForwarded(newMethod)) {
          newMethod = followForwarded(newMethod);
        }
      }
    }
  }
  followForwardingPointersOfReceiversInStackZone();
}