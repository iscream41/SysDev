//------------------------------------------------------------------------
// ファイル名 : Logger.h
// 概要       : ファイルと標準出力にログを出力する。
//              Windows環境－秒まで出力
//              Linux環境－ミリ秒まで出力
//              Windowsの場合は、プリプロセッサに「WINDOWS」を指定すること。
// 履歴       : 2017.04.14 森下天明 新規作成。
//------------------------------------------------------------------------
#ifndef LOGGER_H
#define LOGGER_H

//------------------------------------------------------------------------
// インクルード                                              
//------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#ifndef WINDOWS
#include <sys/time.h>
#endif

//------------------------------------------------------------------------
// define定義                                                 
//------------------------------------------------------------------------
// ログレベル
#define DEBUG	1	// デバッグ
#define INFO	2	// インフォメーション
#define WARN	3	// ワーニング
#define ERROR	4	// エラー

//------------------------------------------------------------------------
// 外部変数                                                 
//------------------------------------------------------------------------
// 現在設定されているログレベル
extern int g_iLogLevel;

// ログファイルポインタ
extern FILE* g_pLogFile;

//------------------------------------------------------------------------
// マクロ定義                                                 
//------------------------------------------------------------------------
//------------------------------------------------------------------------
// ログ出力マクロ
// (例)
//   LOG(DEBUG, "parameter[%s]", msg);
//   LOG(INFO, "count[%d] string[%s]", count, text);
//------------------------------------------------------------------------
#ifndef WINDOWS
// Linux用
#define LOG(level, fmt, ...)                         \
{                                                    \
	struct tm* local;                                \
	char sDatetime[ 20 ] = {0};                      \
	char sLogLevel[ 6 ] = {0};                       \
	struct timeval tmVal;                            \
                                                     \
	if(g_iLogLevel <= level && NULL != g_pLogFile) { \
		gettimeofday(&tmVal, NULL);                  \
		local = localtime(&tmVal.tv_sec);            \
		sprintf(sDatetime,                           \
			"%04d/%02d/%02d %02d:%02d:%02d.%03d",    \
			local->tm_year + 1900,                   \
			local->tm_mon + 1,                       \
			local->tm_mday,                          \
			local->tm_hour,                          \
			local->tm_min,                           \
			local->tm_sec,                           \
			(int)(tmVal.tv_usec/1000));              \
		if(DEBUG == level) {                         \
			strcpy(sLogLevel, "DEBUG");              \
		}                                            \
		else if(INFO == level) {                     \
			strcpy(sLogLevel, "INFO ");              \
		}                                            \
		else if(WARN == level) {                     \
			strcpy(sLogLevel, "WARN ");              \
		}                                            \
		else if(ERROR == level) {                    \
			strcpy(sLogLevel, "ERROR");              \
		}                                            \
		fprintf(g_pLogFile, "%s %s %s(%d): "fmt"\n", \
			sDatetime,                               \
			sLogLevel,                               \
			__FUNCTION__,                            \
			__LINE__,                                \
			## __VA_ARGS__);                         \
		fflush(g_pLogFile);                          \
		printf("%s %s %s(%d): "fmt"\n",              \
			sDatetime,                               \
			sLogLevel,                               \
			__FUNCTION__,                            \
			__LINE__,                                \
			## __VA_ARGS__);                         \
	}                                                \
}    
#else
// Windows用
#define LOG(level, fmt, ...)                         \
{                                                    \
	struct tm* local;                                \
	time_t now = time(NULL);                         \
	char sDatetime[ 20 ];                            \
	char sLogLevel[ 6 ];                             \
                                                     \
	if(g_iLogLevel <= level && NULL != g_pLogFile) { \
		memset(sDatetime, 0x00, sizeof(sDatetime));  \
		memset(sLogLevel, 0x00, sizeof(sLogLevel));  \
                                                     \
		local = localtime(&now);                     \
		sprintf(sDatetime,                           \
			"%04d/%02d/%02d %02d:%02d:%02d",         \
			local->tm_year + 1900,                   \
			local->tm_mon + 1,                       \
			local->tm_mday,                          \
			local->tm_hour,                          \
			local->tm_min,                           \
			local->tm_sec);                          \
                                                     \
		if(DEBUG == level) {                         \
			strcpy(sLogLevel, "DEBUG");              \
		}                                            \
		else if(INFO == level) {                     \
			strcpy(sLogLevel, "INFO ");              \
		}                                            \
		else if(WARN == level) {                     \
			strcpy(sLogLevel, "WARN ");              \
		}                                            \
		else if(ERROR == level) {                    \
			strcpy(sLogLevel, "ERROR");              \
		}                                            \
		fprintf(g_pLogFile, "%s %s %s(%d): "fmt"\n", \
			sDatetime,                               \
			sLogLevel,                               \
			__FUNCTION__,                            \
			__LINE__,                                \
			## __VA_ARGS__);                         \
		fflush(g_pLogFile);                          \
		printf("%s %s %s(%d): "fmt"\n",              \
			sDatetime,                               \
			sLogLevel,                               \
			__FUNCTION__,                            \
			__LINE__,                                \
			## __VA_ARGS__);                         \
	}                                                \
}    
#endif

//------------------------------------------------------------------------
// プロトタイプ宣言
//------------------------------------------------------------------------
//------------------------------------------------------------------------
// 関数名 : ログ初期化処理
// 概要   : ログを初期化する。ログ出力する前に必ず呼び出すこと。
// 引数   : (I)出力するログレベル(DEBUG/INFO/WARN/ERROR)
//          (I)ログファイルを出力するディレクトリ
// 戻り値 : なし
// 履歴   : 2017.04.14 森下天明 新規作成。
//------------------------------------------------------------------------
void LogInit(
	int		iLevel,	// (I)ログレベル(DEBUG/INFO/WARN/ERROR)
	char*	psDir	// (I)ログ出力ディレクトリ
);

//------------------------------------------------------------------------
// 関数名 : ログクローズ処理
// 概要   : 終了時にログをクローズする。
// 引数   : なし
// 戻り値 : なし
// 履歴   : 2017.04.14 森下天明 新規作成。
//------------------------------------------------------------------------
void LogClose();

//------------------------------------------------------------------------
// 関数名 : ログレベル設定処理
// 概要   : 出力するログレベルを設定する。
// 引数   : (I)ログレベル(DEBUG/INFO/WARN/ERROR)
// 戻り値 : なし
// 履歴   : 2017.04.14 森下天明 新規作成。
//------------------------------------------------------------------------
void SetLogLevel(
	int		iLevel	// (I)ログレベル(DEBUG/INFO/WARN/ERROR)
);

#endif
//--------
// EOF
//--------

