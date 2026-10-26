//	Vocabulary.cpp

/*
		Adventure vocabulary & encryption
*/

#include	"Adventure.h"	/* #define preprocessor equates	*/



typedef struct
{
	char	*aword;
	short	acode;
}
WAC;

// Each of the first three types (motion, verb and object)
// can have the same word name because of the search in English
// If a word such as "rod" is duplicated in the same type,
// Only the lowest numbered value will be found
// unless the search range is narrowed to exclude the lowest value.

// motion//0xxx
// verb//1xxx)
// Object//2xxx = 1000 + Object#
// Help and Do nothing//3xxx
// XXX//9xxx
WAC	wc[] =
{
					
//	Movements
					
	{"hill",			2},
	{"road",			2},
	{"enter",			3},
	{"downstream",		4},
	{"upstream",		5},
	{"forest",			6},
	{"forward",			7},
	{"onward",			7},
	{"retreat",			8},
	{"return",			8},
	{"back",			8},
	{"valley",			9},
	{"stairs",			10},
	{"outside",			11},
	{"leave",			11},
	{"exit",			11},
	{"out",				11},
	{"building",		12},
	{"house",			12},
	{"gully",			13},
	{"stream",			14},
	{"rock",			15},
	{"bed",				16},
	{"crawl",			17},
	{"cobblestone",		18},
	{"inside",			19},
	{"inward",			19},
	{"in",				19},
	{"surface",			20},
	{"nowhere",			21},
	{"null",			21},
	{"dark",			22},
	{"passage",			23},
	{"tunnel",			23},
	{"low",				24},
	{"canyon",			25},
	{"awkward",			26},
	{"giant",			27},
	{"view",			28},
	{"ascend",			29},
	{"upward",			29},
	{"above",			29},
	{"up",				29},
	{"u",				29},
	{"downward",		30},
	{"descend",			30},
	{"down",			30},
	{"d",				30},
	{"pit",				31},
	{"outdoors",		32},
	{"crack",			33},
	{"steps",			34},
	{"dome",			35},
	{"left",			36},
	{"right",			37},
	{"hall",			38},
	{"jump",			39},
	{"barren",			40},
	{"over",			41},
	{"across",			42},
	{"east",			43},
	{"e",				43},
	{"west",			44},
	{"w",				44},
	{"north",			45},
	{"n",				45},
	{"south",			46},
	{"s",				46},
	{"ne",				47},
	{"northeast",		47},
	{"se",				48},
	{"southeast",		48},
	{"sw",				49},
	{"southwest",		49},
	{"nw",				50},
	{"northwest",		50},
	{"debris",			51},
	{"hole",			52},
	{"wall",			53},
	{"broken",			54},
	{"y2",				55},
	{"climb",			56},
//	{"describe",		57}, // moved to verbs
	{"floor",			58},
	{"room",			59},
	{"slit",			60},
	{"slab",			61},
	{"xyzzy",			62},
	{"depression",		63},
	{"entrance",		64},
	{"plugh",			65},
	{"secret",			66},
	{"cave",			67},
	{"cross",			69},
	{"bedquilt",		70},
	{"plover",			71},
	{"oriental",		72},
	{"cavern",			73},
	{"shell",			74},
	{"reservoir",		75},
	{"office",			76},
	{"main",			76},
	{"fork",			77},
					
//	Verbs
					
	{"capture",			1001},
	{"carry",			1001},
	{"catch",			1001},
	{"steal",			1001},
	{"keep",			1001},
	{"take",			1001},
	{"tote",			1001},
	{"get",				1001},
	
	{"discard",			1002},
	{"release",			1002},
	{"drop",			1002},
	{"dump",			1002},
	{"free",			1002},
	{"put",				1002},
	
	{"mumble",			1003},
	{"chant",			1003},
	{"utter",			1003},
	{"sing",			1003},
	{"say",				1003},
	
	{"unlock",			1004},
	{"ul",				1004},
	{"un",				1004},
	{"u",				1004},		// only works when object is specified
	
	{"open",			1004},
	{"op",				1004},
	{"o",				1004},
	
	{"nothing",			1005},
	
	{"close",			1006},
	{"cl",				1006},
	{"c",				1006},
	{"lock",			1006},
	
	{"light",			1007},
	{"on",				1007},

	{"extinguish",		1008},
	{"off",				1008},
	
	{"shake",			1009},
	{"swing",			1009},
	{"wave",			1009},
	
	{"placate",			1010},
	{"calm",			1010},
	{"tame",			1010},
	
	{"continue",		1011},
	{"explore",			1011},
	{"proceed",			1011},
	{"follow",			1011},
	{"travel",			1011},
	{"go",				1011},
	{"goto",			1011},
	{"turn",			1011},
	{"walk",			1011},
	{"run",				1011},
	{"crawl",			1011},
	{"jump",			1011},
	
	{"attack",			1012},
	{"strike",			1012},
	{"fight",			1012},
	{"kill",			1012},
	{"hit",				1012},
	
	{"pour",			1013},
	{"empty",			1013},
	{"drip",			1013},
	{"squirt",			1013},
	
	{"devour",			1014},
	{"eat",				1014},
	{"consume",			1014},

	{"drink",			1015},
	{"chug",			1015},
	{"gulp",			1015},
	{"slurp",			1015},
	
	{"rub",				1016},
	{"clean",			1016},
	{"polish",			1016},
	
	{"throw",			1017},
	{"toss",			1017},
	{"pitch",			1017},
	
	{"quit",			1018},
	{"q",				1018},
	
	{"find",			1019},
	{"seek",			1019},
	
	{"inventory",		1020},
	{"inv",				1020},
	{"i",				1020},
	
	{"feed",			1021},

	{"fill",			1022},
	{"refill",			1022},
	
	{"detonate",		1023},
	{"blowup",			1023},
	{"ignite",			1023},
	{"blast",			1023},
								
	{"score",			1024},

	{"friendly",		1025},
	{"brief",			1026},
	
	{"read",			1027},
	{"peruse",			1027},
	
	{"break",			1028},
	{"destroy",			1028},
	{"shatter",			1028},
	{"smash",			1028},
	
	{"disturb",			1029},
	{"wake",			1029},
	{"scare",			1029},
									
	{"suspend",			1030},
	{"pause",			1030},

	{"hours",			1031},

	{"log",				1032},
									
	{"describe",		1033},
	{"examine",			1033},
	{"touch",			1033},
	{"look",			1033},
	{"l",				1033},
		
	{"water",			1037},		// aw, see if this works for water plant ???

	{"oil",				1038},		// aw, see if this works for oil door ???

	{"map",				1039},
	
	{"fee",				1040},		// fee fie foe foo must be consecutive in this order
	{"fie",				1041},
	{"foe",				1042},
	{"foo",				1043},
	{"fum",				1044},
									
	{"save",			1050},
	{"checkpoint",		1050},
	{"cp",				1050},

	{"rcp",				1051},
	{"restore",			1051},

	{"reset",			1052},
	{"restart",			1052},

	{"crypt",			1053},

	{"input",			1054},

	{"vocablength",		1055},
	{"vocabl",			1055},
	{"vocab",			1055},
	{"wordlength",		1055},
	{"wordl",			1055},

	{"hints",			1056},

	{"autocp",			1057},
	{"autocheckpoint",	1057},
	{"autosave",		1057},
					
//	Objects
									
	{"keys",			2001},
	{"key",				2001},
	{"lamp",			2002},
	{"lantern",			2002},
	{"grate",			2003},
	{"cage",			2004},
	{"rod",				2005},
	{"wand",			2005},
	{"rod",				2006},		// this (ROD2) must be rod+1 and is just here as a place holder
	{"steps",			2007},
	{"bird",			2008},
	{"door",			2009},
	{"pillow",			2010},
	{"velvet",			2010},
	{"snake",			2011},
	{"fissure",			2012},
	{"tablet",			2013},
	{"clam",			2014},
	{"oyster",			2015},
	{"spelunker today",	2016},
	{"spelunker",		2016},
	{"magazine",		2016},
	{"issue",			2016},
	{"dwarves",			2017},
	{"dwarf",			2017},
	{"knives",			2018},
	{"knife",			2018},
	{"rations",			2019},
	{"food",			2019},
	{"bottle",			2020},
	{"jar",				2020},
	{"water",			2021},
	{"h2o",				2021},
	{"oil",				2022},
	{"mirror",			2023},
	{"beans",			2024},
	{"plant",			2024},
	{"plant",			2025},		// this must be beans+1 and is just a place holder
	{"stalactite",		2026},
	{"figure",			2027},
	{"shadow",			2027},
	{"axe",				2028},
	{"drawing",			2029},
	{"pirate",			2030},
	{"dragon",			2031},
	{"chasm",			2032},
	{"troll",			2033},
	{"troll",			2034},
	{"bear",			2035},
	{"message",			2036},
	{"volcano",			2037},
	{"geyser",			2037},
	{"machine",			2038},
	{"vending",			2038},
	{"batteries",		2039},
	{"battery",			2039},
	{"carpet",			2040},
	{"moss",			2040},
	{"map",				2041},
	{"chart",			2041},
	{"tree",			2042},

// Treasures start here.

	{"nugget",			2050},
	{"gold",			2050},
	{"diamonds",		2051},
	{"silver",			2052},
	{"bars",			2052},
	{"jewelry",			2053},
	{"jewels",			2053},
	{"jewel",			2053},
	{"coins",			2054},
	{"treasure",		2055},
	{"chest",			2055},
	{"box",				2055},
	{"eggs",			2056},
	{"nest",			2056},
	{"egg",				2056},
	{"trident",			2057},
	{"pottery",			2058},
	{"shard",			2058},
	{"ming",			2058},
	{"vase",			2058},
	{"emerald",			2059},
	{"platinum",		2060},
	{"pyramid",			2060},
	{"pearl",			2061},
	{"persian",			2062},
	{"rug",				2062},
	{"spices",			2063},
	{"spice",			2063},
	{"chain",			2064},
	{"all",				2999},
					
//	Other
									
	{"abracadabra",		3050},
	{"opensesame",		3050},
	{"sesame",			3050},
	{"shazam",			3050},
	{"hocus",			3050},
	{"pocus",			3050},
	{"abra",			3050},
	{"help",			3051},
	{"?",				3051},
	{"trees",			3064},
	{"tree",			3064},
	{"excavate",		3066},
	{"dig",				3066},
	{"lost",			3068},
	{"mist",			3069},
	{"stop",			3139},
	{"information",		3142},
	{"info",			3142},
	{"swim",			3147},
	{"tport",			3148},
	{"tp",				3148},
	{"tgrab",			3149},
	{"tg",				3149},
	{"dwset",			3150},
	{"piset",			3151},
	{"nodie",			3152},


//	XXX cusswords -- I pulled this list off the internet
//                   and filtered it down to reasonable size
					
	{"anal",			9001},
	{"anus",			9001},
	{"ass",				9001},
	{"asshole",			9001},
	{"asskisser",		9001},
	{"asswipe",			9001},
	{"balls",			9001},
	{"bastard",			9001},
	{"beaver",			9001},
	{"bitch",			9001},
	{"bitty",			9001},
	{"blow",			9001},
	{"blowjob",			9001},
	{"bonehead",		9001},
	{"boner",			9001},
	{"boob",			9001},
	{"booger",			9001},
	{"breast",			9001},
	{"browneye",		9001},
	{"browntown",		9001},
	{"bufu",			9001},
	{"bull",			9001},
	{"bulldager",		9001},
	{"bullhell",		9001},
	{"bullshit",		9001},
	{"bum",				9001},
	{"bung",			9001},
	{"butt",			9001},
	{"buttface",		9001},
	{"buttfuck",		9001},
	{"buttfucker",		9001},
	{"butthead",		9001},
	{"butthole",		9001},
	{"buttpicker",		9001},
	{"chink",			9001},
	{"clit",			9001},
	{"clitoris",		9001},
	{"cobia",			9001},
	{"cock",			9001},
	{"cocksucker",		9001},
	{"cooter",			9001},
	{"cornhole",		9001},
	{"crap",			9001},
	{"crotch",			9001},
	{"cum",				9001},
	{"cumn",			9001},
	{"cunnilingus",		9001},
	{"cunt",			9001},
	{"cuntbreath",		9001},
	{"cuntdog",			9001},
	{"cuntsucker",		9001},
	{"damn",			9001},
	{"damned",			9001},
	{"damnit",			9001},
	{"damnmit",			9001},
	{"darn",			9001},
	{"defecate",		9001},
	{"defecation",		9001},
	{"dick",			9001},
	{"dickbreath",		9001},
	{"dickface",		9001},
	{"dickhead",		9001},
	{"dickless",		9001},
	{"dicksmack",		9001},
	{"dicksucker",		9001},
	{"dike",			9001},
	{"dildo",			9001},
	{"dilldoe",			9001},
	{"dilldos",			9001},
	{"dio",				9001},
	{"dipshit",			9001},
	{"dong",			9001},
	{"douche",			9001},
	{"drugs",			9001},
	{"dumb",			9001},
	{"dumbass",			9001},
	{"dumbbell",		9001},
	{"dyke",			9001},
	{"erection",		9001},
	{"fag",				9001},
	{"fagget",			9001},
	{"faggit",			9001},
	{"faggot",			9001},
	{"fagit",			9001},
	{"fart",			9001},
	{"farting",			9001},
	{"fatass",			9001},
	{"fatso",			9001},
	{"fellatio",		9001},
	{"feltching",		9001},
	{"friggin",			9001},
	{"fuck",			9001},
	{"fuckbutt",		9001},
	{"fucked",			9001},
	{"fucker",			9001},
	{"fuckface",		9001},
	{"fuckhead",		9001},
	{"fucking",			9001},
	{"fuckup",			9001},
	{"furburger",		9001},
	{"gazongers",		9001},
	{"genitalia",		9001},
	{"goddam",			9001},
	{"goddammit",		9001},
	{"goddamn",			9001},
	{"gonads",			9001},
	{"gook",			9001},
	{"hell",			9001},
	{"ho",				9001},
	{"hoar",			9001},
	{"homo",			9001},
	{"honker",			9001},
	{"honkey",			9001},
	{"honky",			9001},
	{"hooker",			9001},
	{"hooters",			9001},
	{"horny",			9001},
	{"hot",				9001},
	{"hotwet",			9001},
	{"hussy",			9001},
	{"jackass",			9001},
	{"jacking",			9001},
	{"jackoff",			9001},
	{"jap",				9001},
	{"jerk",			9001},
	{"jew",				9001},
	{"jizz",			9001},
	{"juice",			9001},
	{"keaster",			9001},
	{"kike",			9001},
	{"knob",			9001},
	{"kraut",			9001},
	{"lesbian",			9001},
	{"lesbo",			9001},
	{"lessy",			9001},
	{"lick",			9001},
	{"lier",			9001},
	{"lucifer",			9001},
	{"masterbate",		9001},
	{"masturbate",		9001},
	{"masturbating",	9001},
	{"masturbation",	9001},
	{"mother",			9001},
	{"motherfucker",	9001},
	{"muff",			9001},
	{"mutherfucker",	9001},
	{"nigger",			9001},
	{"nuts",			9001},
	{"pecker",			9001},
	{"peckerhead",		9001},
	{"penis",			9001},
	{"peter",			9001},
	{"pimp",			9001},
	{"piss",			9001},
	{"pissed",			9001},
	{"polack",			9001},
	{"poo",				9001},
	{"poontang",		9001},
	{"prick",			9001},
	{"prostitute",		9001},
	{"puntang",			9001},
	{"pussy",			9001},
	{"pussydick",		9001},
	{"pussylips",		9001},
	{"queer",			9001},
	{"quefe",			9001},
	{"quief",			9001},
	{"retard",			9001},
	{"rimjob",			9001},
	{"sadomasochism",	9001},
	{"schlong",			9001},
	{"schmeka",			9001},
	{"screw",			9001},
	{"scrog",			9001},
	{"sex",				9001},
	{"sexual",			9001},
	{"sexually",		9001},
	{"sheister",		9001},
	{"shit",			9001},
	{"shitass",			9001},
	{"shitface",		9001},
	{"shithead",		9001},
	{"shithloe",		9001},
	{"shithole",		9001},
	{"shiting",			9001},
	{"shlong",			9001},
	{"shmegma",			9001},
	{"shoot",			9001},
	{"shooting",		9001},
	{"shut",			9001},
	{"slackass",		9001},
	{"slag",			9001},
	{"sleeze",			9001},
	{"slit",			9001},
	{"slut",			9001},
	{"slutty",			9001},
	{"smagna",			9001},
	{"snatch",			9001},
	{"sodomy",			9001},
	{"sperm",			9001},
	{"spic",			9001},
	{"stain",			9001},
	{"strap",			9001},
	{"stupid",			9001},
	{"suck",			9001},
	{"sucker",			9001},
	{"suckes",			9001},
	{"sucking",			9001},
	{"taint",			9001},
	{"tit",				9001},
	{"tits",			9001},
	{"titty",			9001},
	{"tramp",			9001},
	{"transsexual",		9001},
	{"transvestite",	9001},
	{"tuna",			9001},
	{"twat",			9001},
	{"ugly",			9001},
	{"urinate",			9001},
	{"urination",		9001},
	{"urine",			9001},
	{"vagina",			9001},
	{"vertical",		9001},
	{"vitrator",		9001},
	{"wang",			9001},
	{"wanker",			9001},
	{"wetback",			9001},
	{"whacker",			9001},
	{"whore",			9001},
	{"wop",				9001},
					
};

