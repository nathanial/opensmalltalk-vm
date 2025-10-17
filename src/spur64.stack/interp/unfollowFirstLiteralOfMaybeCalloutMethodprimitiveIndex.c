/* Extracted from interp.c:65409 (function
 * unfollowFirstLiteralOfMaybeCalloutMethodprimitiveIndex). */

/*	Follow the first literal of either a primitiveCallout or
        primitiveExternalCall primitive method. This
        will be an ExternalFunction for primitiveCallout or a four element Array
        for primitiveExternalCall.
        This is here to avoid following all the literals in a method, which
   would be slow. Remember forwarders are unlikely, so we only want to follow
   what is necessary, and for an FFI call or external primitive only the first
   literal is salient. */

/* StackInterpreter>>#unfollowFirstLiteralOfMaybeCalloutMethod:primitiveIndex:
 */

static sqInt
unfollowFirstLiteralOfMaybeCalloutMethodprimitiveIndex(sqInt methodObj,
                                                       sqInt primIndex) {
  sqInt firstLiteral;
  sqInt found;

  found = 0;

  /* inlined self literal: 0 ofMethod: methodObj for clarity... */
  firstLiteral =
      longAt((void *)((methodObj + BaseHeaderSize) +
                      ((((usqInt)(LiteralStart) << (shiftForWord()))))));
  if (/* isOopForwarded: */
      ((!(firstLiteral & (tagMask())))) &&
      ((!((longAt((void *)(firstLiteral))) &
          ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
    found = 1;
    firstLiteral = fixFollowedFieldofObjectwithInitialValue(
        LiteralStart, methodObj, firstLiteral);
  }
  if (followForwardedObjectFieldstoDepth(
          firstLiteral,
          (primIndex == PrimNumberFFICall
               ? (primitiveAccessorDepthTable[PrimNumberFFICall]) - 1
               : 0))) {
    found = 1;
  }
  return found;
}