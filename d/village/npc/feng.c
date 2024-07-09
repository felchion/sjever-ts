// feng.c 冯铁匠

inherit NPC;
inherit F_DEALER;

string ask_ylj4();
string ask_ylj5();

void create()
{
        set_name("冯铁匠", ({ "feng tiejiang", "feng", "tiejiang" }) );
        set("gender", "男性" );
        set("age", 53);
        set("long", 
"冯铁匠想是常年弯腰打铁，背已驼了，双眼被烟火薰的又红又细，\n"
"左腿残废，肩窝下撑著一根拐杖。\n");
        set("combat_exp", 50000);

        set("str", 24);
        set("dex", 17);
        set("con", 28);
        set("int", 27);
        set("shen_type", 1);

        set("attitude", "friendly");
        set_skill("dodge", 70);
        set_skill("hammer", 40);
        set_skill("parry", 60);
        set_skill("force", 40);
        set_temp("apply/attack", 40);
        set_temp("apply/defense", 40);
        set("vendor_goods", ({
                __DIR__"obj/hammer",
        }));
        set("inquiry", ([
                "name": "小人姓冯，村里人都叫我冯铁匠。",
                "here": "这里是小人糊口的铺子，小人就住在後边的屋子。",
                "锄头": "锄头... 抱歉，锄头已经卖光了...",
                "铁锤": "铁锤？小人做的铁锤坚固又耐用，一把只要三百文钱。",
                "铸剑": (: ask_ylj4 :),
                "师尊": (: ask_ylj5 :),
                "冯默风" : "唉～，这个名字小人已经好久没有听到过了...",
                "奇门遁甲" : "那都是小时候学的，平时没事解解闷罢了。",
        ]) );
        setup();
        add_money("coin", 50);
       	carry_object(ARMOR_D("armor/cloth"))->wear();
}


int accept_object(object me, object ob)
{
        object sword;
        if (ob->query("id") =="mowen xuantie") {
            if(me->query_temp("ylj/step4")) {
            	me->delete_temp("ylj/step4");
            	me->set_temp("ylj/step5", 1 );
                say("冯铁匠说道：啊？！墨纹玄铁，用此铁铸剑需要用到桃花岛铸剑的不传之密，未得师尊许可，在下......\n");
	        return 1;
              	}
	        else say("这东西我要来没用。\n");
        }
        else if (ob->query("id") == "ling pai") {
             if(me->query_temp("ylj/stepend") && environment(this_object())->query("short") == "铁匠铺" ) {
                sword = unew(BINGQI_D("sword/youlong-jian"));
        	if( clonep(sword) ) {
        		sword->move(me);
	        	me->delete_temp("ylj/stepend");
                	say("冯默风大喜道：在下无以为报，只有尽心铸剑以报深恩......\n");
		        return 1;
                	}
              	}
		else say("这东西我要来没用。\n");
        }
return 0;
}

void destroy(object ob)
{
        destruct(ob);
        return;
}

string ask_ylj4()
{
        object me;
        me = this_player();
        if ( !me->query_temp("ylj/step3") )
        	return "剑在后面屋里，自己进去取吧。\n";
        me->delete_temp("ylj/step3");
        command("look "+me->query("id"));
       	me->set_temp("ylj/step4",1);
	return "这位" + RANK_D->query_respect(me) + "，是否带了铸剑的材料？\n";
}

string ask_ylj5()
{
        object me, obj;
        me = this_player();
        if ( !me->query_temp("ylj/step5") )
        	return "在下师尊的名讳，可不是说与你听的。\n";
        me->delete_temp("ylj/step5");
	obj = new(__DIR__"obj/ling");
        if( clonep(obj) ) {
        	obj->move(me);
	       	me->set_temp("ylj/step6",1);
		return "我本是桃花岛弟子，由于师尊黄药师丢失了九阴真经，一\n
怒之下把我们师兄弟的腿脚打断，逐出了师门，好久没见师尊了，不知师尊的\n
身体如何。你有时间的话，能否为我跑一趟桃花岛，我这有一块令牌是当年师\n
尊送给我的，凭此信物拜访我师傅，说默风无时无刻不在想念他老人家......\n";
       	}
       	return "在下师尊的名讳，可不是说与你听的。\n";
}
