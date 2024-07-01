extends CanvasLayer

func _input(e):
	if e.is_action_pressed("ui_open_inventory"):
		get_node("AllInventory").show()

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	pass
