# Format c++ files
fmt:
    clang-format -i $(fd . src/ --extension cc --extension h)