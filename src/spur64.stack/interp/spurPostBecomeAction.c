/* Extracted from interp.c:63846 (function spurPostBecomeAction). */

/*	Insulate the stack zone from the effects of a become.
	All receivers must be unfollowed for two reasons:
	1. inst var access is direct with no read barrier
	2. super sends (always to the receiver) have no class check and so don't
	trap for forwarded receivers. This is an issue for primitives that assume
	their receiver
	is valid and don't validate.
	Super sends require an explicit check to ensure receivers in super sends
	are unforwarded.
	e.g. super doSomethingWith: (self become: other) forwards the receiver
	self pushed on the
	stack. So we could avoid following non-pointer receivers. But this is too
	tricky, Instead, we
	always follow receivers.
	Methods must be unfollowed since bytecode access is direct with no read
	barrier. But this only needs to be done if the becomeEffectsFlags indicate
	that a
	CompiledMethod was becommed.
	The scheduler state must be followed, but only if the becomeEffectsFlags
	indicate that a pointer object was becommed. */
/*	For VM profiling */

	/* StackInterpreter>>#spurPostBecomeAction: */

static NoDbgRegParms void
spurPostBecomeAction(sqInt theBecomeEffectsFlags)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
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
	memset(GIV(atCache), 0, AtCacheTotalSize * (sizeof(GIV(atCache)[0])));
	if (theBecomeEffectsFlags) {
		if (((theBecomeEffectsFlags & BecameActiveClassFlag) != 0)) {
			/* begin flushBecommedClassesInMethodCache */
			for (i = 0; i < MethodCacheSize; i += MethodCacheEntrySize) {
				c = GIV(methodCache)[i + MethodCacheClass];
				s = GIV(methodCache)[i + MethodCacheSelector];
				if ((c != 0)
				 && ((s != 0)
				 && (isForwarded(classOrNilAtIndex(c))))) {
					GIV(methodCache)[i + MethodCacheClass] = 0;
					GIV(methodCache)[i + MethodCacheSelector] = 0;
				}
			}
		}
		if (((theBecomeEffectsFlags & BecamePointerObjectFlag) != 0)) {
			/* begin followForwardingPointersInScheduler */
			schedAssoc = longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(SchedulerAssociation) << (shiftForWord()))))));

			/* the GC follows pointers in the special objects array for us. */
			assert(!(isForwarded(schedAssoc)));

			/* Make sure the active process has been followed. */
			followForwardedObjectFieldstoDepth(schedAssoc, 1);
			sched = longAt((void *)((schedAssoc + BaseHeaderSize) + ((((usqInt)(ValueIndex) << (shiftForWord()))))));

			/* begin followObjField:ofObject: */
			procLists = longAt((void *)((sched + BaseHeaderSize) + ((((usqInt)(ProcessListsIndex) << (shiftForWord()))))));
			assert(isNonImmediate(procLists));
			if ((!((longAt((void *)(procLists))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
				procLists = fixFollowedFieldofObjectwithInitialValue(ProcessListsIndex, sched, procLists);
			}

			/* Follow all links in the process list to ensure the lists are valid. */
			toDoLimit = ((/* begin numSlotsOf: */
	assert((classIndexOf(procLists)) > (isForwardedObjectClassIndexPun())),
(((numSlots = byteAt((void *)(procLists + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
			? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(procLists - BaseHeaderSize)))) << 8)))))) >> 8
			: numSlots))) - 1;
			for (iSqInt = 0; iSqInt <= toDoLimit; iSqInt += 1) {
				/* begin followObjField:ofObject: */
				list = longAt((void *)((procLists + BaseHeaderSize) + ((((usqInt)(iSqInt) << (shiftForWord()))))));
				assert(isNonImmediate(list));
				if ((!((longAt((void *)(list))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
					list = fixFollowedFieldofObjectwithInitialValue(iSqInt, procLists, list);
				}

				/* begin followObjField:ofObject: */
				first = longAt((void *)((list + BaseHeaderSize) + ((((usqInt)(FirstLinkIndex) << (shiftForWord()))))));
				assert(isNonImmediate(first));
				if ((!((longAt((void *)(first))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
					first = fixFollowedFieldofObjectwithInitialValue(FirstLinkIndex, list, first);
				}

				/* begin followObjField:ofObject: */
				last = longAt((void *)((list + BaseHeaderSize) + ((((usqInt)(LastLinkIndex) << (shiftForWord()))))));
				assert(isNonImmediate(last));
				if ((!((longAt((void *)(last))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
					last = fixFollowedFieldofObjectwithInitialValue(LastLinkIndex, list, last);
				}
				while (first != last) {
					/* begin followObjField:ofObject: */
					next = longAt((void *)((first + BaseHeaderSize) + ((((usqInt)(NextLinkIndex) << (shiftForWord()))))));
					assert(isNonImmediate(next));
					if ((!((longAt((void *)(next))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
						next = fixFollowedFieldofObjectwithInitialValue(NextLinkIndex, first, next);
					}
					first = next;
				}
			}

			/* begin followForwardingPointersInSpecialObjectsArray */
			/* begin followSemaphoreIn:at: */
			obj = longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(TheLowSpaceSemaphore) << (shiftForWord()))))));
			if ((!((longAt((void *)(obj))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
				/* begin followForwarded: */
				assert(isUnambiguouslyForwarder(obj));
				referent = longAt((void *)((obj + BaseHeaderSize) + (0U << (shiftForWord()))));
				while (/* isOopForwarded: */
					((!(referent & (tagMask()))))
				 && ((!((longAt((void *)(referent))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
					referent = longAt((void *)((referent + BaseHeaderSize) + (0U << (shiftForWord()))));
				}
				obj = referent;

				/* begin storePointer:ofObject:withValue: */
				assert(validStorePointerArgs(TheLowSpaceSemaphore, GIV(specialObjectsOop), obj));
				assert(isNonImmediate(GIV(specialObjectsOop)));
				if (oopisGreaterThanOrEqualTo(GIV(specialObjectsOop), GIV(oldSpaceStart))) {
					if (/* isYoung: */
						((!(obj & (tagMask()))))
					 && (oopisLessThan(obj, GIV(oldSpaceStart)))) {
						/* begin possibleRootStoreInto: */
						if (!((byteAt((void *)(GIV(specialObjectsOop) + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
							remember(GIV(specialObjectsOop));
						}
					}
				}

				/* most stores into young objects */
				longAtput((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(TheLowSpaceSemaphore) << (shiftForWord()))))),obj);
			}

			/* begin followSemaphoreIn:at: */
			obj = longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(TheInterruptSemaphore) << (shiftForWord()))))));
			if ((!((longAt((void *)(obj))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
				/* begin followForwarded: */
				assert(isUnambiguouslyForwarder(obj));
				referent = longAt((void *)((obj + BaseHeaderSize) + (0U << (shiftForWord()))));
				while (/* isOopForwarded: */
					((!(referent & (tagMask()))))
				 && ((!((longAt((void *)(referent))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
					referent = longAt((void *)((referent + BaseHeaderSize) + (0U << (shiftForWord()))));
				}
				obj = referent;

				/* begin storePointer:ofObject:withValue: */
				assert(validStorePointerArgs(TheInterruptSemaphore, GIV(specialObjectsOop), obj));
				assert(isNonImmediate(GIV(specialObjectsOop)));
				if (oopisGreaterThanOrEqualTo(GIV(specialObjectsOop), GIV(oldSpaceStart))) {
					if (/* isYoung: */
						((!(obj & (tagMask()))))
					 && (oopisLessThan(obj, GIV(oldSpaceStart)))) {
						/* begin possibleRootStoreInto: */
						if (!((byteAt((void *)(GIV(specialObjectsOop) + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
							remember(GIV(specialObjectsOop));
						}
					}
				}

				/* most stores into young objects */
				longAtput((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(TheInterruptSemaphore) << (shiftForWord()))))),obj);
			}

			/* begin followSemaphoreIn:at: */
			obj = longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(TheTimerSemaphore) << (shiftForWord()))))));
			if ((!((longAt((void *)(obj))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
				/* begin followForwarded: */
				assert(isUnambiguouslyForwarder(obj));
				referent = longAt((void *)((obj + BaseHeaderSize) + (0U << (shiftForWord()))));
				while (/* isOopForwarded: */
					((!(referent & (tagMask()))))
				 && ((!((longAt((void *)(referent))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
					referent = longAt((void *)((referent + BaseHeaderSize) + (0U << (shiftForWord()))));
				}
				obj = referent;

				/* begin storePointer:ofObject:withValue: */
				assert(validStorePointerArgs(TheTimerSemaphore, GIV(specialObjectsOop), obj));
				assert(isNonImmediate(GIV(specialObjectsOop)));
				if (oopisGreaterThanOrEqualTo(GIV(specialObjectsOop), GIV(oldSpaceStart))) {
					if (/* isYoung: */
						((!(obj & (tagMask()))))
					 && (oopisLessThan(obj, GIV(oldSpaceStart)))) {
						/* begin possibleRootStoreInto: */
						if (!((byteAt((void *)(GIV(specialObjectsOop) + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
							remember(GIV(specialObjectsOop));
						}
					}
				}

				/* most stores into young objects */
				longAtput((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(TheTimerSemaphore) << (shiftForWord()))))),obj);
			}

			/* begin followSemaphoreIn:at: */
			obj = longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(TheFinalizationSemaphore) << (shiftForWord()))))));
			if ((!((longAt((void *)(obj))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
				/* begin followForwarded: */
				assert(isUnambiguouslyForwarder(obj));
				referent = longAt((void *)((obj + BaseHeaderSize) + (0U << (shiftForWord()))));
				while (/* isOopForwarded: */
					((!(referent & (tagMask()))))
				 && ((!((longAt((void *)(referent))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
					referent = longAt((void *)((referent + BaseHeaderSize) + (0U << (shiftForWord()))));
				}
				obj = referent;

				/* begin storePointer:ofObject:withValue: */
				assert(validStorePointerArgs(TheFinalizationSemaphore, GIV(specialObjectsOop), obj));
				assert(isNonImmediate(GIV(specialObjectsOop)));
				if (oopisGreaterThanOrEqualTo(GIV(specialObjectsOop), GIV(oldSpaceStart))) {
					if (/* isYoung: */
						((!(obj & (tagMask()))))
					 && (oopisLessThan(obj, GIV(oldSpaceStart)))) {
						/* begin possibleRootStoreInto: */
						if (!((byteAt((void *)(GIV(specialObjectsOop) + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
							remember(GIV(specialObjectsOop));
						}
					}
				}

				/* most stores into young objects */
				longAtput((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(TheFinalizationSemaphore) << (shiftForWord()))))),obj);
			}
			xArray = longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(ExternalObjectsArray) << (shiftForWord()))))));
			if ((!((longAt((void *)(xArray))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
				/* begin followForwarded: */
				assert(isUnambiguouslyForwarder(xArray));
				referent = longAt((void *)((xArray + BaseHeaderSize) + (0U << (shiftForWord()))));
				while (/* isOopForwarded: */
					((!(referent & (tagMask()))))
				 && ((!((longAt((void *)(referent))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
					referent = longAt((void *)((referent + BaseHeaderSize) + (0U << (shiftForWord()))));
				}
				xArray = referent;

				/* begin splObj:put: */
				/* begin storePointer:ofObject:withValue: */
				assert(validStorePointerArgs(ExternalObjectsArray, GIV(specialObjectsOop), xArray));
				assert(isNonImmediate(GIV(specialObjectsOop)));
				if (oopisGreaterThanOrEqualTo(GIV(specialObjectsOop), GIV(oldSpaceStart))) {
					if (/* isYoung: */
						((!(xArray & (tagMask()))))
					 && (oopisLessThan(xArray, GIV(oldSpaceStart)))) {
						/* begin possibleRootStoreInto: */
						if (!((byteAt((void *)(GIV(specialObjectsOop) + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
							remember(GIV(specialObjectsOop));
						}
					}
				}

				/* most stores into young objects */
				longAtput((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(ExternalObjectsArray) << (shiftForWord()))))),xArray);
			}
			toDoLimit = ((/* begin numSlotsOf: */
	assert((classIndexOf(xArray)) > (isForwardedObjectClassIndexPun())),
(((numSlots = byteAt((void *)(xArray + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
			? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(xArray - BaseHeaderSize)))) << 8)))))) >> 8
			: numSlots))) - 1;
			for (ipdelta = 0; ipdelta <= toDoLimit; ipdelta += 1) {
				/* begin followSemaphoreIn:at: */
				obj = longAt((void *)((xArray + BaseHeaderSize) + ((((usqInt)(ipdelta) << (shiftForWord()))))));
				if ((!((longAt((void *)(obj))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
					/* begin followForwarded: */
					assert(isUnambiguouslyForwarder(obj));
					referent = longAt((void *)((obj + BaseHeaderSize) + (0U << (shiftForWord()))));
					while (/* isOopForwarded: */
						((!(referent & (tagMask()))))
					 && ((!((longAt((void *)(referent))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
						referent = longAt((void *)((referent + BaseHeaderSize) + (0U << (shiftForWord()))));
					}
					obj = referent;

					/* begin storePointer:ofObject:withValue: */
					assert(validStorePointerArgs(ipdelta, xArray, obj));
					assert(isNonImmediate(xArray));
					if (oopisGreaterThanOrEqualTo(xArray, GIV(oldSpaceStart))) {
						if (/* isYoung: */
							((!(obj & (tagMask()))))
						 && (oopisLessThan(obj, GIV(oldSpaceStart)))) {
							/* begin possibleRootStoreInto: */
							if (!((byteAt((void *)(xArray + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
								remember(xArray);
							}
						}
					}

					/* most stores into young objects */
					longAtput((void *)((xArray + BaseHeaderSize) + ((((usqInt)(ipdelta) << (shiftForWord()))))),obj);
				}
			}
		}
		if (((theBecomeEffectsFlags & (BecamePointerObjectFlag + BecameCompiledMethodFlag)) != 0)) {
			/* begin followForwardingPointersInProfileState */
			/* begin profileStateDoUpdating: */
			if (GIV(profileProcess)) {
				if ((result = ((!((longAt((void *)(GIV(profileProcess)))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))
							? followForwarded(GIV(profileProcess))
							: 0))) {
					GIV(profileProcess) = result;
				}
			}
			if (GIV(profileMethod)) {
				if ((result = ((!((longAt((void *)(GIV(profileMethod)))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))
							? followForwarded(GIV(profileMethod))
							: 0))) {
					GIV(profileMethod) = result;
				}
			}
			if (GIV(profileSemaphore)) {
				if ((result = ((!((longAt((void *)(GIV(profileSemaphore)))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))
							? followForwarded(GIV(profileSemaphore))
							: 0))) {
					GIV(profileSemaphore) = result;
				}
			}
			if (((theBecomeEffectsFlags & BecameCompiledMethodFlag) != 0)) {
				/* begin followForwardedMethodsInMethodCache */
				for (i = 0; i < MethodCacheSize; i += MethodCacheEntrySize) {
					c = GIV(methodCache)[i + MethodCacheClass];
					s = GIV(methodCache)[i + MethodCacheSelector];
					m = GIV(methodCache)[i + MethodCacheMethod];
					if ((c != 0)
					 && ((s != 0)
					 && ((m != 0)
					 && (/* isOopForwarded: */
						((!(m & (tagMask()))))
					 && ((!((longAt((void *)(m))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))))))) {
						/* begin followForwarded: */
						assert(isUnambiguouslyForwarder(m));
						referent = longAt((void *)((m + BaseHeaderSize) + (0U << (shiftForWord()))));
						while (/* isOopForwarded: */
							((!(referent & (tagMask()))))
						 && ((!((longAt((void *)(referent))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
							referent = longAt((void *)((referent + BaseHeaderSize) + (0U << (shiftForWord()))));
						}
						m = referent;
						GIV(methodCache)[i + MethodCacheMethod] = m;
					}
				}

				/* begin followMethodNewMethodAndInstructionPointer */
				if ((!((longAt((void *)(GIV(method)))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
					ipdelta = (/* method:includesAddress: */
							(GIV(instructionPointer) > GIV(method))
						 && (GIV(instructionPointer) < (GIV(method) + ((((numSlotsOfAny(GIV(method))) << (shiftForWord()))) + BaseHeaderSize)))
								? GIV(instructionPointer) - GIV(method)
								: 0);

					/* begin followForwarded: */
					assert(isUnambiguouslyForwarder(GIV(method)));
					referent = longAt((void *)((GIV(method) + BaseHeaderSize) + (0U << (shiftForWord()))));
					while (/* isOopForwarded: */
						((!(referent & (tagMask()))))
					 && ((!((longAt((void *)(referent))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
						referent = longAt((void *)((referent + BaseHeaderSize) + (0U << (shiftForWord()))));
					}
					GIV(method) = referent;
					if (ipdelta) {
						GIV(instructionPointer) = GIV(method) + ipdelta;
					}
				}
				if (/* isOopForwarded: */
					((!(GIV(newMethod) & (tagMask()))))
				 && ((!((longAt((void *)(GIV(newMethod)))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
					/* begin followForwarded: */
					assert(isUnambiguouslyForwarder(GIV(newMethod)));
					referent = longAt((void *)((GIV(newMethod) + BaseHeaderSize) + (0U << (shiftForWord()))));
					while (/* isOopForwarded: */
						((!(referent & (tagMask()))))
					 && ((!((longAt((void *)(referent))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
						referent = longAt((void *)((referent + BaseHeaderSize) + (0U << (shiftForWord()))));
					}
					GIV(newMethod) = referent;
				}
			}
		}
	}
	followForwardingPointersOfReceiversInStackZone();
}