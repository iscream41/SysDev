//------------------------------------------------------------------------
// ファイル名 : Excution.h
// 概要      : 処理実行に関するヘッダーファイル
// 履歴      : 2019.7.18 平 大真 新規作成
//------------------------------------------------------------------------
//------------------------------------------------------------------------
// インクルードガード
//------------------------------------------------------------------------
#ifndef EXCUTION_H
#define	EXCUTION_H

//------------------------------------------------------------------------
// インクルード
//------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Math.h>
#include <ctype.h>
#include "List.h"
#include "Logger.h"

//-------------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------------
int Excution(TItem* pkNode);
int CalcArithmetic(TItem* pkNode);
int AssignVar(TItem* pkNode);
int Output(TItem* pkNode) ;



#endif	/* EXCUTION_H */

