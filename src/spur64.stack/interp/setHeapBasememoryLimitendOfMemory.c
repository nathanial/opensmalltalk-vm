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
{
    sqInt actualEdenBytes;
    sqInt oldStart;
    sqInt reserve;
    sqInt survivorBytes;
    sqInt totalBytes;

	reserve = interpreterAllocationReserveBytes();
	newSpaceStart = baseOfHeap;
	oldSpaceStart = (baseOfHeap + edenBytes) + reserve;
	totalBytes = oldSpaceStart - newSpaceStart;

	/* begin newSpaceStart:newSpaceBytes:survivorBytes: */
	survivorBytes = ((((oldSpaceStart - newSpaceStart) - reserve) / 7 /* scavengerDenominator */) & ~7);
	actualEdenBytes = (((totalBytes - survivorBytes) - survivorBytes) & ~7);
	assert((((totalBytes - actualEdenBytes) - survivorBytes) - survivorBytes) < (allocationUnit()));

	/* for tenuring we require older objects below younger objects.  since allocation
	   grows up this means that the survivor spaces must precede eden. */
	(pastSpace.start = newSpaceStart);
	(pastSpace.limit = newSpaceStart + survivorBytes);
	(futureSpace.start = (pastSpace.limit));
	(futureSpace.limit = ((pastSpace.limit)) + survivorBytes);
	(eden.start = (futureSpace.limit));
	(eden.limit = newSpaceStart + totalBytes);
	assert((((futureSpace).limit)) <= (newSpaceStart + totalBytes));
	assert((((((eden).start)) % (allocationUnit())) + ((((eden).limit)) % (allocationUnit()))) == 0);
	assert((((((pastSpace).start)) % (allocationUnit())) + ((((pastSpace).limit)) % (allocationUnit()))) == 0);
	assert((((((futureSpace).start)) % (allocationUnit())) + ((((futureSpace).limit)) % (allocationUnit()))) == 0);

	/* begin initFutureSpaceStart */
	oldStart = futureSurvivorStart;
	futureSurvivorStart = (futureSpace.start);
	tenuringProportion = 0.9;
	freeStart = ((eden).start);
	pastSpaceStart = ((pastSpace).start);
	freeOldSpaceStart = memEnd;
	endOfMemory = memLimit;
	return baseOfHeap;
}