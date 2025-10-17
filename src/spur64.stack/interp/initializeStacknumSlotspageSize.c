/* Extracted from interp.c:26313 (function initializeStacknumSlotspageSize). */

/*	Initialize the stack pages. For testing I want stack addresses to be
	disjoint from
	normal memory addresses so stack addresses are negative. The first address
	is -pageSize bytes. So for example if there are 1024 bytes per page and 3
	pages then the pages are organized as
	
	byte address: -1024 <-> -2047 | -2048 <-> -3071 | -3072 <-> -4096 |
	page 3			page 2			page 1
	mem index: 769 <-> 513 | 512 <-> 257 | 256 <-> 1 |
	
	The byte address is the external address corresponding to a real address
	in the VM.
	mem index is the index in the memory Array holding the stack, an index
	internal to
	the stack pages. The first stack page allocated will be the last page in
	the array of pages
	at the highest effective address. Its base address be -1024 and grow down
	towards -2047. */
/*	The lFoo's are to get around the foo->variable scheme in the C call to
	allocStackPages below.
 */

	/* InterpreterStackPages>>#initializeStack:numSlots:pageSize: */

static NoDbgRegParms void
initializeStacknumSlotspageSize(char *theStackPages, sqInt stackSlots, sqInt slotsPerPage)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt count;
    sqInt index;
    sqInt numPages;
    StackPage *page;
    char *pageStructBase;
    usqIntptr_t structStackPageSize;
    sqInt theIndex;

	/* For initialization in the C code. */
	GIV(stackMemory) = theStackPages;
	structStackPageSize = sizeof(CogStackPage);
	GIV(bytesPerPage) = slotsPerPage * BytesPerWord;
	numPages = GIV(numStackPages);

	/* Because stack pages grow down baseAddress is at the top of a stack page and so to avoid
	   subtracting BytesPerWord from baseAddress and lastAddress in the init loop below we simply
	   push the stackPage array up one word to avoid the overlap.  This word is extraStackBytes. */
	pageStructBase = (theStackPages + (numPages * GIV(bytesPerPage))) + BytesPerWord;
	GIV(pages) = ((StackPage *) pageStructBase);

	/* Simulation only.  Since addresses are negative the offset is positive.  To make all
	   stack addresses negative we make the offset a page more than it needs to be so the
	   address of the last slot in memory (the highest address in the stack, or its start) is
	   - pageByteSize
	   and the address of the first slot (the lowest address, or its end) is
	   - pageByteSize * (numPages + 1) */

	/* make sure there's enough headroom */
	assert((((stackPageByteSize()) - (stackLimitBytes())) - (stackLimitOffset())) >= (stackPageHeadroom()));
	for (index = 0; index < numPages; index += 1) {
		/* begin stackPageAt: */
		page = stackPageAtpages(index, GIV(pages));
		(page->lastAddress = (char *)theStackPages + (index * GIV(bytesPerPage)));
		(page->baseAddress = ((page->lastAddress)) + GIV(bytesPerPage));
		(page->stackLimit = ((page->baseAddress)) - ((((stackPageFrameBytes()) < (((stackPageByteSize()) - (((FrameSlots + LargeContextSlots) + 1) * BytesPerWord)))) ? (stackPageFrameBytes()) : (((stackPageByteSize()) - (((FrameSlots + LargeContextSlots) + 1) * BytesPerWord))))));
		(page->realStackLimit = (page->stackLimit));
		(page->baseFP = 0);
		(page->nextPage = stackPageAt((index == (numPages - 1)
				? 0
				: index + 1)));
		(page->prevPage = stackPageAt((index
				? index - 1
				: numPages - 1)));
	}

	/* The overflow limit is the amount of stack to retain when moving frames from an overflowing
	   stack to reduce thrashing.  See stackOverflowOrEvent:mayContextSwitch: */

	/* begin stackPageAt: */
	page = stackPageAtpages(0, GIV(pages));
	GIV(overflowLimit) = ((((page->baseAddress)) - ((page->realStackLimit))) * 3) / 5;
	for (index = 0; index < numPages; index += 1) {
		/* begin stackPageAt: */
		page = stackPageAtpages(index, GIV(pages));
		assert((pageIndexFor((page->baseAddress))) == index);
		assert((pageIndexFor(((page->baseAddress)) - ((slotsPerPage - 1) * BytesPerWord))) == index);
		assert((stackPageFor((page->baseAddress))) == page);
		assert((stackPageFor((page->stackLimit))) == page);

		/* begin initializePageTraceToInvalid: */
		(page->trace = StackPageTraceInvalid);
	}
	GIV(mostRecentlyUsedPage) = stackPageAtpages(0, GIV(pages));
	page = GIV(mostRecentlyUsedPage);
	count = 0;
	do {
		count += 1;
		theIndex = pageIndexForstackMemorybytesPerPage((page->baseAddress), GIV(stackMemory), GIV(bytesPerPage));
		assert((stackPageAt(theIndex)) == page);
		assert((pageIndexFor((page->baseAddress))) == theIndex);
		assert((pageIndexFor((page->stackLimit))) == theIndex);
		assert((pageIndexFor(((page->lastAddress)) + BytesPerWord)) == theIndex);
	} while(((page = (page->nextPage))) != GIV(mostRecentlyUsedPage));
	assert(count == numPages);
	assert(pageListIsWellFormed());
}