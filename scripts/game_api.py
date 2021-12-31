import os 
dir_path = os.path.dirname(os.path.realpath(__file__))
funcs = {
  "_spawn_mob": {"first": "spawn", 
                 "second": "mob",
                 "args": ["mob_type_id", "x", "y"],
                 "id": 0},

  "_drop_mob": {"first": "drop", 
                 "second": "mob",
                 "args": ["entity_id"],
                 "id": 1},

  "_interact_npc": {"first": "interact", 
                    "second": "npc",
                    "args": ["entity_id", "interaction_value"],
                    "id": 2},

  "_spawn_item": {"first": "spawn", 
                  "second": "item",
                  "args": ["item_id", "x", "y"],
                  "id": 3},

  "_drop_item": {"first": "drop", 
                 "second": "item",
                 "args": ["entity_id"],
                 "id": 4},
}


def _print_args(func_name: str) -> None:
  print(f"  args: ")
  for a in funcs[func_name]["args"]:
    print(f"   - {a}")

def _search_func(first: str, second: str) -> str:
  '''
  Search the function list for the command, provide similar ones if not found exact match
  '''
  func_name = ""
  for k,v in funcs.items():
    if (v["first"] == first) & (v["second"] == second):
      func_name = k
      return func_name
  
  ### still didnt return:
  return func_name

def _search_func_by_id(func_id: int) -> str:
  '''
  Search the function list by id
  '''
  func_name = ""
  for k,v in funcs.items():
    if (v["id"] == func_id):
      func_name = k
      return func_name
  
  ### still didnt return:
  return func_name


def list_funcs() -> None:
  print("\n")
  for k,v in funcs.items():
    print(f"[{v['id']}]: {k}")


def _build_script(arg_dict: dict, func_id: int) -> None:
  txt = f"""func_id = {func_id}; vars = {str(arg_dict)}"""
  with open(f"{dir_path}/console.py",'w') as f:
    f.write(txt)

def parse_command(command: str) -> None:
  '''
  First two arguments will be parsed into function name, rest will become arguments for the function
  '''
  cmds = command.split(" ")
  n_args = cmds.__len__()
  func_name = ""

  if n_args < 2:
    # check for invoking function by the function id
    if (n_args == 1) & (cmds[0].isnumeric()):
      func_name = _search_func_by_id(int(cmds[0]))
    else:
      print(f"\nWe definately need more arguments than {n_args}\n") 
  else:
    func_name = _search_func(cmds[0], cmds[1])

  if func_name == "":
    print(f"\n Cant find any function")
  else:
    print(f"\n Function: {func_name}")
    _print_args(func_name)
    arg_dict = _request_args(func_name)
    _build_script(arg_dict, funcs[func_name]['id'])
  


def _request_args(func_name: str) -> dict:
  '''
  Request arguments per function. Input every single arguments by itself, returns dictionary of provided inputs
  '''
  args_dict = dict()
  print("\n")
  for a in funcs[func_name]["args"]:
    args_dict[a] = input(f"{a}: ")
    args_dict[a] = float(args_dict[a])
  return args_dict

def print_help(command: str) -> None:
  '''
  Print help for the command
  '''
  # remove the ? first
  c = command.replace("?","")

