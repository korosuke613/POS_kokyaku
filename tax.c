#include "pos.h"

int taxprint(ThreadParameter *threadParam){
    char sendBuf[BUFSIZE];
    int  sendLen;
    pthread_t selfId;                      //自分自身のスレッドID

    char sql[BUFSIZE];
    PGresult *res;	//PGresultオブジェクト
    int i, resultRows, resultFields;

    selfId = pthread_self(); //自分自身のスレッドIDを取得


    //顧客の最大人数を取得
    //SQLコマンド実行
    sprintf(sql, "SELECT * FROM tax_rate ORDER BY country_id, category_id");
    res = PQexec(threadParam->con, sql);

    //SQLコマンド実行結果状態を確認
    if(PQresultStatus(res) != PGRES_TUPLES_OK){
        printf("%s", PQresultErrorMessage(res));
        PQfinish(threadParam->con);
        return 1;
    }

    //結果の行数、列数を取得
    resultRows = PQntuples(res);
    resultFields = PQnfields(res);

    sprintf(sendBuf, "商品カテゴリ|国ID|税率%s", ENTER);
     sendLen = strlen(sendBuf); 
    send(threadParam->soc, sendBuf, sendLen, 0);
    sendBuf[sendLen-1] = '\0'; 
    printf("[C_THREAD %u] SEND=> %s\n", selfId, sendBuf); 
    

    for(i=0;i<resultRows;i++){
        sprintf(sendBuf, "%s %s %s%s",
                PQgetvalue(res, i, 0),
                PQgetvalue(res, i, 1),
                PQgetvalue(res, i, 2),
                ENTER
               );
        sendLen = strlen(sendBuf);
        send(threadParam->soc, sendBuf, sendLen, 0); 
        sendBuf[sendLen-1] = '\0';
        printf("[C_THREAD %u] SEND=> %s\n", selfId, sendBuf);
    }

    /* PGresultに割当られた記憶領域を開放 */
    PQclear(res);

    return 0;	
}


int taxupdate(ThreadParameter *threadParam, int category_small_id ,int country_id, double tax_rate){
    char sendBuf[BUFSIZE];
    int  sendLen;
    pthread_t selfId;                      //自分自身のスレッドID

    char sql[BUFSIZ];
    PGresult *res;

    sprintf(sql, "UPDATE tax_rate SET(tax_rate) = (%lf) WHERE country_id = %d AND category_id = %d",
            tax_rate,
            country_id,
            category_small_id
           );
    res = PQexec(threadParam->con, sql);

    //SQLコマンド実行結果状態を確認
    if(PQresultStatus(res) != PGRES_COMMAND_OK){
        sprintf(sendBuf, "-ERR XXX%s", ENTER);   //エラーコード 
        sendLen = strlen(sendBuf);
        send(threadParam->soc, sendBuf, sendLen, 0);
        printf("[C_THREAD %u] SEND=> %s\n", selfId, sendBuf);

        sprintf(sendBuf, "%s%s", PQresultErrorMessage(res), ENTER);
        sendLen = strlen(sendBuf);
        send(threadParam->soc, sendBuf, sendLen, 0);
        printf("[C_THREAD %u] SEND=> %s\n", selfId, sendBuf);

        PQclear(res);
        return 1;
    }


    printf("+OK\n"); //成功コード

    /* PGresultに割当られた記憶領域を開放 */
    PQclear(res);


    return 0;
}

int taxinsert(ThreadParameter *threadParam, int category_small_id ,int country_id, double tax_rate){
    char sendBuf[BUFSIZE];
    int  sendLen;
    pthread_t selfId;                      //自分自身のスレッドID

    char sql[BUFSIZ];

    PGresult *res;
    sprintf(sql, "INSERT INTO tax_rate VALUES(%d,%d,%lf)",
            category_small_id,
            country_id,
            tax_rate
           );
    res = PQexec(threadParam->con, sql);

    //SQLコマンド実行結果状態を確認
    if(PQresultStatus(res) != PGRES_COMMAND_OK){
        sprintf(sendBuf, "-ERR XXX%s", ENTER);   //エラーコード 
        sendLen = strlen(sendBuf);
        send(threadParam->soc, sendBuf, sendLen, 0);
        printf("[C_THREAD %u] SEND=> %s\n", selfId, sendBuf);

        sprintf(sendBuf, "%s%s", PQresultErrorMessage(res), ENTER);
        sendLen = strlen(sendBuf);
        send(threadParam->soc, sendBuf, sendLen, 0);
        printf("[C_THREAD %u] SEND=> %s\n", selfId, sendBuf);

        PQclear(res);
        return 1;
    }


    printf("+OK\n"); //成功コード

    /* PGresultに割当られた記憶領域を開放 */
    PQclear(res);


    return 0;
}        
