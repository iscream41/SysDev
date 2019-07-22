//-------------------------------------------------------------------------
// ファイル名 : Common.c
// 概要      : 複数のファイルから呼び出される関数を管理するソースファイル。
// 履歴      : 2019.07.16 平大真 新規作成
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
// インクルード
//-------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "List.h"
#include "Logger.h"
#include "Common.h"
#include "Parse.h"

//----------------------------------------------------------------------------
// 関数定義
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//関数名 : JudgeNumberRange
//概要　 : 読み込んだ数値の桁数が範囲内(-2147483647～2147483647)に収まっているかを判定
//引数　 : (I)トークン
//　　　   (I)トークン数
//戻り値 : 一致 : 0
//        不一致 : 1
//履歴　 : 2019.07.12 平大真　新規作成
//----------------------------------------------------------------------------
int JudgeNumberRange(char* psTmpToken, int iTokenCount) 
{
    LOG(INFO,"JudgeNumberRange呼び出し");
    LOG(DEBUG,"iTokenCount = %d",iTokenCount);
    
    iTokenCount--;
    LOG(DEBUG,"iTokenCount = %d",iTokenCount);
    
    if(iTokenCount <= 9) {
        return 0;
    }
    else if(iTokenCount == 10) {
        
        //数値の桁数が範囲内に収まる
        if(strcmp(psTmpToken, MaxNumber) < 0) {
            return 0;
        }
        else {
            printf("範囲外の数値を使用しています。\n");
            LOG(WARN,"%d行目 : 範囲外の数値を使用しています。",g_iCodeCount);
            return 1;
        }
    }
    else {
        
    }
}

//----------------------------------------------------------------------------
//関数名 : JudgeMinus
//概要　 : 読み込んだ文字が単項演算子(-)であるかを判定
//引数　 : (I)トークン配列
//戻り値 : 一致 : 0
//        不一致 : 1
//履歴　 : 2019.07.12 平大真　新規作成
//----------------------------------------------------------------------------
int JudgeMinus(char* psTmpToken[]) 
{
    if(psTmpToken[g_iTokenCount][0] == '-') {
        return 0;
    }
    else {
        return 1;
    }
}

//----------------------------------------------------------------------------
//関数名 : JudgeArithmetic
//概要　 : 読み込んだ文字が四則演算子であるかどうかを判定
//引数　 : (I)トークン
//戻り値 : 一致 : 0
//        不一致 : 1
//履歴　 : 2019.07.12 平大真　新規作成
//----------------------------------------------------------------------------
int JudgeArithmetic(int iWord) 
{
    LOG(INFO,"JudgeArithmetic呼び出し");
    LOG(DEBUG,"iWord = %c",iWord);
    if(iWord == '+') {
        return 0;
    }
    else if(iWord == '-') {
        return 0;
    }
    else if(iWord == '*') {
        return 0;
    }
    else if(iWord == '/') {
        return 0;
    }
    else {
        return 1;
    }
}

//----------------------------------------------------------------------------
//関数名 : JudgeAssign
//概要　 : 読み込んだ文字が代入演算子であるかどうかを判定
//引数　 : (I)トークン
//戻り値 : 一致 : 0
//        不一致 : 1
//履歴　 : 2019.07.12 平大真　新規作成
//----------------------------------------------------------------------------
int JudgeAssign(int iWord) 
{
    LOG(INFO,"JudgeAssign呼び出し");
    LOG(DEBUG,"iWord = %c",iWord);
    if(iWord == '=') {
        return 0;
    }
    else {
        return 1;
    }
}

//----------------------------------------------------------------------------
//関数名 : JudgeString
//概要　 : 読み込んだ文字がダブルクォーテーションをであるかどうかを判定
//引数　 : (I)トークン
//戻り値 : 一致 : 0
//        不一致 : 1
//履歴　 : 2019.07.12 平大真　新規作成
//----------------------------------------------------------------------------
int JudgeString(int iWord)
{
    LOG(INFO,"JudgeString呼び出し");
    LOG(DEBUG,"iWord = %c",iWord);
    if(iWord == '"'){
        return 0;
    }
    else {
        return 1;
    }
}

//----------------------------------------------------------------------------
//関数名 : JudgeOpenParen
//概要　 : 読み込んだ文字が開き丸括弧であるかどうかを判定
//引数　 : (I)トークン
//戻り値 : 一致 : 0
//        不一致 : 1
//履歴　 : 2019.07.12 平大真　新規作成
//----------------------------------------------------------------------------
int JudgeOpenParen(int iWord)
{
    LOG(INFO,"JudgeOpenParen呼び出し");
    LOG(DEBUG,"iWord = %c",iWord);
    if(iWord == '(') {
        return 0;
    }
    else {
        return 1;
    }
}

//----------------------------------------------------------------------------
//関数名 : JudgeCloseParen
//概要　 : 読み込んだ文字が閉じ丸括弧であるかどうかを判定
//引数　 : (I)トークン
//戻り値 : 一致 : 0
//        不一致 : 1
//履歴　 : 2019.07.12 平大真　新規作成
//----------------------------------------------------------------------------
int JudgeCloseParen(int iWord) 
{
    LOG(INFO,"JudgeCloseParen呼び出し");
    
    LOG(DEBUG,"iWord = %c",iWord);
    if(iWord == ')') {
        return 0;
    }
    else {
        return 1;
    }
}

