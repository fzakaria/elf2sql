# Examples

The following directory contains interesting ELF examples that can be used for
the conversion to SQL (and back) -- most should come with tests validating
they continue to work.

Note: That the conversion back to ELF does not guarantee the same bit placement
and may choose to omit sections that are not relevant for running an executable
such as `.comment` sections.

# Tiny

This example is part-way through the example outlined in this [blog post](https://www.muppetlabs.com/~breadbox/software/tiny/teensy.html).
The executable uses the [nasm](https://www.nasm.org/) assembler to create an executable
which returns a fixed value and avoids the standard C library.

```console
bazel run //src/examples:tiny
```