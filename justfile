# Format c++ files
fmt:
    buildifier -r .
    buildifier --lint=fix -r .
    run-clang-tidy $(pwd)/src -fix -quiet
    clang-format -i $(fd . src/ --extension cc --extension h)

lint:
    clang-format --dry-run --Werror $(fd . src/ --extension cc --extension h)
    buildifier --lint=warn -r .
    run-clang-tidy $(pwd)/src