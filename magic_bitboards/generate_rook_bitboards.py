import random

def position_to_uint64(x, y):
    return 1 << (x + y * 8)

def print_bitboard(bitboard):
    for y in range(8):
        for x in range(8):
            print((bitboard >> (x + y * 8)) & 1, end="")
        print()
        
def uint64_to_hex_string(uint64):
    if uint64 == 0:
        return "0x0"
    
    return "0x{:016x}".format(uint64)

def generate_rook_attack_table():
    attack_table = [0] * 64
    for y in range(8):
        for x in range(8):
            attack_table[x + y * 8] = generate_rook_attack_table_entry(x, y)
    return attack_table

def generate_rook_attack_table_entry(x, y):
    attack = 0
    for i in range(x + 1, 8):
        if (i, y) == (7, y):
            break
        attack |= position_to_uint64(i, y)
    for i in range(x - 1, -1, -1):
        if (i, y) == (0, y):
            break
        attack |= position_to_uint64(i, y)
    for i in range(y + 1, 8):
        if (x, i) == (x, 7):
            break
        attack |= position_to_uint64(x, i)
    for i in range(y - 1, -1, -1):
        if (x, i) == (x, 0):
            break
        attack |= position_to_uint64(x, i)
    return attack

def list_to_c_uint64_array(name, list):
    result = "const uint64_t " + name + "[" + str(len(list)) + "] = {"
    for i in range(len(list)):
        result +=uint64_to_hex_string(list[i]) + ","
    result = result[:-1]
    result += "};"
    
    return result

def list_to_c_uint8_array(name, list):
    result = "const uint8_t " + name + "[" + str(len(list)) + "] = {"
    for i in range(len(list)):
        result +=str(list[i]) + ","
    result = result[:-1]
    result += "};"
    
    return result

def table_to_c_uint64_array(name, table):
    max_size= max([len(i) for i in table])
    result = "const uint64_t " + name + "[64][" + str(max_size) + "] = {\n"
    for i in range(len(table)):
        result += "{"
        for j in range(len(table[i])):
            result += uint64_to_hex_string(table[i][j]) + ","
        for j in range(len(table[i]), max_size):
            result += "0x0,"
        result = result[:-1]
        result += "},\n"
    result = result[:-2]
    result += "};"
    
    return result


def possible_occupied_positions(mask):
    positions = [mask]
    for i in range(64):
        if mask & (1 << i):
            old_positions = positions.copy()
            for pos in old_positions:
                positions.append(pos & ~(1 << i))
    return positions

def occupied_position_to_attack_pattern(x, y, occupied):
    attacks = 0

    # Loop to check moves in the N direction
    for i in range(y + 1, 8):
        target_pos = position_to_uint64(x, i)
        if occupied & target_pos:
            attacks |= target_pos
            break
        attacks |= target_pos

    # Loop to check moves in the S direction
    for i in range(y - 1, -1, -1):
        target_pos = position_to_uint64(x, i)
        if occupied & target_pos:
            attacks |= target_pos
            break
        attacks |= target_pos

    # Loop to check moves in the E direction
    for i in range(x + 1, 8):
        target_pos = position_to_uint64(i, y)
        if occupied & target_pos:
            attacks |= target_pos
            break
        attacks |= target_pos

    # Loop to check moves in the W direction
    for i in range(x - 1, -1, -1):
        target_pos = position_to_uint64(i, y)
        if occupied & target_pos:
            attacks |= target_pos
            break
        attacks |= target_pos

    return attacks
    


def get_random_magic_number():
    return random.getrandbits(64)

table = generate_rook_attack_table()
    

best_magic_numbers = [0] * 64
best_shifts = [0] * 64
sizes = [None] * 64
attack_tables = [[]] * 64


max_table_size = 17000
while True:
    # Find the index with the biggest size or one with None
    if None in sizes:
        index = sizes.index(None)
    else:
        index = max(range(64), key=lambda i: sizes[i] if sizes[i] is not None else -1)
        
    
    attack = table[index]
    occupied_positions = possible_occupied_positions(attack)
    magic_number = get_random_magic_number()
    for shift in range(50, 64):
        indices = [(((occupied_positions[i] * magic_number) & 0xFFFFFFFFFFFFFFFF) >> shift) for i in range(len(occupied_positions))]
        max_index = max(indices)
        if (sizes[index] is not None and max_index >= sizes[index]) or max_index >= max_table_size:
            continue
        occupied_table = [None] * (max_index + 1)

        found_magic_number = True
        for i in range(len(indices)):
            if occupied_table[indices[i]] is None:
                occupied_table[indices[i]] = occupied_position_to_attack_pattern(index % 8, index // 8, occupied_positions[i])
            elif occupied_table[indices[i]] != occupied_position_to_attack_pattern(index % 8, index // 8, occupied_positions[i]):
                found_magic_number = False
                break

        if found_magic_number:
            best_magic_numbers[index] = magic_number
            best_shifts[index] = shift
            sizes[index] = max_index
            attack_tables[index] = [i if i is not None else 0 for i in occupied_table]

            have_all_magic_numbers = all(size is not None for size in sizes)
            if have_all_magic_numbers:
                print("Found all magic numbers")
                print("Sum of sizes:", max(sizes) * 64)
                print("Max size:", max(sizes))
                print("Min shift:", min(best_shifts))

                out = list_to_c_uint64_array("rook_magic_numbers", best_magic_numbers) + "\n"
                out += list_to_c_uint64_array("rook_attack_masks", table) + "\n"
                out += list_to_c_uint8_array("rook_magic_shifts", best_shifts) + "\n"
                out += table_to_c_uint64_array("rook_attack_tables", attack_tables)
                with open("magic_bitboards/rook_magic_numbers.h", "w") as f:
                    f.write(out)
                    f.flush()
                    f.close()
            else:
                print("Found magic number for index", index, " with size", sizes[index])
                print("Magic number:", uint64_to_hex_string(magic_number))
                print("Shift:", shift)
            break
