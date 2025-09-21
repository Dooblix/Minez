minez_chars = ['>', '<', '+', '-', '[', ']', '.', ':', '#', '!', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '{', '}', '(', ')', 'd', 's', 'x', 'X', '^', '@', '_', '|']

result = ""
for char in minez_chars:
    result += f"curr_ch == '{char}' || "

print(result)