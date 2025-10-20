const std = @import("std");

const c = @cImport({
    @cInclude("sq.h");
    @cInclude("sqVirtualMachine.h");
});

pub fn main() void {
    std.debug.print(
        "VM proxy version detected by Zig: {d}.{d}\n",
        .{ c.VM_PROXY_MAJOR, c.VM_PROXY_MINOR },
    );
}
