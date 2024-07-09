// by iceland

inherit NPC;
inherit F_MASTER;
#include <ansi.h>
#include "cike.h";
int give_lun();
int give_lun2();

string* luns = ({
       BINGQI_D("gold-falun"),
       BINGQI_D("silver-falun"),
       BINGQI_D("bronze-falun"),
       BINGQI_D("iron-falun"),
       BINGQI_D("leaden-falun"),
});


void create()
{
        object weapon;
        set_name("金轮法王", ({ "jinlun fawang", "jinlunfawang","fawang" ,"jinlun"}));
        set("long",@LONG
他身材高瘦，脸颊深陷，双目看似无神，两边太阳穴却高高凸起，面容严肃。
他受封“蒙古国第一国师”，练得一身通天彻地的神功。
LONG
        );
        set("title", HIY "蒙古国第一国师" NOR);
        set("gender", "男性");
        set("class", "huanxi");
        set("age", 65);
        set("attitude", "friendly");
        set("shen_type", -1);
        set("str", 40);
        set("int", 30);
        set("con", 30);
        set("dex", 34);
        set("per", 22);

        set("max_qi", 9500);
        set("max_jing", 6500);
        set("eff_jingli", 7500);
        set("neili", 18000);
        set("max_neili", 18000);
        set("jiali", 100);
        set("combat_exp", 15000000);
        set("score", 28000);
        set("unique", 1);

        set_skill("huanxi-chan", 500);
        set_skill("literate", 520);
        set_skill("force", 500);
        set_skill("longxiang-boruo", 520);
        set_skill("dodge", 500);
        set_skill("yuxue-dunxing", 510);
        set_skill("hammer", 500);
        set_skill("xiangfu-lun", 520);
        set_skill("hand", 500);
        set_skill("dashou-yin",510);
        set_skill("claw", 500);
        set_skill("tianwang-zhua",520);
        set_skill("parry", 500 );

        map_skill("force", "longxiang-boruo");
        map_skill("dodge", "yuxue-dunxing");
        map_skill("hammer", "xiangfu-lun");
        map_skill("parry", "xiangfu-lun");
        map_skill("hand","dashou-yin");
        map_skill("claw","tianwang-zhua");

        prepare_skill("hand","dashou-yin");
        prepare_skill("claw","tianwang-zhua");

        create_family("大轮寺", 10, "弟子");
        set("inquiry", ([
                "武器" : (: give_lun  :),
                "兵器" : (: give_lun  :),
                "风火轮" : (: give_lun2  :),
                
       ]));
        set("lun_count",5);
        set("lun_count2",1);
        set("class", "huanxi");
        set("chat_chance", 2);     
        set("chat_msg", ({
		CYN"金轮法王傲然道：第十层龙象般若功我已融会贯通，古往今来，我是第二人！\n"NOR,
		CYN"金轮法王愤愤的道：中原武人不讲规矩，仗着人多，大伙儿来个一拥而上。\n"NOR,
		CYN"金轮法王愤愤的道: 哼，否则凭我一人之力当可取得武林盟主之位。\n"NOR,
	}) );
        set("chat_chance_combat", 60);
        set("chat_msg_combat", ({
                (: exert_function, "shield jinlun fawang" :),
                (: perform_action, "hammer.wushuai" :), 
	}));

        set_temp("apply/damage", 70);
        set_temp("apply/dodge", 80);
        set_temp("apply/attack", 60);
        set_temp("apply/armor", 60);

        setup();
	if (clonep()){
                weapon = unew(BINGQI_D("gold-falun"));
                if (!weapon) weapon = unew(BINGQI_D("silver-falun"));
                if (!weapon) weapon = unew(BINGQI_D("bronze-falun"));
                if (!weapon) weapon = unew(BINGQI_D("iron-falun"));
                if (!weapon) weapon = unew(BINGQI_D("leaden-falun"));
                weapon->move(this_object());
                weapon->wield();
                carry_object("/d/xueshan/npc/obj/longxiang-jiasha")->wear();       
                add_money("gold",2);
        }
}

void reset()
{
	set("lun_count2", 1);
}

