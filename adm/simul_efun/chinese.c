string chinese_number(int i)
{
	return CHINESE_D->chinese_number(i);
}

string to_chinese(string str)
{
	return CHINESE_D->chinese(str);
}

int is_chinese(string str)
{
	if (!str)
	return 0;

	return pcre_match(str, "^\\p{Han}+$");
}

varargs string c_date(int t,int flag)
{
	string ts = ctime(t);
	string str = ts[8..9];
	if (str == " 1" || str == " 2" || str == " 3" || str == " 4"
	|| str == " 5" || str == " 6" || str == " 7" || str == " 8" || str == " 9")
	str = "0" + ts[8..9];
	
	ts = ts[19..23]+ts[4..6]+str;

	ts = replace_string(ts, "Jan", "01");
	ts = replace_string(ts, "Feb", "02");
	ts = replace_string(ts, "Mar", "03");
	ts = replace_string(ts, "Apr", "04");
	ts = replace_string(ts, "May", "05");
	ts = replace_string(ts, "Jun", "06");
	ts = replace_string(ts, "Jul", "07");
	ts = replace_string(ts, "Aug", "08");
	ts = replace_string(ts, "Sep", "09");
	ts = replace_string(ts, "Oct", "10");
	ts = replace_string(ts, "Nov", "11");
	ts = replace_string(ts, "Dec", "12");
	ts = replace_string(ts, " ", "");
	if(!flag)	return ts;
	else return ts+" "+ctime(t)[11..18];
}