/* Extracted from interp.c:45475 (function setHeapBasememoryLimitendOfMemory). */

/*	Set the dimensions of the heap, answering the start of oldSpace. edenBytes
	holds the desired ``size of eden''
	which is actually the total size of new space minus the reserve. edenBytes
	is then divided up between eden
	and the two survivor spaces, where each survivor space is a
	scavengerDenominator (one seventh) of the total. */
/*	Transcript
	cr; nextPutAll: 'heapBase: '; print: baseOfHeap; nextPut: $/; nextPutAll:
	baseOfHeap hex;
	nextPutAll: ' memLimit '; print: memLimit; nextPut: $/; nextPutAll:
	memLimit hex;
	nextPutAll: ' memEnd '; print: memEnd; nextPut: $/; nextPutAll: memEnd
	hex; cr; flush. */
/*	This is more than a little counter-intuitive. Eden must include
	interpreterAllocationReserveBytes. 
 */

	/* SpurMemoryManager>>#setHeapBase:memoryLimit:endOfMemory: */

static NoDbgRegParms NeverInline sqInt
setHeapBasememoryLimitendOfMemory(sqInt baseOfHeap, sqInt memLimit, sqInt memEnd)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt actualEdenBytes;
    sqInt oldStart;
    sqInt reserve;
    sqInt survivorBytes;
    sqInt totalBytes;

	reserve = interpreterAllocationReserveBytes();
	GIV(newSpaceStart) = baseOfHeap;
	GIV(oldSpaceStart) = (baseOfHeap + GIV(edenBytes)) + reserve;
	totalBytes = GIV(oldSpaceStart) - GIV(newSpaceStart);

	/* begin newSpaceStart:newSpaceBytes:survivorBytes: */
	survivorBytes = ((((GIV(oldSpaceStart) - GIV(newSpaceStart)) - reserve) / 7 /* scavengerDenominator */) & ~7);
	actualEdenBytes = (((totalBytes - survivorBytes) - survivorBytes) & ~7);
	assert((((totalBytes - actualEdenBytes) - survivorBytes) - survivorBytes) < (allocationUnit()));

	/* for tenuring we require older objects below younger objects.  since allocation
	   grows up this means that the survivor spaces must precede eden. */
	(GIV(pastSpace).start = GIV(newSpaceStart));
	(GIV(pastSpace).limit = GIV(newSpaceStart) + survivorBytes);
	(GIV(futureSpace).start = (GIV(pastSpace).limit));
	(GIV(futureSpace).limit = ((GIV(pastSpace).limit)) + survivorBytes);
	(GIV(eden).start = (GIV(futureSpace).limit));
	(GIV(eden).limit = GIV(newSpaceStart) + totalBytes);
	assert((((GIV(futureSpace)).limit)) <= (GIV(newSpaceStart) + totalBytes));
	assert((((((GIV(eden)).start)) % (allocationUnit())) + ((((GIV(eden)).limit)) % (allocationUnit()))) == 0);
	assert((((((GIV(pastSpace)).start)) % (allocationUnit())) + ((((GIV(pastSpace)).limit)) % (allocationUnit()))) == 0);
	assert((((((GIV(futureSpace)).start)) % (allocationUnit())) + ((((GIV(futureSpace)).limit)) % (allocationUnit()))) == 0);

	/* begin initFutureSpaceStart */
	oldStart = GIV(futureSurvivorStart);
	GIV(futureSurvivorStart) = (GIV(futureSpace).start);
	GIV(tenuringProportion) = 0.9;
	GIV(freeStart) = ((GIV(eden)).start);
	GIV(pastSpaceStart) = ((GIV(pastSpace)).start);
	GIV(freeOldSpaceStart) = memEnd;
	GIV(endOfMemory) = memLimit;
	return baseOfHeap;
}