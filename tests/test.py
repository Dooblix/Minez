def divide(a, b):
    if b == 0:
        raise ZeroDivisionError
    
    remainder = a
    quotient = 0

    while True:
        if remainder < b:
            break
        remainder -= b
        quotient += 1

    return (quotient, remainder)

print(divide(13, 3)) 