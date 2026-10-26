#ifndef TravelCave_H
#define TravelCave_H

#include "TravelArray.h"



/* **************************************************************************************** */
//		Travel stuff
/* **************************************************************************************** */

/*
	Routine to fill travel array for a given location
*/
void gettrav( short loc, CTravelArray& refTravel );

/*
	Routine to copy a travel array
*/
void copytrv( const CTravelArray& trav1, CTravelArray& trav2 );

/*
	Routine to list a travel array
*/
void ListTravelArray( CTravelArray& trav, short loc /* just for display */ );

/*
		Routine to reset cave array 
*/
void	InitCaveTable();						


/* **************************************************************************************** */
//		Cave stuff
/* **************************************************************************************** */


/*
		Routine to fill one cave array movement entry
*/
bool	LoadCaveTableEntry( char* lineIn );


/*
		Routine returns the first location in the cave
*/
short	FirstCaveLocation();

/*
		Routine returns the last location in the cave
*/
short	LastCaveLocation();

/*
		Routine validate a location is in the cave
*/
bool	ValidCaveLocation( short loc );



/*
	Routine to output a map of the cave
*/
void cavemap();

/*
	Routine to output a map of one location
*/
void DisplayThisLocationMap( short loc, bool movements = true, bool objects = true, bool roomDesc = false, bool brief = true );

#define	LOC_Neg2_NO_CHANGE -2
#define	LOC_Neg1_FIXED -1
#define	LOC_000_MOVEABLE 0

