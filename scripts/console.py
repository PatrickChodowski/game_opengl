
from scripts.game_api import func_dict

func_name = "_spawn_mob"
# func_name = "_drop_mob"
# func_name = "_interact_npc"

func_id = func_dict[func_name]

# provide all variables you want, they will be filtered by args
vars = dict()
vars['mob_type_id'] = 0
vars['x'] = 300
vars['y'] = 200
vars['r'] = 0.5
vars['entity_id'] = 2

# for _interact_npc
vars['interaction_value'] = 0.05
