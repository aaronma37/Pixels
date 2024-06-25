extends AnimatedSprite2D

var c = 0
var dir = -1
# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	c+=1
	if c > 100:
		dir *= -1
		c = 0
	rotate(dir* delta/35)
