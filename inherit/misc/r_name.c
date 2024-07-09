// named.c
// By snowman@SJ 10/06/2000

string* f_name = ({
"爱", "安",
"柏", "鲍", "贝", "卞", "卜",
"曹", "岑", "昌", "常", "陈", "成",
"呆", "戴", "邓", "狄", "第", "董", "窦",
"耳",
"樊", "范", "方", "费", "冯", "凤", "伏", "福", "付",
"葛", "缑", "顾", "关", "管",
"哈", "韩", "郝", "和", "何", "贺", "候", "后", "胡", "花", "华", "华", "黄", "霍",
"计", "纪", "姜", "蒋", "金", "昝",
"康", "亢", "柯", "孔", "蒯", "况",
"郎", "雷", "李", "廉", "梁", "林", "凌", "柳", "卢", "鲁", "吕", "罗",
"马", "茅", "毛", "米", "苗", "明", "墨", "牟", "穆",
"倪", "年",
"潘", "庞", "彭", "皮", "平",
"戚", "齐", "祁", "钱", "谯", "秦", "琴", "仇", "屈",
"任",
"傻", "商", "邵", "佘", "沈", "施", "石", "时", "史", "舒", "帅", "水", "宋", "苏", "孙",
"谈", "汤", "唐", "陶", "藤", "佟",
"万", "汪", "王", "韦", "魏", "卫", "吴", "五", "伍", "邬",
"奚", "项", "肖", "谢", "熊", "徐", "许", "薛",
"严", "言", "杨", "阳", "姚", "益", "殷", "尹", "英", "盈", "尤", "有", "虞", "余", "禹", "喻", "元", "袁", "岳", "乐", "云",
"藏", "湛", "章", "张", "赵", "郑", "支", "钟", "周", "朱", "祝", "宗", "邹", "褚", "祖",

"百里", "佰赏",
"楚晋", "淳于", "单于",
"东方", "东郭", "东门", "端木", "段千",
"公良", "公孙", "公西", "公羊", "公冶", "谷梁", "归海",
"赫连", "呼延", "皇甫",
"夹谷",
"梁丘", "令狐",
"慕容",
"南宫", "南郭",
"欧阳",
"濮阳",
"漆雕",
"汝鄢",
"上官", "申屠", "师空", "司马", "司徒",
"太叔", "澹台", "涂钦", "拓趾",
"微生", "闻人", "巫马",
"西门", "夏候", "骧驷", "辕轩",
"阎法", "羊舌", "尉迟", "宇文", "乐正",
"宰父", "长孙", "钟离", "仲孙", "诸葛", "颛孔", "宗政", "左丘",
});

string* f_id = ({
"ai", "an",
"bo", "bao", "bei", "bian", "bo",
"cao", "cen", "chang", "chang", "chen", "cheng",
"dai", "dai", "deng", "di", "di", "dong", "dou",
"er",
"fan", "fan", "fang", "fei", "feng", "feng", "fu", "fu", "fu",
"ge", "gou", "gu", "guan", "guan",
"ha", "han", "hao", "he", "he", "he", "hou", "hou", "hu", "hua", "hua", "hua", "huang", "huo",
"ji", "ji", "jiang", "jiang", "jin", "jiu",
"kang", "kang", "ke", "kong", "kuai", "kuang",
"lang", "lei", "li", "lian", "liang", "lin", "ling", "liu", "lu", "lu", "lv", "luo",
"ma", "mao", "mao", "mi", "miao", "ming", "mo", "mou", "mu",
"ni", "nian",
"pan", "pang", "peng", "pi", "ping",
"qi", "qi", "qi", "qian", "qiao", "qin", "qin", "qiu", "qu",
"ren",
"sha", "shang", "shao", "she", "shen", "shi", "shi", "shi", "shi", "shu", "shuai", "shui", "song", "su", "sun",
"tan", "tang", "tang", "tao", "teng", "tong",
"wan", "wang", "wang", "wei", "wei", "wei", "wu", "wu", "wu", "wu",
"xi", "xiang", "xiao", "xie", "xiong", "xu", "xu", "xue",
"yan", "yan", "yang", "yang", "yao", "yi", "yin", "yin", "ying", "ying", "you", "you", "yu", "yu", "yu", "yu", "yuan", "yuan", "yue", "yue", "yun",
"zang", "zhan", "zhang", "zhang", "zhao", "zheng", "zhi", "zhong", "zhou", "zhu", "zhu", "zong", "zhou", "zhu", "zu",

"baili", "baishang",
"chujin", "chunyu", "chanyu",
"dongfang", "dongguo", "dongmen", "duanmu", "duanqian",
"gongliang", "gongsun", "gongxi", "gongyang", "gongye", "guliang", "guihai",
"helian", "huyan", "huangpu",
"jiagu",
"liangqiu", "linghu",
"murong",
"nangong", "nanguo",
"ouyang",
"puyang",
"qidiao",
"ruyan",
"shangguan", "shentu", "shikong", "sima", "situ",
"taishu", "tantai", "tuqin", "tuozhi",
"weisheng", "wenren", "wuma",
"ximen", "xiahou", "xiangsi", "xuanyuan",
"yanfa", "yangshe", "yuchi", "yuwen", "yuezheng",
"zaifu", "zhangsun", "zhongli", "zhongsun", "zhuge", "zhuankong", "zongzheng", "zuoqiu",
});

