/* Extracted from interp.c:27068 (function hackSlimBridgeToat). */

static NoDbgRegParms void
hackSlimBridgeToat(sqInt objOop, sqInt startAddress)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	assert(oopisLessThan(startAddress, objOop));
	assert(oopisLessThanOrEqualTo(objOop, GIV(oldSpaceStart)));
	assert(((startAddress + (allocationUnit())) == objOop)
	 || (((startAddress + (allocationUnit())) + (allocationUnit())) == objOop));
	longAtput((void *)(startAddress),((startAddress + 8 /* allocationUnit */) == objOop
			? ((sqInt)((usqInt)((numSlotsMask())) << (numSlotsFullShift())))
			: ((((usqInt)((numSlotsMask())) << (numSlotsFullShift())))) + 1));
}