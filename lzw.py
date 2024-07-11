# Trying again lol

import math

# generates a lzw encoding
def lzw_encode_gencode(table, maxvalue, raw, current):
    win = bytearray()

    win.append(raw[current])
    value = -1

    try:
        while(bytes(win) in table):
            value = table[bytes(win)]
            current = current + 1
            win.append(raw[current])

        if(maxvalue < 4096):
            table[bytes(win)] = maxvalue
            maxvalue = maxvalue + 1

    except IndexError:
        pass

    return value, maxvalue, current


# Assumption being that table is already filled up with the correct values
def lzw_encode(table, raw):
    codes = []
    current = 0
    maxvalue = len(table)

    while current < len(raw):
        code, maxvalue, current = lzw_encode_gencode(table, maxvalue, raw, current)
        codes.append(code)

    i = 0

    return codes

# encoding and packing the values
def lzw_encode_pack(table, raw, minsiz):
    codes = 0
    pos = 0
    current = 0
    maxvalue = len(table)

    while current < len(raw):
        code, maxvalue, current = lzw_encode_gencode(table, maxvalue, raw, current)
        codes |= code << pos

        pos += minsiz

        if maxvalue == (1 << minsiz) + 1:
            minsiz += 1

    return codes, math.ceil(pos / 8)

# decode it bois
def lzw_decode(table, cmprs):

    uncmprs = bytearray()

    prev= 0
    uncmprs += table[cmprs[prev]]

    for i in range(1, len(cmprs)):
        if cmprs[i] in table:
            uncmprs += table[cmprs[i]]
            table[len(table)] = table[cmprs[prev]] + table[cmprs[i]][0].to_bytes(length = 1)
            prev = i
        else:
            table[len(table)] = table[cmprs[prev]] + table[cmprs[prev]][0].to_bytes(length = 1)
            uncmprs += table[cmprs[i]]
            prev = i

    return bytes(uncmprs)

# decode it bois
def lzw_unpack_decode(table, cmprs, minsiz, terminal):

    uncmprs = bytearray()

    prev = 0
    code = cmprs & ((1 << minsiz) - 1)
    cmprs >>= minsiz

    uncmprs += table[code]
    prev = code

    while True:
        #print(prev)
        if(len(table) == (1 << minsiz) - 1):
            minsiz = minsiz + 1

        code = cmprs & ((1 << minsiz) - 1)
        cmprs >>= minsiz

        if(code == terminal):
            break

        if code in table:
            uncmprs += table[code]
            table[len(table)] = table[prev] + table[code][0].to_bytes(length = 1)
        else:
            table[len(table)] = table[prev] + table[prev][0].to_bytes(length = 1)
            uncmprs += table[code]

        prev = code

    return bytes(uncmprs)

# Packing the bytes into a single big number
def lzw_pack(codes, minsiz):
    i = 0
    packed = 0
    for c in codes:
        packed |= c << i

        if c > (1 << minsiz):
            minsiz += 1

        i = i + minsiz

    return packed
