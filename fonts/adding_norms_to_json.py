import json

file_path = '/home/patrick/Documents/projects/game_opengl/models/1_norse.json'

with open(file_path) as json_file:
    model_data = json.load(json_file)

atlas_width = model_data['w']
atlas_height = model_data['h']

for frame in model_data['frames_list']:
    frame['norm_x_start'] = frame['x'] / atlas_width
    frame['norm_x_end'] = (frame['x'] + frame['w'])/atlas_width
    frame['norm_y_start'] = frame['y'] / atlas_height
    frame['norm_y_end'] = (frame['y'] + frame['h'])/atlas_height

with open(file_path, "w") as write_file:
    json.dump(model_data, write_file, indent=4)
