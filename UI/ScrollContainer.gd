extends ScrollContainer

var max_scroll_length = 0
@onready var scrollbar = get_v_scroll_bar()


# Called when the node enters the scene tree for the first time.
func _ready():
	BattleEngine.connect("battle_event", _process_battle_event)
	scrollbar.connect("changed", handle_scrollbar_changed)
	max_scroll_length = scrollbar.max_value


func _process_battle_event(_n, e):
	get_node("Label").text += "unit" + str(e["owner"]) + ": " + e["type"] + "\n"


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	pass


func handle_scrollbar_changed():
	if max_scroll_length != scrollbar.max_value:
		max_scroll_length = scrollbar.max_value
		self.scroll_vertical = max_scroll_length
