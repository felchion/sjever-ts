//tangfeng@SJ 2004
#include <ansi.h>
inherit NPC;

#define QUESTDIR1 "quest/天龙八部/凌波微步篇/"
#define QUESTDIR2 "quest/天龙八部/天龙营救篇/"
#define QUESTDIR3 "quest/天龙八部/萧峰身世篇/"
#define QUESTDIR4 "quest/天龙八部/大辽救援篇/"

string askduanyu();
string tianlonghelp();
string asksave();
string askxiaofeng();
string askjiumozhi();
string huanggong();

void create()
{
        set_name("范晔", ({ "fan ye", "fan"}));
        set("title", "大理国司马");
        set("gender", "男性");
        set("age", 40);
        set("str", 28);
        set("dex", 20);
        set("long", "此人中等身材，身着锦缎官服，目光如炬，显是
精明人物，尤擅谋略，居大理为官多年功勋赫赫。\n");
        set("max_qi", 2500);
        set("max_jing", 1000);
        set("neili", 2500);
        set("max_neili", 2500);        
        set("eff_jingli",1000);
        set("jiali",50);
        set("combat_exp", 700000);
        set("shen_type", 1);
        set("attitude", "peaceful");
//      create_family("大理", 3 , "弟子");
         
        set_skill("force",120);
        set_skill("sword",135);
        set_skill("strike",130);
        set_skill("qingyan-zhang",130);
        set_skill("dodge",100);
        set_skill("tianlong-xiang",100);
        set_skill("parry",120);
        set_skill("qiantian-yiyang",120);
        set_skill("duanjia-jianfa",135);
        map_skill("parry", "duanjia-jianfa");
        map_skill("dodge", "tianlong-xiang");
        map_skill("strike","qingyan-zhang");
        map_skill("force", "qiantian-yiyang");
        map_skill("sword", "duanjia-jianfa");
        prepare_skill("strike","qingyan-zhang");
	set("inquiry",([
		"段誉" : (: askduanyu :),
		"大理" : "这里便是大理。",
		"天龙寺" : "天龙寺是我段家武学的最高之地。",
		"天龙有难" : (: tianlonghelp :),
		"营救" : (: asksave :),
		"搭救" : (: asksave :),
		"萧峰" : "江湖人称南慕容、北萧峰，端得一条好汉。",
		"营救段誉" : (: asksave :),
		"营救萧峰" : (: askxiaofeng :),
		"慕容复" : "江湖人称南慕容、北萧峰，听说慕容公子年级轻轻，却无所不精。",
		"慕容博" : "我听说是，姑苏慕容不出的杰才，可惜英年早逝。",
		"鸠摩智" : (: askjiumozhi :),
		"六脉神剑" : "传说的我天龙寺最高绝学，我估计这辈子也未能见到。",
	]));
        set_temp("apply/attack", 70);
        set_temp("apply/defense", 70);
        set_temp("apply/armor", 70);
        set_temp("apply/damage", 20);        
        
        setup();
        carry_object("/clone/weapon/gangjian")->wield();
        carry_object("/d/dali/npc/obj/zipao")->wear();
}

string askjiumozhi()
{
	object me;
	me = this_player();
	if (me->query(QUESTDIR1+"over") && !me->query(QUESTDIR2+"start"))
	{
  	  command("nod "+me->query("id"));
		  return "听说，大轮寺第一高僧鸠摩智将来我天龙寺佛法交流，真是我大理之幸啊。";
  }
	if (me->query(QUESTDIR2+"start") && !me->query(QUESTDIR2+"over"))
	{
  	  command("sigh");
  	  command("? "+me->query("id"));
		  return "这魔僧竟然如此欺负我大理无人，什么吐蕃国第一佛法高僧，狗P狗P。";
  }
	if (me->query(QUESTDIR2+"start")&& me->query(QUESTDIR2+"over"))
	{
  	  command("nod "+me->query("id"));
		  return "鸠摩智妄为西域第一佛法高僧，所谓口生莲花，全是一派胡言。";
  }
  /*
  大轮明王鸠摩智是吐蕃国的护国法王，但只听说他具大智慧，精通佛法，每隔五年，开坛讲经说法，西域天竺各地的高僧大德，
  云集大雪山大轮寺，执经问难，研讨内典，闻法既毕，无不欢喜赞叹而去。
  */
  command("nod");
	return "大轮明王鸠摩智是吐蕃国的护国法王，但听说他具大智慧，精通佛法，想来武功也是惊人。";
}
string askxiaofeng()
{
	object me;
	me = this_player();
  if(!me->query(QUESTDIR3+"start"))
  {
  	  command("hoho");
  		return "江湖人称南慕容、北萧峰，端得一条好汉，何来援救之说。";
  }
	if (me->query(QUESTDIR3+"start") && !me->query(QUESTDIR3+"over"))
	{
  	  command("sigh");
  	  command("shrug "+me->query("id"));
		  return "听说这件事情，只是涉及到大辽与大宋之争，不知道主公怎么处理。";
  }

	if (me->query(QUESTDIR3+"bad"))
	{
  	  command("heng");
  	  command("? "+me->query("id"));
                  return "萧峰，难不是你所迫出中原，今日所谓搭救不知什么意思。假惺惺！";
  }
	if (me->query(QUESTDIR3+"start") && me->query(QUESTDIR3+"over")&& !me->query(QUESTDIR4+"start"))
	{
  	  command("sigh");
  	  command("shrug "+me->query("id"));
		  return "听说这件事情，只是涉及到大辽与大宋之争，不知道主公怎么处理。";
  }
	if (me->query(QUESTDIR4+"start")&& !me->query(QUESTDIR4+"over"))
	{
  	  command("nod "+me->query("id"));
		  return "萧峰是我小王子的义兄，自然要救。这位"+RANK_D->query_respect(me) +"可先行一步，随后我和我加小王子一起援到。";
  }
	if (me->query(QUESTDIR4+"start") && me->query(QUESTDIR4+"over"))
	{
  	  command("haha "+me->query("id"));
		  return "这一战真是痛快，"+me->query("name")+"更是威震武林，端得厉害啊。";
  }
  command("hoho");
	return "江湖人称南慕容、北萧峰，端得一条好汉，何来援救之说。";
}
string asksave()
{
	object me;
	me = this_player();
  if(!me->query(QUESTDIR1+"start") &&!me->query(QUESTDIR2+"start"))
  {
  	  command("hoho");
  		return "小王子素来不喜武学，只爱佛法易经，平易近人，何来援救之说？";
  }
	if (me->query(QUESTDIR1+"start") && !me->query(QUESTDIR1+"wuliangover")&& !me->query(QUESTDIR1+"over"))
	{
  	  command("sigh");
  	  command("shrug "+me->query("id"));
		  return "听说，前段时间小王子和镇南王又是吵架一番，至今未有回家，不知"+RANK_D->query_respect(me) +"可否帮助打听一下。";
  }
	if (me->query(QUESTDIR1+"start") && me->query_temp(QUESTDIR1+"yingjiu")&& !me->query(QUESTDIR1+"over"))
	{
  	  command("sigh");
  	  command("shrug "+me->query("id"));
		  return "我知道了，我马上就去，一会叫上华赫艮。";
  }
	if (me->query(QUESTDIR1+"start") && me->query(QUESTDIR1+"over")&& !me->query(QUESTDIR2+"start"))
	{
  	  command("thank "+me->query("id"));
		  return "多谢"+RANK_D->query_respect(me) +"搭救小王子，只是自从万劫谷回来，小王子精神恍惚，不知所中何毒还是受何种内伤。";
  }
	if (me->query(QUESTDIR2+"start") && !me->query(QUESTDIR2+"over")&& !me->query_temp(QUESTDIR2+"caught"))
	{
  	  command("sigh "+me->query("id"));
		  return "小王子自万劫谷回来，总是气血不定，内力怪异，才被送往天龙求救去了。";
  }
	if (me->query(QUESTDIR2+"start") && !me->query(QUESTDIR2+"over"))
	{
  	  command("sigh "+me->query("id"));
		  return "竟然惹出鸠摩智这等高手，但愿王子吉人天相啊。";
  }
  command("hoho");
	return "小王子素来不喜武学，只爱佛法易经，平易近人，何来援救之说？";
}
string askduanyu()
{
	object me;
	me = this_player();
  if(!me->query(QUESTDIR1+"start" )&&!me->query(QUESTDIR2+"start"))
  {
  	  command("hoho");
  		return "小王子素来不喜武学，只爱佛法易经。";
  }
	if (me->query(QUESTDIR1+"start") && !me->query(QUESTDIR1+"wuliangover")&& !me->query(QUESTDIR1+"over"))
	{
  	  command("sigh");
  	  command("shrug "+me->query("id"));
		  return "听说，前段时间小王子和镇南王又是吵架一番，至今未有回家。";
  }
	if (me->query(QUESTDIR1+"start") && me->query_temp(QUESTDIR1+"yingjiu")&& !me->query(QUESTDIR1+"over"))
	{
  	  command("sigh");
  	  command("shrug "+me->query("id"));
		  return "刚有探报，小王子被万劫谷所盘留，万劫谷什么地方啊。";
  }
	if (me->query(QUESTDIR1+"start") && me->query(QUESTDIR1+"over")&& !me->query(QUESTDIR2+"start"))
	{
  	  command("thank "+me->query("id"));
		  return "多谢"+RANK_D->query_respect(me) +"搭救我家小王子。";
  }
	if (me->query(QUESTDIR2+"start") && !me->query(QUESTDIR2+"over")&& !me->query_temp(QUESTDIR2+"caught"))
	{
  	  command("sigh "+me->query("id"));
		  return "小王子自万劫谷回来，总是气血不定，内力怪异，才被送往天龙求救去了。";
  }
	if (me->query(QUESTDIR2+"start") && !me->query(QUESTDIR2+"over"))
	{
  	  command("sigh "+me->query("id"));
		  return "竟然惹出鸠摩智这等高手，但愿王子吉人天相啊。";
  }
  command("hoho");
	return "段誉正是镇南王之子。";
}
string tianlonghelp()
{
	object me;
	me = this_player();
  if(!me->query(QUESTDIR1+"over"))
  {
  	  command("haha");
  		return "天龙寺是我段家武学的最高之地，高手众云，请不要危言耸听。";
  }
	if (me->query(QUESTDIR1+"over") && !me->query(QUESTDIR2+"start"))
	{
  	  command("nod "+me->query("id"));
		  return "听说，大轮寺高僧鸠摩智不日将来我天龙寺，不知是喜是祸。";
  }
	if (me->query(QUESTDIR2+"start") && !me->query(QUESTDIR2+"over"))
	{
  	  command("sigh");
		  return "这魔僧竟然如此欺负我大理无人，什么吐蕃国第一佛法高僧，狗P。";
  }
	if (me->query(QUESTDIR2+"over"))
	{
  	  command("thank "+me->query("id"));
		  return "多谢"+RANK_D->query_respect(me) +"化解这场天龙危机，还将我大理小王子得以解救。";
  }
  command("hoho");
	return "天龙寺是我段家武学的最高之地，高手众云，请不要危言耸听。";
}
