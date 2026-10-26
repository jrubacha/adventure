#ifndef Vocabulary_h
#define Vocabulary_h

#define  VOCAB_TYP_NUL	-1
#define  VOCAB_TYP_MOV	0
#define  VOCAB_TYP_VRB	1
#define  VOCAB_TYP_OBJ	2
#define  VOCAB_TYP_OTH	3
#define  VOCAB_TYP_XXX	9
#define  VOCAB_TYP_ANY	999

#define  VOCAB_TYP_MUL	1000

#define  VOCAB_VAL_MIN	(1)
#define  VOCAB_VAL_MAX	(VOCAB_TYP_MUL-1)
#define  VOCAB_VAL_ANY	(0)

#define  VOCAB_ENTRY_MOV	(VOCAB_TYP_MUL*VOCAB_TYP_MOV)
#define  VOCAB_ENTRY_VRB	(VOCAB_TYP_MUL*VOCAB_TYP_VRB)
#define  VOCAB_ENTRY_OBJ	(VOCAB_TYP_MUL*VOCAB_TYP_OBJ)
#define  VOCAB_ENTRY_OTH	(VOCAB_TYP_MUL*VOCAB_TYP_OTH)
#define  VOCAB_ENTRY_XXX	(VOCAB_TYP_MUL*VOCAB_TYP_XXX)

#define  VOCAB_ENTRY_MIN	(0)
#define  VOCAB_ENTRY_MAX	(VOCAB_ENTRY_XXX+VOCAB_VAL_MAX)


// Prototypes from Vocabulary.CPP

short vocab(char* word, short minVal = VOCAB_ENTRY_MIN, short maxVal = VOCAB_ENTRY_MAX);
char* vocab(short val); // reverse lookup
void outwords();



// Movements

#define	 MOV_NONE 0
#define	 MOV_NOT_ALLOWED 1
#define	 MOV_HILL 2
#define	 MOV_ROAD 2
#define	 MOV_ENTER 3
#define	 MOV_DOWNSTREAM 4
#define	 MOV_UPSTREAM 5
#define	 MOV_FOREST 6
#define	 MOV_FORWARD 7
#define	 MOV_ONWARD 7
#define	 MOV_BACK 8
#define	 MOV_RETREAT 8
#define	 MOV_RETURN 8
#define	 MOV_VALLEY 9
#define	 MOV_STAIRS 10
#define	 MOV_OUTSIDE 11
#define	 MOV_LEAVE 11
#define	 MOV_EXIT 11
#define	 MOV_OUT 11
#define	 MOV_BUILDING 12
#define	 MOV_HOUSE 12
#define	 MOV_GULLY 13
#define	 MOV_STREAM 14
#define	 MOV_ROCK 15
#define	 MOV_BED 16
#define	 MOV_CRAWL 17
#define	 MOV_COBBLESTONE 18
#define	 MOV_INSIDE 19
#define	 MOV_INWARD 19
#define	 MOV_IN 19
#define	 MOV_SURFACE 20
#define	 MOV_NOWHERE 21
#define	 MOV_NULL 21
#define	 MOV_DARK 22
#define	 MOV_PASSAGE 23
#define	 MOV_TUNNEL 23
#define	 MOV_LOW 24
#define	 MOV_CANYON 25
#define	 MOV_AWKWARD 26
#define	 MOV_GIANT 27
#define	 MOV_VIEW 28
#define	 MOV_ASCEND 29
#define	 MOV_UPWARD 29
#define	 MOV_ABOVE 29
#define	 MOV_UP 29
#define	 MOV_U 29
#define	 MOV_DOWNWARD 30
#define	 MOV_DESCEND 30
#define	 MOV_DOWN 30
#define	 MOV_D 30
#define	 MOV_PIT 31
#define	 MOV_OUTDOORS 32
#define	 MOV_CRACK 33
#define	 MOV_STEPS 34
#define	 MOV_DOME 35
#define	 MOV_LEFT 36
#define	 MOV_RIGHT 37
#define	 MOV_HALL 38
#define	 MOV_JUMP 39
#define	 MOV_BARREN 40
#define	 MOV_OVER 41
#define	 MOV_ACROSS 42
#define	 MOV_EAST 43
#define	 MOV_E 43
#define	 MOV_WEST 44
#define	 MOV_W 44
#define	 MOV_NORTH 45
#define	 MOV_N 45
#define	 MOV_SOUTH 46
#define	 MOV_S 46
#define	 MOV_NE 47
#define	 MOV_SE 48
#define	 MOV_SW 49
#define	 MOV_NW 50
#define	 MOV_DEBRIS 51
#define	 MOV_HOLE 52
#define	 MOV_WALL 53
#define	 MOV_BROKEN 54
#define	 MOV_Y2 55
#define	 MOV_CLIMB 56
//#define	 MOV_LOOK 57
//#define	 MOV_DESCRIBE 57
//#define	 MOV_EXAMINE 57
//#define	 MOV_TOUCH 57
//#define	 MOV_L 57
#define	 MOV_FLOOR 58
#define	 MOV_ROOM 59
#define	 MOV_SLIT 60
#define	 MOV_SLAB 61
#define	 MOV_XYZZY 62
#define	 MOV_DEPRESSION 63
#define	 MOV_ENTRANCE 64
#define	 MOV_PLUGH 65
#define	 MOV_SECRET 66
#define	 MOV_CAVE 67
#define	 MOV_CROSS 69
#define	 MOV_BEDQUILT 70
#define	 MOV_PLOVER 71
#define	 MOV_ORIENTAL 72
#define	 MOV_CAVERN 73
#define	 MOV_SHELL 74
#define	 MOV_RESERVOIR 75
#define	 MOV_OFFICE 76
#define	 MOV_MAIN 76
#define	 MOV_FORK 77


