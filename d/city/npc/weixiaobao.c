//weixiaobao.c
//目前没有加上贵宾的限制，等大家熟悉后过几天加上
#include <ansi.h>
inherit NPC;
inherit F_VENDOR;

#define WEAPON_P(i) "/d/city/weapon/weapon"+i
#define WEAPON_TOTAL 15
#define WEAPON_LESS 10
#define WEAPON_NUMBER 3
#define UPDATE_TIME 1

void born_weapon();
void check_time();
void start_weapon();
int born_file(string file);
string ask_tdh1();
string ask_tdh2();
string ask_tdh3();
string ask_tdh4();
string ask_fight();
int do_yes(string arg);

int move_other(string place);
int ask_jiebai();


//匕首、宝衣、蒙汗药”，乃小白龙韦小宝攻守兼备的三大法宝。
void create()
{
  set_name("韦小宝", ({ "wei xiaobao", "wei", "xiaobao" }));
  set("title",HIM"鹿鼎公"NOR);
  set("nickname",HIW"小白龙"NOR);
  set("shen_type", 1);

  set("gender", "男性");
  set("no_ansuan",1);
  set("age",22);
  set("str",10);
  set("con",20);
  set("int",50);
  set("dex",30);
  set("kar",100);
  set("per",30);
  set("pur",5);
  set("long",
"    丝竹悠扬声中，两面大旗招展而出，左面大旗上写着“抚远大将军韦”，
右面大旗上写着“大清鹿鼎公韦”，数百名砍刀手拥着一位少年将军骑马而出。
这位将军头戴红顶子，身穿黄马褂，眉花眼笑，贼忒兮兮，左手轻摇羽扇，宛
若诸葛之亮，右手倒拖大刀，俨然关云之长，正乃韦公小宝是也。

传闻此人极讲义气，你可以尝试结交此人。");
  set_skill("unarmed", 20);
  set_skill("dodge", 900);

  set_skill("youlong-shenfa",901);
  set_skill("dulong-dafa",101);

  map_skill("force","dulong-dafa");
  map_skill("dodge","youlong-shenfa");

  set("max_neili",9000);
  set("max_jingli",9000);

  set_temp("apply/damage", 15);

  set("combat_exp", 4000);
  set("attitude", "friendly");

  set("chat_chance", 1);
  set("quest/baibian",1);
  set("move_seller",1);
  // 韦小宝
  set("chat_msg", ({
    (: move_other("") :),
    "韦小宝得意地说道：匕首、宝衣、蒙汗药”，乃我小白龙韦小宝攻守兼备的三大法宝。\n",
    "韦小宝道：好兄弟讲义气！\n",
    "韦小宝道：大丈夫一言既出，什么马难追，我说过的话，可重不反悔的。\n",
    "韦小宝大喊一声：辣块妈妈！\n",

  }) );
  set("inquiry", ([
    "name": "我就是大清钦差大臣、领内侍卫副大臣、兼骁骑营正黄旗满洲都统、钦赐巴图鲁勇号、赐穿黄马褂、一等鹿鼎公。",
    "秘密": "其实我还是天地会总舵主陈近南入室弟子，青木堂堂主。",
    "神龙教":"我小白龙曾任神龙教白龙使。",
    "独臂神尼":"不知恩师近日可好。",
    "老婆":"老婆多了，麻烦的事也多。",
    "18mo":"那还不是我的专利。",
    "dazao":"你还是去找韦兰铁匠吧。",
    "weilan":"他是这带很有名的兵器打造者，我这里的兵器都是他给我的。",
    "韦兰":"他是这带很有名的兵器打造者，我这里的兵器都是他给我的。",
    "结拜":(: ask_jiebai :),
    "陈近南":"生平不识陈近南，便称英雄也枉然。",
    "茅十八":"不知道十八哥最近如何了。",
    "阿柯":"她是我老婆",
    "双儿":"她是我老婆，目前在扬州！",
    "天地会":(: ask_tdh1 :),
    "小玄子":"小玄子再也不是以前的小玄子了。",
    "韦春芳":"那是我老娘，你怎么认识的？",
    "丽春院":"有空去那里捧捧我老娘的场啊",
    "门朝大海，三河合水万年流":(: ask_tdh2:),
    "青木堂":(: ask_tdh3:),
    "较量": (:ask_fight:),
  ]));
  setup();
  if(clonep())
  {
    start_weapon();
    carry_object(__DIR__"obj/huangmagua")->wear();
  }
}

//个性化设置
string ask_tdh1()
{
  command("say");
  this_object()->set_temp("wei_ask_tdh",1);
  return "地振高冈，一派溪山千古秀。";
}
string ask_tdh2()
{
  if (this_object()->query_temp("wei_ask_tdh") == 1 )
  {
    command("touch "+this_object()->query("id",1));
    this_object()->set_temp("wei_ask_tdh",2);
    return "红花亭畔哪一堂？";
  }
  else return 0;
}
string ask_tdh3()
{
  if (this_object()->query_temp("wei_ask_tdh") == 2 )
  {
    command("cat "+this_player()->query("id",1));
    this_object()->delete_temp("wei_ask_tdh");
    return "原来是本堂兄弟啊";
  }
  else return 0;
}


void relay_emote(object ob, string verb)
{
  if( !userp(ob) ) return;
  switch(verb) {

    case "kiss1":
    case "love":
    case "kiss":
    case "taste":
    case "rose":
    case "boat":
    case "18mo":
    case "ysis":

    if ( (string) ob -> query ("gender") == "男性")
    {
      command ("say 啊！！，"+ ob->query("name") + "，你是同志？！\n") ;
      command ("say 哎哟，" + RANK_D->query_rude(ob)+"我有七个老婆，你不知道？");
    }
    if (ob->query("gender") == "女性" && ( ob->query_per() > 30 || ob->query("age") < query("age") ) )
    {
      command("18mo");
      message_vision("\n韦小宝对$N笑道：“虽然我有很多老婆，多你一个又何妨？。”\n", ob);
      command("consider");
    }
    if (ob->query("gender") == "女性" && ( ob->query_per() < 15 || ob->query("age") > 50 ) )
    {
      command("bite");
      message_vision("\n韦小宝对$N笑道：“你是老太婆了，难道我还感兴趣？”\n", ob);
      command("disapp " + ob->query("id"));
    }

    break;
    default:
    if( random(10)<2 )
    {
      command(verb + " " + ob->query("id"));
      return;
    }
    else
    {
      switch(verb)
      {
        case  "nod":
        case  "smile":
        case  "hehe":
        case  "water":
        case  "snow":
        case  "club":
        case  "haha":
        case  "die":
        case  "kick":
        case  "corpse":
        case  "rich":
        case  "hi":
        case  "puke":
        case  "faint":
        case  "escape":
        case  "raise":
        case  "bite":
        case  "knife":
        case  "drug":
        case  "cook":
        case  "shutup":
        case  "slogan2":
        case  "slap":
        case  "sing":
        case  "sigh":
        case  "shout":
        case  "sister":
        switch(random(50))
        {
          case 0:
          command ("kick "+ ob->query("id"));
          command ("hehe");
          break;
          case 1:
          command ("club "+ ob->query("id"));
          command ("hehe");
          break;
          case 2:
          command ("drug2 "+ ob->query("id"));
          break;
          case 3:
          command ("smile");
          command ("cook "+ ob->query("id"));
          break;
          case 4:
          command ("smash "+ ob->query("id"));
          break;
          case 5:
          command ("knife "+ ob->query("id"));
          command ("sister "+ ob->query("id"));
          break;
          case 6:
          command("cry");
          command("say 啊，"+ob->query("name")+"你"+RANK_D->query_rude(ob)+"居然比我还坏！\n");
          break;
          case 7:
          command ("shutup ");
          command ("shutup "+ ob->query("id"));
          break;
          case 8:
          command("cry");
          command ("corpse "+ ob->query("id"));
          break;
          case 9:
          command ("bug "+ ob->query("id"));
          break;
          case 10:
          command ("brag "+ ob->query("id"));
          break;
          case 11:
          command ("bite");
          command ("shout "+ ob->query("id"));
          break;
          case 12:
          command ("shake");
          break;
          case 13:
          command ("stare "+ob->query("id"));
          command ("hit "+ ob->query("id"));
          break;
          case 14:
          command ("sneer");
          break;
          case 15:
          command ("faint");
          break;
          case 16:
          command ("ah");
          break;
          case 17:
          command ("ysis "+ob->query("id"));
          command ("hehe");
          break;
          case 18:
          command ("blush");
          break;
          case 19:
          command ("walkby");
          break;
          case 20:
          command("18mo");
          break;
          case 21:
          command ("noway");
          break;
          case 22:
          command ("cry");
          break;
          case 23:
          command ("pig");
          command ("corpse "+ob->query("id"));
          break;
          case 24:
          command ("careful");
          break;
          case 25:
          command ("zzz "+ob->query("id"));
          break;
          case 26:
          command ("praise");
          command ("faint");
          break;
          case 27:
          command ("poem "+ob->query("id"));
          break;
          case 28:
          command ("rascal "+ob->query("id"));
          break;
          case 29:
          command ("meeting "+ob->query("id"));
          command ("die "+ob->query("id"));
          break;
          case 30:
          command("giggle "+ob->query("id"));
          break;
          case 31:
          command("cut");
          break;
          case 32:
          command ("sing3") ;
          break;
          case 33:
          command ("escape "+ ob->query("id"));
          break;
          case 34:
          command("faint "+ob->query("id"));
          break;
          case 35:
          command ("kick "+ ob->query("id"));
          command ("dead");
          break;
          case 36:
          command ("slap "+ ob->query("id"));
          command ("hehe");
          break;
          case 37:
          command("faint");
          break;
          case 38:
          command ("laugh "+ ob->query("id"));
          break;
          case 39:
          command("hero");
          break;
          default:
          if( random(10)<3 )
          {
            command(verb + " " + ob->query("id"));
            return;
          }
          break;
          return;
        }
        case "kiss1":
        case "sister":
        case "jump":
        case "love":
        case "lovelook":
        case "praise":
        case "visite":
        case "drinkcup":
        case "goodkid":
        case "pat":
        case "addoil":
        case "laughproud":
        case "smile":
        case "xixi":
        case "hehe":
        case "giggle":
        case "thumb":
        case "admire":
        case "applaud":
        case "hi":
        case "love":
        case "sigh":
        case "haha":
        switch(random(35))
        {
          case 0:
          command ("pat "+ ob->query("id"));
          break;
          case 1:
          command ("hmm");
          break;
          case 2:
          command ("praise "+ ob->query("id"));
          break;
          case 3:
          command ("xixi");
          break;
          case 4:
          command("say "+ob->query("name")+"你说得话我很喜欢听！\n");
          command ("grin "+ ob->query("id"));
          break;
          case 5:
          command("say "+ob->query("name")+"明儿我给你搞个官当当？\n");
          command ("goodkid "+ ob->query("id"));
          break;
          case 6:
          command ("pat "+ ob->query("id"));
          break;
          case 7:
          command("say "+ob->query("name")+"明天我们一起去皇宫？！\n");
          command ("haha");
          break;
          case 8:
          command ("wink "+ ob->query("id"));
          break;
          case 9:
          command ("laughproud "+ ob->query("id"));
          break;
          case 10:
          command("say "+ob->query("name")+",你小子，不错，有前途！\n");
          command ("thank");
          break;
          case 11:
          command ("nod");
          command ("xixi");
          break;
          case 12:
          command ("shake");
          break;
          case 13:
          command("say "+ob->query("name")+",你妈贵姓？\n");
          command ("push "+ob->query("id"));
          break;
          case 14:
          command ("xixi");
          break;
          case 15:
          command ("lovelook "+ob->query("id"));
          break;
          case 16:
          command ("smile "+ob->query("id"));
          break;
          case 17:
          command("say 好，好，下回来我妈开的店，我给你打八折！\n");
          command ("kiss "+ob->query("id"));
          break;
          case 18:
          command ("addoil "+ob->query("id"));
          break;
          case 19:
          command ("xixi");
          break;
          case 20:
          command ("handshake "+ob->query("id"));
          break;
          case 21:
          command ("puke "+ob->query("id"));
          break;
          case 22:
          command ("touch "+ob->query("id"));
          break;
          case 23:
          command ("mimi");
          break;
          case 24:
          command ("slap "+ob->query("id"));
          break;
          case 25:
          command ("thumb "+ob->query("id"));
          break;
          case 26:
          command ("applaud ");
          break;
          case 27:
          command ("hi");
          break;
          case 28:
          command ("hi "+ob->query("id"));
          break;
          case 29:
          command("say 你小子怎么这样？太没大没小吧！\n");
          command("angry");
          break;
          default:
          command(verb + " " + ob->query("id"));
          command("smile");
          break;
          return;
        }
        return;
      }
    }
  }
  return;
}
int ask_jiebai()
{
  object ob;
  int i;
  string str;

  ob = this_player();
  str = RANK_D->query_respect(ob);

  if(ob->query("quest/wxbjiebai"))
  {
    command("say 我们不是结拜过了么？");
    return 1;
  }
  if (!intp(i = ob->query_temp("give_wei_money_gived") ) ) i = 0;
  switch(i){
    case 0:str = "这位"+str+"与我初次见面，怕连对方爹娘是谁都不知道吧？";break;
    case 1:str = "我们还需要多多亲近亲近哦";break;
    case 2:str = "哥们有空陪我去听听说书啊什么的，呵呵";break;
    case 3:str = "哈哈！开玩笑来着啊，你还当真啊！";break;
    default:str = "你无聊不？";break;
  }
  command("say "+str);
  return 1;
}

int move_other(string place)
{
  string *places = ({"/d/city/lichunyuan","/d/dali/dalics","/d/emei/qingyinge","/d/foshan/kedian","/d/fuzhou/zhengting","/d/hengshan/baiyunan","/d/hz/tianxiang","/d/suzhou/canglangting","/d/xiangyang/cross1"});
  object me = this_object();

  tell_room(environment(me),"突然远处传来一声“老公～～”,"+me->name()+"“嗖”的不知道去哪里了。\n");
  if (place=="") me->move(places[random(sizeof(places))]);
  else me->move(place);
  tell_room(environment(me),me->name()+"突然不知道从哪里钻出来，出现在你面前。\n");
  set("startroom",base_name(environment()));
  return 1;
}

int accept_object(object who, object ob)
{

  if (ob->query("money_id"))
  {
    if ( ob->value() < 500000 )
    {
      command("? "+who->query("id",1));
      command("say 你想干什么？");
      return 0;
    }
    command("hehe");
    //索额图理论 嘿嘿
    if (who->query_temp("give_wei_money_times") < 2 )
    {
      if ( !who->query_temp("give_wei_money_times") ) who->set_temp("give_wei_money_times",1);
      else who->add_temp("give_wei_money_times",1);
      switch( who->query_temp("give_wei_money_times") )
      {
        case 0:command("say 这怎么好意思呢？");break;
        case 1:command("say 这怎么好意思呢？");break;
        case 2:command("say 这么贵重，不大好吧？");break;
      }
      return 0;
    }
    who->delete_temp("give_wei_money_times");
    //加某些quest
    if (!who->query_temp("give_wei_money_gived")) who->set_temp("give_wei_money_gived",1);
    else who->add_temp("give_wei_money_gived",1);
    //haha
    command("say 那就只有恭敬不如从命了！");
    call_out("destroying", 1, ob);
    return 1;
  }
  else {
    command("en "+who->query("id",1));
    return 0;
  }
}

void destroying(object ob)
{
  if (!ob) return;
  if (userp(ob)) command("drop "+ob->parse_command_id_list()[0]);
  else destruct(ob);
}

int accept_fight(object ob){
  command("cry2");
  return 0;
}


void init()
{
  object  ob;

  add_action("do_list", "list");
  add_action("do_buy", "buy");
  add_action("do_look","look");
  add_action("do_yes","yes");
  ::init();

  if( interactive(ob = this_player()) && visible(ob) && userp(ob) ){
    if (ob->query("gender") == "女性" && ( ob->query_per()>30 || ob->query("age")<query("age") ) )
    command("18mo "+ob->query("id"));
    else command("look "+ob->query("id"));
  }
}
int do_yes(string arg)
{
  object me = this_player();
  if(!me->query_temp("quest/baibian_ok")) return 0;

  me->delete_temp("quest/baibian_ok");
  me->delete_temp("quest/baibian");

  message_vision("$N想了想还是算了吧。\n",me);
  command("hehe");
  message_vision("$N乘$n不留神把地上的"HIG"银票"NOR"捡了起来塞在自己怀里。\n",this_object(),me);
  message_vision(CYN"$N漫不经心的将一些【神行百变】的诀窍传授给了$n。\n"NOR,this_object(),me);
  me->set("quest/baibian",1);
  log_file("quest/baibian",sprintf("%-8s(%-8s) 成功从韦小宝那里领会神行百变。",me->query("name"),getuid(me)),this_object() );
  //set
  command("look "+getuid(me));
  if(random(me->query("pur"))<10
  && random(me->query("kar"))>10
  && random(me->query("combat_exp"))>200000
  && me->query("gender")!="无性"
  )
  {
    string str ="兄弟";
    if(me->query("gender")=="女性")
    {
      str = "姐弟";
      if(me->query("age") <= query("age")) str ="兄妹";
    }
    command("say 这位"+RANK_D->query_respect(me)+"我们二人比如拜把子做了结义"+str+"，此后咱们二人有福共享，有难同当。不愿同年同月同日生，但愿同月同月同日死。");
    message_vision("$N一把拉过$n摆起红烛，向外跪拜，结为"+str+"。\n",this_object(),me);
    command("say 以后你来光顾我的兵器生意，我就给你优惠。");
    command("laughproud "+getuid(me));
    me->set("quest/wxbjiebai",1);
    log_file("quest/baibian",sprintf("%-8s(%-8s) 成功与韦小宝结拜。Exp:%d,Pur:%d,Kar:%d",me->query("name"),getuid(me),me->query("combat_exp"),me->query("pur"),me->query("kar")),this_object() );
  }
  else command("sigh");
  return 1;
}
///wei 不死

string ask_fight()
{
  object ob = this_player();
  int i = random(5);
  if(!environment(ob)->query("no_fight") || !ob->query_temp("quest/baibian")) return 0;
  if(ob->query_temp("quest/baibian_ok")) return 0;
  while(i--)
  random_move();
  return 0;
}

void kill_ob(object ob)
{
  if(!ob) return;
  if(ob->query("quest/wxbjiebai"))
  {
    tell_object(ob,"他可是跟你拜过把子的哦。\n");
    ob->remove_killer(this_object());
    remove_killer(ob);
    return;
  }
  if(ob->query_temp("quest/baibian_ok"))
  {
    tell_object(ob,"还是先把话说清楚吧。\n");
    ob->remove_killer(this_object());
    remove_killer(ob);
    return;
  }
  if(ob->query_temp("quest/baibian")) switch(ob->query_temp("quest/baibian"))
  {
    case 1:
    case 2:
    case 3:
    command("!!!");
    command("baibian");
    ob->add_temp("quest/baibian",1);
    return;
    case 4:
    case 5:
    case 6:
    command("dao");
    command("say 又是你！");
    command("baibian");
    ob->add_temp("quest/baibian",1);
    return;
    case 7:
    case 8:
    case 9:
    command("faint2");
    command("say 我跟贵教只是有点小过节，何必。。。");
    command("baibian");
    ob->add_temp("quest/baibian",1);
    return;
    default:
    ob->remove_killer(this_object());
    remove_killer(ob);
    ob->set_temp("quest/baibian_ok",1);
    message_vision("$N眼见无路可逃，急中生智从怀里掏出一大把"HIG"银票"NOR"撒了开来。\n$n一楞攻势就缓了下来。\n",this_object(),ob);
    command("say 其实我跟贵教的过节也没什么，只是洪老乌龟年纪大了有点想不明白。");
    command("say 这样好了，你这便放过我，我传你【神行百变】如何？");
    tell_object(ob,HIY"你愿意答应韦小宝的条件么(yes)？\n"NOR);
    return;
  }
  ob->remove_killer(this_object());
  remove_killer(ob);
  command("swear");
}

void unconcious()
{
  remove_all_killer();
  remove_all_enemy();
  say("韦小宝大叫一声: 妈呀!救命。\n");
  reincarnate();
  set("eff_qi", query("max_qi"));
  set("qi", query("max_qi"));
  set("eff_jing", query("max_jing"));
  set("jing", query("max_jing"));
  set("jingli", query("eff_jingli"));
  clear_conditions_by_type("poison");
}

void die()
{
  unconcious();
}

//武器系统

void start_weapon()
{
  remove_call_out("check_time");
  call_out("check_time",5);
}

void check_time()
{
  if (!this_object()) return;

  if ( query("born_weapon_time") && query("born_weapon_time") > time() )
  {
    call_out("check_time",5);
    return;
  }

  born_weapon();
}
void born_weapon()
{
  int i;
  mapping *weapons;
  string name;
  int number;

  weapons = allocate(WEAPON_TOTAL);
  delete("vendor_goods");

  i = WEAPON_LESS + random(WEAPON_TOTAL-WEAPON_LESS + 1);
  while(i--){
    name =  WEAPON_P(i);
    number = 1 + random(WEAPON_NUMBER-1);
    call_out("born_file",random(10),i);
    weapons[i] = (["name":name,"number":number]);
  }
  weapons = filter_array(weapons, (: mapp :));
  set("vendor_goods",weapons);
  set("born_weapon_time",time() + UPDATE_TIME * 1800 );
  command("rumor "HIM"听说韦小宝从韦兰铁匠那里得到了很多"HIY"武器"HIM"！"NOR);
  remove_call_out("check_time");
  call_out("check_time",10);
  return;
}
int born_file(int i)
{
  string name;
  name =  WEAPON_P(i);
  if (!"/adm/daemons/user_weapond"->born_file(name)) return 1;
  //message("wizard:linux", "born_file......."+i+"\n", users());
  return 1;
}
///added for VIP
/*int do_buy(string arg){
object me = this_player();

if ( !arg) return 0;
if ( me->query("registered") == 3 ) return ::do_buy(arg);

if ( me->query("wei_buy_weapon_times") > 10 ) {
message_vision("$n对$N摇了摇头说道：不卖了，不卖了！\n",me,  this_object());
return 1;
}

if (::do_buy(arg)==1)
{
if (!me->query("wei_buy_weapon_times")) me->set("wei_buy_weapon_times",1);
else me->add("wei_buy_weapon_times",1);
}
else
return 0;

return 1;
}
*/
int do_look(string arg)
{
  object me = this_player();
  object wq;
  mapping *mp;
  int i,j;
  string *name;

  name = allocate(WEAPON_TOTAL);

  if ( !arg) return 0;
  //if ( me->query("registered") != 3 ) return 0;
  if (!arrayp(mp = query("vendor_goods"))) return 0;

  mp = filter_array(mp , (: mapp :));
  for(i=0;i<sizeof(mp);i++) name[i] = ((object)mp[i]["name"])->query("id");
  name = filter_array(name, (: stringp :));

  j = member_array(arg,name);
  if (j == -1 ) return 0;

  if (mp[j]["number"] <= 0 ) return 0;//卖光了就不让看 :)

  if (!objectp(wq = new(mp[j]["name"]))) return 0;

  message_vision("$N跟$n私语几句，$n仔细想了想后就从包裹里拿出一"+wq->query("unit")+wq->name()+"交给了$N。\n",me,  this_object());
  tell_object(me,"\n你开始仔细琢磨这"+wq->query("unit") + wq->name()+"。\n\n"+wq->query("long")+"\n");
  message_vision("$n一下子从$N手里拿过"+wq->name()+"塞回自己的包裹里。\n",me,  this_object());
  me->start_busy(1);
  destruct(wq);
  if (wq)
  command("tell linux bug bug bug");
  return 1;
}