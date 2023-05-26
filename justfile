# Format c++ files
fmt:
    clang-format -i $(fd . src/ --extension cc --extension h)
    buildifier -r .
    buildifier --lint=fix -r .

lint:
    buildifier -r . --lint=warn