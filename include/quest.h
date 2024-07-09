
#ifndef _QUEST_H_
#define _QUEST_H_
/*
jsj
"d/huashan/jinshe/obj/tiexia.c"
"d/huashan/jinshe/obj/jinshenmiji.c"

whdmd
"d/city/npc/zhanggui.c"
"d/city/obj/whdmd-book.c" 
"d/city/obj/budai.c"

jybgz
"clone/npc/xu.c"
"d/lanzhou/obj/jiuyin-book2.c"
"d/lanzhou/npc/chen.c"

hubo
"d/thd/npc/zhou.c"

lbwb
"d/dali/wuliang/dong2.c"
"d/dali/obj/dodge-book.c"

lbwbh
"d/dali/obj/dodge-book.c"

lbj
"d/dali/obj/dodge-book.c"
"adm/daemons/combatd"

pxj
"d/fuzhou/hubiao/linzhennan.c"
"d/fuzhou/obj/jiasha.c"
"d/fuzhou/obj/kuihua.c"

jyzj
"clone/npc/xu.c"
"d/thd/npc/zhou.c"
"d/thd/obj/jiuyin-book1.c"
"feature/attibute.c" +int
"inherit/char/char.c" +exp

szj
"d/city/npc/juyou"
"d/suzhou/npc/dingdian.c"
"d/xueshan/npc/diyun.c"

hmg
d/jiaxing/river.c
d/baituo/shitui.c
kungfu/class/baituo/guairen.c
clone/npc/xu.c
feature/attibute.c

ynj
d/jiaxing/npc/aqing
d/suzhou/shanpo

txj
d/xiakedao/shiroom24.c

sxbb
clone/npc/xu.c
cmds/std/go.c
d/city/npc/weixiaobo.c

yttl & yttlc
clone/npc/xu.c
kungfu/class/gaibang/guojing.c
d/village/npc/feng.c

bsmg 
clone/npc/xu.c
d/dali/wuling/dong2.c
d/dali/obj/bm_book.c
inhert/skill/perform.c

bdbq 
d/dali/wuliang/npc/zhong_ling
d/dali/wuling/songlin3.c
kungfu/skill/base/force/heal.c

*/

