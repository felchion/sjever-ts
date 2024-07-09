
#ifndef _COL_H_
#define _COL_H_



#define COLL ([ \
	"jingtie po" 		: ([ \
		"name":		HIW"精铁魄"NOR,\
		"long":		HIW"闪烁着奇异光泽的白色金属。\n"NOR,\
		"id":		"jingtie po",\
		"unit":	"块",\
		"usage" : "forg_lian",\
		"value":	1,]),\
	"chitong po"     : ([\
		"name":		RED"赤铜魄"NOR,\
		"long":		RED"闪烁着奇异光泽的红色金属。\n"NOR,\
		"unit":	"块",\
		"id":			"chitong po",\
		"usage" : "forg_lian",\
		"value":	5,]),\
	"chenyin po"     : ([\
		"name":		WHT"沉银魄"NOR,\
		"long":		WHT"闪烁着奇异光泽的银色金属。\n"NOR,\
		"unit":	"块",\
		"id":			"chenyin po",\
		"usage" : "forg_lian",\
		"value":	25,]),\
  "liujin po"     : ([\
		"name":		HIY"流金魄"NOR,\
		"long":		HIY"闪烁着奇异光泽的金色金属。\n"NOR,\
		"unit":	"块",\
		"usage" : "forg_lian",\
		"id":			"liujin po",\
		"value":	100,]),\
  "huanxing tie"     : ([\
		"name":		HIW"寒星铁"NOR,\
		"long":		HIW"很普通的铁块，表面偶尔闪烁起一两点寒芒。\n"NOR,\
		"unit":	"块",\
		"usage" : "forg_slot",\
		"id":			"huanxing tie",\
		"value":	1,]),\
  "dihuo tong"     : ([\
		"name":		HIR"地火铜"NOR,\
		"long":		HIR"采自地底流火汇集之处的红铜。\n"NOR,\
		"unit":	"块",\
		"usage" : "forg_slot",\
		"id":			"dihuo tong",\
		"value":	1,]),\
  "xuanbing yin"     : ([\
		"name":		WHT"玄冰银"NOR,\
		"long":		WHT"摸起来感觉极为寒冷的白色金属。\n"NOR,\
		"unit":	"块",\
		"usage" : "forg_slot",\
		"id":			"xuanbing yin",\
		"value":	1,]),\
  "xingchen jin"     : ([\
		"name":		HIY"星晨金"NOR,\
		"long":		HIY"传说中的金属，据说拥有星辰的力量。\n"NOR,\
		"unit":	"块",\
		"usage" : "forg_slot",\
		"id":			"xingchen po",\
		"value":	1,]),\
  "tianhun shi"     : ([\
		"name":		HIM"天魂石"NOR,\
		"long":		HIM"淡紫色的水晶，里面隐约有黑光流动。\n"NOR,\
		"unit":	"颗",\
		"usage" : "forg_slot",\
		"id":			"tianhun shi",\
		"value":	1,]),\
  "heiyao shi"     : ([\
		"name":		BLU"黑曜石"NOR,\
		"long":		BLU"黑漆漆的石头。\n"NOR,\
		"unit":	"颗",\
		"usage" : "embed_ap",\
		"id":			"heiyao shi",\
		"value":	1,]),\
  "chiguan shi"     : ([\
		"name":		RED"赤光石"NOR,\
		"long":		RED"暗红色的石头，入手感觉微温。\n"NOR,\
		"unit":	"颗",\
		"usage" : "embed_ap",\
		"id":			"chiguan shi",\
		"value":	1,]),\
  "ganlan shi"     : ([\
		"name":		HIG"橄榄石"NOR,\
		"long":		HIG"绿色的水晶，轻飘飘几乎没有任何重量。\n"NOR,\
		"unit":	"颗",\
		"usage" : "embed_ap",\
		"id":			"ganlan shi",\
		"value":	1,]),\
  "fengling shi"     : ([\
		"name":		HIW"风灵石"NOR,\
		"long":		HIW"表面有许多空洞的石珠，放在耳边可以听到风声。\n"NOR,\
		"unit":	"颗",\
		"usage" : "embed_ap",\
		"id":			"fengling shi",\
		"value":	1,]),\
  "qinjin shi"     : ([\
		"name":		HIB"青金石"NOR,\
		"long":		HIB"表面泛着蓝光的石头，入手异常沉重。\n"NOR,\
		"unit":	"颗",\
		"usage" : "embed_ap",\
		"id":			"qinjin shi",\
		"value":	1,]),\
])



#define ORE ([ \
	"tiekuang shi" 		: ([ \
		"name" : CYN"铁矿石"NOR,\
		"id" : "tiekuang shi",\
		"refine_id":		"jingtie po",\
		"db_type":	1,]),\
	"tongkuang shi"     : ([\
		"name" : YEL"铜矿石"NOR,\
		"id" : "tongkuang shi",\
		"refine_id":			"chitong po",\
		"db_type":	2,]),\
	"yinkuang shi"     : ([\
		"name" : HIW"银矿石"NOR,\
		"id" : "yinkuang shi" ,\
		"refine_id":			"chenyin po",\
		"db_type":	3,]),\
  "jinkuang shi"     : ([\
		"name" :	HIY"金矿石"NOR,\
		"id" : "jinkuang shi",\
		"refine_id":			"liujin po",\
		"db_type":	4,]),\
])

#endif
