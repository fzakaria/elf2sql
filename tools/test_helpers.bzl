"""Helper macros for working with sh_test and other test rules"""

def test_return_code(name, ret_code, binary):
    """Run a binary and verify the return code

    Args:
        name: The name of the test
        ret_code: The expected return code integer
        binary: The binary label to run
    """
    native.sh_test(
        name = name,
        size = "small",
        srcs = ["//tools:test_return_code.sh"],
        # The following command runs my_program and verifies the return code
        args = [
            "$(rootpath %s)" % binary,
            str(ret_code),
        ],
        data = [
            binary,
        ],
    )
