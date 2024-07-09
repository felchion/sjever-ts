// zhang.c 张三丰
// Modified by Lklv for raozhi Quest @@

#include <ansi.h>
inherit NPC;
inherit F_MASTER;

string ask_me_1();
string ask_me_2();
string ask_me_3();
string ask_me_4();
string ask_me_5();
string ask_raozhi();
string ask_no_save();

#define NO_SAVE_ID "/log/skills/no_lifesave"

void create()
{
        object weapon;
        set_name("张三丰", ({ "zhang sanfeng", "zhang", "sanfeng" }));
        set("nickname", "邋遢真人");
        set("long",
                "他就是武当派开山鼻祖、当今武林的泰山北斗张三丰真人。\n"
                "身穿一件污秽的白色道袍，不修边幅。\n"
                "身材高大，年满百岁，满脸红光，须眉皆白。\n");
        set("gender", "男性");
        set("age", 100);
        set("attitude", "friendly");
        set("str", 33);
        set("int", 45);
        set("con", 33);
        set("dex", 33);
        set("per", 28);
        set("unique", 1);

        set("max_qi", 50000);
        set("max_jing", 20500);
        set("eff_jingli", 18000);
        set("max_neili", 23000);
        set("neili", 46000);
        set("jiali", 200);
        set("combat_exp", 15000000);
        set("shen", 64000);
        set("chat_chance_combat", 50);
        set("chat_msg_combat", ({
                (: exert_function, "yinyun" :),
                (: perform_action, "cuff.dongjing" :),
                (: perform_action, "sword.chan" :),
                (: perform_action, "cuff.an" :),
                (: perform_action, "cuff.yinyang" :),
                (: perform_action, "cuff.gangrou" :),
                (: perform_action, "sword.sanhuan" :),
                (: perform_action, "cuff.zhannian" :),
                (: perform_action, "sword.lian" :),
                (: perform_action, "cuff.luanhuan" :),
                (: perform_action, "cuff.ji" :)
        }));
        set_skill("force", 500);
        set_skill("yinyun-ziqi", 530);
        set_skill("dodge", 500);
        set_skill("tiyunzong", 520);
        set_skill("cuff", 500);
        set_skill("taiji-quan", 530);
        set_skill("parry", 500);
        set_skill("sword", 500);
        set_skill("taiji-jian", 520);
        set_skill("taoism", 300);
        set_skill("blade", 500);
        set_skill("literate", 520);
        set_skill("zhenshan-mianzhang", 520);
        set_skill("strike", 500);
        set_skill("wudang-quan", 520);
        set_skill("xuanxu-daofa", 520);
        map_skill("force", "yinyun-ziqi");
        map_skill("dodge", "tiyunzong");
        map_skill("cuff", "taiji-quan");
        map_skill("parry", "taiji-jian");
        map_skill("sword", "taiji-jian");
        map_skill("strike", "zhenshan-mianzhang");
        map_skill("blade", "xuanxu-daofa");
        prepare_skill("cuff", "taiji-quan");
        set("inquiry", ([
                "真武剑" : (: ask_me_1 :),
                "考验" : (: ask_me_2 :),
                "太极拳理" : (: ask_me_3 :),
                "离山" : (: ask_me_4 :),
                "下山" : (: ask_me_4 :),
                "教诲" : (: ask_me_5 :),
//		"敌对" : (: ask_no_save :),
                "绕指柔剑" : (: ask_raozhi :),
        ]));
        set("wan_count", 1+random(2) );
        create_family("武当派", 1, "开山祖师");
        set("class", "taoist");
        set_temp("apply/damage", 70);
        set_temp("apply/dodge", 80);
        set_temp("apply/attack", 100);
        set_temp("apply/armor", 70);
        set_temp("tj/太极", 1);
        set("taiji_quest", 1);
        setup();
        if (clonep()){
                weapon = unew(BINGQI_D("sword/zhenwu-jian"));
                if (!weapon) weapon = new(BINGQI_D("zhujian"));
                weapon->move(this_object());
                weapon->wield();
                carry_object("/d/wudang/obj/white-robe")->wear();
                carry_object("/d/wudang/obj/daolv")->wear();
        }
}

