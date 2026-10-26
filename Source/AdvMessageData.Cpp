//	AdvMessageData.cpp

#include "Adventure.h"
#include "LongIntArray.h"
#include "GameState.h"
#include "CaveArray.h"


/*
        File definitions
*/

char szTextFilePath[_MAX_PATH+1] = "";


char	rawFileSpec[_MAX_PATH+1];
char	crptFileSpec[_MAX_PATH+1];

void	MakeFileName( const char* szFile, const char* szExt, char* szFileSpec )
{
	strcpy( szFileSpec, szTextFilePath );
	strcat( szFileSpec, szFile );
	strcat( szFileSpec, szExt );
}

char*	szFnExtRaw = ".TXT";
char*	szFnExtCrpt = ".DAT";


const char* adv_file_names[] = {
	"ADVENT0",
	"ADVENT1",
	"ADVENT2",
	"ADVENT3",
	"ADVENT4",
	"ADVENT5"
};

#define NUM_ADV_FILES (sizeof(adv_file_names) / sizeof(adv_file_names[0]))

FILE* fdsDatIn[NUM_ADV_FILES];

CLongIntArray idx[NUM_ADV_FILES];

#define ADVENT0	0
#define ADVENT1	1
#define ADVENT2	2
#define ADVENT3	3
#define ADVENT4	4
#define ADVENT5	5

void	setTextFilePath( char* szPath )
{
	int nStrLen = strlen( szPath );
	char szDelim[2] = "\\";

	strcpy( szTextFilePath, szPath );

	if ( nStrLen == 0 )
		return;

	if ( szTextFilePath[nStrLen-1] == szDelim[0] )
		return;

	if ( stricmp( &szTextFilePath[nStrLen-4], ".EXE" ) == 0)
	{
		char* pDelim = strrchr(szTextFilePath, szDelim[0] );

		if ( pDelim )
			*pDelim = '\0';
		else
			szTextFilePath[0] = '\0';
	}

	nStrLen = strlen( szTextFilePath );

	if ( nStrLen > 0 )
		strcat( szTextFilePath, szDelim );
}

void IntAdvText()
{
	static bool bFirstTime = true;
										
	if ( !bFirstTime )
		return;
										
	for ( int fdIdx = 0; fdIdx < NUM_ADV_FILES; ++fdIdx )
		fdsDatIn[fdIdx] = NULL;
										
	bFirstTime = false;
}



bool load_advent_file_data(  bool bDisplayErrors, const char* szFileName, CLongIntArray& idxCur, short advNdx )
{
	bool	bRc = true;
	char	itxt[255];
	long	lFileOffset = -1;
	int		nPrevEntryNumber = 0;
	FILE* fdCur = NULL;
						
	fdCur = fopen( szFileName, "rb" );
	if ( fdCur == NULL )
	{
		if ( bDisplayErrors )
			printf( "Sorry, I can't open %s...\n", szFileName );
		return false;
	}
	
	idxCur.ResetMemory();

	do
	{
		memset( itxt, 0, sizeof(itxt) );
		fgets( itxt, sizeof(itxt), fdCur);

		CryptAdvStr( itxt );

		if (itxt[0] == '#')
		{
			int nEntryNumber = atoi(&itxt[1]);
			long lPrevOffset = 0;

			if ( nEntryNumber < nPrevEntryNumber )
			{
				printf( "Error reading file (%s) %s < %d.\n", szFileName, itxt, nPrevEntryNumber );
				bug(BUG_AdvMessageData_Cpp+11);
				return false;
			}

			if ( lPrevOffset = idxCur[nEntryNumber] != 0 )
			{
				printf( "Error reading file (%s) %s duplicated.\n", szFileName, itxt );
				bug(BUG_AdvMessageData_Cpp+12);
				return false;
			}

			if ( advNdx >= ADVENT1 )
			{
				lFileOffset = ftell(fdCur);
				idxCur[nEntryNumber] = lFileOffset;
			}
			nPrevEntryNumber = nEntryNumber;
		}				/* if (itxt[0])	*/
		else if ( advNdx == ADVENT0 )
		{
			if (itxt[0] == '!')
				gameState.LoadStateTableEntry( itxt );
			else if (itxt[0] == '~')
				LoadCaveTableEntry( itxt );
		}
	}					/* while fgets	*/
	while ( !feof(fdCur) && !ferror(fdCur) );
										
	if ( ferror(fdCur) )
	{
		if ( bDisplayErrors )
			printf( "Error reading file (%s).\n", szFileName );
		bRc = false;
	}
	if ( fdCur != NULL )
	{
		fclose(fdCur);
		fdCur = NULL;
	}
										
	return bRc;
}

