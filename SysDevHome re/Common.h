//-------------------------------------------------------------------------
// ファイル名  : Common.h
// 概要       : 複数のファイルから呼び出される関数を管理するヘッダーファイル。
// 履歴       : 2019.07.17 平大真 新規作成。
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
// インクルードガード
//-------------------------------------------------------------------------
#ifndef COMMON_H
#define	COMMON_H

//------------------------------------------------------------------------
// インクルード                                              
//------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "List.h"
#include "Logger.h"

//------------------------------------------------------------------------
// プロトタイプ宣言                                           
//------------------------------------------------------------------------
int JudgeNumberRange(char* psTmpToken, int iTokenCount);
int JudgeMinus(char* psTmpToken[]);
int JudgeArithmetic(int iWord);
int JudgeAssign(int iWord);
int JudgeString(int iWord);
int JudgeOpenParen(int iWord);
int JudgeCloseParen(int iWord);
int JudgeReserved(char* psTmpToken);
int JudgeComment(FILE* pFile, int iWord);
char* MakeToken(FILE* pFile, int iTokenCount);
void ReleaseTree(TItem* pkNode);

//-------------------------------------------------------------------------
// 定数
//-------------------------------------------------------------------------
#define MaxNumber "2147483647" //数値が取りうる最大値
#define FuncPrint "Print"      //組み込み関数(Print)用の文字列

//-------------------------------------------------------------------------
// 外部定数
//-------------------------------------------------------------------------
extern int g_iCodeCount; //ソースファイルの行数を格納
extern int g_iTokenCount ; //何番目のトークンまで解析が終了しているのかを格納

#endif	/* COMMON_H */