void init()
{
        object ob;

        ::init();
        ob = this_player();
        if (interactive(ob = this_player()) && visible(ob) && !is_fighting()) {
                remove_call_out("greeting");
                call_out("greeting", 1, ob);
        }

}

void greeting(object me)
{
        mapping myfam;
        int shen;

        if (!me || !present(me)) return;

        if ( file_name(environment(this_object())) != this_object()->query("startroom")) return;

        myfam = (mapping)me->query("family");
        shen = me->query("shen");

        if (me->query("taiji_zsf") >= 6 && me->query("taiji_quest"))
                me->delete("taiji_quest");

        if(!myfam) return;
        else if(myfam["family_name"] != "武当派")
                command("say 老道在此清修，是什么风把这位"+RANK_D->query_respect(me)+"吹来了？");
        else if(myfam["family_name"] == "武当派" && me->query_skill("juehu-shou",1)>10
               && random(10)<= 2 && me->query("juehu_nouse")<1){
                command("look " + me->query("id"));
                message_vision("只见张三丰微微将袍袖一抖。\n", me);
                write("你忽然觉得身体微微一热。\n",me);
                me->set("juehu_nouse",1);
                command("say 我武当素有侠名，择徒甚严，我封住你赤阳两穴，那绝户手过于阴毒，以后不要再练了。");
        }
        else if(shen < 0 && shen > -15000)
                command("say  想学好功夫，要先修正做人，你目光微带邪气，还是多去做几件侠义之事吧。");
        else if(shen < -15000){
                command("say 你身为武当弟子，行素不正，今天你就不在是我武当弟子了。");
                command("expell " + me->query("id"));
        }
        else command("nod");
}

void attempt_apprentice(object ob)
{
        mapping fam = ob->query("family");
        if ((!fam || fam["family_name"] != "武当派")
         && ob->query("expell/family_name") != "武当派"){
                command("say "+RANK_D->query_respect(ob)+"与本派素无来往，不知此话从何谈起？");
                return;
        }
        if ((int)ob->query_skill("yinyun-ziqi", 1) < 140) {
                command("say 我武当派乃内家武功，最重视内功心法。");
                command("say " + RANK_D->query_respect(ob) +
                        "是否还应该在氤氲紫气上多下点功夫？");
                return;
        }
        if ((int)ob->query("shen") < 100000) {
                command("say 学武之人，德义为先，功夫的高低倒还在其次，未练武，要先学做人。");
                command("say 在德行方面，" + RANK_D->query_respect(ob) +
                        "是否还做得不够？");
                return;
        }
        if (ob->query_int() < 32) {
                command("say 我武当派武功全从道藏悟出。");
                command("say 要能达到炉火纯青之境，体质什么的倒是无关紧要，悟性却是半点也马虎不得。");
                command("say " + RANK_D->query_respect(ob) + "的悟性还大有潜力可挖，还是请回吧。");
                return;
        }
        command("say 哈哈哈哈！！！！");
        command("say 想不到老道在垂死之年，又觅得"+ob->name(1)+"这样一个可塑之才，真是可喜可贺。");
        command("recruit " + ob->query("id"));
}

void kill_ob(object me)
{
        if(!wizardp(me))
                command("crazy " + me->query("id"));
        if(!this_object()->query_temp("weapon"))
                command("enable parry taiji-quan");
        command("exert taiji");
        ::kill_ob(me);
}

void unconcious()
{
          command("say 贫道今日始信天外有天，人外有人！");
        ::unconcious();
}

string ask_me_1()
{
        object me=this_player();
        if (me->query("family/family_name") != "武当派")
                return RANK_D->query_respect(me)+"与本派素无来往，不知此话从何谈起？";

        command("pat "+ me->query("id"));
           return "此剑乃本门镇山之宝！不经过考验我如何能放心交你保管？\n";
}

