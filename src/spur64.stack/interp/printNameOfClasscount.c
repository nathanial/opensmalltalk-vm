/* Extracted from interp.c:60311 (function printNameOfClasscount). */

/*	Details: The count argument is used to avoid a possible infinite recursion
	if classOop is a corrupted object.
 */

	/* StackInterpreter>>#printNameOfClass:count: */

static NoDbgRegParms void
printNameOfClasscount(sqInt classOop, sqInt cnt)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    usqInt numSlots;

	if (!GIV(classNameIndex)) {
		print("??nil cnidx??");
		return;
	}
	if ((!classOop)
	 || ((classOop == 0)
	 || (cnt <= 0))) {
		print("bad class");
		return;
	}
	numSlots = numSlotsOf(classOop);
	if ((numSlots == GIV(metaclassNumSlots))
	 && (GIV(metaclassNumSlots) > GIV(thisClassIndex))) {
		printNameOfClasscount(longAt((void *)((classOop + BaseHeaderSize) + ((((usqInt)(GIV(thisClassIndex)) << (shiftForWord())))))), cnt - 1);
		print(" class");
	}
	else {
		if (numSlots <= GIV(classNameIndex)) {
			print("bad class");
		}
		else {
			printStringOf(longAt((void *)((classOop + BaseHeaderSize) + ((((usqInt)(GIV(classNameIndex)) << (shiftForWord())))))));
		}
	}
}