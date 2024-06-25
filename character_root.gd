extends Node2D

var base_body_names = [
	"l_arm",
	"r_arm",
	"l_leg",
	"r_leg",
	"body",
	"face",
	"hair",
	"head",
]

var z_indices = {
	"1h_weapon": 0,
	"l_arm": 10,
	"r_arm": 50,
	"l_leg": 20,
	"r_leg": 40,
	"body": 30,
	"face": 70,
	"hair": 80,
	"head": 60,
	"e_upper_body": 41,
	"e_lower_body": 41,
	"e_shoulders": 61,
	"e_r_arm": 55,
	"e_l_arm": 15,
	"e_r_hand": 52,
	"e_l_hand": 12,
}

# animation_type/
var idle_transforms = {
	"num_frames": 6,
	"frame_dur": 2,
	0:
	{
		"1h_weapon": Vector2(11, 9),
		"l_arm": Vector2(4, -1),
		"r_arm": Vector2(-5, -1),
		"l_leg": Vector2(1, 7),
		"r_leg": Vector2(-2, 8),
		"body": Vector2(0, 0),
		"face": Vector2(1, -6),
		"hair": Vector2(-1, -10),
		"head": Vector2(0, -9),
		"e_upper_body": Vector2(1, 0),
		"e_lower_body": Vector2(0, 0),
		"e_shoulders": Vector2(0, 0),
		"e_r_arm": Vector2(-5, -1),
		"e_l_arm": Vector2(4, -1),
		"e_r_hand": Vector2(-10, 11),
		"e_l_hand": Vector2(11, 10),
	},
	1:
	{
		"1h_weapon": Vector2(11, 9),
		"l_arm": Vector2(4, -1),
		"r_arm": Vector2(-5, -1),
		"l_leg": Vector2(1, 7),
		"r_leg": Vector2(-2, 8),
		"body": Vector2(0, 0),
		"face": Vector2(1, -6),
		"hair": Vector2(-1, -10),
		"head": Vector2(0, -9),
		"e_upper_body": Vector2(1, 0),
		"e_lower_body": Vector2(0, 0),
		"e_shoulders": Vector2(0, 0),
		"e_r_arm": Vector2(-5, -1),
		"e_l_arm": Vector2(4, -1),
		"e_r_hand": Vector2(-10, 11),
		"e_l_hand": Vector2(11, 10),
	},
	2:
	{
		"1h_weapon": Vector2(11, 9),
		"l_arm": Vector2(4, -1),
		"r_arm": Vector2(-5, -1),
		"l_leg": Vector2(1, 7),
		"r_leg": Vector2(-2, 8),
		"body": Vector2(0, 0),
		"face": Vector2(1, -6),
		"hair": Vector2(-1, -10),
		"head": Vector2(0, -9),
		"e_upper_body": Vector2(1, 0),
		"e_lower_body": Vector2(0, 0),
		"e_shoulders": Vector2(0, 0),
		"e_r_arm": Vector2(-5, -1),
		"e_l_arm": Vector2(4, -1),
		"e_r_hand": Vector2(-10, 11),
		"e_l_hand": Vector2(11, 10),
	},
	3:
	{
		"1h_weapon": Vector2(11, 9),
		"l_arm": Vector2(4, -1),
		"r_arm": Vector2(-5, -1),
		"l_leg": Vector2(1, 7),
		"r_leg": Vector2(-2, 8),
		"body": Vector2(0, 0),
		"face": Vector2(1, -6),
		"hair": Vector2(-1, -10),
		"head": Vector2(0, -9),
		"e_upper_body": Vector2(1, 0),
		"e_lower_body": Vector2(0, 0),
		"e_shoulders": Vector2(0, 0),
		"e_r_arm": Vector2(-5, -1),
		"e_l_arm": Vector2(4, -1),
		"e_r_hand": Vector2(-10, 11),
		"e_l_hand": Vector2(11, 10),
	},
	4:
	{
		"1h_weapon": Vector2(11, 9),
		"l_arm": Vector2(4, -1),
		"r_arm": Vector2(-5, -1),
		"l_leg": Vector2(1, 7),
		"r_leg": Vector2(-2, 8),
		"body": Vector2(0, 0),
		"face": Vector2(1, -6),
		"hair": Vector2(-1, -10),
		"head": Vector2(0, -9),
		"e_upper_body": Vector2(1, 0),
		"e_lower_body": Vector2(0, 0),
		"e_shoulders": Vector2(0, 0),
		"e_r_arm": Vector2(-5, -1),
		"e_l_arm": Vector2(4, -1),
		"e_r_hand": Vector2(-10, 11),
		"e_l_hand": Vector2(11, 10),
	},
	5:
	{
		"1h_weapon": Vector2(11, 10),
		"l_arm": Vector2(4, 0),
		"r_arm": Vector2(-5, 0),
		"l_leg": Vector2(1, 7),
		"r_leg": Vector2(-2, 8),
		"body": Vector2(0, 1),
		"face": Vector2(1, -5),
		"hair": Vector2(-1, -9),
		"head": Vector2(0, -8),
		"e_upper_body": Vector2(1, 1),
		"e_lower_body": Vector2(0, 0),
		"e_shoulders": Vector2(0, 1),
		"e_r_arm": Vector2(-5, 0),
		"e_l_arm": Vector2(4, 0),
		"e_r_hand": Vector2(-10, 12),
		"e_l_hand": Vector2(11, 11),
	},
}

