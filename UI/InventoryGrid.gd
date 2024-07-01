extends GridContainer

var num_slots = 60

# Called when the node enters the scene tree for the first time.
func _ready():
	for i in num_slots:
		var slot = load("res://UI/ItemSlot.tscn").instantiate()
		add_child(slot)
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	pass
