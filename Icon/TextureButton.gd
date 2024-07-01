extends TextureButton


# Called when the node enters the scene tree for the first time.
func _ready():
	mouse_entered.connect(_on_mouse_entered)
	pass # Replace with function body.

func _on_mouse_entered():
	print("AA")

func _on_mouse_exited():
	print("BB")
# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	pass
