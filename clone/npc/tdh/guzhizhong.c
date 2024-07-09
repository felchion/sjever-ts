// guzhizhong.c 古至中 tdh

inherit NPC;
#include "tdh_npc.h"

void create()
{
	set_name("古至中", ({ "gu zhizhong", "gu", "zhizhong" }));
	set("gender", "男性");
	set("age", 46);
	set("shen_type", 1);
	set("天地会/tangkou","赤火堂");
	set("天地会/title","香主");
	set("title", HIW"天地会赤火堂香主"NOR);
	set("long","他是天地会的赤火堂香主。");

        set("str", 25);
        set("int", 25);
        set("con", 25);
        set("dex", 22);
        set("max_qi", 2000);
        set("max_jing", 1400);
        set("max_jingli", 1500);
        set("neili", 3000);
        set("max_neili", 3000);
        set("jiali", 50);
        set("combat_exp", 1500000);

	set_skill("cuff", 200);
	set_skill("strike", 200);
	set_skill("dodge", 200);
	set_skill("parry", 200);
	set_skill("force", 200);
	set_skill("taizu-quan", 200);
	set_skill("weituo-zhang", 200);
	set_skill("yijin-jing", 200);
	set_skill("shaolin-shenfa", 200);

	map_skill("force", "yijin-jing");
	map_skill("cuff","taizu-quan");
	map_skill("strike","weituo-zhang");
	map_skill("dodge","shaolin-shenfa");
	map_skill("parry","weituo-zhang");
        random(3)>1 ? prepare_skill("cuff","taizu-quan"):prepare_skill("strike","weituo-zhang");

	set("inquiry", ([
		"万云龙": (: ask_join :),
		"天地会": (: ask_tdh :),
		"洪门": (: ask_tdh :),
		"反清复明": (: ask_fq :),
		"陈近南": (: ask_chen :),
		"郑成功": (: ask_zheng :),
		"国姓爷": (: ask_zheng :),
		"三点革命诗": (: ask_sandian :),
		"切口": (: ask_qk :),
		"暗号": (: ask_qk :),
		"明复清反母地父天": (: ask_qk :),
		"活动": (: ask_job :),
		"job": (: ask_job :),
		"联络": (: ask_target :),
		"rumors": "听说最近鞑子的爪牙活动很厉害，到处捉人。\n",
		"加入": "加入什么？\n",
		"入会": "入什么会？我可不懂。\n",
		"台湾": "当初多亏了国姓爷，赶走了红毛鬼，收复台湾。\n",
		"朝廷": "朝廷再也不是以前的朝廷了……\n",
		"明朝": "当年洪武皇帝多英明，可惜子孙不争气……断送了大好河山。\n",
		"鞑子": "鞑子兵可真狠毒，‘扬州十日，嘉定三屠’你没听说过?\n",
		"总舵主": "你说的是哪一个啊？\n",
		"李志成": "那个反贼，当初要不是他兴兵作乱，鞑子也不进不来山海关。\n",
		"吴三桂": "那个大汉奸，把大好河山送与了鞑子……老子总有一天……\n",
		"鳌拜": "那个狗鞑子，早晚我把他连同鞑子皇帝一起宰了。\n",
		"吴之荣": "那个狗官，早晚我把他活剥了。\n",
		"吕留良": "那可是一个大学者啊，你认识？\n",
		"顾炎武": "他可是大大的有学问啊，读书人，咱比不得。\n",
		"吴六奇": "吴……哦……我好像……没听说过。\n",
		"丐帮": "据说天地会会里面也有很多丐帮的兄弟。\n",
		"凝血神爪": "据说陈总舵主练有这么一门功夫，很是厉害。\n",

	]));
	setup();
        carry_object(ARMOR_D("cloth"))->wear();
        carry_object(ARMOR_D("shoes"))->wear();
        add_money("silver", 10 + random(20));
}
