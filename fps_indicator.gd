extends Label
func _ready():
	set_scale(Vector2(.25,.25))

func _process(delta: float) -> void:
	set_text("FPS " + String.num(Engine.get_frames_per_second()))
