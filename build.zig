const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const root_module = b.createModule(.{
        .root_source_file = b.path("zig/poc/vm_headers_test.zig"),
        .target = target,
        .optimize = optimize,
    });

    root_module.addIncludePath(b.path("platforms/Cross/vm"));
    root_module.addIncludePath(b.path("platforms/unix/vm"));
    root_module.addIncludePath(b.path("include"));
    root_module.addIncludePath(b.path("src/spur64.stack"));
    root_module.addIncludePath(b.path("scripts/clang_stubs"));
    root_module.addCMacro("LSB_FIRST", "1");

    const exe = b.addExecutable(.{
        .name = "vm-headers-test",
        .root_module = root_module,
    });

    b.installArtifact(exe);

    const run_cmd = b.addRunArtifact(exe);
    run_cmd.step.dependOn(b.getInstallStep());
    if (b.args) |args| {
        run_cmd.addArgs(args);
    }

    const run_step = b.step("run", "Run the Zig VM header proof of concept");
    run_step.dependOn(&run_cmd.step);
}
