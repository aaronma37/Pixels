extends GDExample2

signal took_damage_0;
var owner_signals = {}

# Called when the node enters the scene tree for the first time.
func _ready():
	self.connect("battle_event", _process_battle_event)

	for c in range(6):
		add_user_signal("owner_signal"+str(c))
		owner_signals[c] = Signal(self, "owner_signal"+str(c))
	add_user_signal("owner_signal-1")
	owner_signals[-1] = Signal(self, "owner_signal-1")

func _process_battle_event(n,e):
	owner_signals[e["owner"]].emit(n, e)
	#print(e)

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	pass