void attempt_apprentice(object ob)
{
       if ((string)ob->query("family/family_name") != "大轮寺") {
                command("say"+ RANK_D->query_respect(ob) +
                        "不是本派弟子，在这里捣什么乱？！");
                return;
        }

        if ((int)ob->query_skill("longxiang-boruo", 1) < 150) {
                command("say 我大轮寺各种武功均和内功相辅相成。");
                command("say"+ RANK_D->query_respect(ob) +
                        "再熬几个通宵，多多钻研钻研本门的内功吧。");
                return;
        }
        if( (string)ob->query("gender") == "无性" ){
                command ("say 施主身体残缺，虽说佛祖普渡众生，可是...");
                return;
        }

        if( (int)ob->query_con(1) < 30) {
               command("sigh");
               command("say"+ RANK_D->query_respect(ob) +"你根骨太差，领悟不了高深功夫。");
               command("shake");
               return;
        }

        if (!(int)ob->query_temp("win_deb")){
                command ("say 你还是先找我弟子达而巴试试你的修为吧。\n");
                return;
	} 
        command("say 我也正在找寻一个资质上佳的弟子授我衣钵。你不顾艰辛诚意拜我为师。");
        if (!ob->query("fawang")){
		command("chat* 朗声说道：好！"+ ob->query("name") + RANK_D->query_respect(ob) + "智勇双全，可传我衣钵。\n");
		command("chat* 仰天长笑道：“本座苦觅数年，终于找到传人，百年之后亦无憾矣！”");
	}
	command("recruit " + ob->query("id"));
	ob->set("fawang", 1);
	ob->set("title", HIY "大轮寺金轮法王座下弟子" NOR);
}

int give_lun()
{ 
	mapping fam; 
	object me, ob,lun;

	me = this_object();
	ob = this_player();
       
	if (ob->query_temp("give_lun") == 1){
		command("say 你不是刚从我这里拿了轮子吗，你当我白痴啊！");
		return 1;
	}
	if (!(fam = this_player()->query("family")) || fam["family_name"] != "大轮寺"){
		command("say 你与本派素无来往，不知此话从何谈起？");
		return 1;
	}
	if ( objectp(present("iron falun", (ob))))  { 
		command("say 你身上不是已经有轮了吗？");
		return 1;  
	} 
	if ( objectp(present("iron falun", environment(me)))){ 
		command("say 地上不是有只轮子吗？你眼睛瞎啦？？");
		return 1;
	} 
      
	if(query("lun_count") < 1){
		command("say 你来晚了，我这里现在没有轮供你使用。");
		return 1;
	}
	add("lun_count",-1);       
	lun = unew( luns[random(sizeof(luns))] );
	if(clonep(lun)){
		lun->move(ob);
		command("say 好吧，这只法轮你拿回去好好钻研。");
		ob->set_temp("give_lun",1);
		message_vision("$N交给$n一只法轮。\n", me, ob);
		return 1;
	}
	command("say 你来晚了，暂时我身边没有多余的法轮可以给你了。");
	return 1;
}

int give_lun2()
{ 
	mapping fam; 
	object me, ob,lun2;

	me = this_object();
	ob = this_player();
       
	if (!(fam = this_player()->query("family")) || fam["family_name"] != "大轮寺"){
		command("say 你与本派素无来往，不知此话从何谈起？");
		return 1;
	}
	if ((int)ob->query_skill("longxiang-boruo", 1) < 200) {
		command("say 我大轮寺各种武功均和内功相辅相成, 你的内力修为不够，我不放心把本派宝物交给你。");
		return 1;
	}
      
	if(query("lun_count2") < 1){
		command("say 你来晚了，我这里现在没有风火轮供你使用。");
		return 1;
	}
	add("lun_count2",-1);       
	lun2 = unew(BINGQI_D("bestfalun"));
	if( clonep(lun2)){
		lun2->move(ob);
		command("say 好吧，这是本门之宝风火轮，你拿回去好好练功，光大我门。");
		message_vision("$N交给$n一只风火轮。\n", me, ob);
		return 1;
	}
	command("say 你来晚了，风火轮已给你的师兄取走了。");
	return 1;
}
