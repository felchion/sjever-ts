// tiejiang.c
// for bqp only
// Modify by Looklove@SJ 2001/1/02

#include <ansi.h>
inherit NPC;

#define MAX_DAMAGE 100
#define MIN_DAMAGE 10
#define file_exists(p) (file_size(p)>0)

string color,cname,weapon_type,info;

string *banned_name = ({
"自己","天神","书剑","尸体","某人","我们","你们","他们","大家","他妈的","毛泽东","邓小平","江泽民","巫师",
"操你妈","日你娘","你奶奶的","你妈的","干你妈","干你娘","杀手","雪人","闪电","逍遥","法轮功","反革命",
"共产党","操你妈的","操你姥姥","操你姐姐","操你妹妹","鸡巴","鸡吧",
});

void create_weapon(object client);
void remove_serving(object me);
int do_confirm(string str);
int check_legal_name(string name);

mapping weapon_map = ([
        "sword" :    "剑器",
        "blade" :    "刀",
        "feibiao" :  "飞镖",
        "staff" :    "杖",
        "club" :     "棍",
]);

string ansi_convert(string str)
{
        str = replace_string(str, "$RED$", RED);
        str = replace_string(str, "$GRN$", GRN);
        str = replace_string(str, "$YEL$", YEL);
        str = replace_string(str, "$BLU$", BLU);
        str = replace_string(str, "$MAG$", MAG);
        str = replace_string(str, "$CYN$", CYN);
        str = replace_string(str, "$WHT$", WHT);
        str = replace_string(str, "$HIR$", HIR);
        str = replace_string(str, "$HIG$", HIG);
        str = replace_string(str, "$HIY$", HIY);
        str = replace_string(str, "$HIB$", HIB);
        str = replace_string(str, "$HIM$", HIM);
        str = replace_string(str, "$HIC$", HIC);
        str = replace_string(str, "$HIW$", HIW);
        str = replace_string(str, "$NOR$", NOR);
        return str;
}

int check_legal_name(string name)
{
        int i = strlen(name);
        if (i < 2 || i > 4) {
                write("对不起，您要的兵器名称必须是二到四个中文字。\n");
                return 0;
        }


        if(!is_chinese(name) ) {
          write("对不起，请您用「中文」取名称。\n");
          return 0;
        }

        if( member_array(name, banned_name)!=-1 ) {
                write("对不起，这种名称会造成其他人的困扰。\n");
                return 0;
        }
        return 1;
}


void create()
{
        set_name("蓝铁匠", ({ "lan tiejiang", "lan", "tiejiang" }));
	set("shen_type", 1);

	set("gender", "男性");
	set("age",42);
	set("long", "这是一位饱经风霜的中年大汉,据说他年轻时在西藏贩卖雪莲。\n"+
		    "后来不小心采药时跌断了腿,便流落他乡以打铁为生。\n");
	set_skill("unarmed", 20);
	set_skill("dodge", 20);
	set_temp("apply/damage", 15);

	set("combat_exp", 4000);
	set("attitude", "friendly");
	setup();
	carry_object(ARMOR_D("cloth"))->wear();
}

void init()
{
	object  ob;

        add_action("do_ding", "ding");
	add_action("do_confirm","confirm");
	::init();

	 if( interactive(ob = this_player()) && visible(ob)) 
          say(CYN"蓝铁匠笑咪咪地说道: 这位" + RANK_D->query_respect(ob)+ "可是要打造什么兵器么? \n"NOR);
}

void kill_ob(object ob)
{
        ob->remove_killer(this_object());
        remove_killer(ob);
}

void unconcious()
{
        say("蓝铁匠大叫一声: 妈呀!救命。溜了。\n");
	destruct(this_object());
}

void die()
{
        unconcious();
}

