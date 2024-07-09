// npc: /d/city/npc/wei.c
// Jay 5/15/96
// Modify by Linux@sj 增加ask 韦小宝地址

#include <get_place.h>
inherit NPC;
string ask_beauty();
string ask_me();
void create()
{
        set_name("韦春芳", ({ "wei chunfang", "chunfang", "wei" }) );
        set("gender", "女性" );
	set("title", "丽春院老板娘");
        set("age", 42);
        set("long", 
            "韦春芳是当朝鹿鼎公兼红花会总舵主韦小宝他娘，虽是徐"
	    "娘半老，但风韵尤存。\n");
        set("str", 65);
        set("dex", 25);
        set("con", 25);
        set("int", 25);
	set("per", 18);
        set("shen_type", 1);
        set("no_get","韦春芳对你而言太重了。\n");

	set_skill("force", 20);

        set("combat_exp", 200);

        set("max_qi", 300);
        set("max_jing", 100);
        set("neili", 500);
        set("max_neili", 500);

        set("attitude", "friendly");
        set("inquiry", ([
            "name" : "老娘我就是韦春芳。",
            //"韦小宝" : "那是我的乖儿子, 一双眼睛长得像那个喇嘛, 鼻子有点儿像那个回子。",
            "韦小宝" : (: ask_me :),
            "wei xiaobao" : (: ask_me :),
            "丽春院" : "我们丽春院可是扬州城里头一份的找乐子去处。",
            "here" : "我们丽春院可是扬州城里头一份的找乐子去处。",
            "beauty" : (: ask_beauty :),
       	]) );

        setup();
        setup();
        set("chat_chance", 15);
        set("chat_msg", ({
                "韦春芳得意地说道：当年老娘我标致得很，每天有好几个客人。\n",
                "韦春芳怒骂道：辣块妈妈，要是罗刹鬼、红毛鬼子到丽春院来，老娘用大扫帚拍了出去?。\n",
                "韦春芳对你说道：你一双眼睛贼忒嘻嘻的，真像那个喇嘛！\n",
        }) );
        carry_object("/d/city/obj/flower_shoe")->wear();

        carry_object("/d/city/obj/pink_cloth")->wear();
}
void init()
{
        object ob;

        ::init();
        if (base_name(environment()) != query("startroom")) return;
        if (interactive(ob = this_player()) && visible(ob) && !is_fighting()) {
                remove_call_out("greeting");
                call_out("greeting", 1, ob);
        }
}
string ask_me()
{
       object ob, where;
       object me = this_player();
       ob = find_living("wei xiaobao");
       if (!ob) return "这个小兔崽子，老娘我也不知道他去哪里了。\n";
       where = environment(ob);
       if (!where) return "这个小兔崽子好像消失了耶...\n";  
       if (objectp(present("wei xiaobao", environment(me)))){
          	command("disapp "+me->query("id"));
          	return "我儿子就在这里呀...\n";   
       }
       command("look "+me->query("id"));
       return "这位" + RANK_D->query_respect(me) + "，不瞒您说，我那乖儿子正在" + get_place(base_name(where)) + (string)where->query("short") +"卖兵器呢。";
}
void greeting(object me)
{
	if (!me)
		return;
    	command("look " + me->query("id"));

    	if (me->query("gender")=="无性") {
       	command("say 你当老娘是烂婊子吗？辣块妈妈，老娘满汉蒙藏回都接，就是不伺候太监！");
       	command("kick " + me->query("id"));
       	message("vision", me->name() +"被韦春芳一脚踢出门外。\n",
                environment(me), ({me}));
       	me->move("/d/city/nandajie2");
       	message("vision", me->name() +"被人从丽春院里踢了出来，栽倒在地上，磕掉两颗门牙。\n", 
                environment(me), ({me}));

    	}
    	else {
    		if (me->query("class") =="bonze") {
       		command("say 呦，" + RANK_D->query_respect(me) 
			+"也来光顾我们丽春院啊。");
       		command("say 想当年我接过一个西藏喇嘛，他上床前一定要念经，一面念经，眼珠子就骨溜溜地瞧着我。");
		}
    		if (me->query("gender")=="女性") {
       		command("say 哎呀，这年月大姑娘也逛窑子，成何体统。");
       		command("sigh");
       		command("say 可惜我儿子不在，不然让他伺候你。");
    		}
    	command("say 楼上楼下的姑娘们，客人来了！");
    	}
    	return ;
}

string ask_beauty()
{
	object where,obj,*ob_list;
	string msg, name;
	int i, per = 0;

//	ob_list = sort_array(users(), (: env_cmp :));
	ob_list = users();

	for( i = 0; i < sizeof(ob_list); i++) {
		where = environment(ob_list[i]);
		if( ob_list[i]->query("per") > per 
		&& ob_list[i]->query("gender") == "女性"
		&& !ob_list[i]->query("hooker")
		&& where
		&& strsrch(file_name(where), "/d/") >= 0) {
			obj = ob_list[i];
			per = obj->query("per");
		}
	}

	if ( !obj ) {
	msg ="天哪，现在居然一个姑娘家都没有，肯定都被你们这帮臭流氓吓跑了！\n";
	return msg;
	}

	name = obj->query("name")+"("+obj->query("id")+")";
	switch( per/5 )
	{
	case 0:
	case 1:
	case 2:
		msg ="现在书剑里居然数"+name+"这样三分象人，七分象鬼的母夜叉最漂亮，这还有天理吗？\n";
		break;
	case 3:
		msg =name+"的相貌还不到中人之资，怎么就排上她了呢，大概是山中无老虎，猴子称霸王吧！\n";
		break;
	case 4:
		msg =name+"的相貌不差，泥巴里现在就数她了，赶快帮她找个老公嫁了吧！\n";
		break;
	case 5:
		msg =name+"长得那个美啊，如果到我们丽春院来接客，乖乖龙的东韭菜炒大葱，保管客人踏破门坎子！\n";
		break;
	case 6 :
		msg =name+"别不是仙女下凡吧，如果到咱这院子，恐怕连神仙也是这里的常客呢！\n";
		break;
	}

	return msg;
}