string* name_m = ({
"安",
"霸", "白", "班", "碧", "斌", "宾", "波",
"才", "财", "昌", "长", "超", "诚", "川",
"大", "德华", "鼎", "定", "栋", "斗", "多",
"发", "法", "飞", "锋", "风", "夫", "福", "福", "富",
"钢", "罡", "高", "公", "贯", "光", "国",
"海", "浩", "赫", "宏", "虎", "华", "辉",
"济", "戟", "坚", "键", "健", "剑", "江", "杰", "金", "金", "进", "靖", "峻", "俊",
"康", "可", "克", "匡",
"琅", "礼", "良", "良", "麟", "令", "龙", "隆", "禄",
"满", "民", "明", "鸣",
"宁",
"培", "沛",
"奇", "启", "强", "庆",
"仁", "人", "荣", "若",
"山", "盛", "胜", "时", "世", "寿", "叔", "顺",
"泰", "涛", "添", "铁", "挺", "同",
"万", "为", "伟", "文", "武",
"熙", "羲", "宪", "祥", "祥", "晓", "小", "孝", "雄", "雄", "旭", "学友",
"杨", "亿", "毅", "义", "益", "永", "勇", "友", "宇", "羽", "瑜", "岳",
"振", "峥", "之", "志", "智", "忠", "舟", "宙", "子", "自",
});

string* id_m = ({
"an",
"ba", "bai", "ban", "bi", "bin", "bin", "bo",
"cai", "cai", "chang", "chang", "chao", "cheng", "chuan",
"da", "dehua", "ding", "ding", "dong", "dou", "duo",
"fa", "fa", "fei", "feng", "feng", "fu", "fu", "fu", "fu",
"gang", "gang", "gao", "gong", "guan", "guang", "guo",
"hai", "hao", "he", "hong", "hu", "hua", "hui",
"ji", "ji", "jian", "jian", "jian", "jian", "jiang", "jie", "jin", "jin", "jin", "jing", "jun", "jun",
"kang", "ke", "ke", "kuang",
"lang", "li", "liang", "liang", "lin", "ling", "long", "long", "lu",
"man", "min", "ming", "ming",
"ning",
"pei", "pei",
"qi", "qi", "qiang", "qing",
"ren", "ren", "rong", "ruo",
"shan", "sheng", "sheng", "shi", "shi", "shou", "shu", "shun",
"tai", "tao", "tian", "tie", "ting", "tong",
"wan", "wei", "wei", "wen", "wu",
"xi", "xi", "xian", "xiang", "xiang", "xiao", "xiao", "xiao", "xiong", "xiong", "xu", "xueyou",
"yang", "yi", "yi", "yi", "yi", "yong", "yong", "you", "yu", "yu", "yu", "yue",
"zhen", "zheng", "zhi", "zhi", "zhi", "zhong", "zhou", "zhou", "zi", "zi",
});

string* name_g = ({     //女性专用
"爱",
"宝钗", "碧",
"春", "翠",
"黛", "黛玉", "丹",
"芳", "芬", "凤", "芙",
"姑",
"虹", "红", "花", "惠", "慧",
"娇", "晶", "静", "菊", "娟",
"珂",
"兰", "岚", "莉", "丽", "莲", "琳", "玲",
"美", "敏",
"萍",
"倩", "琴", "芹", "青", "箐",
"蓉",
"姗", "淑",
"婷",
"婉",
"霞", "贤", "香", "湘云",
"嫣然", "艳", "燕", "英", "瑛", "荧", "盈", "颖", "玉", "媛",
"真", "珍", "珠",
});

string* id_g = ({
"ai",
"baochai", "bi",
"chun", "cui",
"dai", "daiyu", "dan",
"fang", "fen", "feng", "fu",
"gu",
"hong", "hong", "hua", "hui", "hui",
"jiao", "jing", "jing", "ju", "juan",
"ke",
"lan", "lan", "li", "li", "lian", "lin", "ling",
"mei", "min",
"ping",
"qian", "qin", "qin", "qing", "qing",
"rong",
"shan", "shu",
"ting",
"wan",
"xia", "xian", "xiang", "xiangyun",
"yanran", "yan", "yan", "ying", "ying", "ying", "ying", "ying", "yu", "yuan",
"zhen", "zhen", "zhu",
});

void create()
{
	seteuid(getuid());
	if (sizeof(f_name) != sizeof(f_id))
		write("姓氏有错误。\n");
	if (sizeof(name_m) != sizeof(id_m))
		write("男名有错误。\n");
	if (sizeof(name_g) != sizeof(id_g))
		write("女名有错误。\n");
}

mapping get_random_name(int gender)
{
	string f_n, f_i, name_1, id_1, *ids;
	int i;

	i = random(sizeof(f_name));
	f_n = f_name[i];
	f_i = f_id[i];

	if (gender) {	// male
		i = random(sizeof(name_m));
		name_1 = name_m[i];
		id_1 = id_m[i];
		if ( random(10) > 6 ){
			i = random(sizeof(name_m));
			if (strwidth(name_1+name_m[i]) <= 4) {
				name_1 += name_m[i];
				id_1 += id_m[i];
			}
		}
	} else {	// female
		i = random(sizeof(name_g));
		name_1 = name_g[i];
		id_1 = id_g[i];
		if (random(10) > 6) {
			i = random(sizeof(name_g));
			if (strwidth(name_1+name_g[i]) <= 4) {
				name_1 += name_g[i];
				id_1 += id_g[i];
			}
		}
	}
	ids = ({ f_i+" "+id_1 });
	switch (random(4)) {
		case 0:
			ids += ({ f_i });
			break;
		case 1:
			ids += ({ id_1 });
			break;
		case 2:
			ids += ({ f_i, id_1 });
	}
	return ([
		"name" : f_n + name_1,
		"id" : ids,
	]);
}
