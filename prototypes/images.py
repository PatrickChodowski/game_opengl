
import os
from PIL import Image

image_dir = "/home/patrick/Documents/projects/game_opengl/blender/tmp"
images = list()
for img in os.listdir(image_dir):
    if img.endswith(".png"):
        images.append(Image.open(f"{image_dir}/{img}"))
widths, heights = zip(*(i.size for i in images))
total_width = sum(widths)
max_height = max(heights)

new_im = Image.new('RGBA', (total_width, max_height))
x_offset = 0
for im in images:
    new_im.paste(im, (x_offset, 0))
    x_offset += im.size[0]

new_im.save('spritesheet.png')
