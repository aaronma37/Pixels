extends Control


# Called when the node enters the scene tree for the first time.
func _ready():
	$PanelContainer/MarginContainer/HBoxContainer/VBoxContainer/CharacterPortrait.register(0)
	$PanelContainer/MarginContainer/HBoxContainer/VBoxContainer/CharacterPortrait2.register(1)
	$PanelContainer/MarginContainer/HBoxContainer/VBoxContainer/CharacterPortrait3.register(2)
	$PanelContainer/MarginContainer/HBoxContainer/VBoxContainer2/CharacterPortrait.register(3)
	$PanelContainer/MarginContainer/HBoxContainer/VBoxContainer2/CharacterPortrait2.register(4)
	$PanelContainer/MarginContainer/HBoxContainer/VBoxContainer2/CharacterPortrait3.register(5)
	BattleEngine.connect("battle_event", _process_battle_event)

	pass  # Replace with function body.


func _process_battle_event(_n, e):
	if e["type"] == "end_match":
		Globals.changeGamestate("Tavern")


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	pass
