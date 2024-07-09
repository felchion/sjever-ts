int main(object me, string arg)
{
  int i=1,j;
  string ip,file;
 //	i=270000;
 	while (ip=read_file("/ip.txt",i,2000)){
		j = i/2000;
		file=sprintf("/ipdat%d.txt",j);
		write_file(file,ip);
    i+=2000;
  }
  write(sprintf("共%d个文件生成!\n",j)); 
	return 1;
}