string ask_me_2()
{
        object me=this_player();
        if (me->query("family/family_name") != "武当派")
                return RANK_D->query_respect(me)+"与本派素无来往，不知此话从何谈起？";
        command("thumb "+ me->query("id"));
           return "你把郭女侠送我的礼物找来，就算你通过考验！你到我年轻时练功的附近找吧。\n";
}
string ask_me_3()
{
        mapping fam;
        string msg;
        object me=this_player();
        fam = me->query("family");

        if (!fam || fam["family_name"] !="武当派")
                return RANK_D->query_respect(me)+"与本派素无来往，不知此话从何谈起？";
        if (fam["master_name"] !="张三丰"){
                command("pat "+me->query("id"));
                return "太极拳理你现在还无法理解。\n";
        }

        if(me->query("taiji_quest")){
                if(me->query("taiji_quest") <= me->query("taiji_zsf"))
                        return "我不是已经教了你歌诀吗？快去练习吧。\n";
                if(me->query("taiji_quest") == 1) msg = " 顺项贯顶两膀松，束肋下气把裆撑，威音开劲两捶争，五指抓地上弯弓。\n";
                if(me->query("taiji_quest") == 2) msg = " 举步轻灵神内缄，莫教断续一气研，左宜右有虚实处，意上寓下后天还。\n";
                if(me->query("taiji_quest") == 3) msg = " 拿主丹田练内功，哼哈二气妙无穷，动分静合屈伸就，缓应急随理贯通。\n";
                if(me->query("taiji_quest") == 4) msg = " 忽隐忽现进则长，一羽不加至道藏，手慢手快皆非似，四两拨千运化良。\n";
                if(me->query("taiji_quest") == 5) msg = " 极柔即刚极虚灵，运若抽丝处处明，开展紧凑乃缜密，待机而动如猫行。\n";
                if(me->query("taiji_quest") == 6){
                        msg = " 棚履挤按四方正，采列肘靠斜角成，乾坤震兑乃八卦，进退顾盼定五行。\n";
                        me->delete("taiji_quest");
                }

                command("nod "+me->query("id"));
                command("whisper "+me->query("id")+msg);
                tell_object(me,"你听了张三丰的讲解，对太极之道又领悟了不少。\n");
                me->add("max_neili", 10+random(20));
                me->add("max_jingli", 10+random(20));
                me->add("shen", random(3000)+1000);
                me->add("taiji_zsf", 1);
                return "这句歌诀，你要认真记住。\n";
        }
        return "太极之意你现在还无法理解。\n";

}

string ask_me_4()
{
        object me=this_player();

        if (me->query("family/family_name") != "武当派")
                return RANK_D->query_respect(me)+"施主既然要离山，贫道就不远送了。";
        command("pat "+ (string)me->query("id"));
        command("say 江湖险恶，身为正道，你要多为百姓做些行侠仗义的事。");
        me->set_temp("jian_quest",1);
        return "记得为师的教诲，若为非做歹，我定不饶你！。\n";
}

string ask_me_5()
{
        object me=this_player();
        object ob;
        string ob_desc;
        int i,j;

        i = me->query("combat_exp")/ 1000000;
        if (i < 2)  i = 2;
         if (i > 5)  i = 5;
        j = me->query_skill("taiji-jian",1)/ 50;
        if (j < 1)  j = 1;
        if (j > 8)  j = 8;

        if (me->query("family/family_name") != "武当派")
		return RANK_D->query_respect(this_player())+"不是贫道弟子，何谈教诲？";
        if (! me->query_temp("jian_quest") || me->query("combat_exp")<100000)
		return "若为非做歹，我定不饶你！去吧。\n";

        if (me->query_temp("jian_quest2"))
		return "记住为师的话，多行侠仗义，给你的剑一定要收好。\n";

	command("nod "+ (string)me->query("id"));
        command("say 我武当乃侠义正道，当以匡扶正义为己任。");
        command("say 送你一把佩剑，记得为师的教诲，去吧。");
        command("party* "+"wave "+(string)me->query("id"));
        me->delete_temp("jian_quest");
        me->add("shen",100+random(100));
        ob=new(BINGQI_D("sword/sw_jian"));
        ob_desc =
		sprintf("剑上刻着一行小字：武当张三丰赠弟子%s(%s)佩剑,见此物如见本掌门。\n",me->query("name"),me->query("id"));
        ob->set("long",ob_desc);
        ob->set("rigidity", i);
        ob->set("weapon_prop/parry", i);
        ob->set("weapon_prop/sword", j);
        ob->set("owner", me->query("id"));
        ob->move(me);
        me->set_temp("jian_quest2",1);
        message_vision("\n$N给了$n一把松纹剑。\n", this_object(), me);
        return "你好自为之。\n";
}

