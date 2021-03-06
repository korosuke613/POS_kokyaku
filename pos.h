/**
 * pos.h
 **/

#ifndef _INCLUDE_POS_
#define _INCLUDE_POS_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include <libpq-fe.h>   //libpqライブラリを使用するために必要

#define BUFSIZE 1024
#define PORT 10000     //POSサーバのポート番号
#define ENTER "\n" //<LF>
#define DATE_END ".\n"

//*** プロトコルコマンド ***//
#define USERS    "USERS"  //顧客情報総合出力コマンド
#define USRPR    "USRPR"  //顧客情報個人出力コマンド
#define USRUP    "USRUP"  //顧客情報更新コマンド
#define TAXPR    "TAXPR"
#define TAXUP    "TAXUP"
#define TAXIN    "TAXIN"
#define FAIRPR   "FAIRPR"
#define FAIRUP   "FAIRUP"
#define FAIRIN   "FAIRIN"
#define FAIRDEL  "FAIRDEL"
#define FAIRDETPR  "FAIRDETPR"
#define FAIRDETIN "FAIRDETIN"
#define FAIRDETDEL "FAIRDETDEL"

//*** レスポンスステータス ***//
#define OK_STAT    "+OK"     //成功
#define ER_STAT    "-ERR"    //失敗

//*** エラーコード ***//


//*** スレッド関数に渡す引数情報構造体 ***//
typedef struct _ThreadParamter {
    struct in_addr c_ipaddr;  //IPアドレス
    in_port_t      c_port;    //ポート番号
    int            soc;       //コネクション用ソケットディスクリプタ
    PGconn         *con;      //PGconnオブジェクト（データベース）
}ThreadParameter;

extern int receive_message(int __s, char *__buf, int __maxlen);
extern int setup_listen(u_short __port);
extern void *pos_controller(void *arg);

//*** 顧客情報管理 ***//
extern int users(ThreadParameter *threadParam, int start_id, int end_id);
extern int usrpr(ThreadParameter *threadParam, int customer_id);
extern int usrup(ThreadParameter *threadParam, int id, char *name, char *gender, int birth, char     *addres, char *tel, char *mail);

extern int taxprint(ThreadParameter *threadParam);
extern int taxupdate(ThreadParameter *threadParam, int category_small_id ,int country_id, double tax_rate);
extern int taxinsert(ThreadParameter *threadParam, int category_small_id ,int country_id, double tax_rate);

extern int fairprint(ThreadParameter *threadParam);
extern int fairupdate(ThreadParameter *threadParam, int fair_id ,char fair_name[BUFSIZ], char fair_start[BUFSIZ], char fair_end[BUFSIZ]);
extern int fairinsert(ThreadParameter *threadParam, char fair_name[BUFSIZ], char fair_start[BUFSIZ], char fair_end[BUFSIZ]);
extern int fairdelete(ThreadParameter *threadParam, int fair_id);

extern int fair_detailprint(ThreadParameter *threadParam, int fair_id);
extern int fair_detailinsert(ThreadParameter *threadParam, int fair_id, int goods_id, double point_rate);
extern int fair_detaidelete(ThreadParameter *threadParam, int fair_id, int goods_id);

#endif
