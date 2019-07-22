//------------------------------------------------------------------------
// ファイル名 : parse.c
// 概要      : 構文解析に関するソースファイル
// 履歴      : 2019.7.17 平 大真 新規作成。
//------------------------------------------------------------------------
//------------------------------------------------------------------------
// インクルード
//------------------------------------------------------------------------
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
//関数名 : MakeTree
//概要　 : 事前処理、式処理関数を呼び出す
//引数　 : (I)トークン配列
//戻り値 : 処理済みの構文木のルート
//履歴　 : 2019.07.17 平大真　新規作成
//----------------------------------------------------------------------------
TItem* MakeTree(char* psToken[]) {

    int iErrorFlag = 0;     //エラー用のフラグ
    TItem* pkNode = NULL;   //構文木の親ノードを格納
    
    //事前処理呼び出し
    iErrorFlag = PreProcess(psToken);
    LOG(DEBUG,"iErrorFlag = %d",iErrorFlag);
    
    if(iErrorFlag == 0) {
        
        //式処理呼び出し
        pkNode = Formula(psToken);
        return pkNode;
    }
    else {
        return NULL;
    }
}

//----------------------------------------------------------------------------
//関数名 : PreProcess
//概要　 : 構文解析の事前処理を行う
//引数　 : (I)トークン配列
//戻り値 : 正常終了 : 0
//　　　   異常終了 : 1
//履歴　 : 2019.07.17 平大真　新規作成
//----------------------------------------------------------------------------
int PreProcess(char* psToken[])
{
    LOG(INFO,"PreProcess呼び出し");
    int i = 0;           //トークン配列の添え字用の変数
    int iParenCount = 0; //丸括弧の数をカウントする変数
   
    while(1){
        
        LOG(DEBUG,"psToken[%d] = %s",i,psToken[i]);
     
        //トークンがセミコロンと一致
        if(psToken[i][0] == ';'){
            
            //文が処理を伴っているかの判定
            if(i >= 2){
                //ここでは何もしない
                break;
            }
            else {
                printf("処理を伴わない文です。\n");
                LOG(WARN,"%d行目 : 処理を伴わない文です。",g_iCodeCount);
                return 1;
            }
        }
        //トークンが開き丸括弧と一致
        else if(psToken[i][0] == '(') {
            
            iParenCount++;
            LOG(DEBUG,"iParenCount = %d",iParenCount);
            
            if(psToken[i+1][0] == ')') {
                printf("括弧内に要素がありません。\n");
                LOG(WARN,"%d行目 : 括弧内に要素がありません。",g_iCodeCount);
                return 1;
            }
            else {
                //ここでは何もしない
            }
        }
        //トークンが閉じ丸括弧と一致
        else if(psToken[i][0] == ')') {
            
            iParenCount--;
            LOG(DEBUG,"iParenCount = %d",iParenCount);

            if(iParenCount >= 0){
                //ここでは何もしない
            }
            else {
                printf("括弧の対応が不正です。\n");
                return 1;
            }
        }
        //トークンが代入演算子と一致
        else if(psToken[i][0] == '=') {
            
            if(i == 1) {
                
                //代入演算子の左辺が変数であるかの判定
                if( isalpha(psToken[i-1][0]) ) {
                    
                    //代入演算子の左辺の変数が予約語であるかの判定
                    if(JudgeReserved(psToken[g_iTokenCount])) {
                        //ここでは何もしない
                    }
                    else {
                        printf("予約語を変数名として使用しています。\n");
                        return 1;
                    }
                }
                else {
                    printf("代入演算子の左辺が不正です。\n");
                    return 1;
                }
            }
            else {
                printf("代入演算子の左辺が不正です。\n");
                return 1;
            }
        }
        //トークンが変数名
        else if(isalpha(psToken[i][0])) {

            //変数名が予約語と一致
            if(JudgeReserved(psToken[i]) == 0) {
            
                //関数名の次が開き丸括弧であるかの判定
                if(psToken[i+1][0] == '(') {
                    //ここでは何しない
                }
                else{
                    printf("関数の呼び出しが不正です。\n");
                    LOG(WARN,"%d行目 : 関数の呼び出しが不正です。",g_iCodeCount); 
                    return 1;
                }
            }
            else {
                //ここでは何もしない
            }
        }
        else {
            //ここでは何もしない
        }
        
        //次のトークンに進める
        i++;
        LOG(DEBUG,"i = %d",i);
    }
    return 0;
}

