extends AnimatedSprite2D

var current_equipment


# Called when the node enters the scene tree for the first time.
func _ready():
	pass  # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	pass


func equip(asset_path):
	current_equipment = load(asset_path).instantiate()
	current_equipment.set_z_index(5)
	add_child(current_equipment)


func equipShoulder(asset_path):
	current_equipment = load(asset_path).instantiate()
	current_equipment.set_z_index(6)
	add_child(current_equipment)


func setDir(dir):
	self.position.x = 0 * dir
