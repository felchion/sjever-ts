#include <ansi.h>
#include <mudlib.h>
  
#define DB_DOBET_NAME "dobet_main"
#define DB_ODDS_NAME "dobet_odds"
#define DB_CHIP_NAME "dobet_chip"

void end_input_bet(string valcode,string valcontext, string valsetup_id, int valbetmax, 
		int valbegin_time, int valstop_time, int valend_time, int valdeposit, object ob);

int check_legal_code(string valcode)
{
	int i;

	i = strlen(valcode);

	if( i < 1 || i > 6 ) {
		write("对不起，赌局代码必须是 1 到 6 个英文字母或数字。\n");
		return 0;
	}

	while(i--)
		if( (valcode[i] < 'a' || valcode[i] > 'z') && (valcode[i] < '0' || valcode[i] > '9') ) {
			write("对不起，赌局代码只能用英文字母或数字。\n");
			return 0;
		}

	return 1;
}

int getsettime(string valstr)
{
	int iday,ihour,imin,isec;
	int inowtime = time();
	string ts;
	int ioldhour,ioldmin,ioldsec;
	
	ts = ctime(inowtime);
	ts = ts[10..15];
	sscanf( ts, "%d:%d:%d",ioldhour,ioldmin,ioldsec);

	sscanf( valstr, "%d %d:%d:%d",iday,ihour,imin,isec);
	if( iday <0 ) iday=0;
//	if( iday >30 ) iday=30;
	inowtime += iday*3600*24;

	if( ihour<0 ) ihour=0;
	if( ihour>23 ) ihour=23;
	inowtime += (ihour-ioldhour)*3600;

	if( imin<0 ) imin=0;
	if( imin>59 ) imin=59;
	inowtime += (imin-ioldmin)*60;

	if( isec<0 ) isec=0;
	if( isec>59 ) isec=59;
	inowtime += isec-ioldsec;
	return inowtime;
}
  
int main(object me, string arg)
{
	if ( !wizardp(me) && ( (!me->query("gamblers"))||(me->query("gamblers")!=1) ) )
		return notify_fail("你没有这个权限?\n");

	write("请先输入您为本次赌局设置的代码，注意如果与系统中现存的重复，将会覆盖原设置\n代码为：");
	input_to("get_code", 1, me);
	return 1;
}

private void get_code(string valcode, object ob)
{
	mixed ret;
	
	ret = DOBET_D->leasedb("select code,context,setup_id,flag,begin_time,stop_time,end_time,deposit,bet_result,remark,max_bet from "+DB_DOBET_NAME+" where U_Site='"+INTERMUD_MUD_NAME+"' AND code='"+valcode+"'");
	if(check_legal_code(valcode)==0){
		write("赌局代码为：");
		input_to("get_code", 1, ob);
	};
	
	if(!ret || sizeof(ret)<1){
		write("请输入赌局的内容，回车表示放弃：");
		input_to("get_context1", 1,valcode, ob );
	}else{
		write("代码为:"HIW+valcode+NOR"已经存在。\n原设定的赌局的内容："HIW+ret[0][1]+NOR"\n");
		write("原赌局的状态为："+DOBET_D->getflag(ret[0][3])+"\n");
		write("请输入赌局的新内容，回车表示不修改：");
		input_to("get_context2", 1,valcode,ret[0][1],ret[0][10],
			ret[0][4],ret[0][5],ret[0][6], ob );
	}
}

private void get_context1(string valcontext, string valcode, object ob)
{
	if( !stringp(valcontext)||(valcontext=="") ){
		write("你放弃了设置\n");
	}else{
		write("请输入赌局的允许一个id最大投注金额，以锭黄金为单位\n");
		write("回车表示放弃：");
		input_to("get_betmax1", 1,valcode, valcontext,ob );
	}
}

