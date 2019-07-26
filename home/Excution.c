//------------------------------------------------------------------------
// ファイル名 : Excution.c
// 概要      : 処理実行に関するソースファイル
// 履歴      : 2019.7.10 平 大真 新規作成。
//------------------------------------------------------------------------
//------------------------------------------------------------------------
// インクルード
//------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include "List.h"
#include "Logger.h"
#include "Common.h"
#include "ManageVar.h"
#include "Excution.h"

//----------------------------------------------------------------------------
// 関数定義
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//関数名 : Excution
//概要　 : CalcArithmetic, AssignVar, Output
//引数　 : (I)構文木のノード
//戻り値 : 正常終了 : 0
//　　　   異常終了 ; 1
//履歴　 : 2019.07.10 平大真　新規作成
//----------------------------------------------------------------------------
int Excution(TItem* pkNode) {
    int iLeft = 0;      //左子ノードの実行が正常であるかどうかの値を格納
    int iRight = 0;     //右子ノードの実行が正常であるかどうかの値を格納
    int iErrorFlag = 0; //Excution関数の戻り値を格納
       
    
    if(pkNode != NULL) {
        LOG(DEBUG,"ノード : %s",(char*)pkNode->pValue);
        iLeft = Excution(pkNode->pkPrev);
        
        iRight = Excution(pkNode->pkNext);
        
        if(iLeft == 1 || iRight == 1) {
            iErrorFlag = 1;
            return iErrorFlag;
        }
        else if(((char*)pkNode->pValue)[0] == '+' || ((char*)pkNode->pValue)[0] == '-' 
                || ((char*)pkNode->pValue)[0] == '*' || ((char*)pkNode->pValue)[0] == '/') {
            
            LOG(DEBUG,"演算子 : %s",(char*)pkNode->pValue);
            
            //四則演算関数呼び出し
            iErrorFlag = CalcArithmetic(pkNode);
            return iErrorFlag;     
        }
        else if(((char*)pkNode->pValue)[0] == '=') {
            
            //代入関数呼び出し
            iErrorFlag = AssignVar(pkNode);
            return iErrorFlag;
        }
        else if(strcmp((char*)pkNode->pValue,FuncPrint) == 0) {
            
            //出力関数呼び出し
            iErrorFlag = Output(pkNode);
            return iErrorFlag; 
        }
        else {
            return iErrorFlag;
        }  
    }
    else {
        return iErrorFlag;
    }    
}

