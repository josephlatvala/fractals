def iterate(c, z, past_z):
    try:
        z = z**2 + c

        if z in past_z:
            return True
        else:
            past_z.append(z)
            return iterate(c, z, past_z)
    except:
        return False

for i in range(-10, 10):
    line = ""
    for j in range(-20, 20):
        in_set = iterate(complex(j / 8, i / 8), 0, [])
        if in_set:
            line += '#'
        else:
            line += '.'
    print(line)
