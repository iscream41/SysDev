//------------------------------------------------------------------------
// ファイル名  : main.c
// 概要       : ファイルを読み込みを行い、字句解析、構文解析、処理実行関数を呼び出す。
// 履歴       : 2019.7.10 平大真 新規作成。
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
#include "Excution.h"
//------------------------------------------------------------------------
// プロトタイプ宣言                                           
//------------------------------------------------------------------------
int GetToken(FILE* pFile, char* psToken[]);
int JudgeLineFeed (int iWord);
int JudgeBlank(int iWord);
void InitializeToken(char* psToken[]);
void ReleaseToken(char* psToken[]);
//-------------------------------------------------------------------------
// 定数
//-------------------------------------------------------------------------
#define DEBUG	1	// デバッグ
#define INFO	2	// インフォメーション
#define WARN	3	// ワーニング
#define ERROR	4	// エラー

//-------------------------------------------------------------------------
// 外部変数
//-------------------------------------------------------------------------
int g_iCodeCount = 1; //ソースファイルの行数を格納
int g_iTokenCount = 0; //何番目のトークンまで解析が終了しているのかを格納

//----------------------------------------------------------------------------
// 関数定義
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//関数名 : main
//概要　 : ファイルを読み込みを行い、字句解析、構文解析、処理実行関数を呼び出す。
//引数　 :(I)コマンドライン引数の総数
//　　　  (I)コマンドライン引数
//戻り値 : 正常終了 : 0
//　　　   異常終了 : 1
//履歴　 : 2019.07.12 平大真　新規作成
//----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    FILE* pFile = NULL;  //ファイルポインタ
    char* psToken[100];  //トークンを格納する配列
    TItem* pkNode = NULL; //構文木のノードを格納
    int iDiff = 0;   //ファイル名を保存した配列の添え字用の変数
    int iFlag = 0;   //ファイルの拡張子が正しいかどうかを管理するフラグ
    

    //argcが1のとき
    if(argc == 1) {
        printf("コマンドライン引数が不正です。\n");
        LOG(WARN,"コマンドライン引数が不正です。");
        return 1;
    }
    
    //argcが2のとき
    else if(argc == 2) {
        
        //ファイル名の長さから拡張子分の要素数を減算することで、
        //配列から拡張子を一文字ずつ参照可能とする
        iDiff = strlen(argv[1]) - 3;
        LOG(DEBUG,"iDiff = %d",iDiff);
        
        
        //ファイルの拡張子が正しいかどうかの条件
        LOG(DEBUG,"argv[1][iDiff] = %c",argv[1][iDiff]);
        if(argv[1][iDiff] == '.') {
            
            LOG(DEBUG,"argv[1][iDiff+1] = %c",argv[1][iDiff+1]);
            if(argv[1][iDiff + 1] == 'c'){
                
                LOG(DEBUG,"argv[1][iDiff+2] = %c",argv[1][iDiff+2]);
                if(argv[1][iDiff + 2] == 'c') {
                    iFlag = 1;
                }
            }
        }
                  
        if(iFlag == 1) {
           
            if((pFile = fopen(argv[1], "r") ) == NULL) {
               printf("ファイルが存在しません。\n");
               LOG(ERROR,"%d行目 : ファイルが存在しません。",g_iCodeCount);
               
               return 1;       
            }
            else {
               //ここでは何もしない
            }
           
            //全処理ループ
            while(1){
                //トークン配列の初期化
                InitializeToken(psToken);
                
                if (GetToken(pFile,psToken) == 0) {
                    
                    LOG(DEBUG,"psToken[0] = %s",psToken[0]);
                    if(psToken[0] == NULL) {
                        break;
                    }
                     
                    //構文解析機能呼び出し
                    pkNode = MakeTree(psToken);
                    if(pkNode == NULL){
                        break;
                    }
                    else {
                        //ここでは何もしない
                    }
                    
                    //処理実行呼び出し
                    if(Excution(pkNode) == 1) {
                        break;
                    }
                    
                    
                    
                    //トークン配列の解放
                    LOG(DEBUG,"g_iTokenCount = %d",g_iTokenCount);
                    ReleaseTree(pkNode);
                    ReleaseToken(psToken);
                    
                    g_iTokenCount = 0;
                    
                }
                else {
                    break;
                }
            }  
        }
        else {
            printf("ファイルの拡張子が異なります。\n");
            LOG(WARN,"%d行目 : ファイルの拡張子が異なります。",g_iCodeCount);
        }   
    }
    //argcが3のとき
    else if(argc == 3) {
        
        //ログレベルを確認
        if(!strcmp(argv[1],"-w")) {
            LogInit(WARN,"./");
        }
        else if(!strcmp(argv[1],"-l")) {
            LogInit(INFO,"./");
        }
        else if(strcmp(argv[1],"-d") == 0) {
            LogInit(DEBUG,"./");
        }
        else {
            printf("不正なオプションを指定しています。\n");
            LOG(WARN,"%d行目 : 不正なオプションを指定しています。",g_iCodeCount);
            return 1;
        }
        
        //ファイル名の長さから拡張子分の要素数を減算することで、
        //配列から拡張子を一文字ずつ参照可能とする
        iDiff = strlen(argv[2]) - 3;
        LOG(DEBUG,"iDiff = %d",iDiff);
        
        
        //ファイルの拡張子が正しいかどうかの条件
        LOG(DEBUG,"argv[2][iDiff] = %c",argv[2][iDiff]);
        if(argv[2][iDiff] == '.') {
            
            LOG(DEBUG,"argv[2][iDiff] = %c",argv[2][iDiff+1]);
            if(argv[2][iDiff + 1] == 'c'){
                
                LOG(DEBUG,"argv[2][iDiff] = %c",argv[2][iDiff+2]);
                if(argv[2][iDiff + 2] == 'c') {
                    
                    iFlag = 1;
                }
            }
        }
        
        //ファイルの拡張子が正しいかどうかの条件
        
        if(iFlag == 1) {
            
            if((pFile = fopen(argv[2], "r") ) == NULL) {
               printf("ファイルが存在しません。\n");
               LOG(ERROR,"%d行目 : ファイルが存在しません。",g_iCodeCount);

               return 1;       
            }
            else {
               //ここでは何もしない
            }
           
            //全処理ループ
            while(1){
                //トークン配列の初期化
                InitializeToken(psToken);
                
                if (GetToken(pFile,psToken) == 0) {
                    
                    LOG(DEBUG,"psToken[0] = %s",psToken[0]);
                    if(psToken[0] == NULL) {
                        break;
                    }
                     
                    //構文解析機能呼び出し
                    pkNode = MakeTree(psToken);
                    if(pkNode == NULL){
                        break;
                    }
                    else {
                        //ここでは何もしない
                    }
                    
                    //処理実行呼び出し
                    if(Excution(pkNode) == 1) {
                        break;
                    }
                    
                    
                    
                    //トークン配列の解放
                    LOG(DEBUG,"g_iTokenCount = %d",g_iTokenCount);
                    ReleaseTree(pkNode);
                    ReleaseToken(psToken);
                    
                    g_iTokenCount = 0;
                    
                }
                else {
                    break;
                }
            }
        }
        else {
            printf("ファイルの拡張子が異なります。\n");
            LOG(WARN,"%d行目 : ファイルの拡張子が異なります。",g_iCodeCount);
        } 
    }
    
    //コマンドライン引数が4以上
    else {
        printf("コマンドライン引数が不正です。\n");
        LOG(WARN,"コマンドライン引数が不正です。");
        return 1;
    }
    return 0;
}

