import os
from game_api import list_funcs, parse_command, print_help

# python3 ./Documents/projects/game_opengl/scripts/app.py
# python3 ./scripts/app.py



def display_title_bar():
    # Clears the terminal screen, and displays a title bar.
    os.system('clear')
              
    print("\t**********************************************")
    print("\t***             Game Terminal              ***")
    print("\t**********************************************")

def get_user_command():
    # Let users know what they can do.
    print("\nPlease provide command. Arguments will be split by whitespaces\n")
    print("[ls] List functions")
    print("[q]  Quit")
    
    return input("Command: ")

### MAIN PROGRAM ###

# Set up a loop where users can choose what they'd like to do.
command = ""

display_title_bar()
while command != 'q':

    command = get_user_command()
    display_title_bar()

    if (command != 'q') & (command != 'ls'):
      parse_command(command)

    elif "?" in command:
      print_help(command)

    elif command == 'q':
      print("\nGoodbye")

    elif command == 'ls':
      list_funcs()

    else:
      print("\nI didn't understand that choice.\n")

