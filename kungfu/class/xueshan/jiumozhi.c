// jiumozhi.c 鸠摩智
// by iceland
// Modified by Lklv 2001.10.23
// Modified by tangfeng 2004

inherit F_MASTER;
#include <ansi.h>
inherit NPC;

#define QUESTDIR2 "quest/天龙八部/天龙营救篇/"

string askduanyu();
//int start_condition(object ob);
int power_condition(object me,object ob,int p);
void create()
{
        set_name("鸠摩智", ({ "jiumo zhi", "jiumozhi","zhi" }));
        set("long",@LONG
他身材匀称，脸上神采飞扬，隐隐似有宝光流动，便如是明珠宝玉，自然生辉，
只消向他看得几眼，便生钦仰亲近之意。
LONG
        );
        set("title", HIY "大轮寺掌门" NOR);
        set("gender", "男性");
        set("class", "huanxi");
        set("age", 55);
        set("attitude", "friendly");
        set("shen_type", -1);
        set("str", 35);
        set("int", 40);
        set("con", 30);
        set("dex", 30);
        set("per", 25);
        set("max_qi", 6500);
        set("eff_jingli", 3500);
        set("max_jing", 6000);
        set("neili", 14000);
        set("max_neili", 14000);
        set("jiali", 80);
        set("combat_exp", 2900000);
        set("score", 290000);
        set("unique", 1);

        set_skill("huanxi-chan", 300);
        set_skill("literate", 450);
        set_skill("force", 450);
        set_skill("longxiang-boruo", 450);
        set_skill("dodge", 490);
				set_skill("poison", 200);
        set_skill("yuxue-dunxing", 490);
        set_skill("strike", 490);
        set_skill("huoyan-dao", 500);
        set_skill("parry", 490 );
        set_skill("staff",490);
        set_skill("wushang-dali",490);

        map_skill("force", "longxiang-boruo");
        map_skill("dodge", "yuxue-dunxing");
        map_skill("strike", "huoyan-dao");
        map_skill("staff","wushang-dali");
        map_skill("parry", "huoyan-dao");
        prepare_skill("strike","huoyan-dao");
        set("chat_chance_combat", 30);
        set("chat_msg_combat", ({
                (: exert_function, "shield jiumo zhi" :),
                (: perform_action, "strike.fen" :), }));

        create_family("大轮寺", 10, "弟子");
        set("class", "huanxi");
        set("chat_chance", 2);
        set("chat_msg", ({
		"鸠摩智低头沉思，说道：嵩山少林寺，大理天龙寺，哼！我雪山大轮寺哪点比不上？！\n",
		"鸠摩智将双掌举到眼前瞧了瞧，冷笑了一声：天下能有几人是我的敌手？！\n",
	}) );
		set("inquiry",([
		"段誉" : (: askduanyu :),
		"大轮寺" : "这里便是大轮寺。",
		"天龙寺" : "天龙寺是大理段家武学的最高之地，传说的六脉神剑就在其中。",
		"降龙十八掌" : "丐帮绝学，可惜未能得见。",
		"萧峰" : "江湖人称南慕容、北萧峰，端得一条好汉。",
		"营救段誉" : (: askduanyu :),
		"慕容复" : "江湖人称南慕容、北萧峰，听说慕容公子年级轻轻，却无所不精。",
		"慕容博" : "真乃姑苏慕容不出的杰才，可惜英年早逝。",
		"六脉神剑" : "传说的我天龙寺最高绝学，我一定要想办法此生得见。",
	]));
	      set_temp("apply/armor", 60);
        set_temp("apply/damage", 50);
        set_temp("apply/attack", 60);
        set_temp("apply/dodge", 50);
        setup();
        carry_object("/d/xueshan/npc/obj/longxiang-jiasha")->wear();

        add_money("gold",1);
}
string askduanyu()
{
	object me;
	me = this_player();
  if(!me->query(QUESTDIR2+"start" ))
  {
  	  command("?");
  		return "小僧不晓得段誉，不知道哪方高手。";
  }
	if (me->query(QUESTDIR2+"start") && !me->query_temp(QUESTDIR2+"jiumozhi")&& !me->query(QUESTDIR2+"over"))
	{
  	  command("shrug "+me->query("id"));
  		return "小僧不晓得段誉，不知道哪方高手。";
  }
	if (me->query(QUESTDIR2+"start") && me->query_temp(QUESTDIR2+"jiumozhi")&& !me->query_temp(QUESTDIR2+"kill_dizi")&& !me->query(QUESTDIR2+"over"))
	{
  	  command("flop "+me->query("id"));
  		return "我知道你知道剧情，但是好像还不应该临到我吧？去你的天龙寺看门去。";
  }
	if (me->query(QUESTDIR2+"start") && me->query_temp(QUESTDIR2+"kill_dizi")&& !me->query_temp(QUESTDIR2+"caught")&& !me->query(QUESTDIR2+"over"))
	{
  	  command("flop");
  	  command("kick "+me->query("id"));
  		return "我知道你知道剧情，但是好像还不应该临到我吧？先去本因那里。";
  }
	if (me->query(QUESTDIR2+"start") && me->query_temp(QUESTDIR2+"caught") && !me->query_temp(QUESTDIR2+"askzhi")&& !me->query(QUESTDIR2+"over"))
	{
  	  command("sneer "+me->query("id"));
  	  kill_ob(me); 
  	  set("quest/try_fight",1);
  	  set("quest/try_fight_id",me->query("id"));
  	  command("haha "+me->query("id"));	  
  	  power_condition(me,this_object(),200+random(100));  	  
		  return "不错，大理段公子正在我这里，你要带走还是拿点本事出来吧。\n";
  }
	if (me->query(QUESTDIR2+"start") && me->query_temp(QUESTDIR2+"caught") && me->query_temp(QUESTDIR2+"askzhi")&& !me->query(QUESTDIR2+"over"))
	{
  	  command("sneer "+me->query("id"));
  	  kill_ob(me); 
  	  power_condition(me,this_object(),300+random(100));  	  
		  return "不错，大理段公子正在我这里，你要带走还是拿点本事出来吧。\n";
  }
	if (me->query(QUESTDIR2+"over"))
	{
  	  command("sneer "+me->query("id"));
		  return "不错，大理段公子正在我这里逗留几天，不过现在早不在这里了。\n";
  }
	return "段誉正是大理镇南王之子。";
}
void attempt_apprentice(object ob)
{
        if( (string)ob->query("gender") == "无性" ) {
                command("say 施主身体残缺，虽说佛祖普渡众生，可是...\n");
                return;
        }
        if ((string)ob->query("family/family_name") != "大轮寺") {
                command("say"+ RANK_D->query_respect(ob) +
                        "不是本寺弟子，在这里捣什么乱？！");
                return;
        }
        if ((int)ob->query_skill("longxiang-boruo", 1) < 150) {
                command("say 我大轮寺各种武功均和内功相辅相成。");
                command("say"+ RANK_D->query_respect(ob) +
                        "再熬几个通宵，多多钻研钻研本门的内功吧。");
                return;
        }
        if (((int)ob->query_con() < 32) || ((int)ob->query_str() < 32)){
                command("hmm");
                command("say 这位" + RANK_D->query_respect(ob) +
                	"体质似乎不适合学习我火焰刀神功，还是尽早想想别的出路吧。");
                return;
        }
	command("say 看你根骨精奇，确实是练武的好材料，好吧，就让我来好好调教调教你吧。");
	command("recruit " + ob->query("id"));
	ob->set("title", HIY "大轮寺掌门座下弟子" NOR);
}
int power_condition(object me,object ob,int p)
{
	int i;
	if(!me || !ob) return 0;
	if(environment(me)!=environment(ob)) return 0;
	
 	i=me->query("max_pot");
	if(p<150) p=150;
	if(i<p) i=p;
	i=i-125;	
  ob->set_skill("huanxi-chan", i+random(50));
  ob->set_skill("literate", i+random(50));
  ob->set_skill("force", i+random(50));
  ob->set_skill("longxiang-boruo",i+random(50));
  ob->set_skill("dodge", i+random(50));
	ob->set_skill("poison", 200);
  ob->set_skill("yuxue-dunxing", i+random(50));
  ob->set_skill("strike", i+random(50));
  ob->set_skill("huoyan-dao", i+random(50));
  ob->set_skill("parry", i+random(50));
  ob->set_skill("staff",i+random(50));
  ob->set_skill("wushang-dali",i+random(50));
  i=me->query("combat_exp");
  ob->set("combat_exp",i);
  i=me->query("neili");	  
  if(i<15000) i=15000;
  ob->set("neili", i);	  
  i=me->query("max_neili");	  
  if(i<8000) i=8000;
  ob->set("max_neili", i);	  
 	i=me->query("max_qi");	  
  ob->set("max_qi", i*2);	  
  ob->set("eff_qi", ob->query("max_qi"));	
  ob->set("qi", ob->query("max_qi"));	
  return 1;
}

void die()
{
	object me,ob=this_object();
	
	if(ob->query("quest/try_fight"))
	  if(ob->query("quest/try_fight_id"))
	  {
	  	me=find_player(ob->query("quest/try_fight_id"));
	  	if(!me) return 0;
	    if(environment(me)!=environment(ob)) return 0;
	    if (me->query(QUESTDIR2+"start") && me->query_temp(QUESTDIR2+"caught") && !me->query_temp(QUESTDIR2+"askzhi")&& !me->query(QUESTDIR2+"over"))
	        me->set_temp(QUESTDIR2+"askzhi",1);
	    ob->delete("quest/try_fight");
	    ob->delete("quest/try_fight_id");
     }
	::die();
}