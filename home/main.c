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
#include "ManageVar.h"

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

#define TokenSize 100   //トークン配列のサイズ

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
    char* psToken[TokenSize];  //トークンを格納する配列
    TItem* pkNode = NULL; //構文木のノードを格納
    int iDiff = 0;   //ファイル名を保存した配列の添え字用の変数
    int iFlag = 0;   //ファイルの拡張子が正しいかどうかを管理するフラグ
    
    
    //----------------------------------------------------------------------------
    // コマンドライン引数が1つのとき
    //----------------------------------------------------------------------------
    if(argc == 1) {
        LogInit(ERROR,"./log/");
        printf("コマンドライン引数が不正です。\n");
        LOG(ERROR,"コマンドライン引数が不正です。")
        return 1;
    }
    
    
    //----------------------------------------------------------------------------
    // コマンドライン引数が2つのとき
    //----------------------------------------------------------------------------
    else if(argc == 2) {
        
        LogInit(ERROR,"./log/");
        //ファイル名の長さから拡張子分の要素数を減算することで、
        //配列から拡張子を一文字ずつ参照可能とする
        iDiff = strlen(argv[1]) - 3;
        
        //ファイルの拡張子が正しいかどうかの条件
        if(argv[1][iDiff] == '.') {
            
            if(argv[1][iDiff + 1] == 'c'){
                
                if(argv[1][iDiff + 2] == 'c') {
                    iFlag = 1;
                }
            }
        }
                  
        if(iFlag == 1) {
           
            if((pFile = fopen(argv[1], "r") ) == NULL) {
               printf("ファイルが存在しません。\n");
               LOG(ERROR,"ファイルが存在しません。");
               return 1;       
            }
            else {
               //ここでは何もしない
            }
        }
        else {
            printf("ファイルの拡張子が異なります。\n");
            LOG(ERROR,"ファイルの拡張子が異なります。");
            return 1;
        }
    }
    
    
    //----------------------------------------------------------------------------
    // コマンドライン引数が3つのとき
    //----------------------------------------------------------------------------
    else if(argc == 3) {
        
        //ログレベルを確認
        if(!strcmp(argv[1],"-w")) {
            LogInit(WARN,"./log/");
        }
        else if(!strcmp(argv[1],"-i")) {
            LogInit(INFO,"./log/");
        }
        else if(strcmp(argv[1],"-d") == 0) {
            LogInit(DEBUG,"./log/");
        }
        else {
            LogInit(ERROR,"./log/");
            printf("不正なオプションを指定しています。\n");
            LOG(ERROR,"不正なオプションを指定しています。");
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
               LOG(ERROR,"ファイルが存在しません。");
               return 1;       
            }
            else {
               //ここでは何もしない
            } 
        }
        else {
            printf("ファイルの拡張子が異なります。\n");
            LOG(ERROR,"ファイルの拡張子が異なります。");
            return 1;
        } 
    }
    //コマンドライン引数が4以上
    else {
        LogInit(ERROR,"./log/");
        printf("コマンドライン引数が不正です。\n");
        LOG(ERROR,"コマンドライン引数が不正です。");
        return 1;
    }
    
    //全処理ループ
    while(1){
        //トークン配列の初期化
        InitializeToken(psToken);

        //トークン生成処理呼び出し
        if (GetToken(pFile,psToken) == 0) {

            if(psToken[0] == NULL) {
                break;
            }
            else {
              LOG(DEBUG,"psToken[0] = %s",psToken[0]);  
            }

            //構文解析機能呼び出し
            pkNode = MakeTree(psToken);
            if(pkNode == NULL){
                break;
            }
            else {
                 LOG(DEBUG,"pkNode->pValue = %s",(char*)pkNode->pValue);
            }

            //処理実行呼び出し
            if(Excution(pkNode) == 1) {
                break;
            }
            else {
                 LOG(DEBUG,"pkNode->pValue = %s",(char*)pkNode->pValue);
            }

            //トークン配列の解放
            LOG(DEBUG,"g_iTokenCount = %d",g_iTokenCount);
            ReleaseTree(pkNode);
            LOG(DEBUG,"構文木ノードを解放しました。");
            
            ReleaseToken(psToken);
            LOG(DEBUG,"トークン配列を解放しました。");

            pkNode = NULL;
            g_iTokenCount = 0;

        }
        else {
            break;
        }
    }

    //解放処理
    DeleteList();
    LOG(DEBUG,"変数リストを解放しました。");
    
    ReleaseTree(pkNode);
    LOG(DEBUG,"構文木ノードを解放しました。");
    
    ReleaseToken(psToken); 
    LOG(DEBUG,"トークン配列を解放しました。");

    
    //ファイルクローズ
    fclose(pFile);
    LOG(DEBUG,"ファイルをクローズしました。");
    
    //ログクローズ
    LogClose();
    return 0;
}

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
    int iTotalTokenCount = 0;      //読み込んだトークン数をカウントする変数
    int iCommentFlag = 0;          //コメント関数の戻り値を格納する変数
    char* psTmpToken = NULL;       //一時的にトークンの格納する変数
    char* psSemicolonToken = NULL; //セミコロンを一時的に格納する変数
    
    
    
    while(1) {
        if(iTotalTokenCount < 100) {
            LOG(DEBUG,"iTotalTokenCount : %d",iTotalTokenCount);
        }
        else {
            printf("1文のトークン数が%dを超えています。\n",TokenSize);
            LOG(WARN,"%d行目 : 1文のトークン数が%dを超えています。",g_iCodeCount,TokenSize);
            return 1;
        }
        
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
                        
                        iTotalTokenCount++;
                        LOG(DEBUG,"iTotalTokenCount = %d", iTotalTokenCount);
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
                        JudgeLineFeed(iWord);
                        
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
                
                if(isalpha(iWord) || isdigit(iWord)) {
                    iWord = fgetc(pFile);                    
                    LOG(DEBUG,"iWord = %c",iWord);
                    
                    iTokenCount++;
                    LOG(DEBUG,"iTokenCount = %d",iTokenCount); 
                }
                else {
                    //EOFを読み込む
                    if(iWord != EOF) {
                        //トークン格納処理
                        psTmpToken = MakeToken(pFile,iTokenCount);
                        LOG(DEBUG,"psTmpToken = %s",psTmpToken);
                        
                        if(psTmpToken == NULL) {
                            //メモリ確保失敗
                            free(psTmpToken);
                            return 1;
                        }
                        else {
                            //ここでは何もしない
                        }
                        
                        
                        //変数名が予約語と一致していないかの判定
                        if(JudgeReserved(psTmpToken)){
                            
                            //変数名が20文字以下であるかの判定
                            if(iTokenCount <= 21) {
                                
                                psToken[iArrayCount] = psTmpToken;
                                LOG(DEBUG,"psToken[%d] = %s",iArrayCount, psToken[iArrayCount]);
                                
                                iArrayCount++;
                                LOG(DEBUG,"iArrayCount = %d",iArrayCount);
                                
                                iTokenCount = 1;
                                LOG(DEBUG,"iTokenCount = %d",iTokenCount);
                                
                                iTotalTokenCount++;
                                LOG(DEBUG,"iTotalTokenCount = %d", iTotalTokenCount);
                                
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
                                
                                iTotalTokenCount++;
                                LOG(DEBUG,"iTotalTokenCount = %d", iTotalTokenCount);
                                
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
                            || !JudgeBlank(iWord) || iWord == '\n' || iWord == ';'){
                        

                        psTmpToken = MakeToken(pFile,iTokenCount);
                        LOG(DEBUG,"psTmpToken = %s",psTmpToken);
                        
                        if(psTmpToken == NULL) {
                            return 1;
                        }
                        else {
                            //ここでは何もしない
                        }
                        
                        //2桁以上の数値であるかを判定
                        if(strlen(psTmpToken) > 2) {
                        
                            //数値の先頭要素が0であるかの判定
                            if(psTmpToken[0] != '0' ) {
                                
                                //数値が範囲内に収まっているかの判定
                                if(!JudgeNumberRange(psTmpToken,iTokenCount)){
                                    psToken[iArrayCount] = psTmpToken;
                                    LOG(DEBUG,"psToken[%d] = %s",iArrayCount, psToken[iArrayCount]);

                                    iArrayCount++;
                                    LOG(DEBUG,"iArrayCount = %d",iArrayCount);

                                    iTokenCount = 1;
                                    LOG(DEBUG,"iTokenCount = %d",iTokenCount);
                                    
                                    iTotalTokenCount++;
                                    LOG(DEBUG,"iTotalTokenCount = %d", iTotalTokenCount);
                                }
                                else {
                                    //メモリ解放
                                    free(psTmpToken);
                                    return 1;
                                }                          
                            }
                            else {
                                //メモリ解放
                                free(psTmpToken);
                                
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
                            
                            iTotalTokenCount++;
                            LOG(DEBUG,"iTotalTokenCount = %d", iTotalTokenCount);
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
        else if(iWord == '/' && (iCommentFlag = JudgeComment(pFile, iWord)) != 0) {
            if(iCommentFlag == 1) {
                //コメント読み飛ばし
            }
            else {
                return 1;
            }
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
                
                iTotalTokenCount++;
                LOG(DEBUG,"iTotalTokenCount = %d", iTotalTokenCount);

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
                
                iTotalTokenCount++;
                LOG(DEBUG,"iTotalTokenCount = %d", iTotalTokenCount);
               
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
                
                iTotalTokenCount++;
                LOG(DEBUG,"iTotalTokenCount = %d", iTotalTokenCount);

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
                
                iTotalTokenCount++;
                LOG(DEBUG,"iTotalTokenCount = %d", iTotalTokenCount);

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
            
            psSemicolonToken = (char*)malloc(sizeof(char) * 2);
            
            if(psSemicolonToken != NULL) {
                psSemicolonToken[0] = (char)fgetc(pFile);
                LOG(DEBUG,"psSemicolonToken[0] = %c",psSemicolonToken[0]);
            }
            else {
                //メモリ確保失敗
                printf("メモリ確保に失敗しました。\n");
                LOG(ERROR,"%d行目 : メモリ確保に失敗しました。",g_iCodeCount);
                return 1;
            }
            
            //終端文字の代入
            psSemicolonToken[1] = '\0';
            
            psToken[iArrayCount] = psSemicolonToken;
            LOG(DEBUG,"psToken[%d] = %s",iArrayCount, psToken[iArrayCount]);
            
            iArrayCount++;
            LOG(DEBUG,"iArrayCount = %d",iArrayCount);
            
            iTokenCount = 1;
            LOG(DEBUG,"iTokenCount = %d",iTokenCount);
            
            iTotalTokenCount++;
            LOG(DEBUG,"iTotalTokenCount = %d", iTotalTokenCount);
            
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
//関数名 : JudgeLineFeed
//概要　 : 読み込んだ文字が改行文字であるかどうかを判定
//引数　 : (I)トークン
//戻り値 : 一致 : 0
//        不一致 : 1
//履歴　 : 2019.07.12 平大真　新規作成
//----------------------------------------------------------------------------
int JudgeLineFeed (int iWord) 
{
    LOG(INFO,"JudgeLineFeed呼び出し");
    LOG(DEBUG,"iWord = %c",iWord);
    if((char)iWord == '\n') {
        g_iCodeCount++;
        return 0;
    }
    else {
        return 1;
    }
}

//----------------------------------------------------------------------------
//関数名 : JudgeBlank
//概要　 : 読み込んだ文字が空白文字であるかどうかを判定
//引数　 : (I)トークン
//戻り値 : 一致 : 0
//        不一致 : 1
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
//関数名 : InitializeToken
//概要　 : トークン配列をNULLを初期化
//引数　 : トークン配列
//戻り値 : なし
//履歴　 : 2019.07.10 平大真　新規作成
//----------------------------------------------------------------------------
void InitializeToken(char* psToken[]) 
{
    int i = 0; //配列の添え字用の変数
    
    for(i=0; i< TokenSize; i++) {
        psToken[i] = NULL;
        LOG(DEBUG,"psToken[%d] = %s",i ,psToken[i]);
    }
} 

//----------------------------------------------------------------------------
//関数名 : ReleaseToken
//概要　 : 確保したトークン配列を解放する
//引数　 : (I)トークン配列
//戻り値 : なし
//履歴　 : 2019.07.11 平大真　新規作成
//----------------------------------------------------------------------------
void ReleaseToken(char* psToken[])
{
    LOG(INFO,"ReleaseToken呼び出し。");
    int iTmpCount = g_iTokenCount;
    
    while(1){
        
        LOG(DEBUG,"iTmpCount = %d",iTmpCount);
        
        if(psToken[iTmpCount] != NULL){
            LOG(DEBUG,"psToken[%d] = %s",iTmpCount,psToken[iTmpCount]);
            free(psToken[iTmpCount]);
            iTmpCount++;
        }
        else {
            break;
        }
    }
}