bool load_advent_file_data( bool bDisplayErrors )
{
	bool	bRc = true;
										
	for ( int fdIdx = 0; fdIdx < NUM_ADV_FILES; ++fdIdx )
	{
		MakeFileName( adv_file_names[fdIdx], szFnExtCrpt, crptFileSpec );
		bRc &= load_advent_file_data( bDisplayErrors, crptFileSpec, idx[fdIdx], fdIdx );
	}
										
	return bRc;
}

/*
		Open advent?.txt files
*/
bool opentxt()
{
	bool bRc = true;
	for ( int fdIdx = 0; fdIdx < NUM_ADV_FILES; ++fdIdx )
	{
		if ( fdsDatIn[fdIdx] == NULL)
		{
			MakeFileName( adv_file_names[fdIdx], szFnExtCrpt, crptFileSpec );
			fdsDatIn[fdIdx] = fopen(crptFileSpec, "r");
			if ( fdsDatIn[fdIdx] == NULL)
			{
				printf("Sorry, I can't open %s...\n", crptFileSpec);
				closetxt();
				bRc = false;
//				exitAdv(1);
			}
		}
		else
		{
			printf("Sorry, %s is already open...\n", crptFileSpec);
			bRc = false;
		}
	}
	return bRc;
}

/*
		Close advent?.txt files
*/
void closetxt()
{
	for ( int fdIdx = 0; fdIdx < NUM_ADV_FILES; ++fdIdx )
	{
		if ( fdsDatIn[fdIdx] != NULL )
		{
			if (fclose(fdsDatIn[fdIdx])  ==  -1)
			{
				MakeFileName( adv_file_names[fdIdx], szFnExtCrpt, crptFileSpec );
				printf("Sorry, I can't close the file...%s\n", crptFileSpec);
			}
			fdsDatIn[fdIdx] = NULL;
		}
	}
}

/*
		Function to Crypt a string.
		Calling this twice on the same string
		restores the orignal string.
*/
void CryptStr(char *szValue, int nLen /* = 1 */ )
{
	int y;
	for (y = 0; y < nLen; y ++)
	{
		if ( szValue[y] >= 0x20 )
			szValue[y] ^= 0x16;
	}
	return;
}


/*
		Function to Crypt a string.
		Calling this twice on the same string
		restores the orignal string.
*/
void CryptAdvStr(char *szString)
{
	int nLen = strlen( szString );
	CryptStr( szString, nLen );
}


/*
		Function to XOREncryptFile2 in file to out file.
		Calling this on an encrypted file
		writes an un-encrypted file.
*/
bool CryptFile2( FILE* pFdIn, FILE* pFdOut, const char *szFileNameIn, const char *szFileNameOut )
{
	bool	bRc = true;
	char	itxt[255];
										
	do
	{
		itxt[0] = '\0';
		fgets(itxt, 255, pFdIn);
		CryptAdvStr( itxt );
		fputs(itxt, pFdOut);
	}
	while ( !feof(pFdIn) && !ferror(pFdIn) && !ferror(pFdOut) );
										
	if ( ferror(pFdIn) )
	{
		printf( "Error reading file (%s).", szFileNameIn );
		bRc = false;
	}
										
	if ( ferror(pFdOut) )
	{
		printf( "Error writting file (%s).", szFileNameOut );
		bRc = false;
	}
										
	return bRc;
}

