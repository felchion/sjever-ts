// cc.c

#include <ansi.h>

int main(object me, string arg)
{
        if (!arg) 
            return notify_fail("你想说什么！\n");;
        
        if (me->query("registered")<4)
        		return notify_fail("你的会员等级不够，无法使用该命令！\n");        
       		 
      //  arg = replace_string(arg, "$ESC$", ESC);
      //  arg = replace_string(arg, "$CSI$", CSI);
        arg = replace_string(arg, "$RED$", RED);
        arg = replace_string(arg, "$GRN$", GRN);
        arg = replace_string(arg, "$YEL$", YEL);
        arg = replace_string(arg, "$BLU$", BLU);
        arg = replace_string(arg, "$MAG$", MAG); 
        arg = replace_string(arg, "$CYN$", CYN);
        arg = replace_string(arg, "$WHT$", WHT);
        arg = replace_string(arg, "$HIR$", HIR);
        arg = replace_string(arg, "$HIG$", HIG);
        arg = replace_string(arg, "$HIY$", HIY);
        arg = replace_string(arg, "$HIB$", HIB);
      //  arg = replace_string(arg, "$HIM$", HIM);
        arg = replace_string(arg, "$HIC$", HIC);
        arg = replace_string(arg, "$HIW$", HIW);
        arg = replace_string(arg, "$NOR$", NOR);
     //   arg = replace_string(arg, "$BRED$", BRED);
     //   arg = replace_string(arg, "$BGRN$", BGRN);
     //   arg = replace_string(arg, "$BBLU$", BBLU);
     //   arg = replace_string(arg, "$BYEL$", BYEL);
     //   arg = replace_string(arg, "$BMAG$", BMAG);
     //   arg = replace_string(arg, "$BCYN$", BCYN);
     //   arg = replace_string(arg, "$BBLK$", BBLK);
     //   arg = replace_string(arg, "$HBRED$", HBRED);
     //  arg = replace_string(arg, "$HBGRN$", HBGRN);
     //   arg = replace_string(arg, "$HBBLU$", HBBLU);
     //   arg = replace_string(arg, "$HBYEL$", HBYEL);
     //   arg = replace_string(arg, "$HBMAG$", HBMAG);
     //   arg = replace_string(arg, "$HBCYN$", HBCYN);
     //   arg = replace_string(arg, "$HBWHT$", HBWHT);
     //   arg = replace_string(arg, "$BLINK$", BLINK);
      //  arg = replace_string(arg, "$BEEP$", "");
        arg += NOR;
        seteuid(getuid());
        return me->force_me("chat "+arg);
}

int help(object me)
{
        write(@HELP
指令格式 : cchat 信息
 
这个指令可以让你闲聊信息加上颜色。

HELP
"                     $NOR$ - 恢复正常颜色\n"
"$RED$ - "RED"红色"NOR"         $HIR$ - "HIR"亮红色\n"NOR
"$GRN$ - "GRN"绿色"NOR"         $HIG$ - "HIG"亮绿色\n"NOR
"$YEL$ - "YEL"土黄色"NOR"       $HIY$ - "HIY"黄色\n"NOR
"$BLU$ - "BLU"深蓝色"NOR"       $HIB$ - "HIB"蓝色\n"NOR
"$MAG$ - "MAG"浅紫色"NOR"       $HIM$ - "HIM"粉红色\n"NOR
"$CYN$ - "CYN"蓝绿色"NOR"       $HIC$ - "HIC"天青色\n"NOR
"$WHT$ - 浅灰色       $HIW$ - "HIW"白色\n"NOR
@HELP 

其中系统自动会在字串尾端加一个 $NOR$。
HELP
);
        return 1;
}