int do_ding(string str)
{
	object me = this_object();
	object customer = this_player();
	string weapon_file;

	// exp < 50k can't order weapon
        if (customer->query("combat_exp") <= 3000)
		return notify_fail("无名小卒,也配打兵器么!\n");

	//  reset 
	customer->delete_temp("bqp/order");

	if (!(str) 
	 || sscanf(str, "%s %s %s", weapon_type,color,cname) != 3)
        	return notify_fail("先看清楚牌子<sign>再说吧.\n");
	
    // check weapon's name is valid ?
        if (!check_legal_name(cname))
        	return notify_fail("请另取名称。\n");

	// check if i am serving for someone else
	if ( me->query_temp("serving") ) {
		command("say 对不起呀,我正忙着呢!\n");
		return 1;
	}

	// check if this kind of weapon is avialable
	weapon_file = __DIR__"/bqp_obj/" + weapon_type + ".c";
	if ( !file_exists(weapon_file) ) 
		return notify_fail("对不起呀,客官,你要的兵器我们没有.\n");	

	// not busy? good, accept new order
	me->set_temp("serving",1);
	me->start_call_out((: call_other,this_object(), "remove_serving",me :), 60 );

	// let player to confirm input
	cname = sprintf("%s%s",color,cname);
	cname = ansi_convert(cname); 

        info = sprintf("\n\t"GRN"兵器: %s\n\t"GRN"名称: %s\n"NOR,weapon_map[weapon_type]+"("+capitalize(weapon_type)+")",cname);
	
	message("vision","看来" + customer->query("name") + "想打造"+cname+ ".\n",environment(me),me);
        command("say 客官要的可是这些? (确定请打<confirm yes|no>) ");
        tell_room(environment(me), info);
	customer->set_temp("bqp/order",1);

	return 1;
}

int do_confirm(string str)
{
        object customer = this_player();
                
        if ( !(str) )
                return notify_fail("请打: confirm yes|no \n");
        
        if ( str == "yes" ) {   
                if (customer->query_temp("bqp/order") ) {
			// may change the weapon's price later
                if (MONEY_D->player_pay(customer,customer->query("combat_exp")/5) == 1 )
	                        create_weapon(customer);
			else {
				remove_call_out("remove_serving");
				this_object()->delete_temp("serving");
				customer->delete_temp("bqp/order");
				return  notify_fail("你身上带的钱不够了!\n");
			}
		}
                return 1;
        }
        else {
                if ( str == "no" ) {
                        this_object()->delete_temp("sreving");
                        tell_object(customer,"客官要点别的什么吗?\n");
			remove_call_out("remove_serving");
			customer->delete_temp("bqp/order");
                }
                else
                        return notify_fail("请用: confirm yes|no \n");
        }
        return 1;
}                             

void create_weapon(object client) 
{
        object me = this_object();
	object weapon;

	int exp,str,damage,weight;
	string weapon_desc;
	
	// randomly select weapon's damage value	
	exp = client->query("combat_exp");
	str = client->query("str");     // should i use query_str() ?
	damage = random(exp * str * MAX_DAMAGE / 30 / 500000 + 1 );
	// str/30 * exp/500k * 50(damage's upperlimit)
 
	if (damage <= MIN_DAMAGE) damage = MIN_DAMAGE;
	if (damage >= MAX_DAMAGE) damage = MAX_DAMAGE; 

	// start to clone new weapon
	weapon = new(__DIR__"bqp_obj/"+ weapon_type);
	weapon->set("name",cname + NOR);
	
	weapon_desc = sprintf("%s%s(%s)的%s\n",weapon->query("long"),client->query("name"),client->query("id"),weapon->query("name")); 
	weapon->set("long",weapon_desc);

	// in order to avoid player selling it to dangpu, set value 0
	weapon->set("value",0);
	weapon->set("no_drop",1);
	weapon->set("no_get",1);

	// randomly select weapon's weight
	// haha. :), my formula is randomly selected too, don't ask me why.
	weight = random(weapon->query_weight() * 2 * str / 30 +1);

	// may change weapon's damage,weight later
	weapon->set("weapon_prop/damage",damage);		
	weapon->set_weight(weight);	

	// move weapon to player
	weapon->move(client);
	command("say 客官,你的兵器做好了!\n");
	message("vision","蓝铁匠交给" + client->query("name") 
	+ "一柄打造好的" + weapon->query("name") +"\n",environment(me),({me,client})); 
	tell_object(client,"蓝铁匠交给你一柄打好的"+weapon->query("name") + "\n");

	// remove some temp flags
	me->delete_temp("serving");
	client->delete_temp("bqp/order");
}

void remove_serving(object me)
{
	me->delete_temp("serving");
}
