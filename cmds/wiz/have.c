

#include <ansi.h>

inherit F_DBASE;
int main()
{
        object *obj;
        int i,a;
        string msg,where,who,out;
        
        msg=HIC"\n【"+MUD_NAME+"宝物列表】\n"NOR;
        msg+=HIB"▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂\n"NOR;
        
           obj=objects((: !living($1) && environment($1) :));
          obj=filter_array(obj,(: ($1->query("treasure") || $1->query("unique") ):));
        i=sizeof(obj);
        
        while (i--) {
                 if( living(environment(obj[i]) ) ) {
                                        who=environment(obj[i])->query("name")+"("+environment(obj[i])->query("id")+")"+NOR"手中" ;
                                        a=1;
                                }
                                else {
                                        where=environment(obj[i])->query("short")+NOR"："+file_name(environment(obj[i]));                      
                                        a=2;
                                }
                                a==1?out=who:out=where;
                            
                                
      
                                
                                msg+=sprintf("%-20s"+HIW+"    此物正在"+HIY"%10s \n"NOR,
                                                        obj[i]->query("name")+"("+obj[i]->query("id")+")",
                                                        out,
                                                        
                                        );
                                }
                                
                
                msg+=HIB"▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂\n"NOR;
                
        this_player()->start_more(msg);
      msg="";
        return 1;
}


