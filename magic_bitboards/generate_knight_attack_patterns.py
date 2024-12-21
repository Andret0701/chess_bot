
def uint64_to_hex_string(uint64):
    if uint64 == 0:
        return "0x0"
    
    return "0x{:016x}".format(uint64)

def list_to_c_uint64_array(name, list):
    result = "const uint64_t " + name + "[" + str(len(list)) + "] = {"
    for i in range(len(list)):
        result += uint64_to_hex_string(list[i]) + ","
    result = result[:-1]
    result += "};"
    
    return result

def generate_knight_attack_table():
    attack_table = [0] * 64
    for y in range(8):
        for x in range(8):
            attack_table[x + y * 8] = generate_knight_attack_table_entry(x, y)
    return attack_table

def generate_knight_attack_table_entry(x, y):
    attack = 0
    if x >= 2:
        if y >= 1:
            attack |= 1 << (x - 2 + (y - 1) * 8)
        if y <= 6:
            attack |= 1 << (x - 2 + (y + 1) * 8)
    if x <= 5:
        if y >= 1:
            attack |= 1 << (x + 2 + (y - 1) * 8)
        if y <= 6:
            attack |= 1 << (x + 2 + (y + 1) * 8)
    if y >= 2:
        if x >= 1:
            attack |= 1 << (x - 1 + (y - 2) * 8)
        if x <= 6:
            attack |= 1 << (x + 1 + (y - 2) * 8)
    if y <= 5:
        if x >= 1:
            attack |= 1 << (x - 1 + (y + 2) * 8)
        if x <= 6:
            attack |= 1 << (x + 1 + (y + 2) * 8)
    return attack

knight_attack_table = generate_knight_attack_table()

with open("magic_bitboards/knight_attack_table.h", "w") as file:
    file.write(list_to_c_uint64_array("knight_attack_table", knight_attack_table) + "\n")