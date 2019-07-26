//------------------------------------------------------------------------
// ファイル名 : List.c
// 概要       : 双方向リストのソースファイル。
// 履歴       : 2017.04.14 森下天明 新規作成。
//------------------------------------------------------------------------
//------------------------------------------------------------------------
// インクルード
//------------------------------------------------------------------------
#include "Logger.h"
#include "List.h"

//------------------------------------------------------------------------
// 関数定義
//------------------------------------------------------------------------
//------------------------------------------------------------------------
// 関数名 : リストアイテム新規作成
// 概要   : リストアイテムを生成する。
//          生成したアイテムは、DeleteItemで削除すること。
//          キーは指定しなくてもよい。
// 引数   : (I)キー
//          (I)バリュー
// 戻り値 : リストアイテム
//          NULL:生成失敗
// 履歴   : 2017.04.14 森下天明 新規作成。
//          2017.08.22 森下天明 第3引数のiValueSizeを削除。
//                              新規アイテムにバリューをセットする際に、
//                              memcpyの第3引数をsizeof(void*)に変更。
//------------------------------------------------------------------------
TItem* CreateItem(
	char*	psKey,		// (I)キー
	void*	pValue		// (I)バリュー
)
{
        printf("create[ %s , %s ]\n", psKey, (char*)pValue);
	TItem*	pkItem = NULL;	// リストアイテム
	int		iLength = 0;	// キーの文字列長

	// 新規アイテムの作成
	pkItem = (TItem*)malloc(sizeof(TItem));
	if (NULL == pkItem) {
		LOG(ERROR, "Memory alloc error.");
		return NULL;
	}
	memset(pkItem, 0x00, sizeof(TItem));

	if (NULL != psKey) {
		// キー領域を確保する
		iLength = strlen(psKey);
		pkItem->psKey = malloc(sizeof(char) * (iLength + 1));
		if (NULL == pkItem->psKey) {
			LOG(ERROR, "Memory alloc error.");
			free(pkItem);
			return NULL;
		}
		memset(pkItem->psKey, 0x00, sizeof(char) * (iLength + 1));

		// 新規アイテムにキーをセット
		strcpy(pkItem->psKey, psKey);
		LOG(DEBUG, "Create list item(%s).", psKey);
	}

	if (NULL != pValue) {
		// 新規アイテムにバリューをセット
		pkItem->pValue = pValue;
	}

	// 新規アイテムのリンクを初期化
	pkItem->pkPrev = NULL;
	pkItem->pkNext = NULL;

	return pkItem;
}

//------------------------------------------------------------------------
// 関数名 : リストアイテム追加
// 概要   : リストにアイテムを追加する。
// 引数   : (I)リストアイテム
//          (I/O)リストの先頭アイテム
//          (I/O)リストの末尾アイテム
// 戻り値 : LIST_SUCCESS     正常終了
//          LIST_PARAM_ERROR パラメータエラー
// 履歴   : 2017.04.14 森下天明 新規作成。
//------------------------------------------------------------------------
int AddItem(
	TItem*	pkItem,		// (I)リストアイテム
	TItem**	pkHead,		// (I/O)リストの先頭アイテム
	TItem**	pkTail		// (I/O)リストの末尾アイテム
)
{
	if (NULL == pkItem) {
		LOG(ERROR, "Parameter error.");
		return LIST_PARAM_ERROR;
	}

	// 1つもシンボルに追加されていない場合
	if (NULL == *(pkHead)) {
		*(pkHead) = pkItem;
		*(pkTail) = pkItem;
	}
	// シンボルに追加済みの場合
	else {
		(*(pkTail))->pkNext = pkItem;
		pkItem->pkPrev = *(pkTail);
		*(pkTail) = pkItem;
	}

	if (NULL != pkItem->psKey) {
		LOG(DEBUG, "Add list item(%s).", pkItem->psKey);
	}

	return LIST_SUCCESS;
}

//------------------------------------------------------------------------
// 関数名 : リストアイテム検索
// 概要   : リストにアイテムを検索する。
// 引数   : (I)キー
//          (I)リストの先頭アイテム
// 戻り値 : リストアイテム
//          NULL 検索結果なし
// 履歴   : 2017.04.14 森下天明 新規作成。
//------------------------------------------------------------------------
TItem* SearchItem(
	char*	psKey,		// (I)キー
	TItem**	pkHead		// (I)リストの先頭アイテム
)
{
	// リストの先頭アイテム
	TItem* pkTmp = *(pkHead);

	if (NULL == psKey) {
		LOG(ERROR, "Parameter error.");
		return NULL;
	}

	while (pkTmp) {
		// シンボル(変数)名が合致すれば返却し終了
		if (0 == strcmp(pkTmp->psKey, psKey)) {
			// 検索結果あり
			LOG(DEBUG, "Search list item(%s)", pkTmp->psKey);
			return pkTmp;
		}

		// 次のアイテム
		pkTmp = pkTmp->pkNext;
	}

	// 検索結果なし
	return NULL;
}

