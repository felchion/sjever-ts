#include <ansi.h>

int main()
{
        object *ob,me;
        string msg;
        int i;
        
        
        ob=objects();
        ob=filter_array(ob,(: environment($1) && ($1->query("rigidity")>5 || $1->query("sharpness")>5) :));
        
        msg=HIC"\n【"+MUD_NAME+"武器列表】\n"NOR;
        msg+=HIB"▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂\n"NOR;
        i=sizeof(ob);
        while(i--) {
                msg+=sprintf("%-20s %10s 硬度：%-5d 锐利度：%-5d\n",
                        ob[i]->query("name")+"("+ob[i]->query("id")+")",
                        environment(ob[i])->short(),
                        ob[i]->query("rigidity"),
                        ob[i]->query("sharpness"),
                        );
                }
                
        msg+=HIB"▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂\n"NOR;

        this_player()->start_more(msg);
        return 1;
}