/* File: flags.h */

/* Purpose: defining the flags */

/*
 * Copyright (c) 2003 DarkGod
 *
 * This software may be copied and distributed for educational, research, and
 * not for profit purposes provided that this copyright and statement are
 * included in all such copies.
 */

/* Stats */
new_flag(STATS);
new_flag(SUST_STATS);

/* Misc bonus */
new_flag(INVIS);
new_flag(LIFE);
new_flag(MANA);
new_flag(SPELL);
new_flag(STEALTH);
new_flag(INFRA);
new_flag(SPEED);
new_flag(BLOWS);
new_flag(LUCK);			/* Luck += pval */

/* Melee */
new_flag(CHAOTIC);
new_flag(VAMPIRIC);
new_flag(VORPAL);
new_flag(IMPACT);
new_flag(CRIT);			/* More critical hits */
new_flag(WOUNDING);		/* Wounds monsters */

/* Res & cie */
new_flag(REFLECT);
new_flag(FREE_ACT);
new_flag(HOLD_LIFE);

new_flag(RESIST);
new_flag(ABSORB);
new_flag(SUSTAIN);     /* Ego-Item gives a Random Sustain */

/* Misc stuff */
new_flag(DECAY);
new_flag(WRAITH);
new_flag(EASY_KNOW);
new_flag(HIDE_TYPE);
new_flag(SHOW_MODS);
new_flag(INSTA_ART);
new_flag(FEATHER);
new_flag(LITE);
new_flag(SEE_INVIS);
new_flag(NORM_ART);
new_flag(REGEN);
new_flag(XTRA_MIGHT);
new_flag(XTRA_SHOTS);
new_flag(IGNORE);
new_flag(BLESSED);
new_flag(CANNOT_REMOVE);

/* Curses */
new_flag(AUTO_CURSE);
new_flag(NO_TELE);
new_flag(NO_MAGIC);
new_flag(FOUL_CURSE);
new_flag(DRAIN_EXP);
new_flag(TELEPORT);
new_flag(AGGRAVATE);
new_flag(CURSED);
new_flag(HEAVY_CURSE);
new_flag(PERMA_CURSE);
new_flag(BLACK_BREATH);		/* Tolkien's Black Breath */
new_flag(CLONE);		/* Can clone monsters */
new_flag(CURSE_NO_DROP);	/* The obj wont be dropped */
new_flag(NO_RECHARGE);		/* Object Cannot be recharged */
new_flag(TEMPORARY);		/* In timeout turns it is destroyed */

/* Misc again :) */
new_flag(PRECOGNITION);		/* Like activating the cheat mode */
new_flag(RECHARGE);		/* For artifact Wands and Staffs */
new_flag(FLY);			/* This one and ONLY this one allow you to fly over trees */
new_flag(COULD2H);		/* Can wield it 2 Handed */
new_flag(MUST2H);		/* Must wield it 2 Handed */
new_flag(LEVELS);		/* Can gain exp/exp levels !! */
new_flag(SPECIAL_GENE);		/* The object can only be generated in special conditions like quests, special dungeons, ... */
new_flag(CLIMB);		/* Allow climbing mountains */
new_flag(FAST_CAST);		/* Rod is x2 time faster to use */
new_flag(CAPACITY);		/* Rod can take x2 mana */
new_flag(CHARGING);		/* Rod recharge faster */
new_flag(CHEAPNESS);		/* Rod spells are cheaper(in mana cost) to cast */
new_flag(FOUNTAIN);		/* Available as fountain (for potions) */
new_flag(EASY_USE);		/* Easily activable */
new_flag(RECHARGED);		/* Object has been recharged once */
new_flag(ULTIMATE);		/* ULTIMATE artifact */
new_flag(FUEL_LITE);		/* fuelable lite */
new_flag(ART_EXP);		/* Will accumulate xp */
new_flag(ATTR_MULTI);		/* Object shimmer -- only allowed in k_info */
new_flag(FULL_NAME);		/* Uses direct name from k_info */
new_flag(BLINK_MOVE);		/* Move via phase door */
new_flag(SPELL_CONTAIN);	/* Can contain a spell */
new_flag(RES_MORGUL);		/* Is not shattered by morgul fiends(nazguls) */
new_flag(MAGIC_BREATH);		/* Can breath anywere */
new_flag(WATER_BREATH);		/* Can breath underwater */
new_flag(WIELD_CAST);		/* Need to be wielded to cast spells from it (if it can be wielded) */
new_flag(AMOUNT);               /* For a single item that has some amount */
new_flag(OBJ_FOUND);
/* ESP/monster races flags */
new_flag(ESP);
new_flag(ESP_HOSTILE);
new_flag(ESP_NEUTRAL);
new_flag(ESP_FRIENDLY);
new_flag(ESP_SLEEPING);
new_flag(ESP_AWAKE);

