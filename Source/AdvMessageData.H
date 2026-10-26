#ifndef AdvMessageData_H
#define AdvMessageData_H

// Prototypes from AdvMessageData.CPP 

void	setTextFilePath( char* szPath );
void	IntAdvText();
bool	opentxt();
void	closetxt();
bool	load_advent_file_data(  bool bDisplayErrors );
void	CryptStr(char *szValue, int nLen = 1);
void	CryptAdvStr(char *szString);
void	crypt( char* forcedAction = NULL );

void	setLogFileName(const char *szFile );
void	deleteLogFile();

bool	appendTextFile(const char cText, int nLevel = 2, const char *szFile = NULL); 
bool	appendTextFile(const char *szText, int nLevel = 2, const char *szFile = NULL); 

/*
	Write a string to stdout.
*/
int puts( const char *string );
/*
	Print formatted output to the standard output stream.
*/
int	printf( const char* fmtStr, ...);

short	rdupto(FILE * fdi, char uptoc, char print, char * string, bool bNewLinePrefix = true);
void	rdskip(FILE * fdi, char skipc, short n, char rewind);

void	SpeakMsgFromAdventFile(short msgNdx, short	advNdx, char delim, short skip, bool bNewLinePrefix);

void	SpeakLocDescLong(short loc, bool bNewLinePrefix = true);	// Print a long m_location description from "advent1.txt"
void	SpeakLocDescShort(short loc, bool bNewLinePrefix = true);	// Print a short m_location description from "advent2.txt"
void	SpeakObjDesc(short item, short state, bool bNewLinePrefix = false);	// Print an object message for a given state from "advent3.txt"
void	SpeakInfoMsg(short msg, bool bNewLinePrefix = false);	// Print a message from "advent4.txt"
void	SpeakHelp(short msg, bool bNewLinePrefix = false);	// Print a help message from "advent5.txt"

short	yes(short msg1, short msg2 = 0, short msg3 = 0, char defaultAnswer = 'y');

