# minez_helper.py
# Copyright (c) 2026 Dooblix
# Licensed under the MIT license. See LICENSE file for details.

import sys
import argparse

def minez_print(string: str, reg: int, clear_garbage: bool) -> str:
    if not string:
        raise ValueError("String cannot be empty")
    _curr_val = ord(string[0])
    result_parts = [f">{reg}"]
    if clear_garbage:
        result_parts.append("x")
    result_parts.append(f"+{_curr_val}#")
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

def minez_list(data: list[int], reg: int, clear_garbage) -> str:
    if not data:
        raise ValueError("List cannot be empty")
    result_parts = [f">{reg}"]
    for val in data:
        if clear_garbage:
            result_parts.append("x")
        if val < 0:
            result_parts.append(f"{val}")
        elif val > 0:
            result_parts.append(f"+{val}" if val > 1 else "+")
        result_parts.append(">")
    return "".join(result_parts[:-1])

def main(argv=None) -> None:
    parser = argparse.ArgumentParser(
        description="minez_helper — generates repetetive Minez code snippets automatically (e.g. printing out strings or initializing lists)"
    )

    parser.add_argument(
        "type",
        choices=["string", "list"],
        help="What should minez_helper generate? ('string' oder 'list')"
    )
    parser.add_argument(
        "data",
        nargs="?",
        help='String to be printet (in "") or list elements (in "", seperated by  \',\').'
    )
    parser.add_argument(
        "--as-list",
        action="store_true",
        help="Generates Minez snippet that stores the given string as a list of ASCII values (only usable with type=string)."
    )
    parser.add_argument(
        "--clear-garbage",
        action="store_true",
        help="Clears used registers using 'x'."
    )
    parser.add_argument(
        "--reg",
        type=int,
        default=1,
        help="String: printing host, List: start register (default: 1)"
    )
    args = parser.parse_args(argv)
    if args.type == "string":
        decoded = bytes(args.data, "utf-8").decode("unicode_escape")
        if args.as_list:
            data_list = [ord(ch) for ch in decoded]
            print(minez_list(data_list, args.reg, args.clear_garbage))
        else:
            print(minez_print(decoded, args.reg, args.clear_garbage))
    elif args.type == "list":
        if args.data is None:
            print("Error: For type 'list', you must provide a list of integers.")
            return
        raw_items = [x.strip() for x in args.data.split(",") if x.strip() != ""]
        data_list = []
        for i, item in enumerate(raw_items, start=1):
            try:
                data_list.append(int(item))
            except ValueError:
                print(f"ValueError: Invalid element at position {i}: '{item}' is not an integer.")
                sys.exit(1)
        print(minez_list(data_list, args.reg, args.clear_garbage))
    
if __name__ == "__main__":
    main()