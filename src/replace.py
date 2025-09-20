with open(r"C:\Users\Tim\Desktop\Self-Made Langs\Minez C\src\test.txt", "r") as f:
    text = f.read().replace("\n", "")

with open(r"C:\Users\Tim\Desktop\Self-Made Langs\Minez C\src\test.txt", "w") as f:
    f.write(text)