//----------------------------------------------------------------------------
//関数名 : CalcArithmetic
//概要　 : 構文木を受け取り、最下層から四則演算を行う。
//引数　 : (I)構文木のノード
//戻り値 : 正常終了 : 0
//　　　   異常終了 : 1
//履歴　 : 2019.07.10 平大真　新規作成
//----------------------------------------------------------------------------
int CalcArithmetic(TItem* pkNode) {
    
    int sum = 0;  //演算結果を格納する変数
    char* psTmpValue = NULL; //ノードの変数の値を格納
    char* psTmpLeftVar = NULL; //左子ノードの変数の値を格納
    char* psTmpRightVar = NULL; //右子ノードの変数の値を格納
    
    psTmpLeftVar = (char*)pkNode->pkPrev->pValue;
    psTmpRightVar = (char*)pkNode->pkNext->pValue;
    
    //左の子ノードが変数
    if(isalpha(psTmpLeftVar[0])) {
       psTmpLeftVar = SearchVar(pkNode->pkPrev);
       LOG(DEBUG,"psTmpLeftVar = %s",psTmpLeftVar);
       
       if(psTmpLeftVar == NULL) {
           printf("未定義の変数を使用しています。\n");
           LOG(WARN,"%d行目 : 未定義の変数を使用しています。",g_iCodeCount);
           return 1;
       }
       else {
           //ここでは何もしない。
       }
    }
    else {
        //ここでは何もしない
    }
    
    
    //右の子ノードが変数
    if(isalpha(psTmpRightVar[0])){
        psTmpRightVar = SearchVar(pkNode->pkNext);
        
        if(psTmpRightVar == NULL) {
            printf("未定義の変数を使用しています。\n");
            LOG(WARN,"%d行目 : 未定義の変数を使用しています。",g_iCodeCount);
            return 1;
        }
        else {
            //ここでは何もしない
        }
    }
    else {
        //ここでは何もしない
    }
    
    
    //左の子ノードが文字列ではない判定
    if(psTmpLeftVar[0] != '"') {
        //右の子ノードが文字列ではない判定
        if(psTmpRightVar[0] != '"') {
            
                //演算結果を代入
                //加算
                if(((char*)pkNode->pValue)[0] == '+'){
                    sum = atoi(psTmpLeftVar) + atoi(psTmpRightVar);
                    LOG(DEBUG,"sum = %d",sum);
                }
                //減算
                else if(((char*)pkNode->pValue)[0] == '-') {
                    sum = atoi(psTmpLeftVar) - atoi(psTmpRightVar);
                    LOG(DEBUG,"sum = %d",sum);
                }
                //乗算
                else if(((char*)pkNode->pValue)[0] == '*') {
                        sum = atoi(psTmpLeftVar) * atoi(psTmpRightVar);
                        LOG(DEBUG,"sum = %d",sum);
                }
                //除算
                else {
                                //0で除算を行っているかの判定
                    if(psTmpRightVar[0] == '0'){
                        printf("0除算は演算不可能です。\n");
                        LOG(WARN,"%d行目 : 0除算は演算不可能です。",g_iCodeCount);
                        return 1;
                    }
                    else {
                        sum = atoi(psTmpLeftVar) / atoi(psTmpRightVar);
                        LOG(DEBUG,"sum = %d",sum);
                    }
                }

                //親ノードのpValueのメモリ領域を解放
                free(pkNode->pValue);
                
                //親ノードのpValueに新たなメモリ領域を確保
                //演算結果が0の場合はlogの結果が無限大になるため、条件を分岐
                if(sum == 0){
                    if( (psTmpValue = (char*)malloc(sizeof(char) * 2)) != NULL ) {
                        sprintf(psTmpValue,"%d",sum);
                        pkNode->pValue = psTmpValue;
                    }
                    else {
                        printf("メモリ確保に失敗しました。\n");
                        LOG(ERROR,"%d行目 : メモリの確保に失敗しました。",g_iCodeCount);
                        return 1;
                    }  
                }
                else {
                    if( (psTmpValue = (char*)malloc(sizeof(char) * (log10(abs(sum)) + 3))) != NULL ) {
                        sprintf(psTmpValue,"%d",sum);
                        pkNode->pValue = psTmpValue;

                        LOG(DEBUG,"new pkNode = %s",(char*)pkNode->pValue);
                    }
                    else {
                        printf("メモリ確保に失敗しました。\n");
                        LOG(ERROR,"%d行目 : メモリの確保に失敗しました。",g_iCodeCount);
                        return 1;
                    }
                }
        }
        else {
            printf("不正な演算です。\n");
            LOG(ERROR,"%d行目 : 不正な演算です。",g_iCodeCount);
            return 1;
        }
    }
    else {
        printf("不正な演算です。\n");
        LOG(ERROR,"%d行目 : 不正な演算です。",g_iCodeCount);
        return 1;
    }  
    
    return 0;
}

//----------------------------------------------------------------------------
//関数名 : AssignVar
//概要　 : ManegeVarを呼び出し、変数に値を代入する。
//引数　 : (I)構文木のノード
//戻り値 : 正常終了 : 0
//　　　   異常終了 : 1
//履歴　 : 2019.07.10 平大真　新規作成
//----------------------------------------------------------------------------
int AssignVar(TItem* pkNode) {
    char* psValue = NULL; //変数に代入する値を格納
    char* psVar = NULL;   //変数を格納
    int iValueLength = 0; //変数に代入する値の文字数を格納
    int i = 0;            //SetVarからの戻り値を格納
    
    //代入する値が変数であるかの判定
    if(isalpha(((char*) pkNode->pkNext->pValue)[0])) {
        psVar = SearchVar(pkNode->pkNext);
        
        if(psVar == NULL) {
            printf("未定義の変数を使用しています。\n");
            LOG(WARN,"%d行目 : 未定義の変数を使用しています。",g_iCodeCount);
            return 1;
        }
        else {
            //文字列の長さを取得
            iValueLength = strlen(psVar);
            LOG(DEBUG,"iValuelength = %d",iValueLength);

            psValue = (char*)malloc(sizeof(char) * iValueLength);
            strcpy(psValue, psVar);

            i = SetVar(pkNode->pkPrev, psValue);
            LOG(DEBUG,"i = %d",i);

            return i; 
        }
    }
    //代入する値が変数ではない場合
    else {
        //文字列の長さを取得
        iValueLength = strlen((char*) pkNode->pkNext->pValue);
        LOG(DEBUG,"iValuelength = %d",iValueLength);

        psValue = (char*)malloc(sizeof(char) * iValueLength);
        strcpy(psValue, (char*) pkNode->pkNext->pValue);

        i = SetVar(pkNode->pkPrev, psValue);
        LOG(DEBUG,"i = %d",i);

        return i; 
    }
}

