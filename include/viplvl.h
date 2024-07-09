#ifndef _VlV_H_
#define _VLV_H_

#include <ansi.h>

static string *v_color = ({"","","",HIY,HIW,HIB,MAG,YEL,BLU,HIC,HIR,BLK+BWHT,"",WHT});
static string *vip_type = ({"非","未注册","普通","黄金","白金","蓝金","紫金","暗金","黑金","青金","赤金","玄金","幻金","星钻","火钻","玄钻","幻石","钻石"});


string Vip_Color(int lvl) {
	string *clr=({MAG,YEL,BLU,CYN,RED,GRN,HIM,HIY,HIB,HIC,HIR,HIG});
	
	if(lvl<12) return v_color[lvl];
	if(lvl==12)	return clr[random(12)]+BWHT;
	if(lvl==17) return WHT;
	if(lvl==13) return ansi_color(27,18);
	if(lvl==14) return ansi_color(207,124);
	if(lvl==15) return ansi_color(232,235);
	if(lvl==16) return ansi_color(232,16+random(239));	
}



string Vip_Class(int lvl) {
	if(lvl>17) lvl=17;
	return Vip_Color(lvl)+vip_type[lvl]+"会员"+NOR;
}

#endif