/*
		Function to XOREncrypt in file to out file.
		Calling this on an encrypted file
		writes an un-encrypted file.
*/
bool CryptFile1(const char *szFileNameIn, const char *szFileNameOut)
{
	bool bRc = true;
	FILE* pFdIn = NULL;
	FILE* pFdOut = NULL;
										
	pFdIn = fopen ( szFileNameIn, "rb" );
	if ( pFdIn == NULL )
	{
		bRc = false;
		printf("Failed to open input file (%s)", szFileNameIn);
	}
	if ( bRc )
	{
		pFdOut = fopen ( szFileNameOut, "wb" );
		if ( pFdOut == NULL )
		{
			bRc = false;
			printf("Failed to open output file (%s)", szFileNameOut);
		}
	}
										
	if ( bRc )
		bRc = CryptFile2( pFdIn, pFdOut, szFileNameIn, szFileNameOut );
										
										
	if ( pFdIn != NULL )
	{
		fclose(pFdIn);
		pFdIn = NULL;
	}
	if ( pFdOut != NULL )
	{
		fclose(pFdOut);
		pFdOut = NULL;
	}
										
	return bRc;
}


/*
		Function to encrypt or decrypt all advent data files.
*/
void crypt( char* forcedAction )
{
	bool bRc = false;
	char answer[1000];
	memset(answer, '\0', sizeof(answer));
	if ( forcedAction != NULL && strlen(forcedAction) )
	{
		strcpy(answer, forcedAction);
	}
	else
	{
		printf("password: ");
		GetInputLine(answer, sizeof(answer));
		if ( strcmp(answer,"xyzzy") != 0 )
		{
			puts("Sorry you can't enter the Crypt without the password.");
			return;
		}
		memset(answer, '\0', sizeof(answer));
		printf("What do you want to do now?: ");
		GetInputLine(answer, sizeof(answer));
	}
										
	if ( strcmp(answer,"encrypt") == 0 )
	{
		closetxt();
		for ( int fdIdx = 0; fdIdx < NUM_ADV_FILES; ++fdIdx )
		{
			MakeFileName( adv_file_names[fdIdx], szFnExtRaw,  rawFileSpec );
			MakeFileName( adv_file_names[fdIdx], szFnExtCrpt, crptFileSpec );
			bRc = CryptFile1(rawFileSpec, crptFileSpec);
		}
		opentxt();
	}
	else if ( strcmp(answer,"decrypt") == 0 )
	{
		closetxt();
		for ( int fdIdx = 0; fdIdx < NUM_ADV_FILES; ++fdIdx )
		{
			MakeFileName( adv_file_names[fdIdx], szFnExtCrpt, crptFileSpec );
			MakeFileName( adv_file_names[fdIdx], szFnExtRaw,  rawFileSpec );
			bRc = CryptFile1(crptFileSpec, rawFileSpec);
		}
		opentxt();
	}
	else
	{
		puts("Sorry you can't do that in here.");
		return;
	}
	if ( bRc && ! forcedAction )
		SpeakInfoMsg(MSG4_054_OK);
}

static char	szLogFileName[_MAX_PATH+1] = "AdventureCCaw1_log.txt";

void setLogFileName( const char* szFileName )
{
	strcpy( szLogFileName, szFileName );
}

void deleteLogFile()
{
	remove( szLogFileName );
}

/*
		append a char to log file.
*/
bool appendTextFile(const char cText, int nLevel, const char *szFile)
{
	char szText[2];
	szText[0] = cText;
	szText[1] = '\0';
	return appendTextFile( szText, nLevel, szFile );
}


/*
		append a string to log file.
*/
bool appendTextFile(const char *szText, int nLevel, const char *szFile)
{
	bool bRc = false;

	if ( gameState.Get_LogLevel(false) < nLevel )
		return false;
							
	if (!szFile)
		szFile = szLogFileName;
							
	if (!szFile || strlen(szFile) == 0)
		return bRc;
										
	FILE* fd = fopen(szFile, "at");
	if (fd != NULL)
	{
		if ( szText )
			fputs(szText, fd);
																				
		if (ferror(fd))
			bRc = false;
		else
			bRc = true;
																				
		fclose(fd);
	}
	return bRc;
}

