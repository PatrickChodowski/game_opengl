from PIL import Image
import json

# variables
name = "lake"
model_id = 8
map_id = 1
frame_width_count = 4
frame_height_count = 2

# consts
model_name = f"{model_id}_{name}"
map_name = f"{map_id}_{name}"
full_path = "/home/patrick/Documents/projects/game_opengl/data"
image_path = f"{full_path}/models/{model_name}___spritesheet.png"
output_model_data_path = f"{full_path}/models/{model_name}.json"
output_map_data_path = f"{full_path}/maps2/{map_name}.json"

# read image:
img = Image.open(image_path)

# data handling -> model data
model_data = dict()
model_data['id'] = model_id
model_data['w'] = img.width
model_data['h'] = img.height
model_data['name'] = name
model_data['frames_list'] = list()
model_data['anim_list'] = list()

# manage frames
frame_width = int(model_data['w']/frame_width_count)
frame_height = int(model_data['h']/frame_height_count)
frame_id = 0

# map data
map_data = dict()
map_data['map_id'] = map_id
map_data['model_id'] = model_id
map_data['name'] = map_name
map_data['tile_width'] = frame_width
map_data['tile_height'] = frame_height
map_data['tile_count_x'] = frame_width_count
map_data['tile_count_y'] = frame_height_count
map_data['_tiles'] = list()

for row in range(frame_height_count):
    for col in range(frame_width_count):
        frame_data = dict()
        frame_data['frame_id'] = frame_id
        frame_data['x'] = col*frame_width
        frame_data['y'] = row*frame_height
        frame_data['w'] = frame_width
        frame_data['h'] = frame_height
        frame_data['label'] = f"{name}_{frame_id}"
        frame_data['norm_x_start'] = frame_data['x']/model_data['w']
        frame_data['norm_x_end'] = (frame_data['x']+frame_data['w'])/model_data['w']
        frame_data['norm_y_start'] = frame_data['y']/model_data['h']
        frame_data['norm_y_end'] = (frame_data['y']+frame_data['h'])/model_data['h']
        model_data['frames_list'].append(frame_data)

        tile_data = dict()
        tile_data['id'] = frame_id
        tile_data['x'] = col
        tile_data['y'] = row
        map_data['_tiles'].append(tile_data)

        frame_id += 1


# write the map data
with open(output_map_data_path, 'w') as outfile:
    json.dump(map_data, outfile)

# write the model data
with open(output_model_data_path, 'w') as outfile:
    json.dump(model_data, outfile)

