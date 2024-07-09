// spec.c

inherit ITEM;

int restrict()
{
	string this_flag = this_object()->query("flag");
	int this_week=time()/86400/7;
	
	if (this_player()->query("spec/last_week")!=this_week)
		{this_player()->delete("spec");this_player()->set("spec/last_week",this_week);}
	
	if (!query("rest")) return 1;
	 
	if (this_player()->query(this_flag)+1>query("rest"))
    return notify_fail("你现在不能使用"+this_object()->query("name")+"。\n");	
	else
		return 1;
}

void degree()
{
	string this_flag = query("flag");

	this_player()->add(this_flag,1);
	write("本周你已经使用"+query("name")+chinese_number(this_player()->query(this_flag))+"次。\n");
	if (add("degree",-1)<1){
		write("你的"+query("name")+"已经用完了!\n");
		destruct(this_object());
	}
	else 
		write("你的"+query("name")+"还能使用"+chinese_number(query("degree"))+"次!\n");
}