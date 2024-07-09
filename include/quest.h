
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
	  "name"	:		"������������",
		"start"	:		15000,
		"up"		:		0,
		"flag"	:		"quest/yttlx/"]),
	"yttlc" 		: ([ 
		"name"	:		"������������",
		"start"	:		10000,
		"up"		:		0,
		"flag"	:		"quest/yttlc/"]),	
	"yttlp" 		: ([ 
		"name"	:		"������������",
		"start"	:		12000,
		"up"		:		0,
		"flag"	:		"quest/yttlp/"]),	
	"yttl" 		: ([ 
		"name"	:		"��������",
		"start"	:		30000,
		"up"		:		0,
		"flag"	:		"quest/yttl/"]),	
	"jsjf" 		: ([ 
		"name"	:		"�����ؼ�",
		"start"	:		3000,
		"up"		:		0,
		"flag"	:		"quest/jinshe/"]),	
	"whdmd" 		: ([ 
		"name"	:		"�廢���ŵ�",
		"start"	:		1500,
		"up"		:		500,
		"env"		:		"��",
		"flag"	:		"quest/whdmd/"]),	
	"jybgz"     : ([
		"name"	:		"�����澭���¾�",
		"start"	:		1500,
		"up"		:		500,
		"env"		:		"����צ",
		"flag"	:		"quest/jiuyin2/"]),
	"hjdf"     : ([
		"name"	:		"���ҵ���",
		"start"	:		12000,
		"up"		:		0,
		"flag"	:		"quest/hjdf/"]),
	"bgz"     : ([
		"name"	:		"�����澭���׹�צ",
		"start"	:		15000,
		"up"		:		0,
		"flag"	:		"quest/jybgz/"]),
	"jyzz"     : ([
		"name"	:		"�����澭���ܾ�",
		"start"	:		20000,
		"up"		:		20000,
		"flag"	:		"quest/jyzz/"]),
	"kmq"     : ([
		"name"	:		"����ȭ",
		"start"	:		3000,
		"up"		:		0,
		"flag"	:		"quest/jiebai/"]),
	"tzq"     : ([
		"name"	:		"̫��ȭ����",
		"start"	:		3000,
		"up"		:		0,
		"flag"	:		"quest/tzq/"]),
	"hubo"     : ([
		"name"	:		"���һ���",
		"start"	:		3000,
		"up"		:		1000,
		"flag"	:		"quest/hubo/"]),
	"sxbb"     : ([
		"name"	:		"���аٱ�",
		"start"	:		9000,
		"up"		:		3000,
		"env"		:		"sxbb",
		"flag"	:		"quest/sxbb/"]),
	"lbwb"     : ([
		"name"	:		"�貨΢��",
		"start"	:		6000,
		"up"		:		2000,
		"env"		:		"����",
		"flag"	:		"quest/�����˲�/�书/"]),
	"lb"     : ([
		"name"	:		"�貨΢������",
		"start"	:		4500,
		"up"		:		1500,
		"env"		:		"����",
		"flag"	:		"quest/lbwbh/"]),
	"lbj"     : ([
		"name"	:		"�貨΢������",
		"start"	:		9000,
		"up"		:		3000,
		"env"		:		"����",
		"flag"	:		"quest/lbwbj/"]),
	"pxj"     : ([
		"name"	:		"��а����",
		"start"	:		7500,
		"up"		:		2500,
		"env"		:		"������Ӱ��",
		"flag"	:		"quest/pixie/"]),
	"hmg"     : ([
		"name"	:		"��󡹦",
		"start"	:		12000,
		"up"		:		4000,
		"env"		:		"��ת",
		"flag"	:		"quest/hama/"]),
	"jyzj"     : ([
		"name"	:		"�����澭���Ͼ�",
		"start"	:		15000,
		"env"		:		"��Ѩ",
		"up"		:		5000,
		"flag"	:		"quest/jiuyin1/"]),
	"szj"     : ([
		"name"	:		"���վ�",
		"start"	:		9000,
		"env"		:		"��Ӱȭ",
		"up"		:		3000,
		"flag"	:		"quest/szj/"]),
	"tsjf"     : ([
		"name"	:		"��ʬ����",
		"start"	:		20000,
		"env"		:		"��Ӱȭ",
		"flag"	:		"quest/tsjf/"]),
	"ynj"     : ([
		"name"	:		"ԽŮ��",
		"start"	:		13500,
		"env"		:		"����|��Ŀ",
		"up"		:		4500,
		"flag"	:		"quest/ynj/"]),
	"txj"     : ([
		"name"	:		"����̫����",
		"start"	:		13000,
		"up"		:		13000,
		"flag"	:		"quest/txj/"]),
	"jy"     : ([
		"name"	:		"�����澭���ܸ�",
		"start"	:		30000,
		"up"		:		30000,
		"flag"	:		"quest/jy/"]),
	"bmsg"     : ([
	  "name"	:		"��ڤ�񹦡��׾�",
		"start"	:		36000,
		"up"		:		12000,
		"flag"	:		"quest/bmsg/"]),
	"bmfy"     : ([
	  "name"	:		"��ڤ�񹦡�����ƪ",
		"start"	:		10000,
		"up"		:		10000,
		"flag"	:		"quest/bmfy/"]),
	"bmjm"     : ([
	  "name"	:		"��ڤ�񹦡�����ƪ",
		"start"	:		12000,
		"up"		:		12000,
		"flag"	:		"quest/bmjm/"]),
	"bmkz"     : ([
	  "name"	:		"��ڤ�񹦡�����ƪ",
		"start"	:		10000,
		"up"		:		10000,
	  "flag"	:		"quest/bmkz/"]),
	"bmzj"     : ([
	  "name"	:		"��ڤ�񹦡��м�ƪ",
		"start"	:		15000,
		"up"		:		15000,
		"flag"	:		"quest/bmzj/"]),
	"bmdb"     : ([
	  "name"	:		"��ڤ�񹦡����ƪ",
		"start"	:		15000,
		"up"		:		15000,
		"flag"	:		"quest/bmdb/"]),
	"bmyq"     : ([
	  "name"	:		"��ڤ�񹦡�����ƪ",
		"start"	:		120000,
		"up"		:		40000,
		"flag"	:		"quest/bmyq/"]),
	"bmcs"     : ([
	  "name"	:		"��ڤ�񹦡�����ƪ",
		"start"	:		24000,
		"up"		:		24000,
		"flag"	:		"quest/bmcs/"]),
	"zms"     : ([
	  "name"	:		"��ɽ��÷��",
		"start"	:		30000,
		"up"		:		0,
		"flag"	:		"quest/zms/"]),
	"bdbq"    : ([
		"name"	:		"�ٶ�����",
		"start"	:		50000,
		"up"		:		0,
		"flag"	:		"quest/bdbq/"]),
	"nxsz"     : ([
	  "name"	:		"��Ѫ��ץ",
		"start"	:		80000,
		"up"		:		80000,
		"flag"	:		"quest/ningxue/"]),
	"qzs"     : ([
		"name"	:		"ǧ������",
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
