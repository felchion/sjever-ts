// promote.c
// last modify at 03rd, JUNE 1997 -KEN.Q.Z.ZHANG.
// modify by Looklove because bbb edit err, slogan bbb

#include <ansi.h>

inherit F_CLEAN_UP;

// check the valid levels.
mapping valid_level = ([
	"admin"		:	"(admin)",
	"assist"	:	"(assist)",
	"arch"          :       "(arch)",
	"wizard"        :       "(wizard)",
	"apprentice"    :       "(apprentice)",
	"immortal"      :       "(immortal)",
	"player"        :       "(player)",
]);

int main(object me, string arg)
{
	object ob;
	string old_status, new_status;
	string* rights;
	int i, pass;

	if( me!=this_player(1) ) return 0;

	if( !arg || sscanf(arg, "%s %s", arg, new_status)!=2 )
		return notify_fail("指令格式：promote <使用者> <等级>\n");

	rights = keys(valid_level);

	// checking the valid levels
	for( i=0; i<sizeof(rights); i++) {
		if( new_status == rights[i] )   {
			new_status = valid_level[rights[i]];
			pass = 1; // valid new level setting.
		}
	}

	// invalid new level setting.
	if( !pass ) return notify_fail("没有这种等级。\n");

        if( !objectp(ob = present(arg, environment(me))) || !userp(ob) )
		return notify_fail("你只能改变使用者的权限。\n");

	if( ob == me ) return notify_fail("你不能改变自己的权限等级。\n");

	if( wiz_level(me) != wiz_level("(arch)") && wiz_level(me) != wiz_level("(admin)"))
		return notify_fail("你没有这种权力。\n");

	if( wiz_level(me) <= wiz_level("(arch)") && wiz_level(new_status) >= wiz_level("(arch)") )
		return notify_fail("你没有这种权力。\n");

	old_status = wizhood(ob);

	if( old_status == new_status )
		return notify_fail(ob->name()+"已经是"+new_status+"了。\n");

	seteuid(getuid());
	if( !(SECURITY_D->set_status(ob, new_status)) )
		return notify_fail("修改失败。\n");

	message_vision("$N将$n的权限从 " + old_status + " 改为 " + new_status + " 。\n", me, ob);
	log_file( "static/promotion", me->query("name")+"("+me->query("id")+") promoted " +
		ob->query("name") + "(" + ob->query("id") + ") to " + new_status + " on " 
		+ ctime(time()) + "。\n" );
	seteuid(getuid());
	if (!wiz_level(ob)) {
		ob->delete("combat_exp");
		ob->set("str", 20);
		ob->set("int", 20);
		ob->set("con", 20);
		ob->set("dex", 20);
		ob->set("per", 20);
		ob->set("pur", 20);
		ob->set("kar", 20);
	}
	ob->setup();
	ob->save();

	return 1;
}

int help(object me)
{
	write(@HELP
指令格式： promote 人物<id> 级别<level>

用来提升权限等级， player，immortal，apprentice，wizard，
arch，admin。
一个 admin 能提升权限至任何等级, 而 arch 只能提升至 arch。
HELP );
    return 1;
}
