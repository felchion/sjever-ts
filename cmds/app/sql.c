
int main(object me,string sql)
{
	mixed ret;
	if(!sql) return notify_fail("failed\n");
	ret = dbquery(sql);
	if(!ret) return notify_fail("exec failed\n");
	me->start_more(sprintf("返回结果：%O\n",ret));
	return 1;
}