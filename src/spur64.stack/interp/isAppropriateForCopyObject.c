/* Extracted from interp.c:66158 (function isAppropriateForCopyObject). */

static NoDbgRegParms sqInt
isAppropriateForCopyObject(sqInt oop)
{
	if (!(((byteAt((void *)(oop + (formatFieldByteOffset())))) & (formatMask())) <= 5 /* lastPointerFormat */)) {
		return 0;
	}
	if (((longAt((void *)(oop))) & (classIndexMask())) == ClassMethodContextCompactIndex) {
		return !(/* isStillMarriedContext: */
			(((((longAt((void *)((oop + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord())))))))) & 7) == 1))
		 && (!(isWidowedContext(oop))));
	}

	/* Note there is no version in CoInterpreterPrimitives such as
	   (objectMemory isCompiledMethod: oop) ifTrue:
	   [^(self methodHasCogMethod: oop) not].
	   because isPointersNonImm: excludes compiled methods and the
	   copy loop in primitiveCopyObject cannot handle compiled methods. */
	return 1;
}