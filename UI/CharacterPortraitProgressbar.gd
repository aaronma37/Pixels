extends ProgressBar


# Called when the node enters the scene tree for the first time.
func _ready():
	pass


func register(_owner):
	BattleEngine.connect("owner_battle_event_" + str(_owner), _f)


func _f(_n, e):
	if e["type"] == "took_damage":
		value = e["remaining_hp"]


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	pass
