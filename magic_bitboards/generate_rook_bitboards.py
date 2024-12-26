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
    # North direction
    for i in range(y + 1, 7):  # Changed to 7 to exclude edge
        attack |= position_to_uint64(x, i)
    
    # South direction
    for i in range(y - 1, 0, -1):  # Changed to stop at 0
        attack |= position_to_uint64(x, i)
    
    # East direction
    for i in range(x + 1, 7):  # Changed to 7 to exclude edge
        attack |= position_to_uint64(i, y)
    
    # West direction
    for i in range(x - 1, 0, -1):  # Changed to stop at 0
        attack |= position_to_uint64(i, y)
    
    return attack

def magic_to_c_struct(name, magic_numbers, shifts, masks, attack_tables):
    offsets = [0] * len(magic_numbers)
    for i in range(1, len(magic_numbers)):
        offsets[i] = offsets[i - 1] + len(attack_tables[i - 1])
    
    result = "const MagicNumber " + name + "[" + str(len(magic_numbers)) + "] = {\n"
    for i in range(len(magic_numbers)):
        result += "    {"
        result += uint64_to_hex_string(magic_numbers[i]) + ","
        result += str(shifts[i]) + ","
        result += uint64_to_hex_string(masks[i]) + ","
        result += str(offsets[i]) + "},\n"
    result = result[:-2]
    result += "};\n\n"
    
    result += "const uint64_t " + name + "_attack_tables[" + str(offsets[-1] + len(attack_tables[-1])) + "] = {\n"
    for i in range(len(attack_tables)):
        for j in range(len(attack_tables[i])):
            result += uint64_to_hex_string(attack_tables[i][j]) + ","
        result += "\n"
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

    # North direction
    for i in range(y + 1, 8):
        target_pos = position_to_uint64(x, i)
        if occupied & target_pos:
            attacks |= target_pos
            break
        attacks |= target_pos

    # South direction
    for i in range(y - 1, -1, -1):
        target_pos = position_to_uint64(x, i)
        if occupied & target_pos:
            attacks |= target_pos
            break
        attacks |= target_pos

    # East direction
    for i in range(x + 1, 8):
        target_pos = position_to_uint64(i, y)
        if occupied & target_pos:
            attacks |= target_pos
            break
        attacks |= target_pos

    # West direction
    for i in range(x - 1, -1, -1):
        target_pos = position_to_uint64(i, y)
        if occupied & target_pos:
            attacks |= target_pos
            break
        attacks |= target_pos

    return attacks


def get_random_magic_number():
    return random.getrandbits(64)


best_magic_numbers = [0] * 64
best_shifts = [50] * 64
sizes = [None] * 64
attack_tables = [[]] * 64

max_table_size = 17000
while True:
    for index in range(64):
        while True:
            attack = table[index]
            occupied_positions = possible_occupied_positions(attack)
            magic_number = get_random_magic_number()
            
            for shift in range(best_shifts[index], 63):
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
                        print("Sum of sizes:", sum(sizes))
                        print("Max size:", max(sizes))
                        print("Min shift:", min(best_shifts))

                        out = magic_to_c_struct("rook_magic_numbers", best_magic_numbers, best_shifts, table, attack_tables)
                        with open("magic_bitboards/rook_magic_numbers.h", "w") as f:
                            f.write(out)
                            f.flush()
                            f.close()
                    else:
                        print("Found magic number for index", index, " with size", sizes[index])
                        print("Magic number:", uint64_to_hex_string(magic_number))
                        print("Shift:", shift)
                    break
            if sizes[index] is not None:
                break