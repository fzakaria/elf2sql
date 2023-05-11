# Development & Contributing

This repository is built using [Bazel](https://bazel.build/). A minimal
comprehension of Bazel is necessary in order to develop and contribute to the
codebase.

## Building

You can easily build everything with the wide target matching pattern rule.

> This repository uses [BuildBuddy](https://www.buildbuddy.io/) to provide
> remote executions and caching. Ideally, the targets should be cached and
> builds should be pretty quick.

```console
bazel build //...
```