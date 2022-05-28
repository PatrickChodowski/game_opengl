
# Go to file>external data>pack all in .blend 

import psutil
def get_size(bytes, suffix="B"):
    factor = 1024
    for unit in ["", "K", "M", "G", "T", "P"]:
        if bytes < factor:
            return f"{bytes:.2f}{unit}{suffix}"
        bytes /= factor
print("="*40, "Memory Information", "="*40)
svmem = psutil.virtual_memory()
print(f"Total: {get_size(svmem.total)}") ; print(f"Available: {get_size(svmem.available)}")
print(f"Used: {get_size(svmem.used)}") ; print(f"Percentage: {svmem.percent}%")


### 

! nvidia-smi

###


#@title Select Blender Version (used for rendering) then execute the cell{ display-mode: "form" }
Blender_Version = 'Blender 3.1' #@param ["Blender 2.79b", "Blender 2.80", "Blender 2.81", "Blender 2.82a", "Blender 2.83.0", "Blender 2.83.3", "Blender 2.90alpha (expiremental)", "Blender 2.90", "Blender 3.1"]

def path_leaf(path):
  import ntpath
  head, tail = ntpath.split(path)
  return tail or ntpath.basename(head)

dl_link = {
    "Blender 2.79b": "https://download.blender.org/release/Blender2.79/blender-2.79b-linux-glibc219-x86_64.tar.bz2",
    "Blender 2.80": "https://download.blender.org/release/Blender2.80/blender-2.80-linux-glibc217-x86_64.tar.bz2",
    "Blender 2.81": "https://download.blender.org/release/Blender2.81/blender-2.81-linux-glibc217-x86_64.tar.bz2",
    "Blender 2.82a": "https://download.blender.org/release/Blender2.82/blender-2.82a-linux64.tar.xz",
    "Blender 2.83.0": "https://download.blender.org/release/Blender2.83/blender-2.83.0-linux64.tar.xz",
    "Blender 2.83.3": "https://download.blender.org/release/Blender2.83/blender-2.83.3-linux64.tar.xz",
    "Blender 2.90alpha (expiremental)": "https://blender.community/5edccfe69c122126f183e2ad/download/5ef635439c12214ca244be6b",
    "Blender 2.90" : "https://download.blender.org/release/Blender2.90/blender-2.90.0-linux64.tar.xz",
    "Blender 3.1" : "https://download.blender.org/release/Blender3.1/blender-3.1.1-linux-x64.tar.xz"
}


dl = dl_link[Blender_Version]
filename = path_leaf(dl)

if (Blender_Version == "Blender 2.90alpha (expiremental)"):
  !wget $dl
  !sudo apt-get install p7zip-full
  !7z x $filename
  !mv blender_290bM_e935447a5370-20200625-1857 blender



else:
  !wget -nc $dl
  !mkdir ./blender && tar xf $filename -C ./blender --strip-components 1



!apt install libboost-all-dev
!apt install libgl1-mesa-dev
!apt install libglu1-mesa libsm-dev


data = "import re\n"+\
    "import bpy\n"+\
    "scene = bpy.context.scene\n"+\
    "scene.cycles.device = 'GPU'\n"+\
    "prefs = bpy.context.preferences\n"+\
    "prefs.addons['cycles'].preferences.get_devices()\n"+\
    "cprefs = prefs.addons['cycles'].preferences\n"+\
    "print(cprefs)\n"+\
    "# Attempt to set GPU device types if available\n"+\
    "for compute_device_type in ('CUDA', 'OPENCL', 'NONE'):\n"+\
    "    try:\n"+\
    "        cprefs.compute_device_type = compute_device_type\n"+\
    "        print('Device found',compute_device_type)\n"+\
    "        break\n"+\
    "    except TypeError:\n"+\
    "        pass\n"+\
    "# Enable all CPU and GPU devices\n"+\
    "for device in cprefs.devices:\n"+\
    "    if not re.match('intel', device.name, re.I):\n"+\
    "        print('Activating',device)\n"+\
    "        device.use = True\n"
with open('setgpu.py', 'w') as f:
    f.write(data)

###

from google.colab import drive
drive.mount('/gdrive')

###

from google.colab import drive
drive.mount('/content/drive')

###

!sudo ./blender/blender -P setgpu.py -b '/content/drive/My Drive/first_respectable_terrain.blend' -o '/content/drive/My Drive/test.png' -f 1

###