//----------------------------------------------------------------------------
//関数名 : JudgeReserved
//概要　 : 読み込んだ文字が予約語であるかどうかを判定
//引数　 : (I)トークン
//戻り値 : 一致 : 0
//        不一致 : 1
//履歴　 : 2019.07.12 平大真　新規作成
//----------------------------------------------------------------------------
int JudgeReserved(char* psTmpToken)
{
    LOG(INFO,"JudgeReserved呼び出し");
    
    LOG(DEBUG,"psTmpToken = %s",psTmpToken);
    if(strcmp(psTmpToken,FuncPrint) == 0) {
        return 0;
    }
    else {
        return 1;
    }
}

//----------------------------------------------------------------------------
//関数名 : JudgeComment
//概要　 : 読み込んだ文字がコメントアウトであるかを判定し、コメントアウトの終わりまで読み飛ばす。
//引数　 : (I)ファイルポインタ
//　　　　　(I)読み込んだ文字
//戻り値 : 一致 : 0
//        不一致 : 1
//履歴　 : 2019.07.10 平大真　新規作成
//----------------------------------------------------------------------------
int JudgeComment(FILE* pFile, int iWord)
{
    LOG(INFO,"JudgeComment呼び出し");
    
    // /(スラッシュ)の次の文字を読み込む
    iWord = fgetc(pFile);
    LOG(DEBUG,"iWord = %c",iWord);
    
    //コメントアウト(//)の判定
    if(iWord == '/') {
        while(1) {
            iWord = fgetc(pFile);
            LOG(DEBUG,"iWord = %c",iWord);
            
            if(iWord != EOF && iWord != '\n' ) {
                //ここでは何もしない
            }
            else {
                g_iCodeCount++;
                LOG(DEBUG,"g_iCodeCount = %d",g_iCodeCount);
                return 0;
            }
        }
    }
    
    //コメントアウト(/*～*/)の判定
    else if(iWord == '*') {
        while(1) {
            
            iWord = fgetc(pFile);
            LOG(DEBUG,"iWord = %c",iWord);
            
            if(iWord != '*') {
                if(iWord != EOF) {
                    if(iWord == '\n') {
                        //改行文字を読んだので、ソースファイルの行数をインクリメント
                        g_iCodeCount++;
                        LOG(DEBUG,"g_iCodeCount = %d",g_iCodeCount);
                    }
                    else {
                        //ここでは何もしない
                    }
                }
                else {
                    printf("コメント文が閉じられていません。\n");
                    LOG(WARN,"%d行目 : コメント文が閉じられていません。",g_iCodeCount);
                    return 1;
                }
            }
            //読み込んだ文字がアスタリスク
            else {
                iWord = fgetc(pFile);
                LOG(DEBUG,"iWord = %c",iWord);
                
                if(iWord == '/'){
                    return 0;
                }
                else {
                    if(iWord == EOF) {
                        printf("コメント文が閉じられていません。\n");
                        LOG(WARN,"%d行目 : コメント文が閉じられていません。",g_iCodeCount);
                        return 1;
                    }
                    //ファイルポインタの位置を調整
                    fseek(pFile,-1,SEEK_CUR);
                }
            } 
        }
    }
    else {
        //ファイルポインタの位置を調整
        fseek(pFile,-1,SEEK_CUR);
        return 0;
    }
}

//----------------------------------------------------------------------------
//関数名 : MakeToken
//概要　 : ソースファイル1文分のトークンを生成する。
//引数　 : (I)ファイルポインタ
//戻り値 : 正常終了 : 0
//        異常終了 : 1
//履歴　 : 2019.07.10 平大真　新規作成
//----------------------------------------------------------------------------
char* MakeToken(FILE* pFile, int iTokenCount) 
{
    LOG(INFO,"MakeToken呼び出し");
    char* psMakeToken = NULL;    //トークンを格納する配列
    int iArrayCount = 0;         //トークン配列の添え字用の変数
    
    //ファイルポインタの位置を調整
    fseek(pFile,-iTokenCount,SEEK_CUR);
    
    //メモリ領域確保
    if( (psMakeToken = (char*)malloc(sizeof(char) * iTokenCount)) != NULL ) {
        
        while(iTokenCount != 1) {
            psMakeToken[iArrayCount] = (char)fgetc(pFile);
            LOG(DEBUG,"psMakeToken[%d] = %c",iArrayCount, psMakeToken[iArrayCount]);
            
            iArrayCount++;
            LOG(DEBUG,"iArrayCount = %d",iArrayCount);
            
            iTokenCount--;
            LOG(DEBUG,"iTokenCount = %d",iTokenCount);
        }
        
        //終端文字代入
        psMakeToken[iArrayCount] = '\0';
        
        return psMakeToken;
        
    }
    //メモリ確保失敗
    else {
        printf("メモリの確保に失敗しました。\n");
        LOG(ERROR,"%d行目 : メモリの確保に失敗しました。",g_iCodeCount);
        return psMakeToken;
    }
}

//----------------------------------------------------------------------------
//関数名 : ReleaseTree
//概要　 : 生成した構文木を解放する
//引数　 : 構文木のノード
//戻り値 : なし
//履歴　 : 2019.07.11 平大真　新規作成
//----------------------------------------------------------------------------
void ReleaseTree(TItem* pkNode) 
{
    LOG(INFO,"ReleaseTree呼び出し");
    
    //構文木かノードを持つ場合、それらを全て解放
    if(pkNode == NULL) {
        //解放するノードがないため、何もしない
    }
    else {
        ReleaseTree(pkNode->pkPrev);
        ReleaseTree(pkNode->pkNext);
        
        free(pkNode->psKey);
        free(pkNode->pValue);
        free(pkNode);
    }
}
