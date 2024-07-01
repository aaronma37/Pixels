extends Label


# Called when the node enters the scene tree for the first time.
func _ready():
	BattleEngine.connect("battle_event", _process_battle_event)


func _process_battle_event(n, e):
	text += e["type"] + "\n"
	print(e)


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	pass
