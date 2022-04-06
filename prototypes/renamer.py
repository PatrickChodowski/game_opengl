
from pathlib import Path
import os
main_path = "/home/patrick/Documents/projects/game_opengl/blender/src/"
model_name = "warrior2"
txt_path = f"{main_path}/{model_name}/Textures/"

result = list(Path(txt_path).rglob("*.[tT][gG][aA]"))

rplcs = {"Mixed_AO": "mixedao",
         "Base_Color": "basecolor",
         "Normal_OpenGL": "normal",
         "Metallic": "metallic",
         "Roughness": "roughness",
         "Height": "height",
         "alpha Unity": "alpha",
         "Fur": "fur",
         "Axe": "axe",
         "Armour": "armour",
         "Shild": "shield",
         "Skeleton": "skeleton",
         "Boots": "boots",
         "Helmet": "helmet",
         "Body": "body",
         "Necklace": "necklace",
         " ": "_"}

new_paths = dict()

for p in result:
    str_p = str(p)
    new_p = str_p
    for old, new in rplcs.items():
        if old in new_p:
            new_p = new_p.replace(old, new)
    if new_p != str_p:
        new_paths[str_p] = new_p

for old_name, new_name in new_paths.items():
    os.rename(old_name, new_name)
