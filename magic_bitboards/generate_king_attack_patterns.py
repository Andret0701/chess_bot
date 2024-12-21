
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


def generate_king_attack_table():
    attack_table = [0] * 64
    for y in range(8):
        for x in range(8):
            attack_table[x + y * 8] = generate_king_attack_table_entry(x, y)
    return attack_table

def generate_king_attack_table_entry(x, y):
    attack = 0
    if x > 0:
        if y > 0:
            attack |= 1 << (x - 1 + (y - 1) * 8)
        attack |= 1 << (x - 1 + y * 8)
        if y < 7:
            attack |= 1 << (x - 1 + (y + 1) * 8)
    if y > 0:
        attack |= 1 << (x + (y - 1) * 8)
    if y < 7:
        attack |= 1 << (x + (y + 1) * 8)
    if x < 7:
        if y > 0:
            attack |= 1 << (x + 1 + (y - 1) * 8)
        attack |= 1 << (x + 1 + y * 8)
        if y < 7:
            attack |= 1 << (x + 1 + (y + 1) * 8)
    return attack

king_attack_table = generate_king_attack_table()

with open("magic_bitboards/king_attack_table.h", "w") as file:
    file.write(list_to_c_uint64_array("king_attack_table", king_attack_table) + "\n")