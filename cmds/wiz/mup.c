// multi-update

inherit F_CLEAN_UP;

int main(object me, string arg)
{
        int i, n;
        object obj, *inv;
        string err;
        mixed *file;
  
        seteuid( geteuid(me) );

        if( !arg )
                return notify_fail("指令格式：mupdate <路径>\n");

        if( arg[strlen(arg)-1]!='/' ) arg += "/";
        if(  file_size(arg)!=-2 )
                return notify_fail("没有这个目录！\n");

        file = get_dir(arg);
        if( !sizeof(file) )
                return notify_fail(arg + " is empty \n");

        for( i = 0; i < sizeof(file); i++ ) {
                if( !sscanf(file[i], "%*s.c") ) 
                        continue;
                
                file[i] = arg + file[i];
                if( file_size(file[i]) < 0 ) {
                        write("没有" + file[i] + "这个档案。\n");
                        continue;
                }

                me->set("cwf", file[i]);

                if (obj = find_object(file[i])) {
                        if( obj==environment(me) ) {
                                if( file_name(obj)==VOID_OB ) {
                                        write("你不能在 VOID_OB 里重新编译 VOID_OB。\n");
                                        continue;
                                }
                                inv = all_inventory(obj);
                                n = sizeof(inv);
                                while(n--)
                                        if( userp(inv[n]) ) inv[n]->move(VOID_OB, 1);
                                        else inv[n] = 0;
                        }
                        destruct(obj);
                 }
                 if (obj) {
                        write("无法清除旧程式码。\n");  
                        continue;
                        }

				err = "/binaries"+file[i][0..<3]+".b";
				write("删除二进制文件 "+err+" ...");
				if( file_size(err) < 0 ) write("不存在。\n");
				else if (rm(err)) {
					write("成功。\n");
				} else write("失败。\n");

				write("重新编译 " + file[i] + " ...");
				err = catch( call_other(file[i], "???") );
                 if (err)
                        printf( "发生错误：\n%s\n", err );
                 else {
                        write("成功！\n");
                        if( (n = sizeof(inv)) && (obj = find_object(file[i]))) {
                               while(n--)
                                      if( inv[n] && userp(inv[n]) ) inv[n]->move(obj, 1);
                               }
                      }
             }

        return 1;
}
