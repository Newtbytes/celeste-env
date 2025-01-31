from PIL import Image


SPR_SIZE = 8

INDENT = ' ' * 4


def is_spr_empty(pixels, x, y):
    for dx in range(8):
        for dy in range(8):
            if pixels[x*8 + dx, y*8 + dy] != 0:
                return False

    return True

def emit_spr(pixels, x, y):
    out = INDENT + f"// tile {x}, {y}\n"
    out += INDENT + "{\n"

    for dy in range(8):
        out += INDENT + INDENT

        for dx in range(8):
            c = pixels[x*8 + dx, y*8 + dy]
            out += f"{c}, "
        
        out += '\n'

    out += INDENT + "},\n"
    
    return out

def emit_sprite_array(fn, name):
    image = Image.open(fn)
    pixels = image.load()

    w, h = image.size

    w_tiles = w // SPR_SIZE
    h_tiles = h // SPR_SIZE

    num_tiles = w_tiles * h_tiles
    spr_bytes = SPR_SIZE * SPR_SIZE

    out = "#pragma once\n\n"
    out += f"const unsigned char {name}[{num_tiles}][{spr_bytes}]"
    out += " = {\n"

    for y in range(h_tiles):
        for x in range(w_tiles):
            out += emit_spr(pixels, x, y)
            out += '\n'
    
    out += "};"

    return out


if __name__ == "__main__":
    import argparse
    import os.path

    parser = argparse.ArgumentParser()
    parser.add_argument("-i", type=str, default="gfx.bmp")
    parser.add_argument("-o", type=str, default="gfx.h")

    args = parser.parse_args()

    input_fn, output_fn = args.i, args.o

    name = os.path.basename(input_fn)
    if '.' in name:
        name = name.split('.')[0]

    out = emit_sprite_array(input_fn, name)

    with open(output_fn, "w") as f:
        f.write(out)
