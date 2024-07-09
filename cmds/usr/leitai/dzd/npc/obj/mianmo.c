
//hehe by ciwei@SJ

inherit ITEM;

int do_zhuang(string arg);

void create()
{
	set_name("面膜",({"mian mo","mian","mo"}));
	set_weight(50);
	if(clonep())
		set_default_object(__FILE__);
	else {
		set("unit", "个");
		set("value",1000);
                set("material", "cloth");                
                set("long",@LONG
面膜？美容用的？哦原来是阿朱曾经用过的面膜，不知道有什么奇怪的用途。
你可以尝试一下伪装(weizhuang)。
LONG
		);
	}
	setup();
}

void init()
{
	add_action("do_zhuang",({"weizhuang","wz",}));
}

void weizhuang_back(object ob)
{
	if(!query_temp("zb_id")) return;
	if(query_temp("zb_master")!=ob) return;
		
	//ob->delete_temp("apply/id");
	//ob->delete_temp("apply/name");
	ob->delete_temp("apply/short");
	ob->delete_temp("apply/long");
	
	delete_temp("zb_id");
	delete_temp("zb_master");	
	
	tell_object(ob,"你恢复了你原来的面目。\n");
}

int do_zhuang(string arg)
{
        object ob = environment();
        object target;
        
        if(!userp(ob)) return 0;
        if(!arg) arg = getuid(ob);
        
        if(query_temp("zb_id")==arg || (!query_temp("zb_id") && arg==getuid(ob)  )  )
        {
        	return notify_fail("你正在伪装成此人。\n");
        }
        
        if(arg==getuid(ob))
        {
        	weizhuang_back(ob);        	
        	return 1;
        }
        if(!objectp(target = LOGIN_D->find_body(arg)))
        {
        	return notify_fail("有这个人么？\n");	
        }        
        //if(wizlevel(target) > wizleve(ob) )
        //	return notify_fail("你反啦你？？\n");
        //
        if(wizardp(target)) return notify_fail("你技术不到家，伪装不成"+target->name()+"。\n");
        //ob->set_temp("apply/name",({target->name(),}));
        //ob->set_temp("apply/id",({target->query("id"),}));
        ob->set_temp("apply/short",({target->short(1),}));        
        ob->set_temp("apply/long",({target->long(),}));
        
        set_temp("zb_id",arg);
        set_temp("zb_master",ob);
        
        tell_object(ob,"你嘿嘿得奸笑了几下。看来你已经成功的把自己假扮成"+target->query("name")+"。\n");
         
        return 1;
}

void remove(string euid)
{
	object ob = environment();
	do
	{
		if(!ob) return;	
		weizhuang_back(ob);
	} while(ob =  environment(ob) );
}

int move(mixed dest, int silently)
{
	int ret;			
	object ob = environment();
	
	ret = ::move(dest,silently);
	
	if( ret && ob && userp(ob) )
	{		
		do
		{
			if(!ob) break;	
			weizhuang_back(ob);
		} while(ob =  environment(ob) );
	}
	
	return ret;
}