// Verbs

#define	 VRB_NULL 0
#define	 VRB_TAKE 1
//#define	 VRB_CAPTURE 1
//#define	 VRB_CARRY 1
//#define	 VRB_CATCH 1
//#define	 VRB_STEAL 1
//#define	 VRB_KEEP 1
//#define	 VRB_TOTE 1
//#define	 VRB_GET 1
#define	 VRB_DROP 2
//#define	 VRB_DISCARD 2
//#define	 VRB_RELEASE 2
//#define	 VRB_DUMP 2
//#define	 VRB_FREE 2
#define	 VRB_SAY 3
//#define	 VRB_MUMBLE 3
//#define	 VRB_CHANT 3
//#define	 VRB_UTTER 3
//#define	 VRB_SING 3
#define	 VRB_OPEN 4
//#define	 VRB_UNLOCK 4
#define	 VRB_NOTHING 5
#define	 VRB_LOCK 6
//#define	 VRB_CLOSE 6
#define	 VRB_ON 7
//#define	 VRB_LIGHT 7
#define	 VRB_OFF 8
//#define	 VRB_EXTINGUISH 8
#define	 VRB_WAVE 9
//#define	 VRB_SHAKE 9
//#define	 VRB_SWING 9
#define	 VRB_CALM 10
//#define	 VRB_PLACATE 10
//#define	 VRB_TAME 10
#define	 VRB_WALK 11
//#define	 VRB_CONTINUE 11
//#define	 VRB_EXPLORE 11
//#define	 VRB_PROCEED 11
//#define	 VRB_FOLLOW 11
//#define	 VRB_TRAVEL 11
//#define	 VRB_GOTO 11
//#define	 VRB_TURN 11
//#define	 VRB_RUN 11
//#define	 VRB_GO 11
#define	 VRB_KILL 12
//#define	 VRB_ATTACK 12
//#define	 VRB_STRIKE 12
//#define	 VRB_FIGHT 12
//#define	 VRB_HIT 12
#define	 VRB_POUR 13
#define	 VRB_EAT 14
//#define	 VRB_DEVOUR 14
#define	 VRB_DRINK 15
#define	 VRB_RUB 16
#define	 VRB_THROW 17
//#define	 VRB_TOSS 17
#define	 VRB_QUIT 18
#define	 VRB_Q 18
#define	 VRB_FIND 19
#define	 VRB_INVENTORY 20
#define	 VRB_INV 20
#define	 VRB_I 20
#define	 VRB_FEED 21
#define	 VRB_FILL 22
#define	 VRB_BLAST 23
//#define	 VRB_DETONATE 23
//#define	 VRB_BLOWUP 23
//#define	 VRB_IGNITE 23
#define	 VRB_SCORE 24
#define	 VRB_FRIENDLY 25
#define	 VRB_BRIEF 26
#define	 VRB_READ 27
//#define	 VRB_PERUSE 27
#define	 VRB_BREAK 28
//#define	 VRB_SHATTER 28
//#define	 VRB_SMASH 28
#define	 VRB_WAKE 29
//#define	 VRB_DISTURB 29
#define	 VRB_SUSPEND 30
//#define	 VRB_PAUSE 30
#define	 VRB_HOURS 31
#define	 VRB_LOG 32

