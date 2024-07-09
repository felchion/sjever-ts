// put.c

inherit F_CLEAN_UP;

int do_put(object me, object obj, object dest);

void create() { seteuid(getuid()); }

int main(object me, string arg)
{
        string target, item;
        object obj, dest, *inv, obj2;
        int i, amount;

        if(!arg) return notify_fail("你要将什么东西放进哪里？\n");

        if( sscanf(arg, "%s in %s", item, target)!=2 )
                return notify_fail("你要给谁什么东西？\n");

        dest = present(target, me);
        if( !dest || living(dest)) dest = present(target, environment(me));
        if( !dest || living(dest))
                return notify_fail("这里没有这样东西。\n");
        if(dest->is_character())
                return notify_fail("你不能这样做！\n");
        if(dest->query("no_put"))
                return notify_fail("你不能把东西放那里！\n");
        if(dest->query("no_unique"))
                return notify_fail("你不能把宝物放在那里面！\n");
        if( !dest->query_max_encumbrance() )  
                return notify_fail("那不是容器。\n");

                       
        if(sscanf(item, "%d %s", amount, item)==2) {
                if( !objectp(obj = present(item, me)) )
                    return notify_fail("你身上没有这样东西。\n");
                if (base_name(obj)[0..12]=="/clone/money/" )
				  					return notify_fail("你不能把钱放到那里面。\n");				  									  
                if( !obj->query_amount() )
                        return notify_fail( obj->name() + "不能被分开。\n");                
                if( amount < 1 )
                        return notify_fail("东西的数量至少是一个。\n");
                if( amount > obj->query_amount() )
                        return notify_fail("你没有那么多的" + obj->name() + "。\n");
                if( obj->query("weapon_prop") && me->is_exert() &&
                        obj == me->query_temp("weapon"))
                               return notify_fail("你正在使用"+me->query_exert()+"，无法给出"+obj->name()+"。\n");
                if( obj->query("weapon_prop") && me->is_perform() &&
                        obj == me->query_temp("weapon"))
                               return notify_fail("你正在使用"+me->query_perform()+"，无法给出"+obj->name()+"。\n");
                 
                else if( amount == (int)obj->query_amount() )
                        return do_put(me, obj, dest);
                else {
                        obj->set_amount( (int)obj->query_amount() - amount );
                        if( obj->query("unique") ) {
                    		write("突然"+to_chinese(amount)+obj->query("unit")+obj->name()+"一闪，竟然失去了踪影！\n");
                    		return 0;
                	}
                        obj2 = new(base_name(obj));
                        obj2->set_amount(amount);
                        obj2->set("broken", obj->query("broken"));
                        if (do_put(me, obj2, dest)) return 1;
                        obj2->move(me);
                        return 0;
                }
        }

        if(item=="all") {
                inv = all_inventory(me);
                for(i=0; i<sizeof(inv); i++){
                        if(inv[i] == me->query_temp("weapon") && 
                          (me->query_exert() || me->query_perform())) continue;
                        if( inv[i] != dest ) do_put(me, inv[i], dest);
                        }
                write("Ok.\n");
                return 1;
        }
        if( !objectp(obj = present(item, me)) )
                return notify_fail("你身上没有这样东西。\n");
        if( !wizardp(me) && obj->query("no_drop") ) {
                if (stringp(obj->query("no_drop")))
                        return notify_fail(obj->query("no_drop"));
                return notify_fail("这个东西不能离开你本身。\n");
        }
        if (!wizardp(me) && obj->query("embedded"))
                 return notify_fail("这个东西现在拿不下来。\n");
        if( obj->query("weapon_prop") && me->is_exert() &&
            obj == me->query_temp("weapon"))
                 return notify_fail("你正在使用"+me->query_exert()+"，无法给出"+obj->name()+"。\n");
        if( obj->query("weapon_prop") && me->is_perform() &&
            obj == me->query_temp("weapon"))
                 return notify_fail("你正在使用"+me->query_perform()+"，无法给出"+obj->name()+"。\n");
                 
        return do_put(me, obj, dest);
}

int do_put(object me, object obj, object dest)
{
        if(obj->query("invisible") && !wizardp(me)) return 0;
        if (!wizardp(me) && obj->query("no_drop"))
                return notify_fail(stringp(obj->query("no_drop"))?
                        obj->query("no_drop"):"这样东西无法离开你。\n");
        if (!wizardp(me) && obj->query("embedded"))
                 return notify_fail("这个东西现在拿不下来。\n");
        
        if (base_name(obj)[0..12]=="/clone/money/" )
				  					return notify_fail("你不能把钱放到那里面。\n");	

        if (obj == dest) return notify_fail("把你放进你的身体？\n");
        if( obj->query("unique") ) {
        	message_vision("只见一"+obj->query("unit")+"$n脱出$N手掌时一闪，竟然失去了踪影！\n", me, obj);
        	destruct(obj);
                return 0;
        }
        if ( obj->move(dest) ) {
                message_vision( sprintf("$N将一%s%s放进%s。\n",
                        obj->query("unit"), obj->name(), dest->name()), me );
                return 1;
        }
        return 0;
}

int help(object me)
{
write(@HELP
指令格式 : put <物品名称> in <某容器>
 
这个指令可以让你将某样物品放进一个容器，当然，首先你要拥有这样物品。
 
HELP
    );
    return 1;
}
