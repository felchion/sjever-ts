private string ipformat(string str)
{
	string ip1,*ip2;
	int i,j;
	
	ip2 = explode(str, ".");
	write(sizeof(ip2));
	if (!sizeof(ip2)==4) 
		return "err";
	for(i=0;i<4;i++){
		j = atoi(ip2[i]);
		if (j<0 || j>255)
			return "err";
		if (sizeof(ip2[i])==1) ip2[i]="0"+ip2[i];
		if (sizeof(ip2[i])==2) ip2[i]="0"+ip2[i];
	}	
	ip1 = implode(ip2,".");
	return ip1;
}

int main(object me, string arg)
{
  int i=1;
  string ip,*ip2;
 	if (!arg)
 		return notify_fail("指令格式：ipdata <路径>\n");
 		
 	while (ip=read_file(arg,i,1)){
    ip = replace_string(ip,"\n","");
    ip2 = explode(ip,"|");
    
//    write(sprintf("%s  :%d \n",ip,sizeof(ip2))); 
    if (sizeof(ip2)>2) {
   		if (sizeof(ip2)==5) ip2[3] = ip2[3]+" "+ip2[4];
 	 	  if (sizeof(ip2)>3)  ip2[2] = ip2[2]+ip2[3];
//			ip ="'"+ip2[0]+"','"+ip2[1]+"','"+ip2[2]+"'\n";
//			write_file("/ipdata",ip);
			dbquery("REPLACE ipdata SET "
				"startip='" + ipformat(ip2[0]) + "',"
				"endip='" + ipformat(ip2[1]) + "',"
				"ipname='" + ip2[2]+"'");
    }
    i++;
  }
  write(sprintf("共%d条IP信息被导入!\n",i)); 
	return 1;
}