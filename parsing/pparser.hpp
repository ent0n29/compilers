#define EPS 0
#define BASE_VARIABLES 256

const int numexpr = 256;
const int expr = 257;
const int parexpr = 258;
const int idexpr = 259;
const int exprlist = 260;
const int primary = 261;
const int binop = 262;
const int binoprhs = 263;
const int idseq = 264;
const int proto = 265;
const int def = 266;
const int external = 267;
const int top = 268;
const int program = 269;
const int AXIOM = 269;

std::vector<int> prod1{-2};	// numexpr : 'number'
std::vector<int> prod2{40,257,41};	// parexpr : '(' expr ')'
std::vector<int> prod3{-3};	// idexpr : 'id'
std::vector<int> prod4{-4,40,260,41};	// idexpr : 'funid' '(' exprlist ')'
std::vector<int> prod5{257,260};	// exprlist : expr exprlist
std::vector<int> prod6{0};	// exprlist : EPS
std::vector<int> prod7{259};	// primary : idexpr
std::vector<int> prod8{256};	// primary : numexpr
std::vector<int> prod9{258};	// primary : parexpr
std::vector<int> prod10{262,261,263};	// binoprhs : binop primary binoprhs
std::vector<int> prod11{0};	// binoprhs : EPS
std::vector<int> prod12{43};	// binop : '+'
std::vector<int> prod13{42};	// binop : '*'
std::vector<int> prod14{261,263};	// expr : primary binoprhs
std::vector<int> prod15{-4,40,264,41};	// proto : 'funid' '(' idseq ')'
std::vector<int> prod16{-3,264};	// idseq : 'id' idseq
std::vector<int> prod17{0};	// idseq : EPS
std::vector<int> prod18{-5,265,257};	// def : 'def' proto expr
std::vector<int> prod19{-6,265};	// external : 'extern' proto
std::vector<int> prod20{266};	// top : def
std::vector<int> prod21{267};	// top : external
std::vector<int> prod22{257};	// top : expr
std::vector<int> prod23{0};	// top : EPS
std::vector<int> prod24{268,59,269};	// program : top ';' program
std::vector<int> prod25{0};	// program : EPS

std::map<int,std::vector<int>> prods = {
	{1,{prod1}},
	{2,{prod2}},
	{3,{prod3}},
	{4,{prod4}},
	{5,{prod5}},
	{6,{prod6}},
	{7,{prod7}},
	{8,{prod8}},
	{9,{prod9}},
	{10,{prod10}},
	{11,{prod11}},
	{12,{prod12}},
	{13,{prod13}},
	{14,{prod14}},
	{15,{prod15}},
	{16,{prod16}},
	{17,{prod17}},
	{18,{prod18}},
	{19,{prod19}},
	{20,{prod20}},
	{21,{prod21}},
	{22,{prod22}},
	{23,{prod23}},
	{24,{prod24}},
	{25,{prod25}}
};
std::map<int,std::map<int,int>> parsing_table;
void set_parsing_table() {
	parsing_table[256]={{-2,1}};
	parsing_table[258]={{40,2}};
	parsing_table[259]={{-3,3}, {-4,4}};
	parsing_table[260]={{40,5}, {-4,5}, {-3,5}, {-2,5}, {41,6}};
	parsing_table[261]={{-4,7}, {-3,7}, {-2,8}, {40,9}};
	parsing_table[263]={{42,10}, {43,10}, {40,11}, {41,11}, {59,11}, {-4,11}, {-3,11}, {-2,11}};
	parsing_table[262]={{43,12}, {42,13}};
	parsing_table[257]={{40,14}, {-4,14}, {-3,14}, {-2,14}};
	parsing_table[265]={{-4,15}};
	parsing_table[264]={{-3,16}, {41,17}};
	parsing_table[266]={{-5,18}};
	parsing_table[267]={{-6,19}};
	parsing_table[268]={{-5,20}, {-6,21}, {40,22}, {-4,22}, {-3,22}, {-2,22}, {59,23}};
	parsing_table[269]={{40,24}, {59,24}, {-6,24}, {-5,24}, {-4,24}, {-3,24}, {-2,24}, {-1,25}};
};

std::map<int,std::string> display = {
	{1,"numexpr : 'number'"},
	{2,"parexpr : '(' expr ')'"},
	{3,"idexpr : 'id'"},
	{4,"idexpr : 'funid' '(' exprlist ')'"},
	{5,"exprlist : expr exprlist"},
	{6,"exprlist : EPS"},
	{7,"primary : idexpr"},
	{8,"primary : numexpr"},
	{9,"primary : parexpr"},
	{10,"binoprhs : binop primary binoprhs"},
	{11,"binoprhs : EPS"},
	{12,"binop : '+'"},
	{13,"binop : '*'"},
	{14,"expr : primary binoprhs"},
	{15,"proto : 'funid' '(' idseq ')'"},
	{16,"idseq : 'id' idseq"},
	{17,"idseq : EPS"},
	{18,"def : 'def' proto expr"},
	{19,"external : 'extern' proto"},
	{20,"top : def"},
	{21,"top : external"},
	{22,"top : expr"},
	{23,"top : EPS"},
	{24,"program : top ';' program"},
	{25,"program : EPS"}
};
