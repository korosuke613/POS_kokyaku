#include "pos.h"

int users(ThreadParameter *threadParam, int start_id, int end_id){
	char sendBuf[BUFSIZE];
	int  sendLen;
	pthread_t selfId;                      //自分自身のスレッドID

	char sql[BUFSIZE];
	PGresult *res;	//PGresultオブジェクト
	int i, resultRows, resultFields;

	selfId = pthread_self(); //自分自身のスレッドIDを取得


	//顧客の最大人数を取得
	//SQLコマンド実行
	sprintf(sql, "SELECT * FROM customer_info ORDER BY customer_id");
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

	//終了顧客数が最大顧客数を越している場合、最大顧客数に合わせる。
	if(end_id > resultRows) end_id = resultRows;
	if(start_id < 1) start_id = 1;

	sprintf(sendBuf, "%d%s", resultRows, ENTER);
	sendLen = strlen(sendBuf);
	send(threadParam->soc, sendBuf, sendLen, 0); 
	printf("[C_THREAD %u] SEND=> %s\n", selfId, sendBuf);

	for(i=start_id-1;i<end_id;i++){
		sprintf(sendBuf, "%s %s %s %s %s %s %s %s\n",
				PQgetvalue(res, i, 0),
				PQgetvalue(res, i, 1),
				PQgetvalue(res, i, 2),
				PQgetvalue(res, i, 3),
				PQgetvalue(res, i, 4),
				PQgetvalue(res, i, 5),
				PQgetvalue(res, i, 6),
				PQgetvalue(res, i, 7),
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

int usrpr(ThreadParameter *threadParam, int customer_id){
	char sendBuf[BUFSIZE];
	int  sendLen;
	pthread_t selfId;                      //自分自身のスレッドID

	int i, resultRows, resultFields;
	char sql[BUFSIZ];
	PGresult *res;  //PGresultオブジェクト

	selfId = pthread_self(); //自分自身のスレッドIDを取得


	//指定の顧客情報を取得
	//SQLコマンド実行
	sprintf(sql, "SELECT * FROM customer_info WHERE customer_id = %d", customer_id);
	res = PQexec(threadParam->con, sql);

	//SQLコマンド実行結果状態を確認
	if(PQresultStatus(res) != PGRES_TUPLES_OK){
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

	sprintf(sendBuf, "%s %s %s %s %s %s %s %s%s",
			PQgetvalue(res, i, 0), 
			PQgetvalue(res, i, 1), 
			PQgetvalue(res, i, 2), 
			PQgetvalue(res, i, 3), 
			PQgetvalue(res, i, 4), 
			PQgetvalue(res, i, 5), 
			PQgetvalue(res, i, 6), 
			PQgetvalue(res, i, 7),
			ENTER
			);


	sendLen = strlen(sendBuf);
	send(threadParam->soc, sendBuf, sendLen, 0);
	printf("[C_THREAD %u] SEND=> %s\n", selfId, sendBuf);


	/* PGresultに割当られた記憶領域を開放 */
	PQclear(res);


	return 0;
}

int usrup(ThreadParameter *threadParam, int id, char *name, char *gender, int birth, char *address, char *tel, char *mail){
  char sendBuf[BUFSIZE];
	int  sendLen;
	pthread_t selfId;                      //自分自身のスレッドID

	char sql[BUFSIZ];
	PGresult *res;

	sprintf(sql, "UPDATE customer_info SET(customer_name,customer_gender,customer_birth,customer_address,customer_phone,customer_email) = ('%s','%s','%d','%s','%s','%s') WHERE customer_id = %d",
			name,
			gender,
			birth,
			address,
			tel,
			mail,
			id
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
