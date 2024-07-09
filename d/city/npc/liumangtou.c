// liumangtou.c 流氓头

inherit NPC;

void create()
{
	set_name("流氓头", ({ "liumang tou", "liumang", "liu" }));
	set("gender", "男性");
	set("age", 25);
	set("long", "他长得奸嘴猴腮的，一看就不像是个好人。\n");
	
	set("combat_exp", 7000);
	set("shen_type", -1);
	set("attitude", "peaceful");
	
	set_skill("unarmed", 40);
	set_skill("dodge", 40);
	set_temp("apply/attack", 20);
	set_temp("apply/defense", 20);

	set("inquiry", ([
		"name" : "连你大爷我‘猪头’王菌生都不认识？还想不想在猪协混了？！",
		"rumors" : "珠宝店那里有些好玩意，象是戒指、玉箫、项链什么的，据说还有本奇书，读了能长生不老，明天带弟兄们去抢回来。",
		"here" : "这里是扬州呀，去看地图吧。",
	]));
	setup();
	carry_object("/clone/misc/cloth")->wear();
	add_money("silver", 18);
}
