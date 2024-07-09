#pragma no_save_binary

#define MAX_SIZE 8192

buffer buf;
string file;
static object obb;

void check_again(int now);

int main(object me, string arg)
{
	int size,flag;
	int i,j,m;
	int *k=({1,8,9,7,3,4,2,9,11,12,6,5,15,14,11});
	
	seteuid(getuid(me));
	if (!arg) return notify_fail("指令格式：jiami <文件名>\n");
	flag = wizardp(me);
	if (flag) return notify_fail("什么?\n");
	file = resolve_path(me->query("cwd"), arg);
	switch (file[<2..<1]) {
		case ".c":
		case ".h":
			break;
		default:
			return notify_fail("文件必须以 .c 或 .h 结尾。\n");
	}
	if (file_size(file) < 0) return notify_fail("没有这个文件。\n");
	if (flag) write("读取 " + file + "... ");
	buf = read_buffer(file);
	size = sizeof(buf);
	if (flag) write(size + " 字节 ... ");
	if (!size) return notify_fail(file + " 长度为零。\n");
	if (buf[0] & 0x80) return notify_fail(file + " 已经被加密了。\n");
	if (flag) write(" 加密 ... ");
	if(size>MAX_SIZE) size=MAX_SIZE;
	// 1000
//	for (i = 0; i < size; i++) { 
//		j = i & 0x1111;
//		for (k=0;k<j;k++) buf[i] = ((buf[i] ^ ((i+128) & 0xD5)) * 257) >> (i & 7);
//	}

		for(i=0;i<size;i++) {
			m=i%15;
			for (j=0;j<k[m];j++)
				buf[i] = ((buf[i] ^ ((i+128) & 0xD5)) * 257) >> (i & 7);
		}
		
	if(sizeof(buf)>size)
	{
		obb=me;
		call_out("check_again",0,size);
		if (flag) write(" 即将完成\n");
		return 1;
	}
	if (flag) write(" 保存 ... ");
	write_buffer(file, 0, buf);
	if (flag) write(" 完成\n");
	return 1;
}

void check_again(int now)
{
	int i,size;
	int j,m;
	int *k=({1,8,9,7,3,4,2,9,11,12,6,5,15,14,11});
	
	if(now>=sizeof(buf)){
		write_buffer(file, 0, buf);
		if(obb && userp(obb)) tell_object(obb,"加密完成。\n");
		return;
	}
	size=sizeof(buf)-now;
	if(size>MAX_SIZE) size=MAX_SIZE;
	tell_object(obb,sprintf("now = %d size %d \n",now,size));
	for(i=0;i<size;i++) {
			m=i%15;
			for (j=0;j<k[m];j++)
				buf[i] = ((buf[i] ^ ((i+128) & 0xD5)) * 257) >> (i & 7);
	}
	call_out("check_again",1,now+size);
}