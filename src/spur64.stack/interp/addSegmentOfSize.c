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
		assert(segAddressSqInt >= (segLimit(&GIV(segments)[0])));
		if (GIV(numSegments) == GIV(numSegInfos)) {
			allocateOrExtendSegmentInfos();
		}
		assert(GIV(numSegments) < GIV(numSegInfos));
		segIndex = (lastSegIndex = GIV(numSegments) - 1);
		GIV(numSegments) += 1;
		while (1) {
			if (segAddressSqInt >= ((((GIV(segments)[segIndex]).segSize)) + (((GIV(segments)[segIndex]).segStart)))) {
				segIndex += 1;
				for (idx = lastSegIndex; idx >= segIndex; idx += -1) {
					GIV(segments)[idx + 1] = (GIV(segments)[idx]);
				}
				newSegIndex = segIndex;
				goto l1;
			}
			segIndex -= 1;
		}
		/* end insertSegmentFor: */
l1:

		/* Simulation insertion code duplicates entries if newSegIndex ~= numSegments - 1 */
		newSeg = (&(GIV(segments)[newSegIndex]));
		(newSeg->segStart = ((usqIntptr_t)segAddress));
		(newSeg->segSize = allocatedSize);
		(newSeg->swizzle = 0);
		assert(!(segmentOverlap()));
		bridgeFromto((&(GIV(segments)[newSegIndex - 1])), newSeg);
		bridgeFromto(newSeg, (!(newSegIndex == (GIV(numSegments) - 1))
				? (&(GIV(segments)[newSegIndex + 1]))
				: 0));
		GIV(totalHeapSizeIncludingBridges) += allocatedSize;

		/* test isInMemory: */
		for (i = 0; i < GIV(numSegments); i += 1) {
			assert(isInSegments(((GIV(segments)[i]).segStart)));
			assert(isInSegments((segLimit(&GIV(segments)[i])) - BytesPerWord));
			assert((!(isInSegments(segLimit(&GIV(segments)[i]))))
			 || ((i < (GIV(numSegments) - 1))
			 && ((segLimit(&GIV(segments)[i])) == (((GIV(segments)[i + 1]).segStart)))));
			assert((!(isInSegments((((GIV(segments)[i]).segStart)) - BytesPerWord)))
			 || ((i > 0)
			 && ((segLimit(&GIV(segments)[i - 1])) == (((GIV(segments)[i]).segStart)))));
		}
		return newSeg;
	}

	/* sent to the manager so that the simulator can increase memory to simulate a new segment */
	return null;
}