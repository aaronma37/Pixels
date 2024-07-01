extends TextureRect

var rarity_map = {0: "#FFFFFF", 1: "#77DD77"}
var slot_map = {0: "Head", 1: "Body", 2: "Hands", 3: "Shoulders"}
var is_draggable = true

func _get_drag_data(_pos):
	print(_pos)
	if !is_draggable:
		return
	var preview_texture = load("res://Icon/icon.tscn").instantiate()
 
	#preview_texture.texture = texture
	#preview_texture.expand_mode = 1
	#preview_texture.size = Vector2(30,30)
 
	var preview = Control.new()
	preview.add_child(preview_texture)
 
	set_drag_preview(preview)
	texture = null
	print(preview)
	return {"A": 1}
	#return preview_texture.texture
	
	#set_drag_preview()

func _can_drop_data(_pos, data):
	print("can", _pos, data)
	return data
 
 
func _drop_data(_pos, data):
	print("drop", _pos, data)
	#texture = data

# Called when the node enters the scene tree for the first time.
func _ready():
	mouse_entered.connect(_on_mouse_entered)
	mouse_exited.connect(_on_mouse_exited)
	#get_node("TextureButton").get_node("Panel").get_node("Area2D").input_event.connect(_on_right_click)
	pass # Replace with function body.

func _on_mouse_entered():
	var icon_data = {"item_name": "Basic Robes", "item_rarity": 0, "item_slot": 1, "item_level": 1, "item_rank": 1, "affinities": ["Sharp", "Haunted"]}
	setIcon(icon_data)
	get_node("Panel").set_visible(true)
	
func _on_mouse_exited():
	get_node("Panel").set_visible(false)

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	pass

func setIcon(icon_data):
	get_node("Panel").get_node("EquipmentTitle").clear()
	get_node("Panel").get_node("EquipmentTitle").append_text("[color=" + rarity_map[icon_data["item_rarity"]] + "]" + icon_data["item_name"] + "[/color]")
	
	get_node("Panel").get_node("Slot").clear()
	get_node("Panel").get_node("Slot").append_text(slot_map[icon_data["item_slot"]])
	for n in get_node("Panel").get_node("HBoxContainer").get_children():
		get_node("Panel").get_node("HBoxContainer").remove_child(n)
		n.queue_free()
	var current_equipment = load("res://affinity_label.tscn").instantiate()
	current_equipment.get_node("Label").set_text(icon_data["affinities"][0])
	get_node("Panel").get_node("HBoxContainer").add_child(current_equipment)