private void get_context2(string valcontext,string valcode, string oldvalcontext, int valbetmax, 
	int valbegin_time, int valstop_time, int valend_time, object ob)
{
	if( !stringp(valcontext)||(valcontext=="") )
		valcontext=oldvalcontext;

	write(sprintf("原赌局的最大投注金额是："HIW"%d锭黄金\n"NOR,valbetmax));
	write("请输入赌局的允许一个id最大投注金额，以锭黄金为单位\n");
	write("回车表示不修改：");
	input_to("get_betmax2", 1,valcode,valcontext,valbetmax,
			valbegin_time, valstop_time, valend_time , ob );
}

private void get_betmax1(string valbetmax, string valcode, string valcontext,object ob)
{
	int ibetmax;
	
	if( !stringp(valbetmax)||(valbetmax=="") ){
		write("你放弃了设置\n");
	}else{
		sscanf( valbetmax, "%d",ibetmax);
		if(ibetmax<=0){
			write("最大投注金额不能小于零\n");
			write("请输入赌局的允许一个id最大投注金额，以锭黄金为单位\n");
			write("回车表示放弃：");
			input_to("get_betmax1", 1,valcode, valcontext,ob );
		};
		write(sprintf("你输入的最高投注金额为："HIW"%d锭黄金\n"NOR,ibetmax));
		write("请输入赌局的允许开始投注时间，输入示例为 4 12:00:00,表示四天后的中午12点正\n");
		write("回车表示放弃：");
		input_to("get_begintime1", 1,valcode, valcontext,ibetmax,ob );
	}
}

private void get_betmax2(string valbetmax, string valcode, string valcontext, int valoldbetmax, 
	int valbegin_time, int valstop_time, int valend_time, object ob)
{
	int ibetmax;
	
	if( !stringp(valbetmax)||(valbetmax=="") )
		ibetmax=valoldbetmax;
	else{
		sscanf( valbetmax, "%d",ibetmax);
		if(ibetmax<=0){
			write("最大投注金额不能小于零\n");
			write("请输入赌局的允许一个id最大投注金额，以锭黄金为单位\n");
			write("回车表示放弃：");
			input_to("get_betmax2", 1,valcode,valcontext,
					valbegin_time, valstop_time, valend_time , ob );
		};
	};
	write(sprintf("你输入的最高投注金额为："HIW"%d锭黄金\n"NOR,ibetmax));
	write("原赌局的开始投注时间是："HIW+DOBET_D->longtime(valbegin_time)+NOR"\n");
	write("请输入赌局的允许开始投注时间，输入示例为 4 12:00:00,表示四天后的中午12点正\n");
	write("回车表示不修改：");
	input_to("get_begintime2", 1,valcode,valcontext,ibetmax,
			valbegin_time, valstop_time, valend_time , ob );
}

private void get_begintime1(string valbegin_time,string valcode, string valcontext, int valbetmax, object ob)
{
	int ibegin_time;
	
	if( !stringp(valbegin_time) ){
		write("你放弃了设置\n");
	}else{
		ibegin_time=getsettime(valbegin_time);
		write("你设置的开始投注时间是："HIW+DOBET_D->longtime(ibegin_time)+NOR"\n");
		write("请输入赌局的投注截止时间，输入示例为 4 12:00:00,表示四天后的中午12点正\n");
		write("回车表示放弃：");
		input_to("get_stoptime1", 1,valcode,valcontext,valbetmax,ibegin_time, ob );
	}
}

private void get_begintime2(string valbegin_time,string valcode, string valcontext, int valbetmax, 
	int oldvalbegin_time, int valstop_time, int valend_time, object ob)
{
	int ibegin_time;
	
	if( !stringp(valbegin_time)||(valbegin_time=="") )
		ibegin_time=oldvalbegin_time;
	else
		ibegin_time=getsettime(valbegin_time);

	write("你设置的开始投注时间是："HIW+DOBET_D->longtime(ibegin_time)+NOR"\n");
	write("原赌局的投注截止时间是："HIW+DOBET_D->longtime(valstop_time)+NOR"\n");
	write("请输入赌局的投注截止时间，输入示例为 4 12:00:00,表示四天后的中午12点正\n");
	write("回车表示不修改：");
	input_to("get_stoptime2", 1,valcode,valcontext,valbetmax,
			ibegin_time, valstop_time, valend_time, ob );
}

