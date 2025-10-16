#include <stdint.h>

#if defined(__aarch64__)

void callAndReturnWithStructAddr(void *structAddr, void *procAddr, void *arrayAddr) {
    uint64_t zeroRegs[8] = {0};
    uint64_t *regs = arrayAddr ? (uint64_t *)arrayAddr : zeroRegs;

    __asm__ volatile(
        "ldp x0, x1, [%[regs]]\n\t"
        "ldp x2, x3, [%[regs], #16]\n\t"
        "ldp x4, x5, [%[regs], #32]\n\t"
        "ldp x6, x7, [%[regs], #48]\n\t"
        "mov x8, %[sret]\n\t"
        "blr %[fn]\n\t"
        :
        : [regs] "r"(regs), [sret] "r"(structAddr), [fn] "r"(procAddr)
        : "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7",
          "x8", "x9", "x10", "x11", "x12", "x13", "x14", "x15",
          "x16", "x17", "x18", "memory", "cc");
}

#endif /* __aarch64__ */
