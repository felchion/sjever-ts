// zhangpu.c 铁掌掌谱
// by bren

inherit ITEM;
void setup()
{}

void init()
{
        add_action("do_du", "du");
        add_action("do_du", "study");      
}

void create()
{
        set_name("铁掌掌谱", ({ "tiezhang zhangpu", "zhangpu", "pu","book" }));
        set_weight(600);
        if( clonep() )
                set_default_object(__FILE__);
        else {
                set("unit", "本");
                set("long",
			"铁掌掌谱\n"
			"这是一本落满灰尘的破书，纸质发黄，边缘处也已残缺不全了。\n");
                set("value", 50000);
                set("material", "paper");
                set("treasure", 1);
                set("unique", 1);
                set("no_drop", "这样东西不能离开你。\n");
                set("no_get", "这样东西不能离开那儿。\n");

        }
}

int do_du(string arg)
{
        object me = this_player();
        object where = environment(me);
//        object ob;
//        mapping skill;
        int level; 
	int neili_lost;

	if (!id(arg))
	return 0;

        if (where->query("pigging")){
                write("你还是专心拱猪吧！\n");
                return 1;
        }
        if (me->is_busy()) {
                write("你现在正忙着呢。\n");
                return 1;
        }

        if( me->is_fighting() ) {
                write("你无法在战斗中专心下来研读新知！\n");
                return 1;
        }


        if (!id(arg)) {	
                write("你要读什么？\n");
                return 1;
        }
       
        if( !me->query_skill("literate", 1) ){
                write("你是个文盲，先学点文化(literate)吧。\n");
                return 1;
        }

        switch(random(3)) {
          case 0:
        message("vision", me->name() + "翻出一本书在读。\n", environment(me), me);
              break;
          case 1:
        message("vision", me->name() + "一边读书，浑身骨骼一边发出咯咯的脆响。\n", environment(me), me);
              break;
          case 2:
        message("vision", me->name() + "的脸色时红时青，阴晴不定。\n", environment(me), me);
              break;
        }
        if( (int)me->query("jing") < 15 ) {
                write("你现在过于疲倦，无法专心下来研读新知。\n");
                return 1;
        }
	if (me->query("family/family_name") != "铁掌帮") {
		me->delete_skill("tiezhang-zhangfa");
		write("你研读了一会，发现上面所说的对你而言太深了。\n");
		return 1;
	}

/*        if ( me->query_skill("tiezhang-zhangfa", 1) > ) neili_lost = 15;
	else
	if ( me->query_skill("tiezhang-zhangfa", 1) > 10) neili_lost = 10;
	else neili_lost = 5;
*/

        if( (int)me->query("neili") < 50) {
		write("你内力不够，无法钻研这么高深的武功。\n");
                return 1;
        }
        if( (int)me->query("max_neili") <500){
                write("你的内力不足，无法钻研这么高深的武功。\n");
                return 1;
        }
        
        level = me->query_skill("tiezhang-zhangfa", 1);
        if( (int)me->query("combat_exp") < (int)level*level*level/10 ) {
                write("你的实战经验不足，再怎么读也没用。\n");
                return 1;
        }

       
        if( me->query_skill("tiezhang-zhangfa", 1) > 30){
                write("你研读了一会儿，但是发现上面所说的对你而言都太浅了，没有学到任何东西。\n");
                return 1;
        }

        me->receive_damage("jing", 15);
	me->set("neili",(int)me->query("neili")-neili_lost);
        if( !me->query_skill("tiezhang-zhangfa", 1) ) {
                me->set_skill("tiezhang-zhangfa", 0);
/*
write("                   《葵花宝典》阅读指南\n"
"\n    恭喜你开始研习武林第一功夫－－－－辟邪剑法！\n"
"注意事项：\n"
"一．研习辟邪剑法必先自宫，如你不想让别人发现这个天大的秘密，你必须\n"
"    找一无人所在之处阅读，且当有别的玩家在场时不要使用辟邪剑法战斗，\n"
"    因为当你阅读或使用辟邪剑法时会出现一些阴柔的举动，使别人产生怀\n"
"    疑。切记不可收徒或拜别的玩家为师，以免被人检查你的技能。\n\n"
"二．辟邪剑法功力极强，既可作剑法，又可作腾挪之法。\n\n"
"三．《葵花宝典》不可送人，出售，丢弃，捡拾，唯一的获取方法是你使用\n"
"    你得到的密码去密室寻找。\n\n"
"四．辟邪剑法不可通过拜师或自己锻炼来长进，只能通过研习《葵花宝典》\n"
"    来提高。\n\n"
"         祝你早日成为武林第一高手，称霸天下！\n");
*/
        }
        me->improve_skill("tiezhang-zhangfa", (int)me->query_skill("literate", 1)/3+1);
        write("你研读《铁掌掌谱》，颇有心得。\n");
        return 1;
}