//----------------------------------------------------------------------------
//関数名 : Formula
//概要　 : 式処理を行う
//引数　 : (I)トークン配列
//戻り値 : 正常終了 : 0
//　　　   異常終了 : 1
//履歴　 : 2019.07.17 平大真　新規作成
//----------------------------------------------------------------------------
TItem* Formula(char* psToken[])
{
    TItem* pkNode = NULL;   //構文木の親ノードを格納
    TItem* pkLeft = NULL;   //構文木の左ノードを格納
    TItem* pkRight = NULL;  //構文木の右ノードを格納
    
    pkLeft = Term(psToken);
    LOG(DEBUG,"g_iTokenCount = %d",g_iTokenCount);
    LOG(DEBUG,"psToken[%d] = %s",g_iTokenCount,psToken[g_iTokenCount]);
    
    //ノードが空でない場合、次のトークンを読み込む
    if(pkLeft == NULL) {
        return NULL;
    }
    else {
        while(1) {
            if(psToken[g_iTokenCount][0] == '+' || psToken[g_iTokenCount][0] == '-') {
                //+,-演算子のノードを生成
                pkNode = CreateItem("0",psToken[g_iTokenCount]);
                g_iTokenCount++; 
                LOG(DEBUG,"g_iTokenCount = %d",g_iTokenCount);
                
                if(pkNode == NULL) {
                    printf("メモリの確保に失敗しました。\n");
                    LOG(ERROR,"%d行目 : メモリの確保に失敗しました。",g_iCodeCount);
                    ReleaseTree(pkLeft);
                    return NULL;   
                }
                else {
                    //項処理呼び出し
                    pkRight = Term(psToken);
                    
                    LOG(DEBUG,"g_iTokenCount = %d",g_iTokenCount);

                    if(pkRight == NULL) {
                        printf("メモリの確保に失敗しました。\n");
                        LOG(ERROR,"%d行目 : メモリの確保に失敗しました。",g_iCodeCount);
                        
                        ReleaseTree(pkLeft);
                        ReleaseTree(pkNode);
                        return NULL;
                    }
                    else {
                        pkLeft = JoinNode(pkNode,pkLeft,pkRight); 
                    }
                }  
            }
            else if(psToken[g_iTokenCount][0] == ')'){
                //閉じ丸括弧のトークンを解放
                free(psToken[g_iTokenCount]);
                g_iTokenCount++;

                return pkLeft;
                
            }
            else if(psToken[g_iTokenCount][0] == ';') {
                return pkLeft;
                
            }
        }
    }
}