//----------------------------------------------------------------------------
//関数名 : Output
//概要　 : 構文木を受け取り、ノードの値・文字列を画面に出力
//引数　 : (I)構文木のノード
//戻り値 : 正常終了 : 0
//　　　   異常終了 : 1
//履歴　 : 2019.07.10 平大真　新規作成
//----------------------------------------------------------------------------
int Output(TItem* pkNode) {
    
    int iWordCount = 0;      //トークン文字数を格納
    int iByteCount = 0;      //出力した文字のバイト数を格納
    char* psByteCount = NULL; //出力した文字のバイト数の文字列を格納
    char* sOutputVar = NULL; //変数の値を格納する変数
    
    iWordCount = strlen((char*)pkNode->pkPrev->pValue);
    
    //変数である場合
    if(isalpha((int)(((char*)pkNode->pkPrev->pValue)[0]))) {
        
        //指定した変数が存在しているかの判定
        if( (sOutputVar = SearchVar(pkNode->pkPrev)) == NULL) {
            printf("未定義の変数を使用しています。\n");
            LOG(WARN,"%d行目 : 未定義の変数を使用しています。\n",g_iCodeCount);
            return 1;
        }
        else {
            
            iWordCount = strlen(sOutputVar);
            
            //変数の値が文字列である場合
            if (sOutputVar[0] == '"') {
                iByteCount =  printf("%.*s", iWordCount - 2, sOutputVar + 1);
                printf("\n");
                
                //親ノードのvalueを解放
                LOG(DEBUG,"pkNode->pValue old  = %s", (char*)pkNode->pValue)
                free(pkNode->pValue);
                
                psByteCount = (char*)malloc(sizeof(char) * iByteCount);
                sprintf(psByteCount,"%d",iByteCount);
                pkNode->pValue = psByteCount;
                LOG(DEBUG,"pkNode->pValue new =  %s",(char*)pkNode->pValue);
            }
           
            //変数の値が数値である場合
            else {
                iByteCount = printf("%s",sOutputVar);
                printf("\n");
                
                //親ノードのvalueを解放
                
                LOG(DEBUG,"pkNode->pValue old  = %s", (char*)pkNode->pValue)
                free(pkNode->pValue);

                psByteCount = (char*)malloc(sizeof(char) * iByteCount);
                sprintf(psByteCount,"%d",iByteCount);
                pkNode->pValue = psByteCount;
                LOG(DEBUG,"pkNode->pValue  new = %s",(char*)pkNode->pValue);    
            }
            LOG(DEBUG,"sOutputVar = %s",sOutputVar);
        }
    }
    
    
    //文字列の表示
    else if(((char*)pkNode->pkPrev->pValue)[0] == '"') {
       iByteCount =  printf("%.*s",iWordCount - 2,(char*)pkNode->pkPrev->pValue + 1);
       printf("\n");
       
        //親ノードのvalueを解放
       LOG(DEBUG,"pkNode->pValue old  = %s", (char*)pkNode->pValue)
        free(pkNode->pValue);
        
        psByteCount = (char*)malloc(sizeof(char) * iByteCount);
        sprintf(psByteCount,"%d",iByteCount);
        pkNode->pValue = psByteCount;
        LOG(DEBUG,"pkNode->pValue new = %s",(char*)pkNode->pValue);   
    }
    
    //数値の表示
    else {
        iByteCount = printf("%s",(char*)pkNode->pkPrev->pValue);
        printf("\n");
        
        //親ノードのvalueを解放
        LOG(DEBUG,"pkNode->pValue old  = %s", (char*)pkNode->pValue)
        free(pkNode->pValue);
        
        psByteCount = (char*)malloc(sizeof(char) * iByteCount);
        sprintf(psByteCount,"%d",iByteCount);
        strcpy(pkNode->pValue,psByteCount);
        LOG(DEBUG,"pkNode->pValue new = %s",(char*)pkNode->pValue);
    }
    
    return 0;
}