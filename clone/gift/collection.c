
#include <ansi.h>
#include <collection.h>
#include <armor.h>

inherit BAG;

string *wcolor = ({HIB,WHT,HIW,CYN,HIC,YEL,HIY,MAG,HIM,RED,HIR});  
string *itemlist = keys(COLL);
//string *orelist = keys(ORE);
string *lv_desc = ({"破碎","","稀有","极品","完美","神奇","史诗","传说"});  

string *ore_list = ({"tiekuang shi","tongkuang shi","yinkuang shi","jinkuang shi"});  

void create()
{	
	set_name("背囊", ({"bei Nang","nang","bag"}) );
	set("long",HIY"存放物品的背囊。\n"NOR);
	set("unit", "个");
  set("no_get",1);
  set("no_sell",1);
  set("no_give",1);
  set("no_drop",1);
  set("armor_type", "bag");
  set("armor_prop/ap",2);
  set_weight(10);
  setup();
}

void init()
{
	object me = this_player();
	string myid = me->query("id");
	if (me && present(this_object(), me))	{
		add_action("do_look","look");
	}
	wear();
}

int do_look(string arg) {
	object me = this_player();
	mapping coll;
	string *lvlist= ({"l0","l1","l2","l3","l4","l5","l6","l7","l8","l9"}); 
	string *item;
	string lv,str,str1;
	mixed *ret;
	int i,j,count;
	
	if (!arg) return 0;
	
	
	
	if (arg=="bei nang" || arg=="nang" || arg=="bag") {
		ret = dbquery(sprintf("SELECT * FROM mine WHERE uid='%d';",me->query("BBS_UID")));
		coll = me->query("collection");
		if( (!coll || !mapp(coll)) && !sizeof(ret)) {
			write("你的背囊里没有任何东西。\n");
			return 1;
		}	
		else {
			str = ("你的背囊里有：\n");
			if (sizeof(ret)){
		    for(i=0;i<sizeof(ret);i++)
		      if( ret[i][2]>0 ) {
		      	count++;
		      	str += sprintf("%-50s%8s\n","  "+chinese_number(ret[i][2])+"块"+
		      	ORE[ore_list[ret[i][1]-1]]["name"]+
		      	"("+capitalize(ore_list[ret[i][1]-1])+")","【"+wcolor[0]+chinese_number(0)+"级"+NOR+"】");
		      }	
		  }
			if (coll && mapp(coll)){
				for(i=0;i<8;i++) {
					lv = sprintf("l%d",i);
					if(mapp(coll[lv])) {
						item = keys(coll[lv]);
						if (sizeof(item)<0) continue;
						else {
							for(j=0;j<sizeof(item);j++){
							  if (coll[lv][item[j]] && member_array(item[j],itemlist) != -1) {
								  count++;
								  str1 = sprintf("  %s%s%s%s"NOR"%s(%s)",chinese_number(coll[lv][item[j]]),COLL[item[j]]["unit"],
								    wcolor[i],lv_desc[i],COLL[item[j]]["name"],capitalize(COLL[item[j]]["id"]));
								  str += sprintf("%-50s%8s\n",str1,"【"+wcolor[i]+chinese_number(i)+"级"+NOR+"】");
							  }
							}
					  }
				  }
				}
			}

			if (count>0)
			  write(str);
			else write("你的背囊里没有任何东西。\n");
			return 1; 
		}			  
	}
  else if(member_array(arg,itemlist) != -1 ) {
    for(i=0;i<8;i++) {
			lv = sprintf("l%d",i);
      if(me->query("collection/"+lv+"/"+arg)>0) {
        write(COLL[arg]["long"]);
        return 1;
      }			
		}	
  }
  return 0;
}
