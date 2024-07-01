extends PanelContainer


# Called when the node enters the scene tree for the first time.
func _ready():
	get_node("Button").mouse_entered.connect(_on_mouse_entered)
	pass # Replace with function body.

func _on_mouse_entered():
	print("B")

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	pass
