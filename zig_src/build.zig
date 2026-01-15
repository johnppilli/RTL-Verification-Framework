const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    // Build as a static C library
    const lib = b.addStaticLibrary(.{
        .name = "counter_model",
        .root_source_file = b.path("counter_model.zig"),
        .target = target,
        .optimize = optimize,
    });

    // Allow linking from C/C++
    lib.bundle_compiler_rt = true;

    b.installArtifact(lib);
}
