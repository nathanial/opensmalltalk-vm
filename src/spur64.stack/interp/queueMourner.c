/* Extracted from interp.c:44886 (function queueMourner). */

static NoDbgRegParms void
queueMourner(sqInt anEphemeronOrWeakArray)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt format;
    sqInt objStack;

	assert((isNonImmediate(anEphemeronOrWeakArray))
	 && (((formatOf(anEphemeronOrWeakArray)) == (ephemeronFormat()))
	 || ((formatOf(anEphemeronOrWeakArray)) == (weakArrayFormat()))));
	assert(!((((formatOf(anEphemeronOrWeakArray)) == (ephemeronFormat()))
 && (isonObjStack(anEphemeronOrWeakArray, GIV(mournQueue))))));
	ensureRoomOnObjStackAt(MournQueueRootIndex);

	/* There is no point queueing weak arrays more than once.  Note that it should be impossible
	   for ephemerons to be enqueued more than once since they are turned into non-ephemerons
	   in the relevant sender.  Alas this means that other ephemerons on the same object not
	   identified in this scavenge or GC will not fire until later.  But that's life. */

	/* begin isEphemeron: */
	assert(isNonImmediate(anEphemeronOrWeakArray));
	format = (byteAt((void *)(anEphemeronOrWeakArray + (formatFieldByteOffset())))) & (formatMask());
	if (!(format == (ephemeronFormat()))) {
		if (isonObjStack(anEphemeronOrWeakArray, GIV(mournQueue))) {
			return;
		}
	}
	objStack = GIV(mournQueue);

	/* begin push:onObjStack: */
	assert(addressCouldBeOop(anEphemeronOrWeakArray));
	if (((anEphemeronOrWeakArray & (tagMask())) != 0)) {
		assert(objStack == GIV(markStack));
		assert(addressCouldBeObj(topOfObjStack((0 == (fetchPointerofObject(ObjStackTopx, objStack))
				? fetchPointerofObject(ObjStackNextx, objStack)
				: objStack))));
	}
	else {
		assert(!((objStack == GIV(markStack))
		 && (isWeakNonImm(anEphemeronOrWeakArray))));

		/* There should only be weaklings on the weaklingStack */
		assert((objStack != GIV(weaklingStack))
		 || (isWeakNonImm(anEphemeronOrWeakArray)));
	}

	/* There should be no weaklings on the mark stack. */
	noCheckPushonObjStack(anEphemeronOrWeakArray, objStack);
}