//----------------------------------------------------------------------------
//関数名 : Term
//概要　 : 項処理を行う
//引数　 : (I)トークン配列
//戻り値 : 正常終了 : 0
//　　　   異常終了 : 1
//履歴　 : 2019.07.17 平大真　新規作成
//----------------------------------------------------------------------------
TItem* Term(char* psToken[])
{
    TItem* pkNode = NULL;   //構文木の親ノードを格納
    TItem* pkLeft = NULL;   //構文木の左ノードを格納
    TItem* pkRight = NULL;  //構文木の右ノードを格納
    
    pkLeft = Factor(psToken);
    
    LOG(DEBUG,"psToken[%d] = %s",g_iTokenCount,psToken[g_iTokenCount]);
    
    if(pkLeft == NULL) {
        return NULL;
    }
    else {
        while(1) {
            if(psToken[g_iTokenCount][0] == '*' || psToken[g_iTokenCount][0] == '/') {
                //*,/演算子のノードを生成
                pkNode = CreateItem("0",psToken[g_iTokenCount]);
                g_iTokenCount++; 
                LOG(DEBUG,"g_iTokenCount = %d",g_iTokenCount);

                if(pkNode == NULL) {
                    printf("メモリの確保に失敗しました。\n");
                    LOG(ERROR,"%d行目 : メモリの確保に失敗しました。",g_iCodeCount);
                    ReleaseTree(pkLeft);
                    return NULL;   
                }
                else {
                    
                    //因子処理呼び出し
                    pkRight = Factor(psToken);
                    LOG(DEBUG,"g_iTokenCount = %d",g_iTokenCount);

                    if(pkRight == NULL) {
                        printf("メモリの確保に失敗しました。\n");
                        LOG(ERROR,"%d行目 : メモリの確保に失敗しました。",g_iCodeCount);
                        ReleaseTree(pkLeft);
                        ReleaseTree(pkNode);
                        return NULL;
                    }
                    else {
                        pkLeft = JoinNode(pkNode,pkLeft,pkRight);
                    }
                }  
            }
            else {

                return pkLeft;
                
            }
        }
    }
}

