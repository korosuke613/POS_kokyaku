#include "pos.h"

extern PGconn *con;

int users(int start_id, int end_id){
    char sql[BUFSIZ];
    PGresult *res;	//PGresultオブジェクト
    int i, resultRows, resultFields;

    //顧客の最大人数を取得
    //SQLコマンド実行
    sprintf(sql, "SELECT * FROM customer_info ORDER BY customer_id");
    res = PQexec(con, sql);

    //SQLコマンド実行結果状態を確認
    if(PQresultStatus(res) != PGRES_TUPLES_OK){
        printf("%s", PQresultErrorMessage(res));
        PQfinish(con);
        exit(1);
    }

    //結果の行数、列数を取得
    resultRows = PQntuples(res);
    resultFields = PQnfields(res);

    //終了顧客数が最大顧客数を越している場合、最大顧客数に合わせる。
    if(end_id > resultRows) end_id = resultRows;

    printf("最大顧客数は%dです。\n", resultRows);
    for(i=start_id;i<end_id;i++){
        printf("%s %s %s %s %s %s %s %s\n",
                PQgetvalue(res, i, 0),
                PQgetvalue(res, i, 1),
                PQgetvalue(res, i, 2),
                PQgetvalue(res, i, 3),
                PQgetvalue(res, i, 4),
                PQgetvalue(res, i, 5),
                PQgetvalue(res, i, 6),
                PQgetvalue(res, i, 7)
              );
    }

		/* PGresultに割当られた記憶領域を開放 */
		PQclear(res);

		printf("終了しました\n");

    return 0;	
}

int usrpr(int customer_id){
    int i, resultRows, resultFields;
    char sql[BUFSIZ];
    PGresult *res;  //PGresultオブジェクト

    //指定の顧客情報を取得
    //SQLコマンド実行
    sprintf(sql, "SELECT * FROM customer_info WHERE customer_id = %d", customer_id);
    res = PQexec(con, sql);

    //SQLコマンド実行結果状態を確認
    if(PQresultStatus(res) != PGRES_TUPLES_OK){
        printf("-ERR XXX\n");   //エラーコード 
        printf("%s", PQresultErrorMessage(res));
        PQfinish(con);
        exit(1);
    }

    resultFields = PQnfields(res);


    for(i=0;i<resultFields;i++){
        printf("%s ", PQgetvalue(res, 0, i) );
    }
    printf("\n");

		/* PGresultに割当られた記憶領域を開放 */
		PQclear(res);


    return 0;
}

int usrup(char* customer_info){
    char sql[BUFSIZ];
    PGresult *res;
		int id, birth;
		char name[BUFSIZ], gender, address[BUFSIZ], mail[BUFSIZ], tel[BUFSIZ];

		sscanf(customer_info, "%d %s %c %d %s %s %s", &id, name, &gender, &birth, address, tel, mail); 

    sprintf(sql, "UPDATE customer_info SET(customer_name,customer_gender,customer_birth,customer_address,customer_phone,customer_email) = ('%s','%c','%d','%s','%s','%s') WHERE customer_id = %d",
            name,
            gender,
            birth,
            address,
            tel,
            mail,
            id
           );
    res = PQexec(con, sql);

    //SQLコマンド実行結果状態を確認
    if(PQresultStatus(res) != PGRES_COMMAND_OK){
        printf("-ERR XXX\n");   //エラーコード
        printf("%s", PQresultErrorMessage(res));
        PQfinish(con);
        exit(1);
    }

    printf("+OK\n"); //成功コード

		/* PGresultに割当られた記憶領域を開放 */
		PQclear(res);


    return 0;
}
