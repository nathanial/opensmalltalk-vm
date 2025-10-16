/* Extracted from interp.c:54839 (function initStackPagesAndContinueIntowith). */

void
initStackPagesAndContinueIntowith(void (*continuation)(void *), void *argument)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt stackPageBytes;
    usqIntptr_t stackZoneBytes;
    void *theStackMemory;

	/* begin ensureInitializeStackZone */
	if (!GIV(pages)) {
		stackPageBytes = stackPageByteSize();
		stackZoneBytes = (GIV(numStackPages) * ((sizeof(CogStackPage)) + (stackPageByteSize()))) + BytesPerWord;
		theStackMemory = alloca(stackZoneBytes);
		memset(theStackMemory, 0, stackZoneBytes);
		initializeStacknumSlotspageSize(theStackMemory, stackZoneBytes / BytesPerWord, stackPageBytes / BytesPerWord);
	}
	continuation(argument);
}