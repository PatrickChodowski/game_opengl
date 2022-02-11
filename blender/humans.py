#3d humans
from typing import Optional, Tuple
import bpy



def make_character(name: str) -> None:
    '''
    f_la01 -> latino female
    f_af01 -> afro female
    f_as01 -> asian female
    f_ca01 -> caucasian female
    m_la01 -> latino male
    m_af01 -> afro male
    m_as01 -> asian male
    f_ft01 -> elf female
    m_ft01 -> elf male
    m_ft02 -> dwarf male
    '''
    print(f"Initializing character {name}")
    bpy.context.scene.mbcrea_root_data = 'data'
    bpy.context.scene.mblab_character_name = name
    bpy.context.scene.mblab_use_muscle = False
    bpy.context.scene.mblab_use_ik = False
    bpy.ops.mbast.init_character()
    print(f"Character {name} done")
    
  
def get_skeleton_name() -> Optional[str]:
  sk_name = None
  for o in bpy.data.collections['MB_LAB_Character'].objects:
    if 'skeleton' in o.name: 
      sk_name = o.name
  print(f"Skeleton name of MB character is {sk_name}")
  return sk_name


def get_body_name() -> Optional[str]:
  bl_obj_name = None
  for o in bpy.data.objects:
    if "MBlab_bd" in o.name:
      bl_obj_name = o.name
  return bl_obj_name


def _set_type(name: str) -> None:
    '''
    specialtype_brute,specialtype_evil_genius,specialtype_fat_brute,
    specialtype_hero01,specialtype_hero02,type_athletic,type_average01,type_common01,
    type_common02,type_common03,type_common04,type_common05,
    type_extreme_bodybuilder,type_extreme_overweight,type_extreme_underweight,
    type_giant,type_giant_muscled,type_heavybody,type_ideal,type_lightbody,
    type_normobody,type_slender01,type_stocky01,type_stocky02,type_vigorous01
    '''
    bpy.context.object.preset = name



def _set_ethnic(name: str) -> None:
    '''
    afro male:
    aboriginal, african, afroasian, afromediterranean, bantuid, congolid, sudanid

    caucasian male:
    afrocaucasian, alpinid, central_european, east_european, euroartic,
    euromediterranean, mediterranid, nordid, north_european,northwest_european, west_asian

    latino:
    amazonid, central_american, centralid, north_american, silvid, south_american

    asian:
    central_asian, east_asian, north_asian, south_asian
    '''
    bpy.context.object.ethnic = name



def _pick_bodypart(name: str) -> None:
    '''
    Abdomen, Armpit, Arms, Body, Cheeks, Chest, Chin, Ears, Elbows, Expressions, Eyebrows,
    Eyelids, Eyes, Face, Fantasy, Feet, Forehead, Hands, Head, Jaw, Legs, Mouth, Neck,
    Nose, Pelvis, Shoulders, Stomach, Torso, Waist, Wrists
    '''
    bpy.context.scene.morphingCategory = name

    #example: bpy.context.object.Fantasy_PupilCat = 0.8


def _set_rest_post_on() -> None:
    bpy.ops.mbast.button_rest_pose_on()

def _set_rest_post_off() -> None:
    bpy.ops.mbast.button_rest_pose_off()


def _set_skin_editor(name: str, vaue: float) -> None:
  '''
  eyes_hue
  eyes_iris_mix
  eyes_saturation
  eyes_value
  nails_mix
  skin_blush
  skin_bump
  skin_complexion
  skin_freckles
  skin_oil
  skin_roughness
  skin_sss
  skin_veins
  bpy.context.object.eyes_hue = 0.51
  '''
  pass


#bpy.context.object.character_age = 0.5
#bpy.context.object.character_mass = 0.1
#bpy.context.object.character_tone = 0.0

#https://www.youtube.com/watch?v=rtpabZ3mnXQ



def set_tpose(sk_obj_name: str) -> None:
  bpy.data.objects[sk_obj_name].rest_pose = 't-pose'
  print(f"Skeleton {sk_obj_name} set to T-pose")


def import_animation(filepath: str) -> str:
  '''
  Taunt, Unarmed Jump
  Returns armature object name
  '''

  pre_arms = list()
  post_arms = list()
  pre_obj_arms = list()
  post_obj_arms = list()
  for a in bpy.data.armatures:
      pre_arms.append(a.name)

  for o in bpy.data.objects:
    if "Armature" in o.name:
      pre_obj_arms.append(o.name)

  anim_dir = filepath
  #anim_dir = f"/home/patrick/Downloads/mixamo/noskin/{name}.fbx"
  bpy.ops.import_scene.fbx(filepath = anim_dir,
                           use_anim = True,
                           ui_tab="ARMATURE",
                           automatic_bone_orientation=True)
                           #ignore_leaf_bones

  
  for a in bpy.data.armatures:
      post_arms.append(a.name)
    
  for o in bpy.data.objects:
    if "Armature" in o.name:
      post_obj_arms.append(o.name)

  
  arm_name = list(set(post_arms) - set(pre_arms))[0]
  arm_obj_name = list(set(post_obj_arms) - set(pre_obj_arms))[0]
  print(f"  New Armature object name: {arm_obj_name}")
  print(f"  New Armature name: {arm_name}")
  bpy.data.armatures[arm_name].pose_position = 'REST'
  return arm_obj_name


def finalize() -> Optional[str]:
  """
  Returns blender object name
  """
  #bpy.ops.mbast.finalize_character_and_images(filepath=f"/home/patrick/Documents/projects/game_opengl/blender/backup_characters/{name}.png")
  print(f"Finalizing character")
  bl_obj_name = None
  bpy.context.scene.mblab_save_images_and_backup = False
  bpy.ops.mbast.finalize_character()

  for o in bpy.data.objects:
    if "MBlab_sk" in o.name:
      bl_obj_name = o.name

  print(f"Character finalized. Its skeleton object name is {bl_obj_name}")
  return bl_obj_name


def retarget(source_name: str, target_name: str) -> None:
  print(f"Started retargeting from {source_name} to {target_name}")
  bpy.context.scene.rsl_retargeting_armature_source = bpy.data.objects[source_name]
  bpy.context.scene.rsl_retargeting_armature_target = bpy.data.objects[target_name]
  bpy.ops.rsl.build_bone_list()
  bpy.context.scene.rsl_retargeting_auto_scaling = True
  bpy.ops.rsl.retarget_animation()
  # cuts the animation length to match the animation length, I dont know either
  bpy.ops.object.expykit_action_to_range()

  print(f"Finished retargeting, removing source armature {source_name}")
  bpy.data.objects.remove(bpy.data.objects[source_name], do_unlink=True)
  print(f"Source armature {source_name} removed")



def render_anim(obj_name: str) -> None:
  # need to change the dir, name etc.
  bpy.context.scene.render.engine = 'CYCLES'
  bpy.context.scene.cycles.device = 'GPU'
  bpy.data.objects[obj_name].select_set(state=True)
  bpy.ops.view3d.camera_to_view_selected()
  bpy.data.objects[obj_name].select_set(state=False)
  bpy.context.scene.render.resolution_x = 100
  bpy.context.scene.render.resolution_y = 100
  bpy.context.scene.render.image_settings.file_format = 'PNG'
  bpy.context.scene.render.filepath = "/home/patrick/Documents/projects/game_opengl/blender/tmp/"

  bpy.context.scene.frame_step = 5

  #bpy.context.scene.frame_start = 1
  #bpy.context.scene.frame_end = 69


  bpy.ops.render.render(animation=True, write_still=True)