/*** Ego flags ***/

/*** Features flags -- DG ***/
new_flag(NO_WALK);
new_flag(NO_VISION);
new_flag(CAN_LEVITATE);
new_flag(CAN_PASS);
new_flag(MONST_CAN_ENTER);
new_flag(MONST_CAN_ATTACK);
new_flag(MONST_NO_PLACE);
new_flag(FLOOR);
new_flag(WALL);
new_flag(PERMANENT);
new_flag(REMEMBER);
new_flag(NOTICE);
new_flag(DONT_NOTICE_RUNNING);
new_flag(CAN_RUN);
new_flag(DOOR);
new_flag(SUPPORT_LIGHT);
new_flag(CAN_CLIMB);
new_flag(SUPPORT_GROWTH);

/*** Dungeon type flags -- DG ***/
new_flag(MAZE);	/* Is a maze-type dungeon */
new_flag(SMALLEST);	/* Creates VERY small levels like The Maze */
new_flag(SMALL);	/* Creates small levels like Dol Goldor */
new_flag(BIG);	/* Creates big levels like Moria, and Angband dungeons */
new_flag(NO_DOORS);	/* No doors on rooms, like Barrowdowns, Old Forest etc) */
new_flag(WATER_RIVER);	/* Allow a single water streamer on a level */
new_flag(LAVA_RIVER);	/* Allow a single lava streamer on a level */
new_flag(WATER_RIVERS);	/* Allow multiple water streamers on a level */
new_flag(LAVA_RIVERS);	/* Allow multiple lava streamers on a level */
new_flag(CAVE);	/* Allow rooms */
new_flag(CAVERN);	/* Allow cavern rooms */
new_flag(NO_UP);	/* Disallow up stairs */
new_flag(HOT);	/* Corpses on ground and in pack decay quicker through heat */
new_flag(COLD);	/* Corpses on ground and in pack decay quicker through cold */
new_flag(FORCE_DOWN);	/* No up stairs generated */
new_flag(NO_DESTROY);	/* No destroyed levels in dungeon */
new_flag(SAND_VEIN);	/* Like in the sandworm lair */
new_flag(CIRCULAR_ROOMS);	/* Allow circular rooms */
new_flag(EMPTY);	/* Allow arena levels */
new_flag(LITE_EMPTY);	/* Lite up empty arena levels */
new_flag(DAMAGE_FEAT);
new_flag(FLAT);	/* Creates paths to next areas at edge of level, like Barrowdowns */
new_flag(TOWER);	/* You start at bottom and go up rather than the reverse */
new_flag(RANDOM_TOWNS);	/* Allow random towns */
new_flag(DOUBLE);	/* Creates double-walled dungeon like Helcaraxe and Erebor */
new_flag(LIFE_LEVEL);	/* Creates dungeon level on modified 'game of life' algorithm */
new_flag(EVOLVE);	/* Evolving, pulsing levels like Heart of the Earth */
new_flag(ADJUST_LEVEL);	/* Minimum monster level will be equal to dungeon level */
new_flag(NO_RECALL);	/* No recall allowed */
new_flag(NO_STREAMERS);	/* No streamers */