#define	 VRB_DESCRIBE 33
//#define	 VRB_EXAMINE 33
//#define	 VRB_TOUCH 33
//#define	 VRB_LOOK 33
//#define	 VRB_L 33

#define	 VRB_WATER 37
#define	 VRB_OIL 38

#define	 VRB_MAP 39

#define	 VRB_FEE 40
#define	 VRB_FIE 41
#define	 VRB_FOE 42
#define	 VRB_FOO 43
#define	 VRB_FUM 44

#define	 VRB_CHECKPOINT 50
//#define	 VRB_CP 50
//#define	 VRB_SAVE 50
#define	 VRB_RESTORE 51
//#define	 VRB_RCP 51
#define	 VRB_RESET 52
//#define	 VRB_RESTART 53
#define	 VRB_CRYPT 53
#define	 VRB_INPUT 54
#define	 VRB_VOCABLENGTH 55

#define	 VRB_HINTS 56

#define	 VRB_AUTOCP 57

// Objects
// Most listed in ADVENT3.TXT

#define  OBJ_NONE		(short)0
#define  OBJ_NOTHING	(short)0
#define	 OBJ_ALL		(short)999
#define	 OBJ_KEYS		(short)1
//#define	 OBJ_KEY 1
#define	 OBJ_LAMP		(short)2
//#define	 OBJ_HEADLAMP 2
//#define	 OBJ_LANTERN 2
#define	 OBJ_GRATE		(short)3
#define	 OBJ_CAGE		(short)4
#define	 OBJ_ROD		(short)5
#define	 OBJ_ROD2		(short)6
#define	 OBJ_STEPS		(short)7
#define	 OBJ_BIRD		(short)8
#define	 OBJ_DOOR		(short)9
#define	 OBJ_PILLOW		(short)10
//#define	 OBJ_VELVET			(short)10
#define	 OBJ_SNAKE		(short)11
#define	 OBJ_FISSURE	(short)12
#define	 OBJ_TABLET		(short)13
#define	 OBJ_CLAM		(short)14
#define	 OBJ_OYSTER		(short)15
#define	 OBJ_MAGAZINE	(short)16
//#define	 OBJ_SPELUNKERTODAY		(short)16
//#define	 OBJ_SPELUNKER			(short)16
//#define	 OBJ_ISSUE				(short)16
#define	 OBJ_DWARF		(short)17
//#define	 OBJ_DWARVES 17
#define	 OBJ_KNIFE		(short)18
//#define	 OBJ_KNIVES 18
#define	 OBJ_FOOD		(short)19
//#define	 OBJ_RATIONS 19
#define	 OBJ_BOTTLE		(short)20
//#define	 OBJ_JAR 20
#define	 OBJ_WATER		(short)21
//#define	 OBJ_H2O 21
#define	 OBJ_OIL		(short)22
#define	 OBJ_MIRROR		(short)23
#define	 OBJ_PLANT		(short)24
//#define	 OBJ_BEANS 24
#define	 OBJ_PLANT2		(short)25
#define	 OBJ_STALACTITE (short)26
#define	 OBJ_FIGURE		(short)27
//#define	 OBJ_SHADOW 27
#define	 OBJ_AXE		(short)28
#define	 OBJ_DRAWING	(short)29
#define	 OBJ_PIRATE		(short)30
#define	 OBJ_DRAGON		(short)31
#define	 OBJ_CHASM		(short)32
#define	 OBJ_TROLL		(short)33
#define	 OBJ_TROLL2		(short)34
#define	 OBJ_BEAR		(short)35
#define	 OBJ_MESSAGE	(short)36
#define	 OBJ_VOLCANO	(short)37
//#define	 OBJ_GEYSER 37
#define	 OBJ_VENDING	(short)38
//#define	 OBJ_MACHINE 38
#define	 OBJ_BATTERIES	(short)39
//#define	 OBJ_BATTERY 39
#define	 OBJ_CARPET		(short)40
//#define	 OBJ_MOSS 40
#define	 OBJ_MAP		(short)41
#define  OBJ_TREE		(short)42
// Treasures start here.