var walk_transforms = {
	"num_frames": 4,
	"frame_dur": 5,
	0:
	{
		"1h_weapon": Vector2(12, 5),
		"l_arm": Vector2(0, 0),
		"r_arm": Vector2(0, 0),
		"l_leg": Vector2(0, 0),
		"r_leg": Vector2(0, 0),
		"body": Vector2(0, 0),
		"face": Vector2(0, 0),
		"hair": Vector2(0, 0),
		"head": Vector2(0, 0),
		"e_upper_body": Vector2(0, 0),
		"e_lower_body": Vector2(0, 0),
		"e_shoulders": Vector2(0, 0),
		"e_r_arm": Vector2(0, 0),
		"e_l_arm": Vector2(0, 0),
		"e_r_hand": Vector2(0, 0),
		"e_l_hand": Vector2(0, 0),
	},
	1:
	{
		"1h_weapon": Vector2(13, 5),
		"l_arm": Vector2(0, 0),
		"r_arm": Vector2(0, 0),
		"l_leg": Vector2(0, 0),
		"r_leg": Vector2(0, 0),
		"body": Vector2(0, 0),
		"face": Vector2(0, 0),
		"hair": Vector2(0, 0),
		"head": Vector2(0, 0),
		"e_upper_body": Vector2(0, 0),
		"e_lower_body": Vector2(0, 0),
		"e_shoulders": Vector2(0, 0),
		"e_r_arm": Vector2(0, 0),
		"e_l_arm": Vector2(0, 0),
		"e_r_hand": Vector2(0, 0),
		"e_l_hand": Vector2(0, 0),
	},
	2:
	{
		"1h_weapon": Vector2(13, 5),
		"l_arm": Vector2(0, 0),
		"r_arm": Vector2(0, 0),
		"l_leg": Vector2(0, 0),
		"r_leg": Vector2(0, 0),
		"body": Vector2(0, 0),
		"face": Vector2(0, 0),
		"hair": Vector2(0, 0),
		"head": Vector2(0, 0),
		"e_upper_body": Vector2(0, 0),
		"e_lower_body": Vector2(0, 0),
		"e_shoulders": Vector2(0, 0),
		"e_r_arm": Vector2(0, 0),
		"e_l_arm": Vector2(0, 0),
		"e_r_hand": Vector2(0, 0),
		"e_l_hand": Vector2(0, 0),
	},
	3:
	{
		"1h_weapon": Vector2(13, 5),
		"l_arm": Vector2(0, 0),
		"r_arm": Vector2(0, 0),
		"l_leg": Vector2(0, 0),
		"r_leg": Vector2(0, 0),
		"body": Vector2(0, 0),
		"face": Vector2(0, 0),
		"hair": Vector2(0, 0),
		"head": Vector2(0, 0),
		"e_upper_body": Vector2(0, 0),
		"e_lower_body": Vector2(0, 0),
		"e_shoulders": Vector2(0, 0),
		"e_r_arm": Vector2(0, 0),
		"e_l_arm": Vector2(0, 0),
		"e_r_hand": Vector2(0, 0),
		"e_l_hand": Vector2(0, 0),
	},
}
var animations = {"idle": idle_transforms, "walk": walk_transforms}
var current_animation = "idle"
var current_tick = 0
var tick_dur = (1 / 5.0) * 1000
var last_tick_time = Time.get_ticks_msec()
var dir_ = 1