new_flag(NO_SHAFT);	/* No shafts */
new_flag(ADJUST_LEVEL_PLAYER);	/* Uses player level*2 instead of dungeon level for other ADJUST_LEVEL flags */
new_flag(NO_TELEPORT);
new_flag(ASK_LEAVE);
new_flag(NO_STAIR);
new_flag(SPECIAL);
new_flag(NO_NEW_MONSTER);
new_flag(DESC);
new_flag(NO_GENO);
new_flag(NO_BREATH);     /* Oups, cannot breath here */
new_flag(ELVEN);     /* Try to create elven monster ego */
new_flag(DWARVEN);     /* Try to create dwarven monster ego */
new_flag(NO_EASY_MOVE);     /* Forbid stuff like teleport level, probability travel, ... */
new_flag(NO_RECALL_OUT);     /* Cannot recall out of the place */
new_flag(DESC_ALWAYS);     /* Always shows the desc */

/*
 * New monster race bit flags
 */
new_flag(UNIQUE);      /* Unique Monster */
new_flag(QUESTOR);      /* Quest Monster */
new_flag(MALE);      /* Male gender */
new_flag(FEMALE);      /* Female gender */
new_flag(CHAR_CLEAR);      /* Absorbs symbol */
new_flag(CHAR_MULTI);      /* Changes symbol */
new_flag(ATTR_CLEAR);      /* Absorbs color */
new_flag(FORCE_DEPTH);      /* Start at "correct" depth */
new_flag(FORCE_MAXHP);      /* Start with max hitpoints */
new_flag(FORCE_SLEEP);      /* Start out sleeping */
new_flag(FORCE_EXTRA);      /* Start out something */
new_flag(FRIEND);        /* Arrive with a friend */
new_flag(FRIENDS);       /* Arrive with some friends */
new_flag(ESCORT);              /* Arrive with an escort */
new_flag(ESCORT_DONT);         /* Monster shouldn't escort other monsters */
new_flag(ESCORT_FLAGS);        /* Desired flags for escorts */
new_flag(ESCORT_FLAGS_FORBID); /* Undesired flags for escorts */
new_flag(ESCORT_CHARS);        /* Desired display characters for escorts */
new_flag(ESCORT_NAME);         /* Desired name substring for escorts */
new_flag(ESCORT_MIN_LEVEL);    /* Minimum level for escorts */
new_flag(ESCORT_MAX_LEVEL);    /* Maximum level for escorts */
new_flag(ESCORT_IGNORE_DUNGEON);    /* Escorts ignore dungeon rules */
new_flag(ESCORTED_BY_GROUPS);  /* Force escorts to be groups of monsters */
new_flag(NEVER_BLOW);      /* Never make physical blow */
new_flag(NEVER_MOVE);      /* Never make physical move */
new_flag(RAND_MOVEMENT);      /* Moves randomly */
new_flag(ONLY_GOLD);      /* Drop only gold */
new_flag(ONLY_ITEM);      /* Drop only items */
new_flag(DROP_60);      /* Drop an item/gold (60%) */
new_flag(DROP_90);      /* Drop an item/gold (90%) */
new_flag(DROP_NUMBER);  /* Drop items/gold */
new_flag(DROP_GOOD);      /* Drop good items */
new_flag(DROP_GREAT);      /* Drop great items */
new_flag(DROP_USEFUL);      /* Drop "useful" items */
new_flag(DROP_CHOSEN);      /* Drop "chosen" items */

/*
 * New monster race bit flags
 */
new_flag(STUPID);      /* Monster is stupid */
new_flag(SMART);      /* Monster is smart */
new_flag(CAN_SPEAK);  /* TY: can speak */
new_flag(REFLECTING);      /* Reflects bolts */
new_flag(INVISIBLE);      /* Monster avoids vision */
new_flag(COLD_BLOOD);      /* Monster avoids infra */
new_flag(EMPTY_MIND);      /* Monster avoids telepathy */
new_flag(WEIRD_MIND);      /* Monster avoids telepathy? */
new_flag(REGENERATE);      /* Monster regenerates */
new_flag(SHAPECHANGER);  /* TY: shapechanger */
new_flag(ATTR_ANY);  /* TY: Attr_any */
new_flag(POWERFUL);      /* Monster has strong breath */
new_flag(ELDRITCH_HORROR);      /* Sanity-blasting horror    */
new_flag(OPEN_DOOR);      /* Monster can open doors */
new_flag(BASH_DOOR);      /* Monster can bash doors */
new_flag(PASS_WALL);      /* Monster can pass walls */
new_flag(KILL_WALL);      /* Monster can destroy walls */
new_flag(MOVE_BODY);      /* Monster can move monsters */
new_flag(KILL_BODY);      /* Monster can kill monsters */
new_flag(TAKE_ITEM);      /* Monster can pick up items */
new_flag(NO_TAKE_ITEM);   /* Can't pickup items when possesing this monster */
new_flag(KILL_ITEM);      /* Monster can crush items */

