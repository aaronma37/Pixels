extends Node2D

#var layers = ["UILayer", "BackgroundLayer", "CharacterLayer", "ForegroundLayer", "SpellsAndFXLayer"]
var layers = ["UILayer", "BackgroundLayer", "CharacterLayer"]
# Called when the node enters the scene tree for the first time.
func _ready():
	Globals.connect('change_gamestate', _apply_gamestate)
	Globals.changeGamestate("Splash")

func _apply_gamestate(gamestate):
	for l in layers:
		for c in get_node(l).get_children():
			c.hide()
		get_node(l).get_node(gamestate).show()

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	pass
