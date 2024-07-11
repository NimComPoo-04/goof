from lzw import *

class PPMImage:
    def __init__(self, name):
        f = open(name, "rb")

        print(f.readline())
        self.width = int(f.readline())
        self.height = int(f.readline())
        print(int(f.readline()))

        self.data = bytearray(f.read())
        self.codes = bytearray()

        self.colormap = bytearray()
        for i in range(0, 8):
            for j in range(0, 8):
                for k in range(0, 4):
                    self.colormap.append((256 // 8) * i)
                    self.colormap.append((256 // 8) * j)
                    self.colormap.append((256 // 4) * k)

        f.close()


    def write(self, name):
        f = open(name, "wb")
        f.write(bytes(f"P6 {self.width} {self.height} 255\n", encoding='utf-8'))
        f.write(self.data)
        f.close()


    def write_gif(self, name):
        self.quantize()

        table = {}

        GIF_CLEAR_CODE = 256
        GIF_END_INFO_CODE = 257

        for i in range(0, 256):
            table[i.to_bytes(byteorder='little')] = len(table)

        table[GIF_CLEAR_CODE.to_bytes(length = 2, byteorder='little')] = len(table)
        table[GIF_END_INFO_CODE.to_bytes(length = 2, byteorder='little')] = len(table)

        pack, size = lzw_encode_pack(table, self.codes, 9)
        cmprs = pack.to_bytes(length = size, byteorder='little')

        f = open(name, "wb")
        f.write('GIF89a'.encode())
        f.write(self.width.to_bytes(length = 2, byteorder='little'))
        f.write(self.height.to_bytes(length = 2, byteorder='little'))
        f.write(bytes([0xf7, 0x00, 0x00]))
        f.write(self.colormap)
        f.write(bytes([0x2c, 00, 00, 00, 00]))
        f.write(self.width.to_bytes(length = 2, byteorder='little'))
        f.write(self.height.to_bytes(length = 2, byteorder='little'))
        f.write(bytes([0, 8]))

        for i in range(0, size, 255):
            f.write(min(255, size - i).to_bytes(length=1, byteorder='little'))
            f.write(cmprs[i : i + min(255, size - i)])


        f.write('\x00\x3b'.encode())

        f.close()


    def quant(self, r, g, b):
        i = r * 8 // 256
        j = g * 8 // 256
        k = b * 4 // 256

        f = i * 32 + j * 4 + k
        self.codes.append(f)
        t = 3 * f
        return self.colormap[t], self.colormap[t + 1], self.colormap[t + 2]

    def quantize(self):
        for i in range(0, self.height):
            for j in range(0, self.width):

                npos = 3 * (i * self.width + j)

                r, g, b = self.quant(*self.data[npos : npos + 3])

                diff_r = self.data[npos + 0] - r
                diff_g = self.data[npos + 1] - g
                diff_b = self.data[npos + 2] - b

                self.data[npos : npos + 3] = (r, g, b)

                if j + 1 < self.width:
                    npos = 3 * (i * self.width + j + 1)
                    self.data[npos + 0] = min(diff_r * 7 // 16 + self.data[npos + 0], 255)
                    self.data[npos + 1] = min(diff_g * 7 // 16 + self.data[npos + 1], 255)
                    self.data[npos + 2] = min(diff_b * 7 // 16 + self.data[npos + 2], 255)

                if j - 1 >= 0:
                    npos = 3 * (i * self.width + j - 1)
                    self.data[npos + 0] = min(diff_r * 3 // 16 + self.data[npos + 0], 255)
                    self.data[npos + 1] = min(diff_g * 3 // 16 + self.data[npos + 1], 255)
                    self.data[npos + 2] = min(diff_b * 3 // 16 + self.data[npos + 2], 255)

                if i + 1 < self.height:
                    npos = 3 * ((i + 1) * self.width + j)
                    self.data[npos + 0] = min(diff_r * 5 // 16 + self.data[npos + 0], 255)
                    self.data[npos + 1] = min(diff_g * 5 // 16 + self.data[npos + 1], 255)
                    self.data[npos + 2] = min(diff_b * 5 // 16 + self.data[npos + 2], 255)

                if i + 1 < self.height and j + 1 < self.width:
                    npos = 3 * ((i + 1) * self.width + (j + 1))
                    self.data[npos + 0] = min(diff_r // 16 + self.data[npos + 0], 255)
                    self.data[npos + 1] = min(diff_g // 16 + self.data[npos + 1], 255)
                    self.data[npos + 2] = min(diff_b // 16 + self.data[npos + 2], 255)

