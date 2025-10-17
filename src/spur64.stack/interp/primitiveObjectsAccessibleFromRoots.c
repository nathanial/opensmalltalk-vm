/* Extracted from interp.c:20049 (function primitiveObjectsAccessibleFromRoots). */

/*	This primitive is called from Squeak as...
	arrayOfRoots uniquelyAccessibleObjects */
/*	This primitive answers an array of the receiver and every object in its
	proper tree of subParts (ie, that is not refered to from anywhere else
	outside the tree).
 */
/*	This primitive could be used to implement the primitiveStoreImageSegment
	segment, thanks to a suggestion from Igor Stassenko. Currently it is
	used only to debug that primitive. */

	/* InterpreterPrimitives>>#primitiveObjectsAccessibleFromRoots */

static void
primitiveObjectsAccessibleFromRoots(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt arrayOfRoots;
    sqInt result;
    char *sp;

	arrayOfRoots = longAt(GIV(stackPointer));

	/* Essential type checks */
	if (!(/* isArray: */
			((!(arrayOfRoots & (tagMask()))))
		 && (((byteAt((void *)(arrayOfRoots + (formatFieldByteOffset())))) & (formatMask())) == (arrayFormat())))) {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
		return;
	}

	/* Must be indexable pointers */
	result = objectsAccessibleFromRoots(arrayOfRoots);
	if (((((result) & 7) == 1))
	 && (((result >> 3)) == PrimErrNoMemory)) {
		fullGC();
		arrayOfRoots = longAt(GIV(stackPointer));
		result = objectsAccessibleFromRoots(arrayOfRoots);
	}
	if ((((result) & 7) == 1)) {
		/* primitiveFailFor: */
		GIV(primFailCode) = (result >> 3);
	}
	else {
		/* begin methodReturnValue: */
		assert(!((failed())));
		longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),result);
		GIV(stackPointer) = sp;
	}
}