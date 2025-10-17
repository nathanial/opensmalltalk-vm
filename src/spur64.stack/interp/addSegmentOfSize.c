/* Extracted from interp.c:48961 (function addSegmentOfSize). */

	/* SpurSegmentManager>>#addSegmentOfSize: */

static NoDbgRegParms SpurSegmentInfo *
addSegmentOfSize(sqInt ammount)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    usqInt allocatedSize;
    sqInt i;
    sqInt idx;
    sqInt lastSegIndex;
    SpurSegmentInfo *newSeg;
    sqInt newSegIndex;
    void *segAddress;
    sqInt segAddressSqInt;
    sqInt segIndex;

	newSegIndex = 0;
	allocatedSize = 0;
	if ((segAddress = sqAllocateMemorySegmentOfSizeAboveAllocatedSizeInto(ammount, firstGapOfSizeAtLeast(ammount), (&allocatedSize)))) {
		segAddressSqInt = ((usqIntptr_t)segAddress);

		/* begin insertSegmentFor: */
		assert(segAddressSqInt >= (segLimit(&segments[0])));
		if (numSegments == numSegInfos) {
			allocateOrExtendSegmentInfos();
		}
		assert(numSegments < numSegInfos);
		segIndex = (lastSegIndex = numSegments - 1);
		numSegments += 1;
		while (1) {
			if (segAddressSqInt >= ((((segments[segIndex]).segSize)) + (((segments[segIndex]).segStart)))) {
				segIndex += 1;
				for (idx = lastSegIndex; idx >= segIndex; idx += -1) {
					segments[idx + 1] = (segments[idx]);
				}
				newSegIndex = segIndex;
				goto l1;
			}
			segIndex -= 1;
		}
		/* end insertSegmentFor: */
l1:

		/* Simulation insertion code duplicates entries if newSegIndex ~= numSegments - 1 */
		newSeg = (&(segments[newSegIndex]));
		(newSeg->segStart = ((usqIntptr_t)segAddress));
		(newSeg->segSize = allocatedSize);
		(newSeg->swizzle = 0);
		assert(!(segmentOverlap()));
		bridgeFromto((&(segments[newSegIndex - 1])), newSeg);
		bridgeFromto(newSeg, (!(newSegIndex == (numSegments - 1))
				? (&(segments[newSegIndex + 1]))
				: 0));
		totalHeapSizeIncludingBridges += allocatedSize;

		/* test isInMemory: */
		for (i = 0; i < numSegments; i += 1) {
			assert(isInSegments(((segments[i]).segStart)));
			assert(isInSegments((segLimit(&segments[i])) - BytesPerWord));
			assert((!(isInSegments(segLimit(&segments[i]))))
			 || ((i < (numSegments - 1))
			 && ((segLimit(&segments[i])) == (((segments[i + 1]).segStart)))));
			assert((!(isInSegments((((segments[i]).segStart)) - BytesPerWord)))
			 || ((i > 0)
			 && ((segLimit(&segments[i - 1])) == (((segments[i]).segStart)))));
		}
		return newSeg;
	}

	/* sent to the manager so that the simulator can increase memory to simulate a new segment */
	return null;
}