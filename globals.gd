extends Node

var gamestates = ["Spash", "Tavern", "Battle"]

var gamestate = "Splash"
signal change_gamestate

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.


func changeGamestate(_gamestate):
	gamestate = _gamestate
	emit_signal('change_gamestate', gamestate)


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	pass
