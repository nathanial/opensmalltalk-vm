/* Extracted from interp.c:59223 (function printAllStacks). */

/*	Print all the stacks of all running processes, including those that are
        currently suspended.
 */
/*	useful for VM debugging */

/* StackInterpreter>>#printAllStacks */

void printAllStacks(void) {
  sqInt address;
  sqInt classPointer;
  sqInt followingWord;
  usqInt followingWordAddress;
  sqInt linkedListClass;
  sqInt minProcessInstSize;
  sqInt myList;
  sqInt myListClass;
  usqInt numSlots;
  sqInt objOop;
  sqInt objSqInt;
  usqInt p;
  sqInt prevObj;
  sqInt prevPrevObj;
  sqInt pri;
  sqInt proc;
  sqInt processClass;
  sqInt processList;
  sqInt schedLists;
  sqInt startObject;
  sqInt tagBits;

  /* may not be an instance of process. may in exceptional circumstances be
   * nilObject */
  proc = activeProcess();
  printNameOfClasscount(/* fetchClassOf: */
                        ((tagBits = proc & (tagMask()))
                             ? fetchPointerofObject(tagBits, classTableFirstPage)
                             : fetchClassOfNonImm(proc)),
                        5);

  /* begin space */
  printChar(' ');
  printHex(proc);
  print(" priority ");
  printNum(quickFetchIntegerofObject(PriorityIndex, proc));
  cr();
  if (framePointer) {
    printCallStack();
  } else {
    printProcessStack(proc);
  }
  objOop =
      fetchPointerofObject(ValueIndex, fetchPointerofObject(SchedulerAssociation, specialObjectsOop));

  schedLists =
      fetchPointerofObject(ProcessListsIndex, objOop);
  linkedListClass = null;

  /* then the runnable processes */
  p = (highestRunnableProcessPriority ? highestRunnableProcessPriority
                                      : numSlotsOf(schedLists));
  for (pri = (p - 1); pri >= 0; pri += -1) {
    processList = fetchPointerofObject(pri, schedLists);
    if (!(isEmptyList(processList))) {
      if (proc == nilObj) {
        proc = fetchPointerofObject(FirstLinkIndex, processList);
      }
      cr();
      print("processes at priority ");
      printNum(pri + 1);
      printProcsOnList(processList);
    }
    if (!linkedListClass) {
      linkedListClass = fetchClassOfNonImm(processList);
    }
  }
  if (!linkedListClass) {
    classPointer =
        fetchPointerofObject(ClassSemaphore, specialObjectsOop);

    /* begin superclassOf: */
    /* begin followObjField:ofObject: */
    linkedListClass =
        fetchPointerofObject(SuperclassIndex, classPointer);
    assert(isNonImmediate(linkedListClass));
    if ((!((longAt((void *)(linkedListClass))) &
           ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
      linkedListClass = fixFollowedFieldofObjectwithInitialValue(
          SuperclassIndex, classPointer, linkedListClass);
    }
  }
  if (proc == nilObj) {
    cr();
    print("Cannot find a runnable process. Cannot therefore determine class "
          "Process. Cannot therefore print suspended processes");
    return;
  }
  cr();
  print("suspended processes");

  /* Find the root of the Process hierarchy. It is the class, or superclass,
     of a process, that has inst size at least large enough to include myList */
  processClass =
      (!(proc == nilObj) ? /* fetchClassOf: */
           ((tagBits = proc & (tagMask()))
                ? fetchPointerofObject(tagBits, classTableFirstPage)
                : fetchClassOfNonImm(proc))
                         : 0);
  minProcessInstSize = MyListIndex + 1;
  while ((instanceSizeOf(superclassOf(processClass))) >= minProcessInstSize) {
    /* begin superclassOf: */
    /* begin followObjField:ofObject: */
    objOop =
        fetchPointerofObject(SuperclassIndex, processClass);
    assert(isNonImmediate(objOop));
    if ((!((longAt((void *)(objOop))) &
           ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
      objOop = fixFollowedFieldofObjectwithInitialValue(SuperclassIndex,
                                                        processClass, objOop);
    }
    processClass = objOop;
  }
  minProcessInstSize = instanceSizeOf(processClass);

  /* look for all subInstances of process that have a context as a
   * suspendedContext and are on a list other than a LinkedList */

  /* begin allObjectsDoSafely: */
  address = /* startAddressForBridgedHeapEnumeration */
      (pastSpaceStart > (((pastSpace).start))
           ? ((pastSpace).start)
           : (freeStart > (((eden).start)) ? ((eden).start) : oldSpaceStart));

  /* begin objectStartingAt: */
  numSlots = byteAt((void *)(address + (numSlotsFieldByteOffset())));
  startObject =
      (numSlots == (numSlotsMask()) ? address + BaseHeaderSize : address);

  /* begin allEntitiesFrom:do: */
  prevPrevObj = (prevObj = null);
  objSqInt = startObject;
  enableObjectEnumerationFrom(startObject);
  while (1) {
    assert((objSqInt % (allocationUnit())) == 0);
    if (!(oopisLessThan(objSqInt, endOfMemory)))
      break;
    assert((long64At((void *)(objSqInt))) != 0);
    if (((longAt((void *)(objSqInt))) & (classIndexMask())) >
        (lastClassIndexPun())) {
      if ((((byteAt((void *)(objSqInt + (formatFieldByteOffset())))) &
            (formatMask())) <= 5 /* lastPointerFormat */) &&
          (((numSlotsOf(objSqInt)) >= minProcessInstSize) &&
           ((isContext(fetchPointerofObject(SuspendedContextIndex, objSqInt))) &&
            (isKindOfClass(objSqInt, processClass))))) {
        myList =
            fetchPointerofObject(MyListIndex, objSqInt);
        if ((myList != nilObj) &&
            ((((myListClass = fetchClassOfNonImm(myList))) !=
              linkedListClass) &&
             (isKindOfClass(myList, linkedListClass)))) {
          printProcessStack(objSqInt);
        }
      }
    }
    prevPrevObj = prevObj;
    prevObj = objSqInt;

    /* begin objectAfterMaybeSlimBridge:limit: */
    followingWordAddress = addressAfter(objSqInt);
    if (oopisGreaterThanOrEqualTo(followingWordAddress, endOfMemory)) {
      objSqInt = endOfMemory;
      goto l1;
    }
    followingWord = longAt((void *)(followingWordAddress));
    objSqInt =
        ((((usqInt)(followingWord)) >> (numSlotsFullShift())) ==
                 (numSlotsMask())
             ? ((oopisLessThan(objSqInt, oldSpaceStart)) &&
                        ((followingWord & 0xFFFFFFFFFFFFFFLL) == 1)
                    ? (followingWordAddress + BaseHeaderSize) + BaseHeaderSize
                    : followingWordAddress + BaseHeaderSize)
             : followingWordAddress);
    /* end objectAfterMaybeSlimBridge:limit: */
  l1:
    assert(oopisGreaterThan(objSqInt, prevObj));
  }
}