extends PanelContainer


# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.

func register(_owner):
	$HBoxContainer/MarginContainer2/VBoxContainer/MarginContainer/ProgressBar.register(_owner)

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	pass
