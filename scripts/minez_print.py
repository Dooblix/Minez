# minez_print.py
# Copyright (c) Dooblix 2025
# Licensed under the MIT license. See LICENSE file for details.

import sys

def minez_print(string: str, reg: int) -> str:
    if not string:
        raise ValueError("String cannot be empty")
    string = string.replace("\\n", "\n").replace("\\t", "\t")
    _curr_val = ord(string[0])
    result_parts = [f">{reg}x+{_curr_val}#"]
    for ch in string[1:]:
        _next_val = ord(ch)
        if _curr_val < _next_val:
            result_parts.append(f"+{_next_val - _curr_val}#")
        elif _curr_val > _next_val:
            result_parts.append(f"-{_curr_val - _next_val}#")
        else:
            result_parts.append("#")
        _curr_val = _next_val
    return "".join(result_parts)

def main() -> None:
    msg: str = input("String to be printed (Use \\n and \\t for newline and tab): ")
    try:
        reg: int = int(input("Register to be used (int): "))
    except ValueError:
        print("Please input an integer!")
        sys.exit(1)
    try:
        print(minez_print(msg, reg))
    except ValueError as e:
        print(e)
        sys.exit(1)

if __name__ == "__main__":
    main()