func _ready():
	setDir(1)
	setAnimation("walk")
	equip("res://equipment/e_upper_body_basic_robes.tscn")
	equipShoulder("res://equipment/e_shoulders_basic.tscn")
	setupBody()

	var current_equipment
	current_equipment = load("res://equipment/e_r_arm_basic_robes.tscn").instantiate()
	current_equipment.set_name("e_r_arm")
	current_equipment.set_z_index(z_indices["e_r_arm"])
	current_equipment.get_node("AnimatedSprite2D").play("walk")
	current_equipment.get_node("AnimatedSprite2D").set_frame_and_progress(0, 0)
	add_child(current_equipment)

	var e_l_arm_current_equipment
	e_l_arm_current_equipment = load("res://equipment/e_l_arm_basic_robes.tscn").instantiate()
	e_l_arm_current_equipment.set_name("e_l_arm")
	e_l_arm_current_equipment.set_z_index(z_indices["e_l_arm"])
	e_l_arm_current_equipment.get_node("AnimatedSprite2D").play("walk")
	e_l_arm_current_equipment.get_node("AnimatedSprite2D").set_frame_and_progress(0, 0)
	add_child(e_l_arm_current_equipment)

	var e_lower_body_current_equipment
	e_lower_body_current_equipment = (
		load("res://equipment/e_lower_body_basic_robes.tscn").instantiate()
	)
	e_lower_body_current_equipment.set_name("e_lower_body")
	e_lower_body_current_equipment.set_z_index(z_indices["e_lower_body"])
	e_lower_body_current_equipment.get_node("AnimatedSprite2D").play()
	# add_child(e_lower_body_current_equipment)
	# get_node("r_leg").set_visible(false)
	# get_node("l_leg").set_visible(false)

	var e_r_hand_current_equipment
	e_r_hand_current_equipment = (load("res://equipment/e_r_hand_basic_gloves.tscn").instantiate())
	e_r_hand_current_equipment.set_name("e_r_hand")
	e_r_hand_current_equipment.set_z_index(z_indices["e_r_hand"])
	e_r_hand_current_equipment.get_node("AnimatedSprite2D").play("walk")
	e_r_hand_current_equipment.get_node("AnimatedSprite2D").set_frame_and_progress(0, 0)
	add_child(e_r_hand_current_equipment)

	var e_l_hand_current_equipment
	e_l_hand_current_equipment = (load("res://equipment/e_l_hand_basic_gloves.tscn").instantiate())
	e_l_hand_current_equipment.set_name("e_l_hand")
	e_l_hand_current_equipment.set_z_index(z_indices["e_l_hand"])
	e_l_hand_current_equipment.get_node("AnimatedSprite2D").play("walk")
	e_l_hand_current_equipment.get_node("AnimatedSprite2D").set_frame_and_progress(0, 0)
	add_child(e_l_hand_current_equipment)
	pass


func setupBody():
	for b in base_body_names:
		var node = load("res://body/" + b + ".tscn").instantiate()
		node.set_name(b)
		node.set_z_index(z_indices[b])
		node.get_node("AnimatedSprite2D").play("walk")
		node.get_node("AnimatedSprite2D").set_frame_and_progress(0, 0)
		add_child(node)


func setTransforms():
	var transforms = animations[current_animation][current_tick]
	for k in transforms:
		var n = get_node(k)
		if n == null:
			continue
		n.position = Vector2(dir_ * transforms[k][0], transforms[k][1])
		n.scale.x = dir_


func tick():
	current_tick += 1
	if current_tick >= animations[current_animation]["num_frames"]:
		current_tick = 0
	setTransforms()


func setAnimation(a_str):
	current_animation = a_str
	current_tick = 0
	setTransforms()


func setDir(dir):
	dir_ = dir


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	var ms = Time.get_ticks_msec()
	if ms - last_tick_time > tick_dur:
		last_tick_time = ms
		tick()
	pass


func equip(asset_path):
	var current_equipment
	current_equipment = load(asset_path).instantiate()
	current_equipment.set_name("e_upper_body")
	current_equipment.set_z_index(z_indices["e_upper_body"])
	current_equipment.get_node("AnimatedSprite2D").play("walk")
	current_equipment.get_node("AnimatedSprite2D").set_frame_and_progress(0, 0)
	add_child(current_equipment)


func equipShoulder(asset_path):
	var current_equipment
	current_equipment = load(asset_path).instantiate()
	current_equipment.set_name("e_shoulders")
	current_equipment.set_z_index(z_indices["e_shoulders"])
	current_equipment.get_node("AnimatedSprite2D").play("walk")
	current_equipment.get_node("AnimatedSprite2D").set_frame_and_progress(0, 0)
	add_child(current_equipment)