//------------------------------------------------------------------------
// 関数名 : リストアイテム削除
// 概要   : 指定したキーに合致するリストのアイテムを削除する。
// 引数   : (I)キー
//          (I/O)リストの先頭アイテム
//          (I/O)リストの末尾アイテム
// 戻り値 : LIST_SUCCESS     正常終了 
//          LIST_NOTFOUND    該当のアイテムなし
//          LIST_PARAM_ERROR パラメータエラー
// 履歴   : 2017.04.14 森下天明 新規作成。
//------------------------------------------------------------------------
int DeleteItem(
	char*	psKey,		// (I)キー
	TItem**	pkHead,		// (I/O)リストの先頭アイテム
	TItem**	pkTail		// (I/O)リストの末尾アイテム
)
{
	TItem*	pkDel;	// 削除対象のアイテム

	if (NULL == psKey) {
		LOG(ERROR, "Parameter error.");
		return LIST_PARAM_ERROR;
	}

	// 該当するリストアイテムを検索する
	pkDel = SearchItem(psKey, pkHead);
	// 該当なし
	if (NULL == pkDel) {
		LOG(INFO, "List item not found(%s)", psKey);
		return LIST_NOTFOUND;
	}
	// 該当あり
	else {
		// 削除対象アイテムがリストの場合
		if (pkDel == *(pkHead)) {
			// 削除対象アイテムがリストの先頭
			*(pkHead) = (*(pkHead))->pkNext;

			// リストに1件以上ある場合
			if (NULL != *(pkHead)) {
				// 先頭アイテムの前のポインタをクリア
				(*(pkHead))->pkPrev = NULL;
			}
			// リストが0件の場合
			else {
				// 先頭と末尾のアイテムをクリア
				*(pkHead) = NULL;
				*(pkTail) = NULL;
			}
		}
		// 削除対象アイテムがリストの末尾
		else if (pkDel == *(pkTail)) {
			// 末尾アイテムを前のポインタに設定する
			*(pkTail) = (*(pkTail))->pkPrev;
			// 末尾アイテムの次のポインタをクリア
			(*(pkTail))->pkNext = NULL;
		}
		// 削除対象アイテムがリストの中間
		else {
			// リンクのつなぎ直し
			pkDel->pkPrev->pkNext = pkDel->pkNext;
			pkDel->pkNext->pkPrev = pkDel->pkPrev;
		}

		LOG(DEBUG, "Delete list item(%s)", pkDel->psKey);

		// キー領域を解放する
		free(pkDel->psKey);
		pkDel->psKey = NULL;
		// アイテムを解放する
		free(pkDel);
		pkDel = NULL;

		return LIST_SUCCESS;
	}
}

//------------------------------------------------------------------------
// 関数名 : リストアイテム全削除
// 概要   : リストのアイテムを全て削除する。
// 引数   : (I/O)リストの先頭アイテム
//          (I/O)リストの末尾アイテム
// 戻り値 : なし
// 履歴   : 2017.04.14 森下天明 新規作成。
//------------------------------------------------------------------------
void DeleteAllItem(
	TItem** pkHead,		// (I/O)リストの先頭アイテム
	TItem** pkTail		// (I/O)リストの末尾アイテム
)
{
	// 先頭アイテムのポインタを保持
	TItem*	pkTmp = *(pkHead);
	TItem*	pkNext;		// ポインタ退避用

	// リスト内を検索する
	while (pkTmp) {
		// 次のポインタを退避
		pkNext = pkTmp->pkNext;
		// キー領域を解放する
		free(pkTmp->psKey);
		pkTmp->psKey = NULL;
                
                //バリュー領域を解放する
                free(pkTmp->pValue);
                pkTmp->pValue = NULL;
		// アイテムを解放する
		free(pkTmp);
                
		pkTmp = NULL;
		// 次のポインタ
		pkTmp = pkNext;
	}

	// 先頭と末尾のアイテムをクリア
	*(pkHead) = NULL;
	*(pkTail) = NULL;
}

//------------------------------------------------------------------------
// 関数名 : リストサイズ取得
// 概要   : リストのアイテム数を取得する。
// 引数   : (I)リストの先頭アイテム
// 戻り値 : 0～ リストのアイテム数
// 履歴   : 2017.04.14 森下天明 新規作成。
//------------------------------------------------------------------------
int GetListSize(
	TItem**	pkHead		// (I)リストの先頭アイテム
)
{
	int	iCount = 0;	// リストのアイテム数

	// 先頭アイテムのポインタを保持
	TItem*	pkTmp = *(pkHead);

	// リスト内をカウントする
	while (pkTmp) {
		// アイテム数をカウントアップ
		iCount++;
		// 次のポインタ
		pkTmp = pkTmp->pkNext;
	}

	return iCount;
}

//------------------------------------------------------------------------
// 関数名 : リストアイテム取得
// 概要   : インデックスを指定して、リスト内のアイテムを取得する。
// 引数   : (I)リストのインデックス(0～)
//          (I)リストの先頭アイテム
// 戻り値 : リストアイテム
//          NULL 検索結果なし
// 履歴   : 2017.04.14 森下天明 新規作成。
//------------------------------------------------------------------------
TItem* GetItemByIndex(
	int		iIndex,		// (I)リストのインデックス
	TItem**	pkHead		// (I)リストの先頭アイテム
)
{
	int	iPos = 0;	// リストのインデックス

	// 先頭アイテムのポインタを保持
	TItem*	pkTmp = *(pkHead);

	// パラメータチェック
	if (0 > iIndex) {
		LOG(ERROR, "Parameter error.");
		return NULL;
	}

	// リスト内を検索する
	while (pkTmp) {
		// リストのインデックスが合致すれば返却し終了
		if (iIndex == iPos) {
			return pkTmp;
		}

		// リストのインデックスをカウントアップ
		iPos++;
		// 次のポインタ
		pkTmp = pkTmp->pkNext;
	}

	// 検索結果なし
	return NULL;
}
//--------
// EOF
//--------