/*
 * New monster race bit flags
 */
new_flag(NONLIVING);  /* TY: Non-Living (?) */
new_flag(HURT_LITE);      /* Hurt by lite */
new_flag(HURT_ROCK);      /* Hurt by rock remover */
new_flag(IM_POIS);      /* Resist poison a lot */
new_flag(RES_TELE);      /* Resist teleportation */
new_flag(NO_FEAR);      /* Cannot be scared */
new_flag(NO_STUN);      /* Cannot be stunned */
new_flag(NO_CONF);      /* Cannot be confused */
new_flag(NO_SLEEP);      /* Cannot be slept */

/*
 * New monster race bit flags
 */
new_flag(MULTIPLY);  /* Monster reproduces */

/*
 * New monster race bit flags
 */
new_flag(AQUATIC);  /* Aquatic monster */
new_flag(CAN_SWIM);  /* Monster can swim */
new_flag(CAN_FLY);  /* Monster can fly */
new_flag(FRIENDLY);  /* Monster is friendly */
new_flag(PET);  /* Monster is a pet */
new_flag(MORTAL);  /* Monster is a mortal being */
new_flag(SPIDER);  /* Monster is a spider (can pass webs) */
new_flag(NAZGUL);  /* Monster is a Nazgul */
new_flag(MORG_CURSE);  /* If killed the monster grant a Morgothian Curse to the player */
new_flag(POSSESSOR);  /* Is it a dreaded possessor monster ? */
new_flag(NO_DEATH);  /* Cannot be killed */
new_flag(NO_TARGET);  /* Cannot be targeted */
new_flag(AI_ANNOY);  /* Try to tease the player */
new_flag(AI_HUNTER);  /* Follow the player by LOS tracking */
new_flag(AI_SPECIAL_MOVE);  /* For quests */
new_flag(AI_SPECIAL_TARGET);  /* For quests */
new_flag(LURE_FROM_CORRIDOR);
new_flag(NEUTRAL);  /* Monster is neutral */
new_flag(DROP_ART);  /* Monster drop one art */
new_flag(DROP_RANDART);  /* Monster drop one randart */
new_flag(AI_PLAYER);  /* Controlled by the player */
new_flag(NO_THEFT);  /* Monster is immune to theft */
new_flag(SPIRIT);  /* This is a Spirit, coming from the Void */
new_flag(IM_MELEE);  /* IM melee */


/*
 * Monster race flags
 */
new_flag(DUNGEON);
new_flag(WILD_ONLY);
new_flag(WILD_TOWN);
new_flag(WILD_SHORE);
new_flag(WILD_OCEAN);
new_flag(WILD_WASTE);
new_flag(WILD_WOOD);
new_flag(WILD_VOLCANO);
new_flag(WILD_MOUNTAIN);
new_flag(WILD_SWAMP);
new_flag(WILD_GRASS);
new_flag(NO_CUT);
new_flag(CTHANGBAND);
new_flag(BASEANGBAND);
new_flag(ZANGBAND);
new_flag(JOKEANGBAND);
new_flag(ANGBAND);

new_flag(WILD_TOO);


/*
 * Monster race flags
 */
new_flag(DROP_CORPSE);
new_flag(DROP_SKELETON);
new_flag(HAS_LITE);      /* Carries a lite */
new_flag(MIMIC);      /* *REALLY* looks like an object ... only nastier */
new_flag(HAS_EGG);      /* Can be monster's eggs */
new_flag(IMPRESED);      /* The monster can follow you on each level until he dies */
new_flag(WYRM_PROTECT);      /* The monster is protected by great wyrms of power: They'll be summoned if it's killed */
new_flag(DOPPLEGANGER);      /* The monster looks like you */
new_flag(ONLY_DEPTH);      /* The monster can only be generated at the GIVEN depth */
new_flag(NEVER_GENE);      /* The monster cannot be normaly generated */