/*
		Write a string to stdout.
*/
int puts( const char *string )
{
	int nRet = -1;
	if (string)
	{
		if (strcmp(string, "\n") == 0)
			nRet = 0;
		nRet = fputs(string, stdout);
		fflush(stdout);
		appendTextFile(string);
	}
	return nRet;
}


#include <stdarg.h>
/*
		Print formatted output to the standard output stream.
*/
int	printf( const char* fmtStr, ...)
{
	int nRet = -1;
	va_list marker;
	char* buff = NULL;
										
	buff = (char*)calloc(1,10000);
	if (buff == NULL)
		return nRet;
										
	va_start( marker, fmtStr );
	vsprintf(buff, fmtStr, marker);
	va_end( marker );
										
	nRet = puts(buff);
										
	free(buff);
	return nRet;
}


/*
		Function to scan a file up to a specified
		point and either print or return a string.
*/
short rdupto(FILE * fdi, char uptoc, char print, char * string, bool bNewLinePrefix )
{
	char buff[4*1024];
	char* pString = NULL;
	char c = 0;
	int	i = 0;
							
	memset ( buff, 0, sizeof(buff));
							
	if ( string == NULL )
		pString = buff;
	else
		pString = string;
							
	if ( bNewLinePrefix )
		*pString++ = '\n';
										
	while ( ! feof(fdi) && ! ferror(fdi) )
	{
		c = fgetc(fdi);
		CryptStr(&c);
																				
		if  ( c == uptoc )
			break;
																				
		if (c == EOF)
			break;
																				
		if (c != '\r')			// aw ???
			*pString++ = c;
	}
										
	*pString = '\0';
							
	if ( print )
	{
		if ( string == NULL )
			puts(buff);
		else
			puts(string);
	}
										
	return(1);
}

/*
		Function to read a file skipping
		a given character a specified number
		of times, with or without repositioning
		the file.
*/
void rdskip(FILE * fdi, char skipc, short n, char rewind)
{
										
	if (rewind)
	{
		if (fseek(fdi, 0, 0) == -1)
		{
			printf("rdskip rewind error\n");
			bug(BUG_AdvMessageData_Cpp+61);
			return;
		}
	}
	while (n--)
	{
		char c = 0;
		while (!feof(fdi) && ! ferror(fdi))
		{
			c = fgetc(fdi);
			CryptStr(&c);
			if ( c == skipc )
				break;
			if (c == EOF)
			{
				printf("rdskip EOF\n");
				bug(BUG_AdvMessageData_Cpp+62);
				return;
			}
		}
	}
}


/*
		Print a description from "advent?.dat"
*/
void SpeakMsgFromAdventFile(short msgNdx, short	advNdx, char delim, short skip, bool bNewLinePrefix)
{
#if _DEBUG
	if (g_debugFlg)
		printf("Seek msgNdx #%d @ %ld\n", msgNdx, idx[advNdx][msgNdx]);
#endif
	if ( msgNdx < 1 || msgNdx > idx[advNdx].MaxIndexed() )
	{
		short bugNum = BUG_AdvMessageData_Cpp+70+msgNdx*2;
		printf( "msgNdx (%d) > MaxIndexed (%d)\n", msgNdx, idx[advNdx].MaxIndexed() );
		bug(bugNum);
		return;
	}

	if ( idx[advNdx][msgNdx] == 0 )
	{
		short bugNum = BUG_AdvMessageData_Cpp+70+msgNdx*2+1;
		printf( "idx[%d][%d] == 0\n", advNdx, msgNdx );
		bug(BUG_AdvMessageData_Cpp+71);
		return;
	}

	fseek(fdsDatIn[advNdx], idx[advNdx][msgNdx], 0);

	if ( skip > 0 )
		rdskip(fdsDatIn[advNdx], delim, skip, 0);

	rdupto(fdsDatIn[advNdx], delim, 1, 0, bNewLinePrefix);
}


