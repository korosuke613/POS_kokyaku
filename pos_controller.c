
#include "pos.h"

/**
 * ATMメイン処理 (スレッド関数)
 *   [ARGUMENT]
 *      __arg  : スレッド引数構造体(ThreadParameter型)
 *  [RETURN]
 *     NONE
 **/
void *pos_controller(void *__arg){
    ThreadParameter *threadParam = (ThreadParameter *)__arg; //スレッド引数構造体
    char recvBuf[BUFSIZE], sendBuf[BUFSIZE]; 
    int  recvLen, sendLen;
    pthread_t selfId;                      //自分自身のスレッドID
    char comm[10][BUFSIZE];
    int perm1Int, perm2Int;
    double perm1Double;
    int cnt;

    selfId = pthread_self(); //自分自身のスレッドIDを取得
    printf("[C_THREAD %u] POS CONTROLLER START (%d)\n", selfId, threadParam->soc);
    while(1){
        /* リクエストコマンド受信 */
        recvLen = receive_message(threadParam->soc, recvBuf, BUFSIZE);
        if( recvLen < 1 )
            break;
        recvBuf[recvLen-1] = '\0'; // <LF>を消去
        printf("[C_THREAD %u] RECV=> %s\n", selfId, recvBuf);
        /* リクエストコマンド解析 */
        cnt = sscanf(recvBuf, "%s", comm[0]);

        /* コマンド判定 */
        if(strcmp( comm[0], USERS ) == 0){
            /* 顧客情報複数出力 */
            if( sscanf(recvBuf, "%s %d %d", comm[0], &perm1Int, &perm2Int) == 3){    
                users(threadParam, perm1Int, perm2Int);
                sprintf(sendBuf, "%s %s%s", OK_STAT, ENTER, DATE_END);
            }else{
                sprintf(sendBuf, "%s %s%s", ER_STAT, ENTER, DATE_END);
            } 

        }else if(strcmp( comm[0], USRPR ) == 0){
            /* 顧客情報個人出力 */
            if( sscanf(recvBuf, "%s %d", comm[0], &perm1Int) == 2){      
                usrpr(threadParam, perm1Int);
                sprintf(sendBuf, "%s %s%s", OK_STAT, ENTER, DATE_END);
            }else{
                sprintf(sendBuf, "%s %s%s", ER_STAT, ENTER, DATE_END);
            }

        }else if(strcmp( comm[0], USRUP ) == 0){
            /* 顧客情報更新 */
            if( sscanf(recvBuf, "%s %d %s %s %d %s %s %s", comm[0], &perm1Int, comm[1], comm[2], &perm2Int, comm[3], comm[4], comm[5]) == 8){      
                usrup(threadParam, perm1Int, comm[1], comm[2], perm2Int, comm[3], comm[4], comm[5]);
                sprintf(sendBuf, "%s %s%s", OK_STAT, ENTER, DATE_END);
            }else{
                sprintf(sendBuf, "%s %s%s", ER_STAT, ENTER, DATE_END);
            }

        }else if(strcmp( comm[0], TAXPR ) == 0){
            /* 税率表示 */
            taxprint(threadParam);
            sprintf(sendBuf, "%s %s%s", OK_STAT, ENTER, DATE_END);  

        }else if(strcmp( comm[0], TAXUP ) == 0){
            /* 税率更新 */
            if( sscanf(recvBuf, "%s %d %d %lf", comm[0], &perm1Int, &perm2Int, &perm1Double) == 4){
                taxupdate(threadParam, perm1Int, perm2Int, perm1Double);
                sprintf(sendBuf, "%s %s%s", OK_STAT, ENTER, DATE_END);
            }else{
                sprintf(sendBuf, "%s %s%s", ER_STAT, ENTER, DATE_END);
            }

        }else if(strcmp( comm[0], TAXIN ) == 0){ 
            /* 税率追加 */
            if( sscanf(recvBuf, "%s %d %d %lf", comm[0], &perm1Int, &perm2Int, &perm1Double) == 4){
                taxinsert(threadParam, perm1Int, perm2Int, perm1Double);
                sprintf(sendBuf, "%s %s%s", OK_STAT, ENTER, DATE_END);
            }else{
                sprintf(sendBuf, "%s %s%s", ER_STAT, ENTER, DATE_END);
            }

        }else if( strcmp( comm[0], FAIRPR ) == 0){
            /* フェア表示 */
            fairprint(threadParam);
            sprintf(sendBuf, "%s %s%s", OK_STAT, ENTER, DATE_END);
        }else if(strcmp( comm[0], FAIRUP ) == 0){
            /* フェア更新 */
            if( sscanf(recvBuf, "%s %d %s %s %s", comm[0], &perm1Int, comm[1], comm[2], comm[3]) == 5){
                fairupdate(threadParam, perm1Int, comm[1], comm[2], comm[3]);
                sprintf(sendBuf, "%s %s%s", OK_STAT, ENTER, DATE_END);
            }else{
                sprintf(sendBuf, "%s %s%s", ER_STAT, ENTER, DATE_END);
            }   
        }else if(strcmp( comm[0], FAIRIN ) == 0){ 
            /* フェア追加 */
            if( sscanf(recvBuf, "%s %s %s %s", comm[0], comm[1], comm[2], comm[3]) == 4){
                fairinsert(threadParam, comm[1], comm[2], comm[3]);
                sprintf(sendBuf, "%s %s%s", OK_STAT, ENTER, DATE_END);
            }else{
                sprintf(sendBuf, "%s %s%s", ER_STAT, ENTER, DATE_END);
            }   


        }else{
            sprintf(sendBuf, "%s %s%s", ER_STAT, ENTER, DATE_END);
        }

        sendLen = strlen(sendBuf);
        send(threadParam->soc, sendBuf, sendLen, 0);
        sendBuf[sendLen-2] = '\0';
        printf("[C_THREAD %u] SEND=> %s\n", selfId, sendBuf);


    }//END while()

    printf("[C_THREAD %u] POS CONTROLLER END (%d)\n\n", selfId, threadParam->soc);

    /* データベース接続を切断 */
    PQfinish(threadParam->con);
    /* ソケットを切断 */
    close(threadParam->soc);
    /* スレッド引数構造の領域解放 */
    free(threadParam);
}//END 