#define MSG4_000_No_Message 0
#define MSG4_001_Colossal_Cave_Intro 1
#define MSG4_002_dwarf_knife_blocks_way 2
#define MSG4_003_dwarf_throws_axe_at_you 3
#define MSG4_004_threatening_dwarf_in_room_you 4
#define MSG4_005_One_sharp_nasty_knife_thrown_you 5
#define MSG4_006_None_hit_you 6
#define MSG4_007_One_gets_you 7
#define MSG4_008_hollow_voice_says_Plugh 8
#define MSG4_009_There_no_way_that_direction 9
#define MSG4_010_Use_compass_points_or_nearby_objects 10
#define MSG4_011_Use_compass_points_or_name_something 11
#define MSG4_012_don_t_know_how_apply_that_here 12
#define MSG4_013_don_t_understand_that 13
#define MSG4_014_Would_you_care_explain_how 14
#define MSG4_015_not_allowed_more_detail_will_repeat_description 15
#define MSG4_016_pitch_dark_If_proceed_likely_fall_into_pit 16
#define MSG4_017_type_W_rather_than_West 17
#define MSG4_018_trying_catch_bird 18
#define MSG4_019_bird_frightened_cannot_catch 19
#define MSG4_020_Are_you_trying_deal_with_snake 20
#define MSG4_021_can_t_kill_snake 21
#define MSG4_022_really_want_quit_now 22
#define MSG4_023_fell_into_pit_broke_every_bone_body 23
#define MSG4_024_already_carrying_it 24
#define MSG4_025_You_can_t_be_serious 25
#define MSG4_026_bird_was_unafraid_when_you_entered 26
#define MSG4_027_can_catch_bird_but_not_carry_it 27
#define MSG4_028_nothing_here_to_lock 28
#define MSG4_029_not_carrying_it 29
#define MSG4_030_bird_attacks_green_snake_drives_away 30
#define MSG4_031_no_keys 31
#define MSG4_032_no_lock 32
#define MSG4_033_don_t_know_how_lock_or_unlock_such_thing 33
#define MSG4_034_already_locked 34
#define MSG4_035_grate_now_locked 35
#define MSG4_036_grate_now_unlocked 36
#define MSG4_037_already_unlocked 37
#define MSG4_038_no_source_light 38
#define MSG4_039_lamp_now_on 39
#define MSG4_040_lamp_now_off 40
#define MSG4_041_no_way_past_bear_unlock_chain 41
#define MSG4_042_Nothing_happens 42
#define MSG4_043_Where 43
#define MSG4_044_There_nothing_here_attack 44
#define MSG4_045_bird_now_dead_Its_body_disappears 45
#define MSG4_046_Attacking_snake_doesn_t_work_very_dangerous 46
#define MSG4_047_You_killed_dwarf 47
#define MSG4_048_You_attack_dwarf_he_dodges_out_way 48
#define MSG4_049_what_with_bare_hands 49
#define MSG4_050_old_worn_out_magic_word 50
#define MSG4_051_I_know_places_actions_things 51
#define MSG4_052_It_misses 52
#define MSG4_053_It_gets_you 53
#define MSG4_054_OK 54
#define MSG4_055_can_t_unlock_keys 55
#define MSG4_056_trying_to_find_cave 56
#define MSG4_057_don_t_know_where_cave_is_follow_stream 57
#define MSG4_058_need_more_detailed_instructions 58
#define MSG4_059_only_tell_you_what_you_see 59
#define MSG4_060_don_t_know_word 60
#define MSG4_061_What 61
#define MSG4_062_trying_get_into_cave 62
#define MSG4_063_cannot_enter_without_key_no_keys_nearby 63
#define MSG4_064_forest 64
#define MSG4_065_Welcome_ADVENTURE_Original_dev_by_Willie_Crowther 65
#define MSG4_066_Digging_without_shovel_impractical 66
#define MSG4_067_Blasting_requires_dynamite 67
#define MSG4_068_I_m_as_confused_as_you_are 68
#define MSG4_069_Mist_white_vapor 69
#define MSG4_070_feet_now_wet 70
#define MSG4_071_lost_my_appetite 71
#define MSG4_072_Thank_you_delicious 72
#define MSG4_073_You_drink_from_stream_water 73
#define MSG4_074_bottle_now_empty 74
#define MSG4_075_Rubbing_electric_lamp 75
#define MSG4_076_Nothing_happens 76
#define MSG4_077_bottle_empty_ground_wet 77
#define MSG4_078_You_can_t_pour_that 78
#define MSG4_079_Watch_it 79
#define MSG4_080_Which_way 80
#define MSG4_081_killed_might_help_you_out 81
#define MSG4_082_don_t_blame_me 82
#define MSG4_083_clumsy_oaf_you_ve_done_it_again 83
#define MSG4_084_orange_smoke 84
#define MSG4_085_without_orange_smoke_don_t_expect_decent_reincarnation 85
#define MSG4_086_Okay_If_you_re_so_smart_do_it_yourself 86
#define MSG4_087_Hint_Offer 87
#define MSG4_088_crawled_around_holes_back_in_main_passage 88
#define MSG4_089_Please_answer_the_question 89
#define MSG4_090_Reserved 90
#define MSG4_091_dont_remember_how_you_got_here 91
#define MSG4_092_can_t_carry_anything_more_have_drop_something_first 92
#define MSG4_093_can_t_through_locked_steel_grate 93
#define MSG4_094_what_you_want_right_here 94
#define MSG4_095_don_t_fit_through_two_inch_slit 95
#define MSG4_096_suggest_you_across_bridge_instead_jumping 96
#define MSG4_097_There_no_way_across_fissure 97
#define MSG4_098_You_re_not_carrying_anything 98
#define MSG4_099_You_are_currently_holding_following 99
#define MSG4_100_It_s_not_hungry_and_You_have_no_bird_seed 100
#define MSG4_101_snake_has_now_devoured_bird 101
#define MSG4_102_nothing_here_it_wants_eat_Except_you 102
#define MSG4_103_fool_Dwarves_eat_only_coal 103
#define MSG4_104_have_nothing_carry_it 104
#define MSG4_105_bottle_already_full 105
#define MSG4_106_nothing_here_fill_bottle 106
#define MSG4_107_bottle_now_full_water 107
#define MSG4_108_bottle_now_full_oil 108
#define MSG4_109_can_t_fill_that 109
#define MSG4_110_don_t_be_ridiculous 110
#define MSG4_111_rusty_door_wont_open 111
#define MSG4_112_plant_shakes_oil_off_wants_Water 112
#define MSG4_113_hinges_are_rusted_now_won_t_budge 113
#define MSG4_114_oil_freed_up_hinges_door_will_open_now 114
#define MSG4_115_plant_deep_roots_cannot_be_pulled_free 115
#define MSG4_116_Dwarves_knives_vanish_as_they_strike_walls 116
#define MSG4_117_Something_you_re_carrying_won_t_fit_through_tunnel 117
#define MSG4_118_can_t_fit_this_five_foot_clam_through_that_passage 118
#define MSG4_119_can_t_fit_this_five_foot_oyster_through_that_passage 119
#define MSG4_120_advise_put_down_clam_before_opening_it_STRAIN 120
#define MSG4_121_advise_put_down_oyster_before_opening_it_WRENCH 121
#define MSG4_122_don_t_have_anything_to_open_clam 122
#define MSG4_123_don_t_have_anything_to_open_oyster 123
#define MSG4_124_pearl_falls_out_of_clam 124
#define MSG4_125_no_pearl_in_oyster 125
#define MSG4_126_blocked_by_recent_cave_in 126
#define MSG4_127_rustling_noises_behind_you 127
#define MSG4_128_from_shadows_pirate_takes_all_booty 128
#define MSG4_129_Cave_closing_soon_please_exit_now 129
#define MSG4_130_exit_closed_leave_via_main_office 130
#define MSG4_131_you_re_dead_it_s_close_closing_time_call_it_a_day 131
#define MSG4_132_cave_now_closed 132
#define MSG4_133_loud_explosion_hole_You_can_exit 133
#define MSG4_134_loud_explosion_burying_everything 134
#define MSG4_135_loud_explosion_you_are_splashed_across_walls 135
#define MSG4_136_several_awakened_Dwarves_in_room 136
#define MSG4_137_leave_poor_bird_alone 137
#define MSG4_138_whatever_you_want_is_here_somewhere 138
#define MSG4_139_don_t_know_word_stop_Use_quit_if_you_want_give_up 139
#define MSG4_140_can_t_get_there_from_here 140
#define MSG4_141_followed_by_tame_bear 141
#define MSG4_142_end_adventure_say_quit_or_suspend 142
#define MSG4_143_you_wish_quit_now 143
#define MSG4_144_nothing_here_to_fill_vase 144
#define MSG4_145_temperature_change_shattered_vase 145
#define MSG4_146_beyond_power_do 146
#define MSG4_147_I_don_t_know_how 147
#define MSG4_148_too_far_up_to_reach 148
#define MSG4_149_killed_Dwarf_vanished_smoke 149
#define MSG4_150_shell_very_strong_impervious 150
#define MSG4_151_can_t_you_read_start_over 151
#define MSG4_152_axe_bounces_off_dragon_scales 152
#define MSG4_153_dragon_nasty_you_d_best_not_try_get_by 153
#define MSG4_154_bird_attacks_dragon_gets_burnt 154
#define MSG4_155_On_what 155
#define MSG4_156_I_ll_only_describe_place_full_first_time 156
#define MSG4_157_Trolls_skin_as_tough_as_rhinoceros 157
#define MSG4_158_troll_catches_axe_tosses_it_back 158
#define MSG4_159_troll_catches_treasure_scurries_away 159
#define MSG4_160_troll_refuses_let_you_cross 160
#define MSG4_161_no_longer_any_way_across_chasm 161
#define MSG4_162_as_you_reach_other_side_bridge_buckles 162
#define MSG4_163_bear_lumbers_toward_troll_who_shriek 163
#define MSG4_164_axe_misses_lands_near_bear_where_you_can_t_get_it 164
#define MSG4_165_what_your_bare_hands_Agains_HIS_bear_hands 165
#define MSG4_166_bear_wants_to_be_friend 166
#define MSG4_167_For_crying_out_loud_poor_thing_already_dead 167
#define MSG4_168_bear_wolfs_down_food_becomes_friendly 168
#define MSG4_169_bear_still_chained_wall 169
#define MSG4_170_chain_still_locked 170
#define MSG4_171_chain_now_unlocked 171
#define MSG4_172_chain_now_locked 172
#define MSG4_173_nothing_here_chain_can_be_locked 173
#define MSG4_174_nothing_here_eat 174
#define MSG4_175_want_hint 175
#define MSG4_176_need_help_getting_out_maze 176
#define MSG4_177_make_passages_look_less_alike_by_dropping_things 177
#define MSG4_178_trying_explore_beyond_plover_room 178
#define MSG4_179_no_way_explore_region_without_falling_pit 179
#define MSG4_180_need_help_getting_out_wits_end 180
#define MSG4_181_Don_t_go_west 181
#define MSG4_182_Gluttony_not_Troll_vice 182
#define MSG4_183_lamp_getting_dim_best_wrap_up_or_find_batteries 183
#define MSG4_184_lamp_has_run_out_power 184
#define MSG4_185_no_point_wandering_out_here_So_let_s_call 185
#define MSG4_186_rustling_noises_from_darkness 186
#define MSG4_187_lamp_getting_dim_go_back_for_batteries 187
#define MSG4_188_lamp_getting_dim_replacing_batteries 188
#define MSG4_189_lamp_getting_dim_no_spare_batteries 189
#define MSG4_190_magazine_in_Dwarvish 190
#define MSG4_191_not_maze_where_pirate_treasure_chest 191
#define MSG4_192_looks_like_clue_cost_you_10_points 192
#define MSG4_193_says_something_strange_about_this_place 193
#define MSG4_194_says_same_thing_as_before 194
#define MSG4_195_don_t_understand 195
#define MSG4_196_Congratulations_bringing_light_into_dark_room 196
#define MSG4_197_You_strike_mirror_shatters 197
#define MSG4_198_vase_hurled_ground 198
#define MSG4_199_woke_nearest_Dwarf 199
#define MSG4_200_Is_this_acceptable 200
#define MSG4_201_no_point_suspending_demo_game 201
#define MSG4_202_Rank_Amateur 202
#define MSG4_203_novice_class_Adventurer 203
#define MSG4_204_seasoned_adventurer 204
#define MSG4_205_junior_master 205
#define MSG4_206_Master_Adventurer_class_C 206
#define MSG4_207_Master_Adventurer_class_B 207
#define MSG4_208_Master_Adventurer_class_A 208
#define MSG4_209_Adventurer_Grandmaster 209
#define MSG4_210_Adventurer_Wizzerd  210
#define MSG4_211_Reserved 211
#define MSG4_212_Reserved 212
#define MSG4_213_Reserved 213
#define MSG4_214_Reserved 214
#define MSG4_215_Handle_with_care 215
#define MSG4_216_Cost_15_points_open_map 216
#define MSG4_217_Map_out_of_date 217
#define MSG4_218_Proably_good_choice 218
#define MSG4_219_Reserved 219
#define MSG4_220_BriefMessageOff 220
#define MSG4_221_BriefMessageOn 221
#define MSG4_222_FriendlyOff 222
#define MSG4_223_FriendlyOn 223
#define MSG4_224_LogToFileOff 224
#define MSG4_225_LogToFileL1 225
#define MSG4_226_LogToFileL2 226
#define MSG4_227_Reserved 227
#define MSG4_228_CaveOpen_24_7_ForNow 228
#define MSG4_229_Reserved 229
#define MSG4_230_Continue 230
#define MSG4_231_VocabLen5 231
#define MSG4_232_VocabLen20 232
#define MSG4_233_OfferHintsOff 233
#define MSG4_234_OfferHintsOn 234
#define MSG4_235_AutoSaveOff 235
#define MSG4_236_AutoSaveOn 236

#endif	//	#ifndef AdvMessageData_H