#define	 OBJ_NUGGET		(short)50
//#define	 OBJ_GOLD 50
#define	 OBJ_DIAMONDS	(short)51
#define	 OBJ_SILVER		(short)52
//#define	 OBJ_BARS 52
//#define	 OBJ_JEWELRY 53
#define	 OBJ_JEWELS		(short)53
//#define	 OBJ_JEWEL 53
#define	 OBJ_COINS		(short)54
#define	 OBJ_CHEST		(short)55
//#define	 OBJ_TREASURE 55
//#define	 OBJ_BOX 55
#define	 OBJ_EGGS		(short)56
//#define	 OBJ_NEST 56
//#define	 OBJ_EGG 56
#define	 OBJ_TRIDENT	(short)57
#define	 OBJ_VASE		(short)58
//#define	 OBJ_POTTERY 58
//#define	 OBJ_SHARD 58
//#define	 OBJ_MING 58
#define	 OBJ_EMERALD	(short)59
#define	 OBJ_PYRAMID	(short)60
//#define	 OBJ_PLATINUM	(short)60
#define	 OBJ_PEARL		(short)61
#define	 OBJ_RUG		(short)62
//#define	 OBJ_PERSIAN 62
#define	 OBJ_SPICES		(short)63
//#define	 OBJ_SPICE 63
#define	 OBJ_CHAIN		(short)64
#define	 OBJ_LAST_TREASURE	(short)64		/* OBJ # of last treasure, must be < 100*/


// Other misclanious words

//#define	 OTH_ABRACADABRA 50
//#define	 OTH_OPENSESAME 50
//#define	 OTH_SESAME 50
#define	 OTH_SHAZAM 50
//#define	 OTH_HOCUS 50
//#define	 OTH_POCUS 50
//#define	 OTH_ABRA 50
#define	 OTH_HELP 51
//#define	 OTH_? 51
#define	 OTH_TREES 64
#define	 OTH_TREE 64
#define	 OTH_EXCAVATE 66
#define	 OTH_DIG 66
#define	 OTH_LOST 68
#define	 OTH_MIST 69
#define	 OTH_STOP 139
#define	 OTH_INFORMATION 142
#define	 OTH_INFO 142
#define	 OTH_SWIM 147
#define	 OTH_TPORT 148
#define	 OTH_TGRAB 149
#define	 OTH_DWSET 150
#define	 OTH_PISET 151
#define	 OTH_NODIE 152

//	XXX

#define	 OTH_XXX 1


#endif	//	#ifndef Vocabulary_h