//----------------------------------------------------------------------------
// 関数定義
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//関数名 : GetToken
//概要　 : 文字に応じた判定関数を呼び出し、1文分のトークン配列を取得する。
//引数　 : トークン配列
//戻り値 : 正常終了 : 0
//        異常終了 : 1
//履歴　 : 2019.07.10 平大真　新規作成
//----------------------------------------------------------------------------
int GetToken(FILE* pFile, char* psToken[]) 
{
    LOG(INFO,"GetToken呼び出し");
    int iTokenCount = 1;           //トークン数をカウントする変数
    int iArrayCount = 0;           //トークン配列の添え字用の変数
    int iWord = 0;                 //ファイルから読み込んだ一文字を格納する変数
    char* psTmpToken = NULL;       //一時的にトークンの格納する変数
    char* psSemicolonToken = NULL; //セミコロンを一時的に格納する変数
    
    
    while(1) {
        iWord = fgetc(pFile);
        LOG(DEBUG,"iWord = %c",iWord);
        
        //文字列判定
        if(!JudgeString(iWord)) {
            
            iWord = fgetc(pFile);
            LOG(DEBUG,"iWord = %c",iWord);
            
            iTokenCount++;
            LOG(DEBUG,"iTokenCount = %d",iTokenCount);
            
            while(1) {
                
                //閉じのダブルクォーテーションを読み込む
                if(!JudgeString(iWord)) {
                    
                    //閉じのダブルクォーテーションの次まで読み込む
                    iWord = fgetc(pFile);
                    LOG(DEBUG,"iWord = %c",iWord);
                    
                    //終端文字用のインクリメント
                    iTokenCount++;
                    LOG(DEBUG,"iTokenCount = %d",iTokenCount);
                    
                    if(iWord == EOF) {
                        printf("セミコロン(;)が不足しています。\n");
                        LOG(WARN,"%d行目 : セミコロン(;)が不足しています。",g_iCodeCount);
                        return 1;
                    }
                    else {
                        
                        //トークン格納処理 
                        psTmpToken = MakeToken(pFile,iTokenCount);
                        LOG(DEBUG,"psTmpToken = %s",psTmpToken);
                        
                        psToken[iArrayCount] = psTmpToken;
                        LOG(DEBUG,"psToken[%d] = %s",iArrayCount, psToken[iArrayCount]);
                        
                        iArrayCount++;
                        LOG(DEBUG,"iArrayCount = %d",iArrayCount);
                        
                        iTokenCount = 1;
                        LOG(DEBUG,"iTokenCount = %d",iTokenCount);
                    }
                    
                    break;

                }
                else {
                    //文字列を閉じるダブルクォーテーションを読み込まずにEOFに到達
                    if( iWord == EOF) {
                        printf("文字列が閉じられていません。\n");
                        LOG(WARN,"%d行目 : 文字列が閉じられていません。",g_iCodeCount);
                        
                        return 1;
                    }
                    else {
                        iWord = fgetc(pFile);
                        LOG(DEBUG,"iWord = %c",iWord);
                        
                        iTokenCount++;
                        LOG(DEBUG,"iTokenCount = %d",iTokenCount);
                    }
                }
            }
        }
        
        
        //アルファベット判定
        else if(isalpha(iWord)) {
            
            while(1) {
                
                if(isalpha(iWord)) {
                    iWord = fgetc(pFile);                    
                    LOG(DEBUG,"iWord = %c",iWord);
                    
                    iTokenCount++;
                    LOG(DEBUG,"iTokenCount = %d",iTokenCount);
                    
                }
                else {
                    //EOFを読み込む
                    if( iWord != EOF) {
                        //トークン格納処理
                        psTmpToken = MakeToken(pFile,iTokenCount);
                        LOG(DEBUG,"psTmpToken = %s",psTmpToken);
                        
                        //変数名が予約語と一致しない、かつ
                        //変数名が20文字以下であるかの判定
                        if(JudgeReserved(psTmpToken)){
                            
                            if(iTokenCount <= 20) {
                                
                                psToken[iArrayCount] = psTmpToken;
                                LOG(DEBUG,"psToken[%d] = %s",iArrayCount, psToken[iArrayCount]);
                                
                                iArrayCount++;
                                LOG(DEBUG,"iArrayCount = %d",iArrayCount);
                                
                                iTokenCount = 1;
                                LOG(DEBUG,"iTokenCount = %d",iTokenCount);
                                
                                break;
                                
                            }
                            else {
                                
                                LOG(DEBUG,"psTmpToken = %s",psTmpToken);
                                free(psTmpToken);
                                
                                printf("変数名の文字列長が21文字以上です。\n");
                                LOG(WARN,"%d行目 : 変数名の文字列長が21文字以上です。",g_iCodeCount);
                                
                                return 1;
                            }
                            
                        } 
                        else {
                                psToken[iArrayCount] = psTmpToken;
                                LOG(DEBUG,"psToken[%d] = %s",iArrayCount, psToken[iArrayCount]);
                                
                                iArrayCount++;
                                LOG(DEBUG,"iArrayCount = %d",iArrayCount);
                                
                                iTokenCount = 1;
                                LOG(DEBUG,"iTokenCount = %d",iTokenCount);
                                
                                break;
                        }                   
                    }
                    //アルファベットの次がEOF
                    else { 
                        printf("セミコロン(;)が不足しています。\n");
                        LOG(WARN,"%d行目 : セミコロン(;)が不足しています。",g_iCodeCount);

                        return 1;
                    }
                }
            }
        }
        
        //数値判定
        else if(isdigit(iWord)) {
            
            while(1) {
                if(isdigit(iWord)) {
                    iWord = fgetc(pFile);
                    LOG(DEBUG,"iWord = %c",iWord);
                    
                    iTokenCount++;
                    LOG(DEBUG,"iTokenCount = %d",iTokenCount);
                }
                else {
                    //EOFを読み込む
                    if( iWord == EOF) {
                        printf("セミコロン(;)が不足しています。\n");
                        LOG(WARN,"%d行目 : セミコロン(;)が不足しています。",g_iCodeCount);

                        return 1;
                    }
                    
                    //四則演算子、閉じ丸括弧、空白文字、改行文字、セミコロンと一致
                    
                    else if(!JudgeArithmetic(iWord) || !JudgeCloseParen(iWord) 
                            || !JudgeBlank(iWord) || !JudgeLineFeed(iWord) || iWord == ';'){
                        

                        psTmpToken = MakeToken(pFile,iTokenCount);
                        LOG(DEBUG,"psTmpToken = %s",psTmpToken);
                        
                        //2桁以上の数値であるかを判定
                        if(strlen(psTmpToken) > 2) {
                        
                            //数値の先頭要素が0であるかの判定
                            if(psTmpToken[0] != '0' ) {
                                if(!JudgeNumberRange(psTmpToken,iTokenCount)){
                                    psToken[iArrayCount] = psTmpToken;
                                    LOG(DEBUG,"psToken[%d] = %s",iArrayCount, psToken[iArrayCount]);

                                    iArrayCount++;
                                    LOG(DEBUG,"iArrayCount = %d",iArrayCount);

                                    iTokenCount = 1;
                                    LOG(DEBUG,"iTokenCount = %d",iTokenCount);
                                }
                                else {
                                    printf("範囲外の数値を使用しています。\n");
                                    LOG(WARN,"%d行目 : 範囲外の数値を使用しています。",g_iCodeCount);
                                    return 1;
                                }                          
                            }
                            else {
                                printf("不正な数値の並びです。\n");
                                LOG(WARN,"%d行目 : 不正な数値の並びです。",g_iCodeCount);
                                return 1;
                            }
                        }
                        else {
                                
                            psToken[iArrayCount] = psTmpToken;
                            LOG(DEBUG,"psToken[%d] = %s",iArrayCount, psToken[iArrayCount]);

                            iArrayCount++;
                            LOG(DEBUG,"iArrayCount = %d",iArrayCount);

                            iTokenCount = 1;
                            LOG(DEBUG,"iTokenCount = %d",iTokenCount);
 
                        }
                    }
                    else {
                        printf("数値の後続要素が不正です。\n");
                        LOG(WARN,"%d行目 : 数値の後続要素が不正です。",g_iCodeCount);
                        return 1;
                    }
                    
                    break;
                }
            }
        }
        
        //コメント判定
        else if(iWord == '/' && JudgeComment(pFile, iWord)) {
            
        }       
        
        //四則演算子判定
        else if(!JudgeArithmetic(iWord))  {
            
            iWord = fgetc(pFile);
            LOG(DEBUG,"iWord = %c",iWord);
            
            iTokenCount++;
            LOG(DEBUG,"iTokenCount = %d",iTokenCount);
            
            if( iWord == EOF) {
                printf("セミコロン(;)が不足しています。\n");
                LOG(WARN,"%d行目 : セミコロン(;)が不足しています。",g_iCodeCount);

                return 1;
            }
            else {
                psTmpToken = MakeToken(pFile,iTokenCount);
                LOG(DEBUG,"psTmpToken = %s",psTmpToken);

                psToken[iArrayCount] = psTmpToken;
                LOG(DEBUG,"psToken[%d] = %s",iArrayCount, psToken[iArrayCount]);

                iArrayCount++;
                LOG(DEBUG,"iArrayCount = %d",iArrayCount);

                iTokenCount = 1;
                LOG(DEBUG,"iTokenCount = %d",iTokenCount);

               //MakeTokenでポインタの位置は戻っているため、fseek不要
            }
        }
        
        //代入演算子判定
        else if(!JudgeAssign(iWord)) {
                        
            iWord = fgetc(pFile);
            LOG(DEBUG,"iWord = %c",iWord);
            
            iTokenCount++;
            LOG(DEBUG,"iTokenCount = %d",iTokenCount);

            
            if( iWord == EOF) {
                printf("セミコロン(;)が不足しています。\n");
                LOG(WARN,"%d行目 : セミコロン(;)が不足しています。",g_iCodeCount);

                return 1;
            }
            else {
                psTmpToken = MakeToken(pFile,iTokenCount);
                LOG(DEBUG,"psTmpToken = %s",psTmpToken);

                psToken[iArrayCount] = psTmpToken;
                LOG(DEBUG,"psToken[%d] = %s",iArrayCount, psToken[iArrayCount]);

                iArrayCount++;
                LOG(DEBUG,"iArrayCount = %d",iArrayCount);

                iTokenCount = 1;
                LOG(DEBUG,"iTokenCount = %d",iTokenCount);
               
               //MakeTokenでポインタの位置は戻っているため、fseek不要
            }
        }
        
        //開き丸括弧判定
        else if(!JudgeOpenParen(iWord)) {
            iWord = fgetc(pFile);
            LOG(DEBUG,"iWord = %c",iWord);
            
            iTokenCount++;
            LOG(DEBUG,"iTokenCount = %d",iTokenCount);
            
            if( iWord == EOF) {
                printf("セミコロン(;)が不足しています。\n");
                LOG(WARN,"%d行目 : セミコロン(;)が不足しています。",g_iCodeCount);

                return 1;
            }
            else {
                psTmpToken = MakeToken(pFile,iTokenCount);
                LOG(DEBUG,"psTmpToken = %s",psTmpToken);

                psToken[iArrayCount] = psTmpToken;
                LOG(DEBUG,"psToken[%d] = %s",iArrayCount, psToken[iArrayCount]);

                iArrayCount++;
                LOG(DEBUG,"iArrayCount = %d",iArrayCount);

                iTokenCount = 1;
                LOG(DEBUG,"iTokenCount = %d",iTokenCount);

               //MakeTokenでポインタの位置は戻っているため、fseek不要
            }
            
        }
        
        //閉じ丸括弧判定
        else if(!JudgeCloseParen(iWord)) {
            iWord = fgetc(pFile);
            LOG(DEBUG,"iWord = %c",iWord);
            
            iTokenCount++;
            LOG(DEBUG,"iTokenCount = %d",iTokenCount);
            
            if( iWord == EOF) {
                printf("セミコロン(;)が不足しています。\n");
                LOG(WARN,"%d行目 : セミコロン(;)が不足しています。",g_iCodeCount);

                return 1;
            }
            else {
                psTmpToken = MakeToken(pFile,iTokenCount);
                LOG(DEBUG,"psTmpToken = %s",psTmpToken);

                psToken[iArrayCount] = psTmpToken;
                LOG(DEBUG,"psToken[%d] = %s",iArrayCount, psToken[iArrayCount]);

                iArrayCount++;
                LOG(DEBUG,"iArrayCount = %d",iArrayCount);

                iTokenCount = 1;
                LOG(DEBUG,"iTokenCount = %d",iTokenCount);

               //MakeTokenでポインタの位置は戻っているため、fseek不要
            }
        }
        
        //改行文字判定
        else if(!JudgeLineFeed(iWord)) {
            LOG(DEBUG,"iWord = %c",iWord);
        }
        
        //空白文字判定
        else if(!JudgeBlank(iWord)) {
            LOG(DEBUG,"iWord = %c",iWord);
        }
        
        //セミコロン判定
        else if(iWord == ';') {
            LOG(DEBUG,"iWord = %c",iWord);
            
            //セミコロンの前にポインタを調整
            fseek(pFile,-1,SEEK_CUR);
            
            if( (psSemicolonToken = (char*)malloc(sizeof(char) * 2)) != NULL ) {
                psSemicolonToken[0] = (char)fgetc(pFile);
                LOG(DEBUG,"psSemicolonToken[0] = %s",psSemicolonToken[0]);
            }
            else {
                //メモリ確保失敗
                printf("メモリ確保に失敗しました。\n");
                LOG(ERROR,"%d行目 : メモリ確保に失敗しました。",g_iCodeCount);
                
                
            }
            
            //終端文字の代入
            psSemicolonToken[1] = '\0';
            
            
            psToken[iArrayCount] = psSemicolonToken;
            LOG(DEBUG,"psToken[%d] = %s",iArrayCount, psToken[iArrayCount]);
            
            iArrayCount++;
            LOG(DEBUG,"iArrayCount = %d",iArrayCount);
            
            iTokenCount = 1;
            LOG(DEBUG,"iTokenCount = %d",iTokenCount);
            
            break;
        }
        
        //不正字句判定
        else {
            LOG(DEBUG,"iWord = %c",iWord);
            LOG(DEBUG,"psToken[0] = %s",psToken[0]);
            
            if(iWord == EOF && psToken[0] == NULL) {
                return 0;
            }
            else if(iWord == EOF) {
                printf("セミコロン(;)が不足しています。\n");
                LOG(WARN,"%d行目 : セミコロン(;)が不足しています。",g_iCodeCount);
                return 1;
            }
            else {
                printf("不正な字句が使用されています。\n");
                LOG(WARN,"%d行目 : 不正な字句が使用されています。",g_iCodeCount);
                
                return 1;
            }   
        }        
    }  
    return 0;
}