int accept_object(object who, object ob)
{
        object sword, me;
        mapping fam;
        me = this_object();

        if (!(fam = this_player()->query("family")) || fam["family_name"] !="武当派"){
             command("thank "+ (string)who->query("id"));
             command("say 你与本派素无来往，不知为何送如此厚礼？");
             return 0;
        }

        if (fam["generation"] == 2)
        if ((string)ob->query("id") == "tie luohan"){
            command("goodkid "+(string)who->query("id"));
            call_out("destroying", 1, ob);
            if (present("zhenwu jian", me)){
            	  command("give zhenwu to "+(string)who->query("id"));
                  command("say 你如果用这剑为非做歹，我必取你性命！");
                  return 1;
            }
            else{
                  sword = unew(BINGQI_D("sword/zhenwu-jian"));
                  if (!clonep(sword)) {
                    command("say 啊！真老糊涂了，剑不知放哪了。");
                    return 1;
                  }
                  sword->move(this_player());
                  command("say 你如果用这剑为非做歹，我必取你性命！");
                  return 1;
            }
        }
        command("? "+ (string)who->query("id"));
        command("say 今天不是老道的生日，你送什么礼呀！");
        return 0;
}

void destroying(object obj)
{
    	if (!obj) return;
    	if (userp(obj)) command("drop "+obj->parse_command_id_list()[0]);
    	else destruct(obj);
}

string ask_no_save()
{
        object ob, me = this_player();
        string *no_save, id, tmp;

        me->add_busy(1);
        if (me->query("family/family_name") != "武当派")
                return RANK_D->query_respect(this_player())+"施主这样问是什么意思？";

        command("hmm "+ (string)me->query("id"));

        if( me->query("combat_exp") < 1000000 )
        	return "本门弟子，只要行得正，还怕什么？\n";

        if( me->query("last_kzf") )
        	return "你行为不正，还要来挑拨师门？\n";

        if( !stringp(id = me->query("last_killer")) )
        	return "本门弟子，只要行得正，还怕什么？\n";

        if( sscanf(id, "%s(%s)", tmp, id) != 2 )
        	return "本门弟子，只要行得正，还怕什么？\n";

        id = lower_case(id);

        ob = LOGIN_D->find_body(id);
        if ( !ob || !userp(ob) )
        	return "此人是谁，你要查清楚了再来。\n";

        if( ob->query("family/family_name") == "武当派" )
        	return "同门师兄弟，应该和睦相处，你们这样，老道十分伤心啊！\n";

        if( file_size(NO_SAVE_ID) > 0 )
        	no_save = explode(read_file(NO_SAVE_ID), "\n");
        else no_save = ({});

        if( member_array(id, no_save) != -1 )
        	return "此人老道已经知道了，你以后看见要小心应付！\n";
        no_save += ({ id });
	id = implode(no_save, "\n") + "\n";

        write_file(NO_SAVE_ID, id, 1);

        command("nod");
        return "江湖险恶，要小心应付，一定要记得为师的教诲啊！\n";
}

string ask_raozhi()
{
        object me=this_player();
        if (me->query("family/family_name") != "武当派")
                return RANK_D->query_respect(me)+"与本派素无来往，不知此话从何谈起？";

        if (me->query("raozhi")){
        	command("knock "+me->query("id"));
		return "臭记性！教过你的剑招怎么忘记了？";
	}

        if (!me->query_temp("raozhi"))
		return RANK_D->query_respect(me)+"为何有此一问？";

	if (me->query_skill("yinyun-ziqi", 1) < 200
	 || me->query_skill("taiji-jian", 1) < 200)
		return RANK_D->query_respect(me)+"功夫太低，这些你现在还无法领会。";

	if (random(3)>1 || me->query_temp("raozhi")>2){
		me->set_temp("raozhi",3);
		return "我新创了一招剑法，不过今天不想教给你，你改天再来吧。\n";
	}
	me->set("raozhi",1);
	command("pat "+ me->query("id"));
	command("secret "+ me->query("id"));
	return "那是我新创的一招剑法，还没有在对阵中用过，现在我就教给你吧。\n";
}