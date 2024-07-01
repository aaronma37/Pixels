extends Control

var tavern_state_map = {"default": "DefaultTavernUI", "all_inventory": "AllInventory"}
var tavern_state = 0

func _input(e):
	if e.is_action_pressed("ui_open_inventory"):
		_changeTavernState("all_inventory")
	elif e.is_action_pressed("ui_cancel"):
		_changeTavernState("default")


# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.

func _changeTavernState(_tavern_state):
	tavern_state=_tavern_state
	for c in get_children():
		c.hide()
	get_node(tavern_state_map[tavern_state]).show()
		
# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	pass