//----------------------------------------------------------------------------
//関数名 : Factor
//概要　 : 因子処理を行う
//引数　 : (I)トークン配列
//戻り値 : 正常終了 : 0
//　　　   異常終了 : 1
//履歴　 : 2019.07.17 平大真　新規作成
//----------------------------------------------------------------------------
TItem* Factor(char* psToken[])
{
    TItem* pkNode = NULL;   //構文木の親ノードを格納
    TItem* pkLeft = NULL;   //構文木の左ノードを格納
    TItem* pkRight = NULL;  //構文木の右ノードを格納
    char* psZero = NULL;    //文字列"0"を格納するための変数
    
    LOG(DEBUG,"g_iTokenCount = %d",g_iTokenCount);
    LOG(DEBUG,"psToken[%d] = %s",g_iTokenCount,psToken[g_iTokenCount]);
    
    if(psToken[g_iTokenCount][0] == '"' || isdigit(psToken[g_iTokenCount][0])){
        pkNode = CreateItem("0",psToken[g_iTokenCount]);
        
        if(pkNode != NULL) {
            g_iTokenCount++;
            LOG(DEBUG,"g_iTokenCount = %d",g_iTokenCount);
            LOG(DEBUG,"psToken[%d] = %s",g_iTokenCount,psToken[g_iTokenCount]);
            return pkNode;
        }
        else {
            printf("メモリの確保に失敗しました。\n");
            LOG(ERROR,"%d行目 : メモリの確保に失敗しました。",g_iCodeCount);
            return NULL;
        }   
    }
    //トークンが関数名と一致
    else if(JudgeReserved(psToken[g_iTokenCount]) == 0) {
       
       pkNode = CreateItem("0", psToken[g_iTokenCount]);
       
       if(pkNode != NULL) {
           //開き丸括弧読み飛ばし
           g_iTokenCount++;
           LOG(DEBUG,"g_iTokenCount = %d",g_iTokenCount);
           free(psToken[g_iTokenCount]);
           
           g_iTokenCount++;
           LOG(DEBUG,"g_iTokenCount = %d",g_iTokenCount);
           pkLeft = Formula(psToken);
           
           if(pkLeft != NULL){
               //ノード結合
               pkNode = JoinNode(pkNode,pkLeft,pkRight);
               return pkNode;
           }
           else {
               
               ReleaseTree(pkNode);
               printf("メモリ確保に失敗しました。\n");
               LOG(ERROR,"%d行目 : メモリ確保に失敗しました。",g_iCodeCount);
               return NULL;
           }
       }
       else {
           printf("メモリ確保に失敗しました。\n");
           LOG(ERROR,"%d行目 : メモリ確保に失敗しました。",g_iCodeCount);
           return NULL;
       }  
    }
    
    //トークンが変数
    else if(isalpha(psToken[g_iTokenCount][0])) {
        
        pkLeft = CreateItem("0",psToken[g_iTokenCount]);
        
        if(pkLeft != NULL) {
            g_iTokenCount++;
            
            if(psToken[g_iTokenCount][0] == '=') {
                pkNode = CreateItem("0", psToken[g_iTokenCount]);
                g_iTokenCount++;
                
                if(pkNode != NULL) {
                    pkRight = Formula(psToken);
                    
                    if(pkRight != NULL) {
                        
                        pkNode = JoinNode(pkNode,pkLeft,pkRight);
                        return pkNode;
                    }
                    else {
                        printf("メモリ確保に失敗しました。\n");
                        LOG(ERROR,"%d行目 : メモリの確保に失敗しました。",g_iCodeCount);
                        ReleaseTree(pkLeft);
                        ReleaseTree(pkNode);
                        return NULL;
                    }
                }
                else {
                    ReleaseTree(pkLeft);
                    return NULL;
                }   
            }
            else {
                return pkLeft;
            }
        }
        else {
           printf("メモリ確保に失敗しました。\n");
           LOG(ERROR,"%d行目 : メモリ確保に失敗しました。",g_iCodeCount);
           return NULL;
        }
    }
    
    else if(psToken[g_iTokenCount][0] == '(') {
        //開き丸括弧のトークンを解放
        free(psToken[g_iTokenCount]);
        
        g_iTokenCount++;
        
        //式処理呼び出し
        pkNode = Formula(psToken);
        
        if(pkNode != NULL) {
            
            return pkNode;
        }
        else {
            return NULL;
        }
    }
    else if(JudgeMinus(psToken) == 0) {
        pkNode = CreateItem("0", psToken[g_iTokenCount]);
        g_iTokenCount++;
        
        if(pkNode != NULL) {
            //メモリ領域確保
            psZero = (char*)malloc(sizeof(char) * 2 );
            
            if(psZero != NULL) {
                psZero[0] = '0';
                
                //終端文字
                psZero[1] = '\0';
                
                //構造体生成
                pkLeft = CreateItem("0",psZero);
                
                if(pkLeft != NULL) {
                    //式処理呼び出し
                    pkRight = Formula(psToken);
                    
                    if(pkRight != NULL) {
                        //ノード結合
                        pkNode = JoinNode(pkNode, pkLeft, pkRight);
                        return pkNode;
                    }
                    else {
                      return NULL;  
                    }
                }
                else {
                    free(psZero);
                    printf("メモリ確保に失敗しました。\n");
                    LOG(ERROR,"%d行目 : メモリ確保に失敗しました。",g_iCodeCount);
                    return NULL;
                }
            }
            else {
                printf("メモリ確保に失敗しました。\n");
                LOG(ERROR,"%d行目 : メモリ確保に失敗しました。",g_iCodeCount);
                return NULL;
            }
        }
        else {
            printf("メモリ確保に失敗しました。\n");
            LOG(ERROR,"%d行目 : メモリ確保に失敗しました。",g_iCodeCount);
            return NULL;
        }   
    }
}

//----------------------------------------------------------------------------
//関数名 : joinNode
//概要　 : ノードを結合する
//引数　 : (I)構文木の親ノード
//　　　   (I)構文木の左ノード
//　　　   (I)構文木の右ノード
//戻り値 : 正常終了 : 0
//　　　   異常終了 : 1
//履歴　 : 2019.07.17 平大真　新規作成
//----------------------------------------------------------------------------
TItem* JoinNode(TItem* pkNode, TItem* pkLeftNode, TItem* pkRightNode)
{
    //左子ノードを親ノードに結合
    pkNode->pkPrev = pkLeftNode;
    
    //右子ノードを親ノードに結合
    pkNode->pkNext = pkRightNode;
    
    return pkNode;
}

