extends Control

var scene = preload("res://character/character_root.tscn")
var characters = {}

var start_positions = {0: 300, 1: 350, 2: 400, 3: 850, 4: 900, 5: 950}


# Called when the node enters the scene tree for the first time.
func _ready():
	for i in range(6):
		characters[i] = scene.instantiate()
		add_child(characters[i])
		characters[i].position.x = start_positions[i]
		characters[i].position.y = 300
		characters[i].scale = Vector2(2, 2)
		if i > 2:
			characters[i].setDir(-1)


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	pass
