from lzw import *
from image import *

dicto = '# ABCDEFGHIJKLMNOPQRSTUVWXYZ'
test = 'TOBEORNOTTOBE TOBEORNOTTOBE TOBEORNOTTOBE#'

table = {}
dectable = {}

def main():
    """
    global table 

    for a in dicto:
        table[a.encode()] = len(table) 
        dectable[len(dectable)] = a.encode()

    codes = lzw_encode_pack(table, test.encode(), 5)
    out = lzw_unpack_decode(dectable, codes, 5, 0)
    print(out)
    """

    img = PPMImage('test.ppm')
    # img.quantize()
    # img.write('out.ppm')
    img.write_gif('out.gif')


if __name__ == '__main__':
    main()