new_flag(NOTE_DIES);   /* i.e., " is destroyed." instead of " is killed." */
new_flag(KILLED_VERB); /* i.e., "destroyed" instead of "killed" */
new_flag(SOUND_KILL);  /* Sound to make when killed */
new_flag(FORCE_MONST_NAME); /* String to force monster_desc() to use. */


#if 0
/*
 * Hack -- choose "intelligent" spells when desperate
 */

#define RF4_INT_MASK \
   (RF4_S_ANIMAL)

#define RF5_INT_MASK \
  (RF5_HOLD | RF5_SLOW | RF5_CONF | RF5_BLIND | RF5_SCARE)

#define RF6_INT_MASK \
   (RF6_BLINK |  RF6_TPORT | RF6_TELE_LEVEL | RF6_TELE_AWAY | \
    RF6_HEAL | RF6_HASTE | RF6_TRAPS | \
    RF6_S_KIN | RF6_S_HI_DEMON | RF6_S_MONSTER | RF6_S_MONSTERS | \
    RF6_S_ANT | RF6_S_SPIDER | RF6_S_HOUND | RF6_S_HYDRA | \
    RF6_S_ANGEL | RF6_S_DRAGON | RF6_S_UNDEAD | RF6_S_DEMON | \
    RF6_S_HI_DRAGON | RF6_S_HI_UNDEAD | RF6_S_WRAITH | RF6_S_UNIQUE | \
    RF6_S_THUNDERLORD | RF6_S_BUG | RF6_S_RNG | RF6_S_ANIMALS)


/*
 * Hack -- "bolt" spells that may hurt fellow monsters
 */
#define RF4_BOLT_MASK \
  (RF4_ARROW_1 | RF4_ARROW_2 | RF4_ARROW_3 | RF4_ARROW_4)

#define RF5_BOLT_MASK \
   (RF5_BO_ACID | RF5_BO_ELEC | RF5_BO_FIRE | RF5_BO_COLD | \
    RF5_BO_POIS | RF5_BO_NETH | RF5_BO_WATE | RF5_BO_MANA | \
    RF5_BO_PLAS | RF5_BO_ICEE | RF5_MISSILE)

#define RF6_BOLT_MASK \
   0L


/* Hack -- summon spells */

#define RF4_SUMMON_MASK \
    (RF4_S_ANIMAL)

#define RF5_SUMMON_MASK \
    0L

#define RF6_SUMMON_MASK \
    (RF6_S_KIN | RF6_S_HI_DEMON | RF6_S_MONSTER | RF6_S_MONSTERS | RF6_S_ANT | \
     RF6_S_SPIDER | RF6_S_HOUND | RF6_S_HYDRA | RF6_S_ANGEL | RF6_S_DEMON | \
     RF6_S_UNDEAD | RF6_S_DRAGON | RF6_S_HI_UNDEAD | RF6_S_HI_DRAGON | \
     RF6_S_WRAITH | RF6_S_UNIQUE | RF6_S_THUNDERLORD | RF6_S_BUG | RF6_S_RNG)
#endif



/*
 * Store flags
 */
new_flag(RARE);
new_flag(VERY_RARE);
new_flag(COMMON);
new_flag(ALL_ITEM);     /* Works as the BM */
new_flag(RANDOM);
new_flag(FORCE_LEVEL);
new_flag(MUSEUM);
new_flag(STORE_MAINTAIN_TURNS);


/* SKill flags */
new_flag(HIDDEN);      /* Starts hidden */
new_flag(AUTO_HIDE);      /* Tries to rehide at calc_bonus */
new_flag(RANDOM_GAIN);      /* Can be randomly gained by certain quests & such */