#define NUM_VOCAB_WORDS	(sizeof(wc)/sizeof(wc[0]))

int	NumVocabWords()
{
	return NUM_VOCAB_WORDS;
}


int compareWc( const void *arg1, const void *arg2 )
{
	WAC* wac1 = (WAC*)arg1;
	WAC* wac2 = (WAC*)arg2;
	int nRc = _stricmp( wac1->aword,  wac2->aword );
	if ( nRc == 0 )
		nRc = wac1->acode - wac2->acode;
	return nRc;
}

void SortVocabTableWc()
{
	qsort( (void *)wc, NumVocabWords(), sizeof( wc[0] ), compareWc );
}

void SortVocabTables()
{
	static bool bFirst = true;
	if ( ! bFirst )
		return;
	SortVocabTableWc();
	bFirst = false;
}



short binary(char * w, WAC wctable[]
, short maxwc
, short nVocabCmpLen
)
{
	short	lo, mid, hi, check;
									
	lo = 0;
	hi = maxwc - 1;
	while (lo <= hi) 
	{
		mid = (lo + hi) / 2;
		check = strncmp(w, wctable[mid].aword, nVocabCmpLen);
		if ( check < 0)
			hi = mid - 1;
		else if (check > 0)
				lo = mid + 1;
		else
			return(mid);
	}
	return(-1);
}