//----------------------------------------------------------------------------
// 関数定義
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//関数名 : JudgeLineFeed
//概要　 : 読み込んだ文字が改行文字であるかどうかを判定
//引数　 : (I)トークン
//戻り値 : 正常終了 : 0
//        異常終了 : 1
//履歴　 : 2019.07.12 平大真　新規作成
//----------------------------------------------------------------------------
int JudgeLineFeed (int iWord) 
{
    LOG(INFO,"JudgeLineFeed呼び出し");
    LOG(DEBUG,"iWord = %c",iWord);
    if((char)iWord == '\n') {
        return 0;
    }
    else {
        return 1;
    }
}


//----------------------------------------------------------------------------
// 関数定義
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//関数名 : JudgeBlank
//概要　 : 読み込んだ文字が空白文字であるかどうかを判定
//引数　 : (I)トークン
//戻り値 : 正常終了 : 0
//        異常終了 : 1
//履歴　 : 2019.07.12 平大真　新規作成
//----------------------------------------------------------------------------
int JudgeBlank(int iWord) 
{
    LOG(INFO,"JudgeBlank呼び出し");
    LOG(DEBUG,"iWord = %c",iWord);
    if(iWord == ' '){
        return 0;
    }
    else {
        return 1;
    }
}

//----------------------------------------------------------------------------
// 関数定義
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//関数名 : InitializeToken
//概要　 : トークン配列をNULLを初期化
//引数　 : トークン配列
//戻り値 : なし
//履歴　 : 2019.07.10 平大真　新規作成
//----------------------------------------------------------------------------
void InitializeToken(char* psToken[]) 
{
    int i = 0; //配列の添え字用の変数
    
    for(i=0; i< 100; i++) {
        psToken[i] = NULL;
    }
} 

//----------------------------------------------------------------------------
// 関数定義
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//関数名 : ReleaseToken
//概要　 : 確保したトークン配列を解放する
//引数　 : (I)トークン配列
//戻り値 : なし
//履歴　 : 2019.07.11 平大真　新規作成
//----------------------------------------------------------------------------
void ReleaseToken(char* psToken[])
{
    
    int iTmpCount = g_iTokenCount;
    
    while(1){
        
        LOG(DEBUG,"iTmpCount = %d",iTmpCount);
        LOG(DEBUG,"psToken[%d] = %s",iTmpCount,psToken[iTmpCount]);
        if(psToken[iTmpCount] != NULL){
            free(psToken[iTmpCount]);
            iTmpCount++;
           // LOG(DEBUG,"g_iTokenCount = %d",iTmpCount);
        }
        else {
            break;
        }
    }
}