mapping QUEST = ([ 
	"yttlx"     : ([
	  "name"	:		"倚天屠龙·持",
		"start"	:		15000,
		"up"		:		0,
		"flag"	:		"quest/yttlx/"]),
	"yttlc" 		: ([ 
		"name"	:		"倚天屠龙·残",
		"start"	:		10000,
		"up"		:		0,
		"flag"	:		"quest/yttlc/"]),	
	"yttlp" 		: ([ 
		"name"	:		"倚天屠龙·破",
		"start"	:		12000,
		"up"		:		0,
		"flag"	:		"quest/yttlp/"]),	
	"yttl" 		: ([ 
		"name"	:		"倚天屠龙",
		"start"	:		30000,
		"up"		:		0,
		"flag"	:		"quest/yttl/"]),	
	"jsjf" 		: ([ 
		"name"	:		"金蛇秘籍",
		"start"	:		3000,
		"up"		:		0,
		"flag"	:		"quest/jinshe/"]),	
	"whdmd" 		: ([ 
		"name"	:		"五虎断门刀",
		"start"	:		1500,
		"up"		:		500,
		"env"		:		"断",
		"flag"	:		"quest/whdmd/"]),	
	"jybgz"     : ([
		"name"	:		"九阴真经·下卷",
		"start"	:		1500,
		"up"		:		500,
		"env"		:		"三绝爪",
		"flag"	:		"quest/jiuyin2/"]),
	"hjdf"     : ([
		"name"	:		"胡家刀法",
		"start"	:		12000,
		"up"		:		0,
		"flag"	:		"quest/hjdf/"]),
	"bgz"     : ([
		"name"	:		"九阴真经·白骨爪",
		"start"	:		15000,
		"up"		:		0,
		"flag"	:		"quest/jybgz/"]),
	"jyzz"     : ([
		"name"	:		"九阴真经·总诀",
		"start"	:		20000,
		"up"		:		20000,
		"flag"	:		"quest/jyzz/"]),
	"kmq"     : ([
		"name"	:		"空明拳",
		"start"	:		3000,
		"up"		:		0,
		"flag"	:		"quest/jiebai/"]),
	"tzq"     : ([
		"name"	:		"太祖拳·秘",
		"start"	:		3000,
		"up"		:		0,
		"flag"	:		"quest/tzq/"]),
	"hubo"     : ([
		"name"	:		"左右互搏",
		"start"	:		3000,
		"up"		:		1000,
		"flag"	:		"quest/hubo/"]),
	"sxbb"     : ([
		"name"	:		"神行百变",
		"start"	:		9000,
		"up"		:		3000,
		"env"		:		"sxbb",
		"flag"	:		"quest/sxbb/"]),
	"lbwb"     : ([
		"name"	:		"凌波微步",
		"start"	:		6000,
		"up"		:		2000,
		"env"		:		"洛神",
		"flag"	:		"quest/天龙八部/武功/"]),
	"lb"     : ([
		"name"	:		"凌波微步·幻",
		"start"	:		4500,
		"up"		:		1500,
		"env"		:		"洛神",
		"flag"	:		"quest/lbwbh/"]),
	"lbj"     : ([
		"name"	:		"凌波微步·疾",
		"start"	:		9000,
		"up"		:		3000,
		"env"		:		"洛神",
		"flag"	:		"quest/lbwbj/"]),
	"pxj"     : ([
		"name"	:		"辟邪剑法",
		"start"	:		7500,
		"up"		:		2500,
		"env"		:		"葵花飞影术",
		"flag"	:		"quest/pixie/"]),
	"hmg"     : ([
		"name"	:		"蛤蟆功",
		"start"	:		12000,
		"up"		:		4000,
		"env"		:		"逆转",
		"flag"	:		"quest/hama/"]),
	"jyzj"     : ([
		"name"	:		"九阴真经·上卷",
		"start"	:		15000,
		"env"		:		"冲穴",
		"up"		:		5000,
		"flag"	:		"quest/jiuyin1/"]),
	"szj"     : ([
		"name"	:		"神照经",
		"start"	:		9000,
		"env"		:		"无影拳",
		"up"		:		3000,
		"flag"	:		"quest/szj/"]),
	"tsjf"     : ([
		"name"	:		"躺尸剑法",
		"start"	:		20000,
		"env"		:		"无影拳",
		"flag"	:		"quest/tsjf/"]),
	"ynj"     : ([
		"name"	:		"越女剑",
		"start"	:		13500,
		"env"		:		"戳腕|刺目",
		"up"		:		4500,
		"flag"	:		"quest/ynj/"]),
	"txj"     : ([
		"name"	:		"白首太玄经",
		"start"	:		13000,
		"up"		:		13000,
		"flag"	:		"quest/txj/"]),
	"jy"     : ([
		"name"	:		"九阴真经·总纲",
		"start"	:		30000,
		"up"		:		30000,
		"flag"	:		"quest/jy/"]),
	"bmsg"     : ([
	  "name"	:		"北冥神功·首卷",
		"start"	:		36000,
		"up"		:		12000,
		"flag"	:		"quest/bmsg/"]),
	"bmfy"     : ([
	  "name"	:		"北冥神功·防御篇",
		"start"	:		10000,
		"up"		:		10000,
		"flag"	:		"quest/bmfy/"]),
	"bmjm"     : ([
	  "name"	:		"北冥神功·机敏篇",
		"start"	:		12000,
		"up"		:		12000,
		"flag"	:		"quest/bmjm/"]),
	"bmkz"     : ([
	  "name"	:		"北冥神功·控制篇",
		"start"	:		10000,
		"up"		:		10000,
	  "flag"	:		"quest/bmkz/"]),
	"bmzj"     : ([
	  "name"	:		"北冥神功·招架篇",
		"start"	:		15000,
		"up"		:		15000,
		"flag"	:		"quest/bmzj/"]),
	"bmdb"     : ([
	  "name"	:		"北冥神功·躲避篇",
		"start"	:		15000,
		"up"		:		15000,
		"flag"	:		"quest/bmdb/"]),
	"bmyq"     : ([
	  "name"	:		"北冥神功·御气篇",
		"start"	:		120000,
		"up"		:		40000,
		"flag"	:		"quest/bmyq/"]),
	"bmcs"     : ([
	  "name"	:		"北冥神功·长生篇",
		"start"	:		24000,
		"up"		:		24000,
		"flag"	:		"quest/bmcs/"]),
	"zms"     : ([
	  "name"	:		"天山折梅手",
		"start"	:		30000,
		"up"		:		0,
		"flag"	:		"quest/zms/"]),
	"bdbq"    : ([
		"name"	:		"百毒不侵",
		"start"	:		50000,
		"up"		:		0,
		"flag"	:		"quest/bdbq/"]),
	"nxsz"     : ([
	  "name"	:		"凝血神抓",
		"start"	:		80000,
		"up"		:		80000,
		"flag"	:		"quest/ningxue/"]),
	"qzs"     : ([
		"name"	:		"千蛛万毒手",
		"start"	:		7500,
		"up"		:		2500,
		"flag"	:		"quest/qianzhu/"]),
]);

static string *quest_list=({
  "yttl","yttlc","yttlx","yttlp","tzq",
  "jsjf","whdmd","jyzj","jybgz","bgz","jy","jyzz",
  "hjdf","kmq","hubo","hmg","lbwb","lb","lbj",
  "bmsg","bmfy","bmjm","bmdb","bmzj","bmcs","bmyq",
  "zms","bdbq","sxbb","ynj","szj","tsjf","pxj","qzs","txj"
});  

#endif
