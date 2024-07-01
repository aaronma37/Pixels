extends RichTextLabel

var format_string = "Max HP:  %s.\nAttack Speed: %s\n Cast Speed:  %s\nSpell Power: %s\nMovespeed: %s\n Resistances: %s\nArmor: %s\nSpell Crit Chance %s\nPhysical Crit Chance %s\nSpell Crit Mod: %s\nPhysical Crit Mod: %s"

var b = [100, 1, 1, 100, 1, 0, 0, 0, 0, 0, 0]


# Called when the node enters the scene tree for the first time.
func _ready():
	var actual_string = format_string % b
	set_text(actual_string)
	pass  # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	pass
