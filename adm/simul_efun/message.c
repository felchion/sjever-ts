// message.c

varargs object snoop( object snooper, object snoopee )
{
	if (wiz_level(snooper) < wiz_level(snoopee))
		return 0;
	if (objectp(snoopee))
		return efun::snoop(snooper, snoopee);
	return efun::snoop(snooper);
}

int living(object me)
{
	if (me->query("disable_type")) return 0;
	return efun::living(me);
}

void notify_fail(string msg)
{
	return efun::notify_fail("[0;33m"+replace_string(msg,"[0;0m","[0;33m")+"[0;0m"); //定义出错提示为黄色
} 

varargs void message_vision(string msg, object me, object you,int flag)
{
	string my_gender, your_gender, my_name, your_name;
	string str1, str2, str3;
	object *receiver;

	my_name= me->name();
	my_gender = me->query("gender");
	if(msg[0]=='' && msg[6]=='m' ) my_name = my_name + msg[0..6];
	str1 = replace_string(msg,  "$P", gender_self(my_gender)?gender_self(my_gender):"[1;31m"+"替换错误");
	str1 = replace_string(str1, "$N", gender_self(my_gender)?gender_self(my_gender):"[1;31m"+"替换错误");
	str3 = replace_string(msg,  "$P", gender_pronoun(my_gender)?gender_pronoun(my_gender):"[1;31m"+"替换错误");
	str3 = replace_string(str3, "$N", my_name?my_name:"[1;31m"+"替换错误");
	if( you ) {
		your_name= you->name();
		your_gender= you->query("gender");
		if(msg[0]=='' && msg[6]=='m' ) your_name = your_name + msg[0..6];
		str2 = replace_string(msg,  "$P", gender_pronoun(my_gender)?gender_pronoun(my_gender):"[1;31m"+"替换错误");
		str2 = replace_string(str2, "$N", my_name?my_name:"[1;31m"+"替换错误");
		str2 = replace_string(str2, "$p", gender_self(your_gender)?gender_self(your_gender):"[1;31m"+"替换错误");
		str2 = replace_string(str2, "$n", gender_self(your_gender)?gender_self(your_gender):"[1;31m"+"替换错误");
		message("vision", str2, you);
		str1 = replace_string(str1, "$p", gender_pronoun(your_gender)?gender_pronoun(your_gender):"[1;31m"+"替换错误");
		str1 = replace_string(str1, "$n", your_name?your_name:"[1;31m"+"替换错误");
		str3 = replace_string(str3, "$p", gender_pronoun(your_gender)?gender_pronoun(your_gender):"[1;31m"+"替换错误");
		str3 = replace_string(str3, "$n", your_name?your_name:"[1;31m"+"替换错误");
	  if (!flag)
	    receiver = filter_array(all_inventory(environment(me)),(: userp($1) && ($1->query("env/env_msg") || member_array($(me),($1->query_enemy()))!=-1 || member_array($(you),($1->query_enemy()))!=-1) :));
	  else 
	    receiver = all_inventory(environment(me));
	}
	else if (!flag)
	  receiver = filter_array(all_inventory(environment(me)),(: userp($1) && ($1->query("env/env_msg") || member_array($(me),($1->query_enemy()))!=-1) :));
	else
	  receiver = all_inventory(environment(me));
	message("vision", str1, me);	 
	if(!me->query("IN_CP"));
	message("vision", str3,  receiver, ({ me, you}) );
}

varargs void tell_object(object ob, string str,int flag)
{
	if(flag) call_out((:message:),0,"tell_object", str, ob);
	else message("tell_object", str, ob);
}

 void tell_room(mixed ob, string str, object *exclude,int flag)
{
	object *receiver;
	if( ob ) {
		if( objectp(ob) && !flag){
		  receiver = 	filter_array(all_inventory(ob),(: $1->query("env/env_msg") :));
		  message("tell_room", str, receiver, exclude);
	  }
	  else
	    message("tell_room", str, ob, exclude);  
  }
}

void shout(string str)
{
	message("shout", str, users(), this_player());
}

varargs void write(string str, int flag)
{
	if(flag){
		if( this_player() )
		call_out((:message:),0,"write", str, this_player());
		else if( previous_object() )
		call_out((:message:),0,"write", str, previous_object());
		else
		efun::write(str);
	}
	else {
		if( this_player() )
		message("write", str, this_player());
		else if( previous_object() )
		message("write", str, previous_object());
		else
		efun::write(str);
	}
}

varargs void say(string str, mixed exclude)
{
	object ob = previous_object();

	if (ob->is_character() && !living(ob) && base_name(ob) != CORPSE_OB)
		return;
	message("say", str, environment(ob), ob);
}

void err_msg(string str){
	CHANNEL_D->do_channel(find_object(MASTER_OB),"err",str);
}