/* Race flags */
new_flag(EXPERIMENTAL);     /* Is still under developemnt */
new_flag(RESIST_BLACK_BREATH);     /* Resist black breath */
new_flag(XTRA_MIGHT_BOW);     /* Xtra might with bows */
new_flag(XTRA_MIGHT_SLING);     /* Xtra might with slings */
new_flag(AC_LEVEL);     /* More AC with levels */
new_flag(VAMPIRE);     /* Vampire */
new_flag(CORRUPT);     /* hack-- corrupted */
new_flag(NO_FOOD);     /* little gain from food */
new_flag(NO_GOD);     /* cannot worship */
new_flag(ELF);     /* Is an elf */
new_flag(SEMI_WRAITH);     /* Takes damage when going in walls */
new_flag(NO_SUBRACE_CHANGE);     /* Impossible to change subrace */
new_flag(ANTIMAGIC);     /* antimagic ... hack */
new_flag(MOLD_FRIEND);     /* Not attacked by molds wielded */
new_flag(GOD_FRIEND);     /* Better grace */
new_flag(INNATE_SPELLS);     /* KNown all spells, only need books */
new_flag(EASE_STEAL);     /* Gain xp by stealing */

#if 0
#define PRACE_FLAG2(f)          ((rp_ptr->flags2 | rmp_ptr->flags2 | cp_ptr->flags2 | spp_ptr->flags2) & (f))
#define PRACE_FLAG(f)           ((rp_ptr->flags1 | rmp_ptr->flags1 | cp_ptr->flags1 | spp_ptr->flags1) & (f))
#define PRACE_FLAGS(f)          PRACE_FLAG(f)
#endif

/* Monster corpse related flags */
new_flag(MONSTER_OBJ);
new_flag(MONSTER_DECAY);

/* Vairous specific flags */
new_flag(SPELL_IDX);
new_flag(STICK_BASE);
new_flag(STICK_MAX);
new_flag(CHARGE_NUM);
new_flag(ROD_TIP);
new_flag(EXPLODE);
new_flag(FLAG_MIMIC_IDX);
new_flag(ROD_MANA);
new_flag(GOLD_VALUE);
new_flag(FUEL);
new_flag(MUSIC);
new_flag(MIMIC_DUR);
new_flag(RUNE_TYPE);
new_flag(RUNE_TYPE2);
new_flag(RUNE_MANA);
new_flag(EGG_HATCH);
new_flag(CUMBER_GLOVES);
new_flag(JUNKART_ACTICATION);
new_flag(LITE_SUN);                     /* Shine like the sun(hurts sensible to lite) */

new_flag(DAM);

/* Redraw flags */
new_flag(PR_MISC);         /* Display Race/Class */
new_flag(PR_TITLE);                /* Display Title */
new_flag(PR_LEV);          /* Display Level */
new_flag(PR_EXP);          /* Display Experience */
new_flag(PR_STATS);                /* Display Stats */
new_flag(PR_ARMOR);                /* Display Armor */
new_flag(PR_HP);           /* Display Hitpoints */
new_flag(PR_MANA);         /* Display Mana */
new_flag(PR_GOLD);         /* Display Gold */
new_flag(PR_DEPTH);                /* Display Depth */
new_flag(PR_HEALTH);               /* Display Health Bar */
new_flag(PR_CUT);          /* Display Extra (Cut) */
new_flag(PR_STUN);         /* Display Extra (Stun) */
new_flag(PR_HUNGER);               /* Display Extra (Hunger) */
new_flag(PR_PIETY);                /* Display Piety */
new_flag(PR_BLIND);                /* Display Extra (Blind) */
new_flag(PR_CONFUSED);             /* Display Extra (Confused) */
new_flag(PR_AFRAID);               /* Display Extra (Afraid) */
new_flag(PR_POISONED);             /* Display Extra (Poisoned) */
new_flag(PR_STATE);                /* Display Extra (State) */
new_flag(PR_SPEED);                /* Display Extra (Speed) */
new_flag(PR_STUDY);                /* Display Extra (Study) */
new_flag(PR_SANITY);               /* Display Sanity */
new_flag(PR_EXTRA);                /* Display Extra Info */
new_flag(PR_BASIC);                /* Display Basic Info */
new_flag(PR_MAP);          /* Display Map */
new_flag(PR_WIPE);         /* Hack -- Total Redraw */
new_flag(PR_MH);           /* Display Monster hitpoints */
new_flag(PR_DTRAP);                /* Display Extra (DTrap) */

