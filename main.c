#include"pos.h"

#if 1
char *dbHost = "kite.cs.miyazaki-u.ac.jp";
char *dbPort = "5432";
char *dbName = "db39";
char *dbLogin = "dbuser39";
char *dbPwd = "dbpass39";
char connInfo[BUFSIZ];
PGconn *con;
#else
char *dbHost = "localhost";
char *dbPort = 5432;
char *dbName = "testdb";
char *dbLogin = "Futa";
char *dbPwd = "bitiken613";
char connInfo[BUFSIZ];
PGconn *con;
#endif


int main(void){

#if 1
    sprintf(connInfo, "host=%s port=%s dbname=%s user=%s password=%s",
            dbHost, dbPort, dbName, dbLogin, dbPwd);
#else
    sprintf(connInfo, "host=%s dbname=%s user=%s password=%s",
        dbHost, dbName, dbLogin, dbPwd);
#endif

		con = PQconnectdb(connInfo);


    if(PQstatus(con) == CONNECTION_BAD){
        printf("no\n");
    }else{
        printf("ok\n");
    }

    users(0,100);
    usrpr(5);
    usrup("7 平木場風太 1 19950613 鹿児島県 0800000000 korosuke@gmail.com");

		/* データベース接続を切断 */
		PQfinish(con);

    return 0;

}
