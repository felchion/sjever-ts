string s_power(int i)
{
	if( i == 20 )	return HIR"完美"NOR;
	if( i >= 15 )	return HIW"极高"NOR;
	if( i >= 10 )	return HIG"很高"NOR;	
	if( i >= 5 )	return HIY"不错"NOR;	
	return "一般";
}

void token_reload()
{
	string str;
	int i;
	if (query("reloaded")) return;
	
	set("reloaded",1);
	
	if(mapp(this_player()->query("token/"+query("id"))))
	{			
		if (i=this_player()->query("token/"+query("id")+"/parry")) 
		{
			set("armor_prop/parry",i);
			str="招架率";
		}	
		else if (i=this_player()->query("token/"+query("id")+"/dodge"))
		{
			set("armor_prop/dodge",i);
			str="躲避率";
		}
		else if (i=this_player()->query("token/"+query("id")+"/ap"))
		{
			set("armor_prop/ap",i);
			i -= 20;
			str="合气加成";
		}
		
		if(!query("armor_prop/ap")) set("armor_prop/ap",20);
		
		set("long",query("long")+" "+this_player()->query("token/"+query("id")+"/desc")+"\n"NOR
			+"\n这个"+query("name")+"看起来有着"+s_power(i)+"的"+str+"。\n");
		wear();
	}
}

int do_discard(string arg)
{
	object me = this_player();
	if (arg!=query("id"))
	return notify_fail("你要丢弃什么?\n");

	write(HIW+query("name")+HIW+"有增加"+chinese_number(query("armor_prop/ap"))+"点合气度的效果。\n");
	write(BLINK+HIR+"你要丢弃"+query("name")+BLINK+HIR+",确定吗？[Y|N]："+NOR);
	input_to("confirm_discard",me);
	return 1;
}

private void confirm_discard(string arg,object me)
{
	if( arg[0]=='y' || arg[0]=='Y' )
	{
		me->delete("token/"+query("id"));
		write("你卸下"+query("name")+"轻轻地丢到没人注意的脚落。\n");
		me->add_temp("apply/eq_armor",-(this_object()->query("armor_prop/armor")));
		me->add_temp("apply/ap",-(this_object()->query("armor_prop/ap")));
		destruct(this_object());
	}
	else
	{
		write("\n你决定保留"+ query("name")+"。\n");
	}
}

int query_autoload()
{
	if(this_object()->query("equipped")) return 1;
	return 0;
}

int unequip()
{
	object ob = environment();

	if (ob && userp(ob) 
	&& base_name(previous_object())=="/cmds/std/remove")
		return notify_fail("如果不想戴了你就把它丢弃(discard)了吧。\n");

	if ( base_name(previous_object())[0..15] == "/clone/spec/item" )
	  return 0;
	   
	//log_file("token_remove",base_name(previous_object()));
	return ::unequip();
}