/* Must get the spells from a book */
new_flag(GET_BOOK_SPELLS);

new_flag(FORGET);

/* For traps and others */
new_flag(LEVEL);
new_flag(AFFECT_MONSTER);
new_flag(AFFECT_PLAYER);
new_flag(TRAP_CHAR);
new_flag(TRAP_ATTR);
new_flag(TRAP_POWER);
new_flag(CONSUME);
new_flag(TRAP);
new_flag(TRAP_IDX);
new_flag(TRAP_AWARE);

new_flag(BLESS_BLADE);
new_flag(RES_CONTINUUM);

new_flag(LEVEL_CHANGER);
new_flag(WATER);
new_flag(LAVA);
new_flag(QUEST_CHANGER);

new_flag(CHATABLE);		/* Monster is chatable, as such a "bump" will trigger chat, if not hostile */

new_flag(CANNOT_MOVE);		/* Cannot move, no DM-like powers */

new_flag(SPEEDS);               /* Individual speed modifiers */

new_flag(SAVE_LEVEL);		/* Level must be permanently saved */

new_flag(DUNGEON_GUARDIAN);		/* Dungeon guardian */
new_flag(DUNGEON_ARTIFACT);		/* Dungeon guarded art */
new_flag(DUNGEON_OBJECT);		/* Dungeon guarded obj */

new_flag(FACTION);		/* Default faction for that monster or trap*/
new_flag(APPARENT_FACTION);	/* Trap or monster disguising its faction */

new_flag(INVENTORY);		/* Contains other items ! */

new_flag(SKILL_BONUS);

new_flag(CONTAINS_BUILDING);	/* This spot contains a building entrance */
new_flag(TERRAIN_NAME);         /* Name to display for terrain at this spot */
new_flag(TERRAIN_TEXT);         /* Text to display for terrain at this spot */

new_flag(OPEN);                 /* This "door" (or object) is open */
new_flag(CLOSED);               /* This object is closed */
new_flag(JAMMED);               /* This "door" is jammed */
new_flag(DESTROY_INTO);         /* Turns into this when tunneled/destroyed/whatever */
new_flag(ALTAR);                /* This is a god's altar */
new_flag(LOCKED);               /* This "door" (or object) is locked */
new_flag(UNLOCKED);             /* This "door" has an unlocked lock */
new_flag(SECRET);               /* This "door" is secret */
new_flag(FIND_INTO);            /* Turns into this when secret is discovered*/

new_flag(DUNGEON_FORCE_MONSTER_EGO); /* The "dungeon" may force ego monsters by the use of this flag containing a function */
new_flag(ALWAYS_EGO); 		/* Always be an ego type */

new_flag(MULTIPLES);		 /* Generate multiple items */

new_flag(ON_MAKE);		/* What do do when this object is made */

new_flag(SPELLBOOK);		/* Contains spells */

new_flag(BLIND);		/* Blinded */
new_flag(FEAR);			/* Scared */
new_flag(PARALYZED);		/* Paralyzed */
new_flag(HALLUCINATE);          /* Hallucinating */
new_flag(CONFUSED);		/* Confused */

new_flag(AI_STATES);
new_flag(AI);

new_flag(ACTION);
new_flag(Y);
new_flag(X);

new_flag(SURFACE_LITE);

new_flag(ROOM_DEFINITION);

/* Window flags */
new_flag(PW_ALL);
new_flag(PW_M_LIST);
new_flag(PW_MESSAGE);
new_flag(PW_MONSTER);
new_flag(PW_OBJECT);
new_flag(PW_OVERHEAD);
new_flag(PW_INVEN);
new_flag(PW_EQUIP);
new_flag(PW_PLAYER);

new_flag(CHANCE);

new_flag(MONSTER_HORDE);	/* Allows monster hordes in this place, chance is randint(MONSTER_HORDE) <= dun_level */

new_flag(GENERATOR_GET_QUANTITY); /* Request quantity for dungeon generation */

new_flag(MONEY_PICKUP);

new_flag(NO_PUSHBACK);
