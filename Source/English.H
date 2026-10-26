#ifndef ENGLISH_H
#define ENGLISH_H

/*
	English variables
*/
extern	short	e_verb, e_object, e_motion;
extern	char	e_word1[],e_word2[];

// Prototypes from ENGLISH.CPP

short english();
void ReportUnknowWord();
short analyze(char * word, short * type, short * value, bool bReportError, short lowWordVal = VOCAB_ENTRY_MIN, short hiWordVal = VOCAB_ENTRY_MAX);
void getwords();
void GetInputLine(char*	buff, short nMaxLen, bool bTolower = true);
void SetInputType( char* szTypeName );

#endif //	#ifndef ENGLISH_H