#define	LOC_Neg1_PLAYER -1
#define	LOC_000_Limbo 0
#define LOC_001_FirstNormalEntry 1
#define LOC_001_end_road 1
#define LOC_002_hill_road 2
#define LOC_003_well_house 3
#define LOC_004_valley 4
#define LOC_005_forest 5
#define LOC_006_forest 6
#define LOC_007_slit_streambed 7
#define LOC_008_outside_grate 8
#define LOC_009_below_grate 9
#define LOC_010_cobble_crawl 10
#define LOC_011_debris_room 11				// first dark room
#define LOC_012_EW_canyon 12
#define LOC_013_bird_chamber 13
#define LOC_014_top_pit 14
#define LOC_015_hall_mists 15
#define LOC_016_crack 16
#define LOC_017_east_bank_fissure 17
#define LOC_018_nugget_gold_room 18
#define LOC_019_hall_mt_king 19
#define LOC_020_bottom_pit 20
#define LOC_021_didn_t_make_it 21
#define LOC_022_dome_unclimbable 22
#define LOC_023_west_end_twopit_room 23
#define LOC_024_east_pit 24
#define LOC_025_west_pit 25
#define LOC_026_clamber_plant 26
#define LOC_027_west_fissure_hall_mists 27
#define LOC_028_low_NS_passage 28
#define LOC_029_S_side_chamber 29
#define LOC_030_E_chamber_hall_mountain_king 30
#define LOC_031_GT_DOLLAR_LT 31
#define LOC_032_can_t_get_by_snake 32
#define LOC_033_Y2 33
#define LOC_034_jumble_rock 34
#define LOC_035_window_pit 35
#define LOC_036_dirty_passage 36
#define LOC_037_climbable_pit 37
#define LOC_038_bottom_pit_stream 38
#define LOC_039_rock_room 39
#define LOC_040_low_passage_p_n_hall_mists 40
#define LOC_041_west_end_hall_mists 41
#define LOC_042_maze 42
#define LOC_043_maze 43
#define LOC_044_maze 44
#define LOC_045_maze 45
#define LOC_046_Dead_end 46
#define LOC_047_Dead_end 47
#define LOC_048_Dead_end 48
#define LOC_049_maze 49
#define LOC_050_maze 50
#define LOC_051_maze 51
#define LOC_052_maze 52
#define LOC_053_maze 53
#define LOC_054_Dead_end 54
#define LOC_055_maze 55
#define LOC_056_Dead_end 56
#define LOC_057_brink_of_pit 57
#define LOC_058_Dead_end 58
#define LOC_059_low_passage_p_n_hall_mists 59
#define LOC_060_E_end_long_hall 60
#define LOC_061_W_end_long_hall 61
#define LOC_062_crossover_high_NS_passage_low_EW_one 62
#define LOC_063_Dead_end 63
#define LOC_064_complex_junction 64
#define LOC_065_bedquilt_passage 65
#define LOC_066_swiss_cheese_room 66
#define LOC_067_east_end_twopit_room 67
#define LOC_068_slab_room 68
#define LOC_069_secret_NS_canyon_above_large_room 69
#define LOC_070_secret_NS_canyon_above_sizable_passage 70
#define LOC_071_junction_three_secret_canyons 71
#define LOC_072_large_low_room 72
#define LOC_073_Dead_end_crawl 73
#define LOC_074_secret_EW_canyon 74
#define LOC_075_wide_place_NS_canyon 75
#define LOC_076_too_tight_canyon 76
#define LOC_077_tall_EW_canyon 77
#define LOC_078_canyon_dead_end 78
#define LOC_079_stream_flows_out_pair_1_foot_diameter_pipes 79
#define LOC_080_maze 80
#define LOC_081_Dead_end 81
#define LOC_082_Dead_end 82
#define LOC_083_maze 83
#define LOC_084_maze 84
#define LOC_085_Dead_end 85
#define LOC_086_Dead_end 86
#define LOC_087_maze 87
#define LOC_088_narrow_corridor 88
#define LOC_089_Use_up_or_out_leave_pit 89
#define LOC_090_climbed_plant_out_of_pit 90
#define LOC_091_steep_incline_above_large_room 91
#define LOC_092_giant_room 92
#define LOC_093_passage_here_blocked_by_recent_cave_in 93
#define LOC_094_one_end_northsouth_passage 94
#define LOC_095_cavern_waterfall 95
#define LOC_096_soft_room 96
#define LOC_097_oriental_room 97
#define LOC_098_misty_cavern 98
#define LOC_099_alcove 99
#define LOC_100_plover_room 100
#define LOC_101_dark_room 101
#define LOC_102_arched_hall 102
#define LOC_103_shell_room 103
#define LOC_104_corridor_ragged_sharp_walls 104
#define LOC_105_cul_de_sac 105
#define LOC_106_anteroom 106
#define LOC_107_maze 107
#define LOC_108_Witt_s_end 108
#define LOC_109_mirror_canyon 109
#define LOC_110_window_pit 110
#define LOC_111_top_stalactite 111
#define LOC_112_maze 112
#define LOC_113_reservoir 113
#define LOC_114_Dead_end 114
#define LOC_115_ne_end_repository 115
#define LOC_116_sw_end_repository 116
#define LOC_117_sw_chasm 117
#define LOC_118_sloping_corridor 118
#define LOC_119_secret_canyon 119
#define LOC_120_secret_canyon 120
#define LOC_121_secret_canyon 121
#define LOC_122_ne_chasm 122
#define LOC_123_corridor 123
#define LOC_124_fork_path 124
#define LOC_125_junction_warm_walls 125
#define LOC_126_breath_taking_view 126
#define LOC_127_chamber_boulders 127
#define LOC_128_limestone_passage 128
#define LOC_129_front_barren_room 129
#define LOC_130_barren_room 130
#define LOC_131_maze 131
#define LOC_132_maze 132
#define LOC_133_maze 133
#define LOC_134_maze 134
#define LOC_135_maze 135
#define LOC_136_maze 136
#define LOC_137_maze 137
#define LOC_138_maze 138
#define LOC_139_maze 139
#define LOC_140_Dead_end 140
#define LOC_141_forest		141
#define LOC_142_tree_house	142
#define LOC_143_main_office	143


#endif	//	#ifndef TravelCave_H