private void get_stoptime1(string valstop_time,string valcode, string valcontext,  int valbetmax, 
		int ibegin_time, object ob)
{
	int istop_time;

	if( !stringp(valstop_time)||(valstop_time=="") ){
		write("你放弃了设置\n");
	}else{
		istop_time=getsettime(valstop_time);
		write("你设置的投注截止时间是："HIW+DOBET_D->longtime(istop_time)+NOR"\n");
		write("请输入赌局的派彩时间，输入示例为 4 12:00:00,表示四天后的中午12点正\n");
		write("回车表示放弃：");
		input_to("get_endtime1", 1,valcode,valcontext,valbetmax,ibegin_time,istop_time, ob );
	}
}

private void get_stoptime2(string valstop_time,string valcode, string valcontext,  int valbetmax, 
	int valbegin_time, int oldvalstop_time, int valend_time,object ob)
{
	int istop_time;
	
	if( !stringp(valstop_time)||(valstop_time=="") )
		istop_time=oldvalstop_time;
	else
		istop_time=getsettime(valstop_time);
	
	write("你设置的投注截止时间是："HIW+DOBET_D->longtime(istop_time)+NOR"\n");
	write("原赌局的派彩时间是："HIW+DOBET_D->longtime(valend_time)+NOR"\n");
	write("请输入赌局的派彩时间，输入示例为 4 12:00:00,表示四天后的中午12点正\n");
	write("回车表示不修改：");
	input_to("get_endtime2", 1,valcode,valcontext,valbetmax,
			valbegin_time, istop_time, valend_time, ob );
}

private void get_endtime1(string valend_time,string valcode, string valcontext,  int valbetmax, 
		int ibegin_time, int istop_time, object ob)
{
	int iend_time;
	
	if( !stringp(valend_time)||(valend_time=="") ){
		write("你放弃了设置\n");
	}else{
		iend_time=getsettime(valend_time);
		write("你设置的派彩时间是："HIW+DOBET_D->longtime(iend_time)+NOR"\n");

		if( wizardp(ob)||( (ob->query("gamblers"))&&(ob->query("gamblers")==1) ) ){
			end_input_bet(valcode,valcontext, ob->query("id"),valbetmax,
				ibegin_time, istop_time, iend_time, 0,ob);
		}else{
			write("请输入你为组织这次赌局计划支付的押金，以锭黄金为单位，回车表示放弃：");
			input_to("get_deposit", 1,valcode,valcontext,valbetmax,ibegin_time,istop_time,iend_time, ob );
		};
	}
}

private void get_endtime2(string valend_time, string valcode,string valcontext, int valbetmax, 
	int valbegin_time, int valstop_time, int oldvalend_time,object ob)
{
	int iend_time;
	
	if( !stringp(valend_time)||(valend_time=="") )
		iend_time=oldvalend_time;
	else
		iend_time=getsettime(valend_time);

	write("你设置的派彩时间是："HIW+DOBET_D->longtime(iend_time)+NOR"\n");

	end_input_bet(valcode,valcontext, "*",valbetmax,
		valbegin_time, valstop_time, iend_time, 0,ob);
}

