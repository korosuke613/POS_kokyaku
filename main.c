#include"pos.h"



	char *dbHost = "kite.cs.miyazaki-u.ac.jp";
	char *dbPort = "5432";
	char *dbName = "db39";
	char *dbLogin = "dbuser39";
	char *dbPwd = "dbpass39";
	char connInfo[BUFSIZ];
	PGconn *con;

int main(void){

	sprintf(connInfo, "host=%s port=%s dbname=%s user=%s password=%s",
			dbHost, dbPort, dbName, dbLogin, dbPwd);
	con = PQconnectdb(connInfo);

	if(PQstatus(con) == CONNECTION_BAD){
		printf("no\n");
	}else{
		printf("ok\n");
	}
	
	users(0,100);
	usrpr(8);
	


	return 0;

}