//-------------------------------------------------------------------------
// ファイル名 : ManageVar.h
// 概要       : 変数を管理するためのソースファイル
// 履歴       : 2019.07.11 平大真 新規作成。
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
// インクルードガード
//-------------------------------------------------------------------------
#ifndef MANAGEVAR_H
#define	MANAGEVAR_H

//-------------------------------------------------------------------------
// インクルード
//-------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "List.h"

//-------------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------------
int SetVar(TItem* pkLeft, char* pkRight);
char* SearchVar(TItem* pkLeft);
void DeleteList();


#endif	/* MANAGEVAR_H */
