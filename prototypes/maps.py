
import os
from PIL import Image

# Setup
TILE_WIDTH_COUNT = 20
TILE_HEIGHT_COUNT = 20
TILE_WIDTH = 256
TILE_HEIGHT = 256
TEXTURE_WIDTH = TILE_WIDTH*TILE_WIDTH_COUNT
TEXTURE_HEIGHT = TILE_HEIGHT*TILE_HEIGHT_COUNT

SOURCE_DIR = "/home/patrick/Documents/projects/game_opengl/blender/done/lake_map_small_persp"
DESTINATION_PATH = "/home/patrick/Documents/projects/game_opengl/data/models/8_lake_small_persp__spritesheet.png"

# Read images
images = list()
file_list_str = os.listdir(SOURCE_DIR)
file_list = [int(file.replace(".png", "")) for file in file_list_str]
file_list.sort()

for img in file_list:
    images.append(Image.open(f"{SOURCE_DIR}/{img}.png"))

# Combine images
new_image = Image.new('RGBA', (TEXTURE_WIDTH, TEXTURE_HEIGHT))
x_offset = 0
y_offset = 0
for im in images:
    new_image.paste(im, (x_offset, y_offset))
    x_offset += TILE_WIDTH
    if x_offset == TEXTURE_WIDTH:
        x_offset = 0
        y_offset += TILE_HEIGHT

new_image.save(DESTINATION_PATH)

