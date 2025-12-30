simple_map = {
    "+": "addition",
    "*": "multiplication",
    "/": "division",
    "==": "equal_to",
    "to_string": "to_string"
}

complex_map = {
    "subtraction": ["a", "-", "b"],
    "unary_minus": ["-", "a"],
}

single_operand_mms = [
    "unary_minus"
]

default_mms = {
    "addition": "a + b",
    "subtraction": "a - b",
    "multiplication": "a * b",
    "division": "a / b",
    "unary_minus": "-a",
    "equal_to": "a == b",
}


# Work out which metamethod from the string
def determine_method(method: str):
    # See if any simple criteria match up
    for simple_key in simple_map.keys():
        if simple_key in method:
            return simple_map[simple_key]
    
    # See if all criteria are met for each complex case
    for complex_key in complex_map.keys():
        failed = False
        for substr in complex_map[complex_key]:
            if substr not in method:
                failed = True
                break
        if not failed:
            return complex_key


def get_meta_method_rval(sol_mm_type: str, cpp_types: tuple[str, str]):
    lambda_def = f"[](const {cpp_types[0]} &a" +\
        f"{f", const {cpp_types[1]} &b" if sol_mm_type not in single_operand_mms else ""})"
    return lambda_def +\
        "{" +\
        f"return {default_mms[sol_mm_type]};" +\
        "}"