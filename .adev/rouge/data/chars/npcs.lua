Engine, Game, Character, Sprite = import("Engine", "Game", "Character", "Sprite")

local game = Engine.game()

local npc = Character.new()
npc:name("Male Townsman")
	:age(21)
	:gender("male")
	:graphic(Sprite.new("characters/npc/front"))
game:add_char_definition(npc, "male1")

npc = Character.new()
npc:name("Male Townsman")
	:age(22)
	:gender("male")
	:graphic(Sprite.new("characters/npc/front"))
game:add_char_definition(npc, "male2")

npc = Character.new()
npc:name("Female Townsman")
	:age(18)
	:gender("female")
	:graphic(Sprite.new("characters/npc/front"))
game:add_char_definition(npc, "female1")

npc = Character.new()
npc:name("Female Townsman")
	:age(21)
	:gender("female")
	:graphic(Sprite.new("characters/npc/front"))
game:add_char_definition(npc, "female2")
