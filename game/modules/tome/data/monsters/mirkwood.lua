--Mirkwood monster file 


new_monster_races
{
--elves, elves and more elves
  ['h'] =
    {
    defaults =
    {
        body = default_body.humanoid
        flags =
        {
          ALLOW_IN_MIRKWOOD=true
          OPEN_DOOR=true MOVE_BODY=true
          PASS_WALL=getter.flags
          {
            PASS_TREES=1
          }
        }
    }
    [1] =
    {
      name = "nandorin hunter"
      level = 34 rarity = 5
      desc =
      {
				"Silent and unseen are the ways of the elves, and this hunter even more",
				"so. He fears little, even within Mirkwood Forest, and you are a",
				"distraction to his hunt!",
      }
      color = color.DARK_SEA_GREEN
      speed = 6 life = {5,11} ac = 15
      aaf = 120 sleep = 15
      exp = 320 weight = 1000
      blows =
      {
        {"HIT","PIERCE",{5,5}},
        {"HIT","SLASH",{6,5}},
        {"HIT","SLASH",{2,12}},
      }
      flags =
      {
        RESIST=getter.resists{PIERCE=15 SLASH=10}
        CAN_SWIM=true INVISIBLE=true
        FACTION=FACTION_ELF
        SMART=true MORTAL=true DROP_CORPSE=true DROP_SKELETON=true
				AI = ai.SPELL_AND_MOVE LIMIT_SPELLS = getter.spells{["Arrow"] = 50}
				FRIENDS=getter.friends(80,5)
				AI_STATES = getter.flags
				{
					MOVE_AI = ai.ZOMBIE
					CASTING_AI = ai.SMART_CAST
				}
      }
         spells =
        {
				frequency = 2
				["Arrow"] = { level=50 chance=100 }
        }      
        
      }
      [2] =
      {
        name = "nandorin war party"
        level = 38 rarity = 5
        desc =
        {
          "Here come a party of Wood Elves dressed from war. They are the fiercest",
          "forest fighters upon Middle Earth and you would be wise not to seek",
          "quarrel with them!"
        }
        color = color.TAN
        speed = 10 life = {6,11} ac = 30
        aaf = 55 sleep = 30
        exp = 375 weight = 160
        blows =
        {
          {"HIT","PIERCE",{6,5}},
          {"HIT","PIERCE",{6,5}},
          {"HIT","PIERCE",{6,5}},
        }
        flags =
        {
          RESIST=getter.resists{PIERCE=15 CRUSH=15}
          FACTION=FACTION_ELF
          SMART=true MORTAL=true DROP_CORPSE=true DROP_SKELETON=true
          AI = ai.ZOMBIE FRIENDS=getter.friends(100,11)
        }
      }
      [3] =
      {
        name = "nandorin hero"
        level = 45 rarity = 1
        desc =
        {
          "Wielding an enchanted sword and dagger, this formidable warrior",
          "is among the elite of the elven army.",
        }
        color = color.CRIMSON
        speed = 12 life = {25,12} ac = 18
        aaf = 15 sleep = 30
        exp = 550 weight = 2200
        blows =
        {
          
          {"HIT","PIERCE",{5,5}}, 
          {"HIT","COLD",{5,5}},
          {"HIT","SLASH",{8,8}}, 
          {"HIT","VALINOREAN_FIRE",{8,8}},
        }
        flags =
        {
          RESIST=getter.resists{SLASH=18 PIERCE=13 CRUSH=18 FIRE=100 FEAR=100 POIS=50 VALINOREAN_FIRE=50 DARK_FIRE=-20}
          SMART=true MORTAL=true DROP_CORPSE=true DROP_SKELETON=true
          AI = ai.ZOMBIE FORCE_MAXHP=true DROP_GOOD=true DROP_NUMBER=getter.random(2,4)     
          MANA_CURR = 140 MANA_MAX = 140 MANA_REGENERATE=300          
          DROP_THEME = getter.flags {THEME_COMBAT = 100}
          FACTION=FACTION_ELF ESCORT=true ESCORTED_BY_GROUPS=true
          HAS_LITE=true
        }        
				spells =
				{
				frequency = 4
				["Healing"] = { level=29 chance=80}
				["Summon Animal"] = { level=1 chance=70}
				["Essence of Speed"] = { level=10 chance=80 }
				}
      }
      [4] =
      {
        name = "nandorin warrior"
        level = 30 rarity = 10
        desc =
        {
          "These veteran warriors make up the bulk of the",
          "great army of Mirkwood. They are fearsome foes.",
        }
        color = color.OLD_LACE
        speed = 15 life = {8,10} ac = 18
        aaf = 25 sleep = 35
        exp = 345 weight = 2000
        blows =
        {
          {"HIT","PIERCE",{4,5}},
          {"HIT","SLASH",{6,5}},
          {"HIT","SLASH",{4,6}},
        }
        flags =
        {
          RESIST=getter.resists{SLASH=10 PIERCE=15 CRUSH=5 FEAR=100}
          SMART=true MORTAL=true DROP_CORPSE=true DROP_SKELETON=true
          FACTION=FACTION_ELF FRIENDS=getter.friends(90,14)
          AI = ai.ZOMBIE     
        }
      }
    }
  ['r'] =
    {
      defaults =
      {
          body = default_body.quadruped
          flags =
          {
            ALLOW_IN_MIRKWOOD=true
            OPEN_DOOR=false MOVE_BODY=true
          }
      }
      --
      [1] =
      {
        name = "giant black squirrel"
        level = 30 rarity = 5
        desc =
        {
          "A very large, very black squirrel.",
        }
        color = color.DARK_SLATE_GRAY
        speed = 8 life = {5,10} ac = 5
        aaf = 15 sleep = 5
        exp = 210 weight = 275
        blows =
        {
          {"BITE","PIERCE",{8,3}},
          --{"BITE","PIERCE",{8,3}},
          {"BITE","HALLU",{5,3}},
        }
        flags =
        {
          RESIST=getter.resists{PIERCE=5 CRUSH=10 FEAR=100}
          SMART=false MORTAL=true DROP_CORPSE=true DROP_SKELETON=false
          FACTION=FACTION_ANIMAL
          AI = ai.SNAKE MULTIPLY=true FRIENDS=getter.friends(75,4)   
          PASS_WALL=getter.flags
          {
            PASS_TREES=1
          }
        }
      }
    }
    ['q'] =
    {
      defaults =
      {
          body = default_body.quadruped
          flags =
          {
            ALLOW_IN_MIRKWOOD=true
            OPEN_DOOR=false MOVE_BODY=true
          }
      }
      --
      [1] =
      {
        name = "white stag"
        level = 32 rarity = 5
        desc =
        {
          "A beautiful white stag, with over fourteen points on its magnificent",
          "antlers.",
        }
        color = color.WHITE
        speed = 14 life = {20,10} ac = 6
        aaf = 15 sleep = 30
        exp = 230 weight = 1800
        blows =
        {
          {"HIT","PIERCE",{8,5}},
          {"BUTT","PIERCE",{10,5}},
          {"KICK","CRUSH",{5,5}},
        }
        flags =
        {
          RESIST=getter.resists{PIERCE=10 CRUSH=10 FEAR=100}
          SMART=false MORTAL=true DROP_CORPSE=true 
          AI = ai.SPELL_AND_MOVE
					AI_STATES=getter.flags
					{
						MOVE_AI=ai.ZOMBIE
						CASTING_AI=ai.SMART_CAST
						FEAR_LEVEL=30
					}
          FACTION=FACTION_ANIMAL
        }
        spells =
        {
          frequency = 4
          ["Teleport To"] = { level=40 chance=85 }
        }
      }
      [2] =
      {
        name = "great bear"
        level = 38 rarity = 5
        desc =
        {
					"These enormous shaggy bears make their homes in the shadowy retreats",
					"of Mirkwood forest, and are rarley seen alone.",
        }
        color = color.TAN
        speed = 13 life = {30,12} ac = 30
        aaf = 25 sleep = 20
        exp = 320 weight = 2750
        blows =
        {
          {"BITE","CRUSH",{5,8}},
          {"CLAW","SLASH",{9,3}},
          {"CRUSH","CRUSH",{3,5}},
          {"BITE","PIERCE",{3,6}},
        }
        flags =
        {
          RESIST=getter.resists{FEAR=100}
          SMART=true MORTAL=true DROP_CORPSE=true 
          FRIENDS=getter.friends(100,1) 
          FACTION=FACTION_ANIMAL AI = ai.ZOMBIE
        }
      }
      [3] =
      {
        name = "shaggy great bear"
        level = 45 rarity = 3
        desc =
        {
          "Of the great bears of Mirkwood, this one seems to be somewhat fiercer than the others.",
          "These bears have distinctly longer fur than their more common cousins.",
        }
        color = color.DARK_KHAKI
        speed = 18 life = {30,16} ac = 35
        aaf = 30 sleep = 30
        exp = 320 weight = 3800
        blows =
        {
          {"CLAW","CRUSH",{8,8}},
          {"CLAW","SLASH",{10,6}},
          {"BITE","PIERCE",{2,7}},
        }
        flags =
        {
          RESIST=getter.resists{FEAR=100}
          SMART=true MORTAL=true DROP_CORPSE=true REFLECTING=20
          AI = ai.ZOMBIE FORCE_MAXHP=true
          FACTION=FACTION_ANIMAL
        }
        
      }
    }--end 'q'
    
    --canines
    ['C'] =
    {
      defaults =
      {
          body = default_body.quadruped
          flags =
          {
            ALLOW_IN_MIRKWOOD=true
            OPEN_DOOR=false MOVE_BODY=true
            
          }
      }
      [1] =
      {
        name = "mirkwood warg"
        level = 30 rarity = 5
        desc =
        {
          "Once thought to be extinct, these enormous wolves",
          "are again ravaging the forest.",
        }
        color = color.CHOCOLATE
        speed = 12 life = {5,10} ac = 15
        aaf = 15 sleep = 30
        exp = 320 weight = 2750
        blows =
        {
          {"BITE","PIERCE",{19,4}},
          {"BITE","PIERCE",{19,4}},
        }
        flags =
        {
          RESIST=getter.resists{FEAR=100 DARK_FIRE=50 FIRE=-5}
          SMART=true MORTAL=true DROP_CORPSE=true EVIL=true
          FRIENDS=getter.friends(100,5)
          AI = ai.SPELL_AND_MOVE AI_STATES=getter.flags
					{
						CASTING_AI=ai.SMART_CAST
						MOVE_AI=ai.ZOMBIE
						LURE_FROM_CORRIDOR=true
					}
          FACTION=FACTION_SAURON
        }
        spells =
        {
          frequency = 6
          ["Howl"] = { level=40 chance=90}
        }
      } 
    }--end 'C'
       --ghosts
    ['G'] =
    {
      defaults =
      {
          body = default_body.humanoid
          flags =
          {
            ALLOW_IN_MIRKWOOD=true
            OPEN_DOOR=true MOVE_BODY=true    
            PASS_WALL=getter.flags
            {
              PASS_INCORP=1
            }
          }
      }
      [1] =
      {
        name = "mirkwood spirit"
        level = 35 rarity = 2
        desc =
        {
					"This spirit died of exposure long ago in Mirkwood, insane and alone",
					"it clings to you for comfort, at the same time trying to make you as"
					"dead as she!"
        }
        color = color.OLIVE_DRAB
        speed = 0 life = {8,6} ac = 15
        aaf = 15 sleep = 30
        exp = 300 weight =10
        blows =
        {
          {"TOUCH","INSANITY",{1,2}},
          {"TOUCH","BLIND",{9,4}},
          {"TOUCH","TIME",{5,2}},
        }
        flags =
        {
          RESIST=getter.resists{PIERCE=90 CRUSH=90 SLASH=90 FEAR=100 FIRE=5}
          SMART=true MORTAL=true DROP_CORPSE=false FEMALE=true
          AI = ai.SNAKE INVISIBLE=true AERIAL=true AERIAL=true CAN_FLY=3 
          FACTION=FACTION_UNDEAD
        }
      } 
    }--end 'G'
    --skeletons
    ['s'] =
    {
      defaults =
      {
          body = default_body.humanoid
          flags =
          {
            ALLOW_IN_MIRKWOOD=true
            OPEN_DOOR=true FACTION=FACTION_UNDEAD
            EVIL=true UNDEAD=true NONLIVING=true NO_SLEEP=true NO_CUT=true
            COLD_BLOOD=true SEE_INVIS=15 REGENERATE = 0
          }
      }
      [1] =
      {
        name = "skeletal elf"
        level = 39 rarity = 5
        desc =
        {
					"This wood elf met a grisly end many years ago, and now only its bones",
					"are left. These bones have absorbed the evil energy that abounds in",
					"Mirkwood, and have arisen, picked up the elf's rusty sword and are",
					"out in search of the living to revenge itself on."
        }
        color = color.GOLD
        speed = 1 life = {8,14} ac = 20
        aaf = 15 sleep = 30
        exp = 250 weight = 1000
        blows =
        {
          {"HIT","SLASH",{8,4}},
          {"HIT","SLASH",{1,10}},
          {"HIT","SLASH",{5,5}},
        }
        flags =
        {
          RESIST=getter.resists{PIERCE=65 CRUSH=-20 FEAR=100}
          STUPID=true DROP_CORPSE=false
          AI = ai.ZOMBIE 
          FACTION=FACTION_UNDEAD
        }
      }
      [2] =
      {
        name = "skeletal orc champion"
        level = 40 rarity = 5
        desc =
        {
          "The remains of an ancient orcish champion risen from its mossy",
          "grave at the behest of the Necromancer. The creature snarls at you",
          ", canines especially prominent now it has no lips to curl."
        }
        color = color.TEAL
        speed = 0 life = {20,6} ac = 13
        aaf = 15 sleep = 30
        exp = 260 weight =1200
        blows =
        {
          {"HIT","SLASH",{7,7}},
          {"HIT","SLASH",{7,7}},
          {"HIT","SLASH",{7,7}},
        }
        flags =
        {
          RESIST=getter.resists{PIERCE=68 CRUSH=-20 FEAR=100}
          STUPID=true DROP_CORPSE=false
          FACTION=FACTION_UNDEAD
          AI = ai.ZOMBIE 
        } 
        }
      
    }--end 's'  
    
    ['S'] =
    {
      defaults =
      {
          body = default_body.humanoid
          flags =
          {
            ALLOW_IN_MIRKWOOD=true
            OPEN_DOOR=false MOVE_BODY=true EVIL=true
            FACTION=FACTION_DUNGEON
            PASS_WALL=getter.flags
            {
              PASS_TREES=1
              PASS_WEB=1
            }
          }
      }
      [1] =
      {
        name = "web sprayer"
        level = 30 rarity = 12
        desc =
        {
          "With its enormous blue spotted abdomen, this speciment is quite easy",
          "to spot in any Mathom-house. Getting it dead and in there will,",
          "however, be quite difficult.",
        }
        color = color.STEEL_BLUE
        speed = 5 life = {6,12} ac = 30
        aaf = 15 sleep = 30
        exp = 300 weight = 900
        blows =
        {
          {"BITE","PARALYZE",{8,4}},
          {"BITE","POISON",{19,4}},
          {"BITE","POISON",{15,5}},
        }
        flags =
        {
          RESIST=getter.resists{CRUSH=5 SLASH=6 FEAR=100 FIRE=-5 LITE=-10}
          SMART=true MORTAL=true DROP_CORPSE=false
          AI = ai.SPELL_AND_MOVE
          AI_STATES=getter.flags
          {
            MOVE_AI=ai.ZOMBIE
            CASTING_AI=ai.SMART_CAST
          }          
        }
        spells =
          {
            frequency = 2
            ["Web"] = { level=50 chance=100 }
          }                                                   
        
      } 
      [2] =
      {
        name = "mirkwood spider"
        level = 30 rarity = 2
        desc =
        {
          "The trees around you are alive with chittering as the reflective eyes",
          "of the true Lords of Mirkwood stare out at you. These spiders are",
          "intelligent, and what's worse, co-operate to feed on their prey."
        }
        color = color.DARK
        speed = 5 life = {9,9} ac = 30
        aaf = 60 sleep = 30
        exp = 125 weight = 600
        blows =
        {
          {"BITE","POISON",{8,8}},
        }
        flags =
        {
          RESIST=getter.resists{POIS=100}
          SMART=true MORTAL=true DROP_CORPSE=false
          AI = ai.SPELL_AND_MOVE FRIENDS=getter.friends(90,15)
          AI_STATES=getter.flags
          {
            MOVE_AI=ai.ZOMBIE
            CASTING_AI=ai.SMART_CAST
            LURE_FROM_CORRIDOR=true
          }
        }
        spells =
          {
            frequency = 2
            ["Web"] = { level=30 chance=100 }
          }
      }
     [3] =
      {
        name = "black weaver"
        level = 33 rarity = 5
        desc =
        {
          "Its gray and inky black carapace makes it invisible for all practical",
          "purposes. This spider does not spin webs, but waits carefully for its",
          "prey in the shadows.",
        }
        color = color.ANTIQUE_WHITE
        speed = 5 life = {8,12} ac = 20
        aaf = 15 sleep = 30
        exp = 360 weight = 900
        blows =
        {
          {"BITE","PARALYZE",{8,4}},
          {"BITE","POISON",{9,4}},
        }
        flags =
        {
          RESIST=getter.resists{CRUSH=5 SLASH=5 FEAR=100 FIRE=-5 LITE=-10}
          SMART=true MORTAL=true DROP_CORPSE=false INVISIBLE=true
          AI = ai.ZOMBIE
          AI_STATES=getter.flags
          {
						LURE_FROM_CORRIDOR=true
					}
        }
			}
      [4] =
      {
        name = "hawk spider"
        level = 34 rarity = 5
        desc =
        {
					"This bizarre breed of spider has learned to fly by spinning a web",
					"between six of its legs and moving them rapidly.",
        }
        color = color.UMBER
        speed = 5 life = {8,12} ac = 10
        aaf = 10 sleep = 30
        exp = 330 weight = 700
        blows =
        {
          {"BITE","POISON",{9,4}},
          {"BITE","POISON",{9,4}},
        }
        flags =
        {
          RESIST=getter.resists{CRUSH=5 SLASH=6 FEAR=100 FIRE=-5 LITE=-10}
          MORTAL=true DROP_CORPSE=false AERIAL=true AERIAL=true CAN_FLY=3 SPEEDS=getter.speeds{WALK=10}
          AI = ai.SPELL_AND_MOVE
          AI_STATES=getter.flags
          {
            MOVE_AI=ai.ZOMBIE
            CASTING_AI=ai.SMART_CAST
          }
        }
        spells =
        {
          frequency = 10
          ["Web"] = { level=10 chance=100 }
        }
      }
      [5] =
      {
        name = "moth spider"
        level = 35 rarity = 5
        desc =
        {
          "So named for its attraction to fire and not for any ability of,",
					"flight, this spider hungrily eyes both you and your light source!",
        }
        color = color.GOLD
        speed = 10 life = {8,12} ac = 10
        aaf = 10 sleep = 30
        exp = 360 weight = 900
        blows =
        {
          {"STING","EAT_LITE",{8,4}},
          {"BITE","POISON",{9,5}},
        }
        flags =
        {
          RESIST=getter.resists{CRUSH=5 SLASH=6 FEAR=100 FIRE=66 LITE=-10}
          SMART=true MORTAL=true DROP_CORPSE=false
          AI = ai.SPELL_AND_MOVE
          AI_STATES=getter.flags
          {
            MOVE_AI=ai.ZOMBIE
            CASTING_AI=ai.SMART_CAST
          }          
        }
        spells =
        {
          frequency = 8
          ["Web"] = { level=5 chance=100 }  
        }                                                   
        
      }
      [6] =
      {
        name = "spitting spider"
        level = 38 rarity = 2
        desc =
        {
          "This spider has a modified venom gland that allows it to spit a",
					"combination of venom and webbing which envelopes and paralyzes",
					"its prey (you).",
        }
        color = color.DARK_GREEN
        speed = 15 life = {8,12} ac = 15
        aaf = 15 sleep = 30
        exp = 360 weight = 900
        blows =
        {
          {"SPIT","PARALYZE",{8,4}},
          {"BITE","POISON",{9,4}},
          {"BITE","POISON",{5,5}},
        }
        flags =
        {
          RESIST=getter.resists{CRUSH=5 SLASH=6 FEAR=100 FIRE=-5 LITE=-10}
          SMART=true MORTAL=true DROP_CORPSE=false 
          AI = ai.SPELL_AND_MOVE
          AI_STATES=getter.flags          
          {
            MOVE_AI=ai.ZOMBIE
            CASTING_AI=ai.SMART_CAST
          }          
        }
        spells =
        {
          frequency = 10
          ["Web"] = { level=5 chance=100 }   
          ["Breathe Poison"] = { level=40 chance=80 }
        }
      }
      
      [6] =
      {
        name = "nameless weaver"
        level = 45 rarity = 1
        desc =
        {
          "One of the oldest and most evil of all ",
          "of the spiders in Mirkwood.  Its sight strikes ",
          "fear in the hearts of all who behold it.",
        }
        color = color.RED --FIRE_BRICK
        speed = 20 life = {18,15} ac = 20
        aaf = 15 sleep = 30
        exp = 360 weight = 3200
        blows =
        {
          {"STING","PARALYZE",{8,4}},
          {"STING","HALLU",{5,4}},
          {"BITE","POISON",{8,5}},
          {"BITE","POISON",{8,5}},
          {"BITE","POISON",{8,5}},
        }
        flags =
        {
          RESIST=getter.resists{PIERCE=16 CRUSH=18 SLASH=18 FEAR=100 FIRE=100 COLD=100 VALINOREAN_FIRE=-20 DARK_FIRE=50 LITE=-10}
          SMART=true MORTAL=true DROP_CORPSE=true FORCE_MAXHP=true
          AI = ai.SPELL_AND_MOVE UNIQUE=true 
          MANA_CURR = 40 MANA_MAX = 40 MANA_REGENERATE=500
          AI_STATES=getter.flags
          {
            MOVE_AI=ai.ZOMBIE
            CASTING_AI=ai.SMART_CAST
          }          
        }
        spells =
        {
          frequency = 5
          ["Web"] = { level=80 chance=100 }
          ["Breathe Poison"] = {level=60 chance=100}
          ["Scare"] = { level=60 chance=80}
        }
      }
      
    }--end 'S'
-----------------------------------------------------------
--End Mirkwood monsters
-----------------------------------------------------------
}