short findMinWord(char * w, WAC wctable[]
, short maxwc
, short v1
, short minVal, short maxVal
, short nVocabCmpLen
)
{
	short v2 = v1;
	short wCmp = 0;
	short rCmp = 0;
	
	while ( --v2 >= 0 )
	{
		wCmp = strncmp(w, wctable[v2].aword, nVocabCmpLen);
		if ( wCmp != 0 )
			break;
		if ( wctable[v2].acode < minVal )
			break;
	}
	while ( ++v2 < maxwc )
	{
		wCmp = strncmp(w, wctable[v2].aword, nVocabCmpLen);
		if ( wCmp != 0 )
		{
			v2 = -1;
			break;
		}
		if ( wctable[v2].acode > maxVal )
		{
			v2 = -1;
			break;
		}
		if ( wctable[v2].acode >= minVal )
			break;
	}
	
	if ( v2 < maxwc )
		return v2;
	else
		return (-1);
}


/*
		look-up vocabulary word in lex-ordered table.
		word is the word to look up.
		minVal  is the minimum acceptable value,
		maxVal  is the maximum acceptable value.
		returns -1 if not found.
		returns the smalest acode within the range specified value if found.
*/


short vocab(char * word, short minVal, short maxVal)
{
	short	v1;
	short nVocabCmpLen = gameState.Get_VocabCmpLen(false);	 // set to 5 to be like orignal game
									
	SortVocabTables();
									
	if ((v1 = binary(word, wc, NumVocabWords(), nVocabCmpLen)) >= 0)
	{
		// word found
		v1 = findMinWord(word, wc, NumVocabWords(), v1, minVal, maxVal, nVocabCmpLen);
		if ( v1 >= 0 )
			return(wc[v1].acode);
		else
			return(-1);
	}
	else // word not found
	{
		return(-1);
	}
}

char* vocab(short val) // reverse lookup
{
	SortVocabTables();
					
	for ( int ii = 0; ii < NumVocabWords(); ++ ii )
	{
		if ( wc[ii].acode == val )
			return wc[ii].aword;
	}
	return NULL;
}
/*
		output adventure word list (motion/0xxx & verb/2xxx) only
		6 words/line pausing at 20th line until keyboard active
*/
void outwords()
{
	short	i, j, line;
	short	acode = 0;
									
	j = line = 0;
	for (i = 0; i < NumVocabWords(); ++i)
	{
		acode = wc[i].acode;
																		
		if ( acode < 0 )
			continue;
																		
		if ( acode < VOCAB_ENTRY_OBJ )	// display verbs and movements only.
		{
			printf("%-12s", wc[i].aword);
			if ((++j == 6) || (i == NumVocabWords()-1))
			{
				j = 0;
				puts("\n");
				if (++line == 20)
				{
					line = 0;
					if ( ! yes(MSG4_230_Continue) )
						break;
				}
			}
		}
	}
}


