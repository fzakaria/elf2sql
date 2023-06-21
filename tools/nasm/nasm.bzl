"""Rules for compiling assembly files using NASM https://www.nasm.us/"""

def _nasm_object_impl(ctx):
    src = ctx.file.src
    out = ctx.outputs.out
    args = ctx.actions.args()
    args.add(src.path)
    args.add("-f", "elf64")
    args.add("-o", out.path)

    args.add("-g")
    args.add("-F", "dwarf")

    ctx.actions.run(
        outputs = [out],
        inputs = [src],
        executable = ctx.executable._nasm,
        arguments = [args],
        mnemonic = "NasmObjectCompile",
        progress_message = "Compiling %s to create %s" % (src.path, out.path),
    )

nasm_object = rule(
    attrs = {
        "src": attr.label(
            allow_single_file = [".asm"],
            mandatory = True,
        ),
        "_nasm": attr.label(
            default = Label("//third_party:nasm-binary"),
            executable = True,
            cfg = "exec",
        ),
    },
    outputs = {"out": "%{name}.o"},
    implementation = _nasm_object_impl,
)

def nasm_binary(name, src):
    """Compiles a NASM object file and links it as an ELF64 executable"""
    nasm_object(
        name = "%s-object" % name,
        src = src,
    )

    out = name
    native.genrule(
        name = "%s-bin" % name,
        outs = [out],
        srcs = [":%s-object" % name],
        # cmd = "ld -z noseparate-code -s $< -o $@",
        cmd = "ld -z noseparate-code $< -o $@",
        executable = True,
        message = "Creating the binary %s" % out,
    )
