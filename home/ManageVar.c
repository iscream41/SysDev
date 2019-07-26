//-------------------------------------------------------------------------
// ファイル名 : ManageVar.c
// 概要       : 変数を管理するためのソースファイル
// 履歴       : 2019.07.11 平大真 新規作成。
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
// インクルード
//-------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include "List.h"
#include "Logger.h"
#include "Common.h"
#include "ManageVar.h"

//-------------------------------------------------------------------------
// 外部変数
//-------------------------------------------------------------------------
static TItem* s_pHeadItem = NULL;
static TItem* s_pTailItem = NULL;

//-------------------------------------------------------------------------
// 関数定義
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
// 関数名 : SetVar
// 概要　 : 変数名を受け取り、変数リストに追加する
// 引数　 :(I)変数名,(I)変数の値
// 戻り値 : 正常終了 : 0
// 　　　   異常終了 : 1
// 履歴　 : 2019.07.11 新規作成 平大真
//-------------------------------------------------------------------------
int SetVar( TItem* pkLeft, char* psRight) {
    
    TItem* pkSearchResult = NULL; //SearchItemの検索結果の格納
    TItem* pkNode = NULL;  //生成した構造体を格納
    
    //変数の検索
    pkSearchResult = SearchItem(pkLeft->pValue, &s_pHeadItem);
    
    //変数名が存在しているか判定
    if(pkSearchResult != NULL) {
        free(pkSearchResult->pkPrev);
        
        //既存の変数を正常に削除できているかの判定
        if(DeleteItem(pkSearchResult->psKey, &s_pHeadItem, &s_pTailItem) == 0) {
            
            //構造体を生成
            pkNode = CreateItem(pkLeft->pValue, psRight);
           
            //構造体を生成できた場合、その構造体をリストに追加
            if(pkNode != NULL) {
                AddItem(pkSearchResult, &s_pHeadItem, &s_pTailItem);
                return 0;
            }
            else {
                return 1;
            }
        }
        else {
            return 1;
        }     
    }
    else {
        //新たに変数を確保
        
        pkNode = CreateItem(pkLeft->pValue, psRight);
           
        //構造体を生成できた場合、その構造体をリストに追加
        if(pkNode != NULL) {
            AddItem(pkNode, &s_pHeadItem, &s_pTailItem);
            
            pkSearchResult = SearchItem(pkLeft->pValue, &s_pHeadItem);
            
            return 0;
        }
        else {
            return 1;
        }
    }   
}

//-------------------------------------------------------------------------
// 関数名 : SearchVar
// 概要　 : 受け取った変数名を変数リストから検索する
// 引数　 : (I)変数名
// 戻り値 : 正常終了 : 変数の値
// 　　　   異常終了 : NULL
// 履歴　 : 2019.07.11 平大真　新規作成
//-------------------------------------------------------------------------
char* SearchVar(TItem* pkLeft) {
 
    TItem* pkSearchResult = NULL; //SearchItemの検索結果の格納
    
    //検索結果を変数に代入
    pkSearchResult = SearchItem((char*)pkLeft->pValue, &s_pHeadItem);
    
    //変数リスト内に指定した変数名が存在しているかの条件
    if(pkSearchResult != NULL) {
        
        //変数の値を返す。
        return (char*)pkSearchResult->pValue;
    }
    else {
        return NULL;
    }
 }
 

//-------------------------------------------------------------------------
// 関数名 : DeleteLest
// 概要　 : 変数リストを全て削除
// 引数　 : (I)変数名
// 戻り値 : 正常終了 : 変数の値
// 　　　   異常終了 : NULL
// 履歴　 : 2019.07.11 平大真　新規作成
//-------------------------------------------------------------------------
void DeleteList() {
    LOG(INFO,"DeleteList呼び出し");
    DeleteAllItem(&s_pHeadItem, &s_pTailItem);
    
}