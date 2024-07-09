// database.c
// 
// Created by mxzhao 2004/02/15
//

varargs mixed dbquery(string sql,int flag,object ob,string func,mixed local)
{
	return DB_D->dbquery(sql,flag,ob,func,local);
}

varargs void log_db(object me,object ob,mixed data1,mixed data2,string action){
	LOG_D->log_db(me,ob,data1,data2,action);
}

varargs void log_cr(int from,int amount,int to,int op){
	LOG_D->log_cr(from,amount,to,op);
}