private void get_deposit(string valgold,string valcode, string valcontext,  int valbetmax, 
		int ibegin_time, int istop_time, int iend_time, object ob)
{
	int igold;
	
	if( !stringp(valgold)||(valgold=="") ){
		write("你放弃了设置\n");
	}else{
		sscanf( valgold, "%d",igold);
		if(igold<100){
			write("你设置的要支付的押金低于100锭黄金，请重新输入，回车表示放弃：");
			input_to("get_deposit", 1,valcode,valcontext,valbetmax,ibegin_time,istop_time,iend_time, ob );
		};
		write(sprintf("你设置的要支付的押金是：%d锭黄金\n",igold));

		end_input_bet(valcode,valcontext, ob->query("id"),valbetmax,
				ibegin_time, istop_time, iend_time, igold,ob);
	}
}

void end_input_bet(string valcode,string valcontext, string valsetup_id, int valbetmax, 
		int valbegin_time, int valstop_time, int valend_time, int valdeposit, object ob)
{
	if( valsetup_id!="*" ){
		write("你本次赌局的设置如下\n");
		write(sprintf("代码："HIW"%s"NOR"\n",valcode));
		write(sprintf("内容："HIW"%s"NOR"\n",valcontext));
		write(sprintf("最高投注金："HIW"%d锭黄金"NOR"\n",valbetmax));
		write(sprintf("开始投注时间："HIW"%s"NOR"\n",DOBET_D->longtime(valbegin_time)));
		write(sprintf("投注截止时间："HIW"%s"NOR"\n",DOBET_D->longtime(valstop_time)));
		write(sprintf("派彩时间："HIW"%s"NOR"\n",DOBET_D->longtime(valend_time)));
		write(sprintf("押金："HIW"%d锭黄金"NOR"\n",valdeposit));
		write("确认请输入y，否则取消本次设置：");
	}else{
		write("你对本次赌局做了如修改\n");
		write(sprintf("内容："HIW"%s"NOR"\n",valcontext));
		write(sprintf("最高投注金："HIW"%d锭黄金"NOR"\n",valbetmax));
		write(sprintf("开始投注时间："HIW"%s"NOR"\n",DOBET_D->longtime(valbegin_time)));
		write(sprintf("投注截止时间："HIW"%s"NOR"\n",DOBET_D->longtime(valstop_time)));
		write(sprintf("派彩时间："HIW"%s"NOR"\n",DOBET_D->longtime(valend_time)));
		write("确认请输入y，否则取消本次设置：");
	};
	input_to("bet_set_yes_no", 1,valcode,valcontext, valsetup_id,valbetmax,
		valbegin_time, valstop_time, valend_time, valdeposit,ob);
}

private void bet_set_yes_no(string yes_no,string valcode,string valcontext, string valsetup_id, int valbetmax, 
		int valbegin_time, int valstop_time, int valend_time, int valdeposit,object me)
{
	if( (yes_no !="y") && (yes_no !="Y") ){
		write("你放弃了设置\n");
		return;
	};
	if( DOBET_D->set_bet(valcode,valcontext, valsetup_id,valbetmax,
		valbegin_time, valstop_time, valend_time, valdeposit) != 1)
		write("本次设置失败\n");
	else{
		write("本次设置成功\n");
		write("请使用"HIW"betaddodds "+valcode+" <赌注代码> <赔率说明> <赔率> "NOR"来添加或修改本次赌局的赔率\n");
		log_file("betlog",sprintf("%s%s赌局%s内容%s 最高投注%d %s %s %s 押金%i ",
			me->query("id"),
			((valsetup_id=="*")?"修改":"创建"),
			valcode,valcontext,valbetmax,
			DOBET_D->longtime(valbegin_time),
			DOBET_D->longtime(valstop_time),
			DOBET_D->longtime(valend_time),
			valdeposit));
	};
}
int help(object me)
{
  write(@HELP
--------------------------------------------------
指令格式：betadd
--------------------------------------------------
 betadd 设置赌局或修改
 修改以前的赌局，只能修改
赌局内容：如谁对谁
起始时刻
截止时刻
派彩时刻,也是本赌局的结束时间
--------------------------------------------------

HELP
    );
    return 1;
}