/*
		Print a long location description from "advent1.dat"
*/
void SpeakLocDescLong(short loc, bool bNewLinePrefix)
{
	short	msgNdx	= loc;
	short	advNdx	= ADVENT1;
	char	delim	= '#';
	short	skip	= 0;

	if ( gameState.Get_Friendly(false) )
		printf("Location %d:\n", msgNdx);

//	if ( msgNdx < 1 || msgNdx > idx[advNdx].MaxIndexed() )
//		return;

	SpeakMsgFromAdventFile( msgNdx,  advNdx,  delim, skip, bNewLinePrefix);
}

/*
		Print a short location description from "advent2.txt"
*/
void SpeakLocDescShort(short loc, bool bNewLinePrefix)
{
	short	msgNdx = loc;
	short	advNdx = ADVENT2;
	char	delim	= '#';
	short	skip	= 0;

	if ( gameState.Get_Friendly(false) )
		printf("Location %d:\n", msgNdx);

//	if ( msgNdx < 1 || msgNdx > idx[advNdx].MaxIndexed() )
//		return;

	SpeakMsgFromAdventFile( msgNdx,  advNdx,  delim, skip, bNewLinePrefix);
}

/*
		Print an item (object) description for a given state from "advent3.dat"
*/
void SpeakObjDesc(short item, short state, bool bNewLinePrefix)
{
	short	msgNdx = item;
	short	advNdx = ADVENT3;
	char	delim	= '/';
	short	skip	= state+2;

//	if ( msgNdx < 1 || msgNdx > idx[advNdx].MaxIndexed() )
//		return;

	SpeakMsgFromAdventFile( msgNdx,  advNdx,  delim, skip, bNewLinePrefix);
}

/*
		Print a info message from "advent4.dat"
*/
void SpeakInfoMsg(short msg, bool bNewLinePrefix)
{
	short	msgNdx = msg;
	short	advNdx = ADVENT4;

	char	delim	= '#';
	short	skip	= 0;

	if ( msgNdx < 1 || msgNdx > idx[advNdx].MaxIndexed() )
		return;

	SpeakMsgFromAdventFile( msgNdx,  advNdx,  delim, skip, bNewLinePrefix);
}

/*
		Print a help message from "advent5.dat"
*/
void SpeakHelp(short msg, bool bNewLinePrefix)
{
	short	msgNdx = msg;
	short	advNdx = ADVENT5;
	char	delim	= '#';
	short	skip	= 0;

	if ( msgNdx < 1 || msgNdx > idx[advNdx].MaxIndexed() )
		return;

	SpeakMsgFromAdventFile( msgNdx,  advNdx,  delim, skip, bNewLinePrefix);
}

void MoveCursorBack()
{
}


/*
		Routine to request a yes or no answer to a question.
*/
short yes(short msg1, short msg2, short msg3, char defaultAnswer )
{
	char	answer[80];
										
	if (msg1)
		SpeakInfoMsg(msg1, true);
//	puts(">");
										
	do
	{
		if ( defaultAnswer == 'y' )
			printf("\r(y) or n : ");
		else if ( defaultAnswer == 'n' )
				printf("\ry or (n) : ");
		else
			printf("\ry or n : ");
																				
		printf("\ry or n : (%c) : ", defaultAnswer);
//		MoveCursorBack();
		GetInputLine(answer, sizeof(answer));
		if ( tolower(answer[0]) == '\n' )
		{
			answer[0] = defaultAnswer;
			answer[1] = '\n';
			answer[2] = '\0';
		}
	}
	while ( ! (tolower(answer[0]) == 'y' || tolower(answer[0]) == 'n')  );
										
	if (tolower(answer[0]) == 'n')
	{
		if (msg3)
			SpeakInfoMsg(msg3);
		return(0);
	}
	if (msg2)
		SpeakInfoMsg(msg2);
	return(1);
}

