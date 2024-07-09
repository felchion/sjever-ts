// atoi.c

int atoi(string str)
{
	int v;

	if (!stringp(str) || !sscanf(str, "%d", v))
		return 0;
	return v;
}

string strip(string str)
{
	string prefix, suffix;

	if (!stringp(str))
		return str;
	while (sscanf(str, "%s[%*sm%s", prefix, suffix) == 3)
		str = prefix + suffix;
	return str;
}
//这个函数替换到SQL语句的特殊字符，避免WEB出错。
string specialchars(string arg)
{
  arg = replace_string(arg,"'", "’");
  return arg;
}

varargs string ansi_color(int d,int b) { 
  if(!d&&!b) return "[0;0m";
  if(!b) return "[38;5;"+d+"m";
  if(!d) return "[48;5;"+b+"m";
  return "[38;5;"+d+";48;5;"+b+"m";
}

varargs string format_long(string _str,int flag) {
  buffer str;  
  str = string_encode(_str,"GBK");  
  if(!flag) {
    if(sizeof(str)>64)
    return(string_decode(str[0..63],"GBK")+"\n"+format_long(string_decode(str[64..<1],"GBK"),1));
    else return string_decode(str,"GBK")+"\n";
  }
  else {
    if(sizeof(str)>67)
    return(string_decode(str[0..67],"GBK")+"\n"+format_long(string_decode(str[68..<1],"GBK"),1));
    else return string_decode(str,"GBK")+"\n";
  }
}

varargs string substr(string str,int i,int j) {
	buffer buf;
	buf = string_encode(str,"GBK");
	if(i<0 && j<0) return string_decode(buf[<(-i)..<(-j)],"GBK");
	else if(j<0) return string_decode(buf[i..<(-j)],"GBK");
	else if(i<0) return string_decode(buf[<(-i)..<j],"GBK");
	else return string_decode(buf[i..j],"GBK");
}