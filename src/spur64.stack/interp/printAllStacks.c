/* Extracted from interp.c:59223 (function printAllStacks). */

void
printAllStacks(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
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

	/* may not be an instance of process. may in exceptional circumstances be nilObject */
	proc = activeProcess();
	printNameOfClasscount(/* fetchClassOf: */
		((tagBits = proc & (tagMask()))
			? longAt((void *)((GIV(classTableFirstPage) + BaseHeaderSize) + ((((usqInt)(tagBits) << (shiftForWord()))))))
			: fetchClassOfNonImm(proc)), 5);

	/* begin space */
	printChar(' ');
	printHex(proc);
	print(" priority ");
	printNum(quickFetchIntegerofObject(PriorityIndex, proc));
	cr();
	if (GIV(framePointer)) {
		printCallStack();
	}
	else {
		printProcessStack(proc);
	}
	objOop = longAt((void *)(((longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(SchedulerAssociation) << (shiftForWord()))))))) + BaseHeaderSize) + ((((usqInt)(ValueIndex) << (shiftForWord()))))));

	/* begin fetchPointer:ofObject: */
	schedLists = longAt((void *)((objOop + BaseHeaderSize) + ((((usqInt)(ProcessListsIndex) << (shiftForWord()))))));
	linkedListClass = null;

	/* then the runnable processes */
	p = (GIV(highestRunnableProcessPriority)
				? GIV(highestRunnableProcessPriority)
				: numSlotsOf(schedLists));
	for (pri = (p - 1); pri >= 0; pri += -1) {
		processList = longAt((void *)((schedLists + BaseHeaderSize) + ((((usqInt)(pri) << (shiftForWord()))))));
		if (!(isEmptyList(processList))) {
			if (proc == GIV(nilObj)) {
				proc = longAt((void *)((processList + BaseHeaderSize) + ((((usqInt)(FirstLinkIndex) << (shiftForWord()))))));
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
		classPointer = longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(ClassSemaphore) << (shiftForWord()))))));

		/* begin superclassOf: */
		/* begin followObjField:ofObject: */
		linkedListClass = longAt((void *)((classPointer + BaseHeaderSize) + ((((usqInt)(SuperclassIndex) << (shiftForWord()))))));
		assert(isNonImmediate(linkedListClass));
		if ((!((longAt((void *)(linkedListClass))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
			linkedListClass = fixFollowedFieldofObjectwithInitialValue(SuperclassIndex, classPointer, linkedListClass);
		}
	}
	if (proc == GIV(nilObj)) {
		cr();
		print("Cannot find a runnable process. Cannot therefore determine class Process. Cannot therefore print suspended processes");
		return;
	}
	cr();
	print("suspended processes");

	/* Find the root of the Process hierarchy. It is the class, or superclass,
	   of a process, that has inst size at least large enough to include myList */
	processClass = (!(proc == GIV(nilObj))
				? /* fetchClassOf: */
					((tagBits = proc & (tagMask()))
						? longAt((void *)((GIV(classTableFirstPage) + BaseHeaderSize) + ((((usqInt)(tagBits) << (shiftForWord()))))))
						: fetchClassOfNonImm(proc))
				: 0);
	minProcessInstSize = MyListIndex + 1;
	while ((instanceSizeOf(superclassOf(processClass))) >= minProcessInstSize) {
		/* begin superclassOf: */
		/* begin followObjField:ofObject: */
		objOop = longAt((void *)((processClass + BaseHeaderSize) + ((((usqInt)(SuperclassIndex) << (shiftForWord()))))));
		assert(isNonImmediate(objOop));
		if ((!((longAt((void *)(objOop))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
			objOop = fixFollowedFieldofObjectwithInitialValue(SuperclassIndex, processClass, objOop);
		}
		processClass = objOop;
	}
	minProcessInstSize = instanceSizeOf(processClass);

	/* look for all subInstances of process that have a context as a suspendedContext and are on a list other than a LinkedList */

	/* begin allObjectsDoSafely: */
	address = /* startAddressForBridgedHeapEnumeration */
			(GIV(pastSpaceStart) > (((GIV(pastSpace)).start))
				? ((GIV(pastSpace)).start)
				: (GIV(freeStart) > (((GIV(eden)).start))
						? ((GIV(eden)).start)
						: GIV(oldSpaceStart)));

	/* begin objectStartingAt: */
	numSlots = byteAt((void *)(address + (numSlotsFieldByteOffset())));
	startObject = (numSlots == (numSlotsMask())
				? address + BaseHeaderSize
				: address);

	/* begin allEntitiesFrom:do: */
	prevPrevObj = (prevObj = null);
	objSqInt = startObject;
	enableObjectEnumerationFrom(startObject);
	while (1) {
		assert((objSqInt % (allocationUnit())) == 0);
		if (!(oopisLessThan(objSqInt, GIV(endOfMemory)))) break;
		assert((long64At((void *)(objSqInt))) != 0);
		if (((longAt((void *)(objSqInt))) & (classIndexMask())) > (lastClassIndexPun())) {
			if ((((byteAt((void *)(objSqInt + (formatFieldByteOffset())))) & (formatMask())) <= 5 /* lastPointerFormat */)
			 && (((numSlotsOf(objSqInt)) >= minProcessInstSize)
			 && ((isContext(longAt((void *)((objSqInt + BaseHeaderSize) + ((((usqInt)(SuspendedContextIndex) << (shiftForWord()))))))))
			 && (isKindOfClass(objSqInt, processClass))))) {
				myList = longAt((void *)((objSqInt + BaseHeaderSize) + ((((usqInt)(MyListIndex) << (shiftForWord()))))));
				if ((myList != GIV(nilObj))
				 && ((((myListClass = fetchClassOfNonImm(myList))) != linkedListClass)
				 && (isKindOfClass(myList, linkedListClass)))) {
					printProcessStack(objSqInt);
				}
			}
		}
		prevPrevObj = prevObj;
		prevObj = objSqInt;

		/* begin objectAfterMaybeSlimBridge:limit: */
		followingWordAddress = addressAfter(objSqInt);
		if (oopisGreaterThanOrEqualTo(followingWordAddress, GIV(endOfMemory))) {
			objSqInt = GIV(endOfMemory);
			goto l1;
		}
		followingWord = longAt((void *)(followingWordAddress));
		objSqInt = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) == (numSlotsMask())
					? ((oopisLessThan(objSqInt, GIV(oldSpaceStart)))
					 && ((followingWord & 0xFFFFFFFFFFFFFFLL) == 1)
							? (followingWordAddress + BaseHeaderSize) + BaseHeaderSize
							: followingWordAddress + BaseHeaderSize)
					: followingWordAddress);
		/* end objectAfterMaybeSlimBridge:limit: */
l1:
		assert(oopisGreaterThan(objSqInt, prevObj));
	}
}