#include "pos.h"

int fairprint(ThreadParameter *threadParam){
    char sendBuf[BUFSIZE];
    int  sendLen;
    pthread_t selfId;                      //自分自身のスレッドID

    char sql[BUFSIZE];
    PGresult *res;	//PGresultオブジェクト
    int i, resultRows, resultFields;

    selfId = pthread_self(); //自分自身のスレッドIDを取得


    //顧客の最大人数を取得
    //SQLコマンド実行
    sprintf(sql, "SELECT * FROM fair_info ORDER BY fair_id");
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

    sprintf(sendBuf, "フェアID|フェア名|開始日|終了日%s", ENTER);
     sendLen = strlen(sendBuf); 
    send(threadParam->soc, sendBuf, sendLen, 0);
    sendBuf[sendLen-1] = '\0'; 
    printf("[C_THREAD %u] SEND=> %s\n", selfId, sendBuf); 
    

    for(i=0;i<resultRows;i++){
        sprintf(sendBuf, "%s %s %s %s%s",
                PQgetvalue(res, i, 0),
                PQgetvalue(res, i, 1),
                PQgetvalue(res, i, 2),
                PQgetvalue(res, i, 3), 
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


int fairupdate(ThreadParameter *threadParam, int fair_id ,char fair_name[BUFSIZ], char fair_start[BUFSIZ], char fair_end[BUFSIZ]){
    char sendBuf[BUFSIZE];
    int  sendLen;
    pthread_t selfId;                      //自分自身のスレッドID

    char sql[BUFSIZ];
    PGresult *res;

    sprintf(sql, "UPDATE fair_info SET(fair_name, fair_start, fair_end) = ('%s', '%s', '%s') WHERE fair_id = %d",
            fair_name,
            fair_start,
            fair_end,
            fair_id
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

int fairinsert(ThreadParameter *threadParam, char fair_name[BUFSIZ], char fair_start[BUFSIZ], char fair_end[BUFSIZ]){
    char sendBuf[BUFSIZE];
    int  sendLen;
    pthread_t selfId;                      //自分自身のスレッドID

    char sql[BUFSIZ];
    int fair_id;
    PGresult *res;

    sprintf(sql, "SELECT count(*) FROM fair_info");
    res = PQexec(threadParam->con, sql);

     //SQLコマンド実行結果状態を確認
    if(PQresultStatus(res) != PGRES_TUPLES_OK){
        printf("%s", PQresultErrorMessage(res));
        PQclear(res);
        return 1;
    }

    fair_id = atoi(PQgetvalue(res, 0, 0)) + 1;


    sprintf(sql, "INSERT INTO fair_info VALUES(%d, '%s', '%s', '%s')",
            fair_id,
            fair_name,
            fair_start,
            fair_end
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
