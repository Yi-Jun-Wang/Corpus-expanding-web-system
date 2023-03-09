#pragma once

// ####################################################
// #### 音節(Syllable)之子音(Initial)母音(Final)表 ####
// #### 資料來自教育部 "客家語拼音方案使用手冊"    ####
#define INITIAL_NUM  30
#define FINAL_NUM    81
wchar_t Initial_T[INITIAL_NUM][2][10] = {
	{ L"", L"" },
	{ L"b",L"ㄅ" },      //  0
	{ L"p",L"ㄆ" },      //  1
	{ L"m",L"ㄇ" },      //  2
	{ L"f",L"ㄈ" },      //  3
	{ L"v",L"万" },      //  4
	{ L"d",L"ㄉ" },      //  5
	{ L"t",L"ㄊ" },      //  6
	{ L"n",L"ㄋ" },      //  7
	{ L"l",L"ㄌ" },      //  8
	{ L"g",L"ㄍ" },      //  9

	{ L"k",L"ㄎ" },      // 10
	{ L"ng",L"兀" },     // 11
	{ L"ngi",L"兀ㄧ" },  // 12
	{ L"h",L"ㄏ" },      // 13
	{ L"j",L"ㄐ" },      // 14
	{ L"z",L"ㄗ" },      // 15   // 若後面有接 i*，就算i是屬於韻母，發音仍必需為ㄗ│
	{ L"zi",L"ㄗㄧ" },   // 16   // 後面韻母接上後有可能為 zia 或 ziia ，其發音相同，聲母必需是ㄗ│
	{ L"q",L"ㄑ" },      // 17
	{ L"c",L"ㄘ" },      // 18   // 若後面有接 i，就算i是屬於韻母，發音仍必需為ㄘ│
	{ L"ci",L"ㄘㄧ" },   // 19   // 後面韻母接上後有可能為 cia 或 ciia ，其發音相同，聲母必需是ㄘ│
	
    { L"x",L"ㄒ" },      // 20
	{ L"s",L"ㄙ" },      // 21   // 若後面有接 i，就算i是屬於韻母，發音仍必需為ㄙ│
	{ L"si",L"ㄙㄧ" },   // 22   // 後面韻母接上後有可能為 sia 或 siia ，其發音相同，聲母必需是ㄙ│
	{ L"zh",L"ㄓ" },     // 23
	{ L"ch",L"ㄔ" },     // 24
	{ L"sh",L"ㄕ" },     // 25
	{ L"rh",L"ㄖ" },     // 26
	{ L"v", L"万" },     // 27   
	{ L"bb",L"ㆠ" }      // 28   // 僅用於詔安腔
};

// check 日: ngid


wchar_t Final_T[FINAL_NUM][2][10] = {
	{ L"",  L""},
	{ L"nn",L"" },       //  0，詔安腔特有
	{ L"i",L"ㄧ" },      //  2
	{ L"ii",L"⊥" },     //  1
	{ L"e",L"ㄝ" },      //  3
	{ L"ee",L"" },       //  4，詔安腔特有
	{ L"a",L"ㄚ" },      //  5
	{ L"o",L"ㄛ" },      //  6
	{ L"oo",L"ㄛ" },     //  7，詔安腔特有
	{ L"u",L"ㄨ" },      //  8
	{ L"ie",L"ㄧㄝ" },   //  9

	{ L"eu",L"ㄝㄨ" },   // 10
	{ L"ieu",L"ㄧㄝㄨ" },// 11
	{ L"ia",L"ㄧㄚ" },   // 12
	{ L"ua",L"ㄨㄚ" },   // 13
	{ L"ai",L"ㄞ" },     // 14
	{ L"iai",L"ㄧㄞ" },  // 15，海陸、南四線
	{ L"uai",L"ㄨㄞ" },  // 16，
	{ L"au",L"ㄠ" },     // 17，
	{ L"iau",L"ㄧㄠ" },  // 18，
	{ L"io",L"ㄧㄛ" },   // 19，

	{ L"oi",L"ㄛㄧ" },   // 20，
	{ L"ioi",L"ㄧㄛㄧ" },// 21，
	{ L"iu",L"ㄧㄨ" },   // 22，
	{ L"ui",L"ㄨㄧ" },   // 23，
	{ L"iui",L"ㄧㄨㄧ" },// 24，
	{ L"ue",L"ㄨㄝ" },   // 25，
	{ L"iim",L"⊥ㄇ" },  // 26，
	{ L"im",L"ㄧㄇ" },   // 27，
	{ L"em",L"ㄝㄇ" },   // 28，
	{ L"iem",L"ㄧㄝㄇ" },// 29，

	{ L"am",L"ㄚㄇ" },   // 30，
	{ L"iam",L"ㄧㄚㄇ" },// 31，
	{ L"iin",L"⊥ㄇ" },  // 32，
	{ L"in",L"ㄧㄣ" },   // 33，
	{ L"en",L"ㄝㄣ" },   // 34，
	{ L"ien",L"ㄧㄢ" },  // 35，
	{ L"uen",L"ㄨㄝㄣ" },// 36，
	{ L"an",L"ㄢ" },     // 37，
	{ L"ian",L"ㄧㄢ" },  // 38，
	{ L"uan",L"ㄨㄢ" },  // 39，

	{ L"on",L"ㄛㄣ" },   // 40，
	{ L"ion",L"ㄧㄛㄣ" },// 41，
	{ L"un",L"ㄨㄣ" }, // 42，
	{ L"iun",L"ㄧㄨㄣ" },// 43，
	{ L"ang",L"ㄤ" },    // 44，
	{ L"iang",L"ㄧㄤ" }, // 45，
	{ L"uang",L"ㄨㄤ" }, // 46，
	{ L"ong",L"ㄛㄥ" },  // 47，
	{ L"iong",L"ㄧㄛㄥ" },// 48，
	{ L"ung",L"ㄨㄥ" },  // 49，

	{ L"iung",L"ㄧㄨㄥ" },// 50，
	{ L"er",L"ㄜ" },     // 51，
	{ L"ib",L"ㄧㄅ" },   // 53，
	{ L"iib",L"⊥ㄅ" },  // 52，
	{ L"eb",L"ㄝㄅ" },   // 54，
	{ L"ieb",L"ㄧㄝㄅ" },// 55，
	{ L"ab",L"ㄚㄅ" },   // 56，
	{ L"iab",L"ㄧㄚㄅ" },// 57，
	{ L"id",L"ㄧㄉ" },   // 59，
	{ L"iid",L"⊥ㄉ" },  // 58，

	{ L"ed",L"ㄝㄉ" }, // 60，
	{ L"ied",L"ㄧㄝㄉ" },// 61，
	{ L"ued",L"ㄨㄝㄉ" },// 62，
	{ L"ad",L"ㄚㄉ" }, // 63，
	{ L"iad",L"ㄧㄚㄉ" },// 64，
	{ L"uad",L"ㄨㄚㄉ" },// 65，
	{ L"od",L"ㄛㄉ" }, // 66，
	{ L"iod",L"ㄧㄛㄉ" },// 67，
	{ L"ud",L"ㄨㄉ" },   // 68，
	{ L"iud",L"ㄧㄨㄉ" },// 69，

	{ L"ag",L"ㄚㄍ" },   // 70，
	{ L"iag",L"ㄧㄚㄍ" },// 71，
	{ L"uag",L"ㄨㄚㄍ" },// 72，
	{ L"og",L"ㄛㄍ" },   // 73，
	{ L"iog",L"ㄧㄛㄍ" },// 74，
	{ L"ug",L"ㄨㄍ" }, // 75，
	{ L"iug",L"ㄧㄨㄍ" },// 76，
	{ L"m",L"ㄇ" },      // 77，
	{ L"n",L"ㄋ" },      // 78，
	{ L"ng",L"兀" }      // 79，
};








#define  HAKKA_PINYIN_MAX 1100 //1044
struct SYB_DATA
{
	wchar_t SybTon[20];
	wchar_t Syb[20];
	wchar_t Ton[20];
	wchar_t CSybTon[20];
	wchar_t CSyb[20];
	wchar_t CTon[20];
	int     SybTonID;
	int     SybID;
	int     TonID;
};
#include "Data/Syllable_Table_unicode.h" // <= 讀取教育部四縣音節表時，自動產生
int Parser_No_Syb(wchar_t *S11, wchar_t *No, wchar_t *Syb);
int Syb_Converter(wchar_t *SybTon, SYB_DATA *lp)
{
	int i, j, k, Len;
	wchar_t a, b[20], c[20];

	memset(lp, 0, sizeof(SYB_DATA));

	// 1.Check Input Data
	
	Len = wstrlen(SybTon); if (Len >= 20) return -1; // 拼音字串過長
	for (i = 0; i < Len; i++) SybTon[i] |= 0x20;
	for (i = 0; i < Len; i++) if ((SybTon[i] < L'a') || (SybTon[i] > L'z')) break;
	if (i >= Len) return -2; // 沒有聲調資料
	else if (i <= 0)   return -3; // 拼音資料錯誤
	for (j = i; j < Len; j++) if ((SybTon[j] < L'0') || (SybTon[j] > L'9')) break;
	if (j < Len)  return -4; // 聲調資料錯誤

	// 2.Get SybTon, Syb, Ton
	wstrcpy(lp->SybTon, 20, SybTon);
	wstrcpy(lp->Ton, 20, &SybTon[i]);
	wstrncpy(lp->Syb, SybTon, i);

	// 3.Get CSybTon, CSyb, CTon
	for (j = 0; j < HAKKA_PINYIN_MAX; j++) 
		if (!wstrcmp(lp->Syb, (wchar_t *)&Syllable_Table1_u[6][j][0][0])) break;
	if (j >= HAKKA_PINYIN_MAX) return -6; // 找不到拼音
	else wstrcpy(lp->CSyb, 20, (wchar_t *)&Syllable_Table1_u[6][j][1][0]);
	if      (!wstrcmp(lp->Ton, L"24")) wstrcpy(lp->CTon, 99, L"ˊ");
	else if (!wstrcmp(lp->Ton, L"11")) wstrcpy(lp->CTon, 99, L"ˇ");
	else if (!wstrcmp(lp->Ton, L"31")) wstrcpy(lp->CTon, 99, L"ˋ");
	else if (!wstrcmp(lp->Ton, L"55")) wstrcpy(lp->CTon, 99, L"");
	else if (!wstrcmp(lp->Ton, L"2"))  wstrcpy(lp->CTon, 99, L"ˋ.");
	else if (!wstrcmp(lp->Ton, L"5"))  wstrcpy(lp->CTon, 99, L".");
	wstrcpy(lp->CSybTon, 20, lp->CSyb);
	wstrcat(lp->CSybTon, 20, lp->CTon);

	// 4.Get SybTonID, SybID, TonID
	lp->SybID = j;
	lp->TonID = watoi(lp->Ton);
	lp->SybTonID = lp->TonID * 10000 + lp->SybID;

	return 0;
}


// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// #######################################
// E.讀取教育部四縣675音節拼音表
// #######################################
int Parser_No_Syb(wchar_t *S11, wchar_t *No, wchar_t *Syb)
{
	int Len, Beg, End;
	Len = wstrlen(S11);

	// Extract No
	for (Beg = 0; Beg < Len; Beg++) if ((S11[Beg] >= 0x0030) && (S11[Beg] <= 0x0039)) break;
	if (Beg >= Len) return -1;
	for (End = Beg + 1; End < Len; End++) if ((S11[End] < 0x0030) || (S11[Beg] > 0x0039)) break;
	if (End > (Beg + 10)) return -2;
	wstrncpy(No, &S11[Beg], End - Beg); No[End - Beg] = 0;

	// Extract Syb
	for (Beg = End; Beg < Len; Beg++) if ((S11[Beg] >= L'a') && (S11[Beg] <= L'z')) break;
	if (Beg >= Len) return -1;
	for (End = Beg + 1; End < Len; End++) if ((S11[End] < L'a') || (S11[Beg] > L'z')) break;
	wstrncpy(Syb, &S11[Beg], End - Beg); Syb[End - Beg] = 0;
	if (End > (Beg + 10)) return -2;

	// Extract
	return 0;
}

// ##############################
// #### 讀取客語六腔調音節表 ####
// 資料來自教育部 PinYin_Table_四線~南四線.txt(共六個檔案)

//#define  HAKKA_PINYIN_MAX 1100 //1044
struct HAKKA_PINYIN_TABLE
{
	BOOL Flag;
	// 羅馬拼音(a,b,c,d)
	wchar_t Syb[10];
	wchar_t Ini[10];
	wchar_t Fin[10];
	// 注音拼音(ㄅㄆㄇㄈ)
	wchar_t CSyb[10];
	wchar_t CIni[10];
	wchar_t CFin[10];
	// 國字範例:
	//wchar_t Word[10];
	int     Ini_Fin;
} Hakka_Syb_Table[7][HAKKA_PINYIN_MAX];

int Hakka_Syb_No[7] = { 0,0,0,0,0,0,0 };
int kk[6] = { -1,-1,-1,-1,-1,-1 }, nn[6] = { 0,0,0,0,0,0 };

#define EXTRA_SYB_NO   23
wchar_t Extra_Syb[30][10] = {
	L"mid", L"rhai",  L"rhad", L"rhan", L"uai",   L"hu",   L"bib", L"gann", L"cue", L"zhia",
	L"heb", L"hiann", L"shud", L"honn", L"chied", L"diang", L"gng", L"rho", L"shud", L"teb",
	L"chii", L"iid", L"ui"
};

int Add_Syb_Table(int I, wchar_t *No, wchar_t *Syb)
{
	int i;
	i = watoi(No);
	if ((i >= 0) && (i < HAKKA_PINYIN_MAX))
	{
		if ((i >= kk[I]) || (!Hakka_Syb_Table[I][i].Flag))
		{
			wstrcpy(Hakka_Syb_Table[I][i].Syb, 9, Syb);
			Hakka_Syb_Table[I][i].Flag = true;
			Hakka_Syb_Table[I][i].Ini[0] = 0;
			Hakka_Syb_Table[I][i].Fin[0] = 0;
			Hakka_Syb_Table[I][i].CSyb[0] = 0;
			Hakka_Syb_Table[I][i].CIni[0] = 0;
			Hakka_Syb_Table[I][i].CFin[0] = 0;
			kk[I] = i; nn[I]++;
			Hakka_Syb_No[I] = i;
		}
		return 0;
	}
	else return -1;
}


// ###########################################################
// #############  讀取教育部四縣675音節碼  ###################
// ###########################################################
#define  INI_MAX           26
#define  MID_MAX           10
#define  MID2_MAX           2
#define  FIN_MAX            7

// ################################################
// 讀取四線音碼(來自教育部訂定: SybID=1~674)
wchar_t Ini_Tab[INI_MAX][10] = { L"",L"b" ,L"p" ,L"m" ,L"f" ,L"v" ,L"bb" ,L"d" ,L"t" ,L"n" ,L"l" ,L"r" ,L"g" ,L"k" ,L"ng",L"h" ,L"j" ,L"q" ,L"x" ,L"z" ,L"c" ,L"s" ,L"zh",L"ch",L"sh",L"rh" };
wchar_t CIni_Tab[INI_MAX][10] = { L"",L"ㄅ",L"ㄆ",L"ㄇ",L"ㄈ",L"万",L"ㄅ" ,L"ㄉ",L"ㄊ",L"ㄋ",L"ㄌ",L"r" ,L"ㄍ",L"ㄎ",L"兀",L"ㄏ",L"ㄐ",L"ㄑ",L"ㄒ",L"ㄗ",L"ㄘ",L"ㄙ",L"ㄓ",L"ㄔ",L"ㄕ",L"ㄖ" };
wchar_t Mid_Tab[MID_MAX][10] = { L"",L"ii",L"i" ,L"e", L"ee",L"a" ,L"o" ,L"oo",L"u" ,L"er" };
wchar_t CMid_Tab[MID_MAX][10] = { L"",L"帀",L"ㄧ",L"ㄝ",L"ee",L"ㄚ",L"ㄛ",L"oo",L"ㄨ",L"ㄜ" };
wchar_t Mid2_Tab[MID_MAX][10] = { L"",L"nn" };
wchar_t CMid2_Tab[MID_MAX][10] = { L"",L"nn" };
wchar_t Fin_Tab[FIN_MAX][10] = { L"",L"m" ,L"n" ,L"ng",L"b" ,L"d" ,L"g" };
wchar_t CFin_Tab[FIN_MAX][10] = { L"",L"ㄇ",L"ㄋ",L"兀",L"ㄅ",L"ㄉ",L"ㄍ" };

// #################################################################
// 讀取 Data/PinYin_Table_四縣~南四線.txt，四縣675音節拼音檔
// 產出 Data/Syllable_Table_1.h       ， MultiByte Version(char *)
// 產出 Data/Syllable_Table_unicode.h ， Unicode Version(wchat_t *)
wchar_t  S11[2000], No[100], Syb[100];
char     File[6][100] =
{   "Data/PinYin_Table_四縣.txt",
	"Data/PinYin_Table_海陸.txt",
	"Data/PinYin_Table_大埔.txt",
	"Data/PinYin_Table_饒平.txt",
	"Data/PinYin_Table_詔安.txt",
	"Data/PinYin_Table_南四縣.txt"
};

int Read_SybID()
{
	// 抽取四縣腔680音節拼音
	FILE     *In, *Out, *Out1;
	errno_t  err;
	char     S1[2000];
	int      I, i, j, k, i1, i2, i3, i4, L1, L2, L3, L4;
	
	memset(Hakka_Syb_Table, 0, 7 * HAKKA_PINYIN_MAX * sizeof(HAKKA_PINYIN_TABLE));
	for (I = 0; I < 6; I++)
	{		
		if ((err = fopen_s(&In, &File[I][0], "rb")) != 0) { return -1; }
		fread(S11, 1, 2, In); if (S11[0] != 0xFEFF) { return -2; } // 非 Unicode格式
		while (!feof(In))
		{
			i = Read_Unicode_0D0A(In, S11, sizeof(S11));
			j = Parser_No_Syb(S11, No, Syb); // 取出數字代號 與羅馬拼音
			if (j == 0) 
				k = Add_Syb_Table(I, No, Syb);
			TRACE("j=%d k=%d I=%d No=%s Syb=%s\n", j, k, I, No, Syb);
		}
		fclose(In);
	}

	// 詞庫裡特殊發音mid,...
	for (I = 0; I < 6; I++)
	for (i = 0; i < EXTRA_SYB_NO; i++)
	{
		Hakka_Syb_No[I]++;
		sprintf_s(S1, "%d", Hakka_Syb_No[I]);
		Big5ToUniCode(S1, S11, strlen(S1));
		Add_Syb_Table(I, S11, &Extra_Syb[i][0]);
	}

	// 南四縣特有發音(ngian,ian)
	//k = Add_Syb_Table(L"673", L"ngian"); // 南四線 => "年下"
	//k = Add_Syb_Table(L"674", L"ian");   // 南四線 => "圓滾滾"，"圓子"

	// ############################################
	// #### 將六腔調音節表之羅馬拼音 轉成 注音 ####
	for (I = 0; I < 6; I++)
	for (i = 0; i < HAKKA_PINYIN_MAX; i++)
		if (Hakka_Syb_Table[I][i].Flag)
		{	//Ini
			wstrcpy(Syb, 99, Hakka_Syb_Table[I][i].Syb);
			for (i1 = INITIAL_NUM-1; i1 >= 0 ; i1--) if (!wstrncmp(Syb, &Initial_T[i1][0][0], wstrlen(&Initial_T[i1][0][0]))) break;
			if (i1 <= 0) { i1 = 0; L1 = 0; }
			else         { L1 = wstrlen(&Initial_T[i1][0][0]); }
			
			// Fin
			wstrcpy(Syb, 99, &Hakka_Syb_Table[I][i].Syb[L1]);
			if (!wstrcmp(Syb, L"d"))
				j = j;

			if (wstrlen(Syb) == 0) i4 = 0;
			else
			for (i4 = FINAL_NUM - 1; i4 >= 0; i4--) if (!wstrncmp(Syb, &Final_T[i4][0][0], wstrlen(&Final_T[i4][0][0]))) break;
			if (i4 >= 0)
			{
				//L4 = wstrlen(&Final_T[i4][0][0]);
				wstrcpy(Hakka_Syb_Table[I][i].Fin, 9, &Final_T[i4][0][0]);
				wstrcpy(Hakka_Syb_Table[I][i].CFin, 9, &Final_T[i4][1][0]);
			}
			else //if (i4 == -1)
			{
				if ((!wstrcmp(Syb, L"b"))&&(Hakka_Syb_Table[I][i].Syb[L1-1]==L'i'))
				{
					wstrcpy(Hakka_Syb_Table[I][i].Fin, 9, L"ib");
					wstrcpy(Hakka_Syb_Table[I][i].CFin, 9, L"ㄅ");
				}
				else if ((!wstrcmp(Syb, L"d")) && (Hakka_Syb_Table[I][i].Syb[L1 - 1] == L'i'))
				{
					wstrcpy(Hakka_Syb_Table[I][i].Fin, 9, L"id");
					wstrcpy(Hakka_Syb_Table[I][i].CFin, 9, L"ㄉ");
				}
				else
				{
					wstrcpy(Hakka_Syb_Table[I][i].Fin, 9, L"???");
					wstrcpy(Hakka_Syb_Table[I][i].CFin, 9, L"???");
				}
			}
			
			// 建立 Ini, Mid1, Mid2, Fin 之 Roma/注音表 
			wstrcpy(Hakka_Syb_Table[I][i].Ini, 9, &Initial_T[i1][0][0]);
			wstrcpy(Hakka_Syb_Table[I][i].CIni, 9, &Initial_T[i1][1][0]);
			//wstrcpy(Hakka_Syb_Table[I][i].Fin, 9, &Final_T[i4][0][0]);
			//wstrcpy(Hakka_Syb_Table[I][i].CFin, 9, &Final_T[i4][1][0]);
			wstrcpy(Hakka_Syb_Table[I][i].CSyb, 9, Hakka_Syb_Table[I][i].CIni);
			wstrcat(Hakka_Syb_Table[I][i].CSyb, 9, Hakka_Syb_Table[I][i].CFin);
			if (i1 < 0) i1 = 0;
			if (i4 < 0) i4 = 0;
			Hakka_Syb_Table[I][i].Ini_Fin = i1 + i4 * 100;
		}
	
	// ############################################
	// #### 四海大平安音節[0~5]合併成總表[6] ######
	for (I = 0; I < 6; I++) Hakka_Syb_No[I]++; // 實際多了一個 L""在第 0 個
	Hakka_Syb_No[6] = 0;
	for (I = 0; I < 6; I++)
	{
		for (i = 0; i < Hakka_Syb_No[I]; i++)
		{
			for (j = 0; j < Hakka_Syb_No[6]; j++)
				if (!wstrcmp(Hakka_Syb_Table[I][i].Syb, Hakka_Syb_Table[6][j].Syb)) break;
			if (j >= Hakka_Syb_No[6])
			{
				memcpy(&Hakka_Syb_Table[6][Hakka_Syb_No[6]], &Hakka_Syb_Table[I][i], sizeof(HAKKA_PINYIN_TABLE));
				Hakka_Syb_No[6]++;
			}
		}
		i = 0;
	}
	// 總表排序
	HAKKA_PINYIN_TABLE pp;
	for (i = 0; i < Hakka_Syb_No[6] - 1; i++)
		for (j = i + 1; j < Hakka_Syb_No[6]; j++)
		{
			k = wstrcmp(Hakka_Syb_Table[6][i].Syb, Hakka_Syb_Table[6][j].Syb);
			if (k > 0)
			{
				memcpy(&pp, &Hakka_Syb_Table[6][i], sizeof(HAKKA_PINYIN_TABLE));
				memcpy(&Hakka_Syb_Table[6][i], &Hakka_Syb_Table[6][j], sizeof(HAKKA_PINYIN_TABLE));
				memcpy(&Hakka_Syb_Table[6][j], &pp, sizeof(HAKKA_PINYIN_TABLE));
			}
		}

	// 四線拼音表存檔
	char Syb1[10], CSyb1[20];
	sprintf_s(S1, "Data/Syllable_Table_1.h");
	if ((err = fopen_s(&Out, S1, "wb")) != 0) return -34;
	sprintf_s(S1, "Data/Syllable_Table_unicode.h");
	if ((err = fopen_s(&Out1, S1, "wb")) != 0) return -35;
	//S11[0] = 0xFEFF; fwrite(S11, 1, 2, Out1);
	fprintf(Out, "#define HAKKA_PINYIN_MAX   %d\n", HAKKA_PINYIN_MAX);
	fprintf(Out1, "#define HAKKA_PINYIN_MAX   %d\n", HAKKA_PINYIN_MAX);
	fprintf(Out, "char    Syllable_Table1[7][HAKKA_PINYIN_MAX][2][10]={\n");// , HAKKA_PINYIN_MAX);
	fprintf(Out1, "wchar_t Syllable_Table1_u[7][HAKKA_PINYIN_MAX][2][10]={\n");// , HAKKA_PINYIN_MAX);

	for (I = 0; I < 7; I++)
	{
		fprintf(Out, "{\n");
		fprintf(Out1, "{\n");
		for (i = 0; i < HAKKA_PINYIN_MAX; i++)
		{
			UniCodeToBig5(Hakka_Syb_Table[I][i].Syb, Syb1, wstrlen(Hakka_Syb_Table[I][i].Syb));
			UniCodeToBig5(Hakka_Syb_Table[I][i].CSyb, CSyb1, 2 * wstrlen(Hakka_Syb_Table[I][i].CSyb));
			fprintf(Out, "{ \"%s\", \"%s\" }, // %d-%d\n", Syb1, CSyb1, I, i);
			fprintf(Out1, "{ L\"%s\", L\"%s\" }, // %d-%d\n", Syb1, CSyb1, I, i);
		}

		fprintf(Out, "},\n");
		fprintf(Out1, "},\n");

	}
	fprintf(Out, "\n};");
	fprintf(Out1, "\n};");
	fclose(Out);
	fclose(Out1);

	return 0;
}

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// ################################
// F.讀取教育部客語辭典
// ################################
#define MAX_SYB   HAKKA_PINYIN_MAX

wchar_t Syb_Table[MAX_SYB][10];
int     Syb_TNum[MAX_SYB];
int     Syb_Num = 0;
void Init_Syb()
{
	Syb_Num = 0;
	for (int i = 0; i < MAX_SYB; i++) { Syb_Table[i][0] = 0; Syb_TNum[i] = 0; }
}
int Search_Syb(wchar_t *Syb)
{
	int i;
	for (i = 0; i < Syb_Num; i++)
		if (!memcmp(Syb, &Syb_Table[i][0], 2 * wstrlen(Syb)))
			break;
	if (i < Syb_Num) return i; else return -1;
}
int Insert_Syb(wchar_t *Syb)
{
	int i;
	if ((i = Search_Syb(Syb)) < 0)
	{
		if (Syb_Num >= MAX_SYB) { TRACE("Syb_Num=%d\n", Syb_Num); return -1; }
		wstrncpy(&Syb_Table[Syb_Num][0], Syb, wstrlen(Syb));
		Syb_TNum[Syb_Num] = 1;
		Syb_Num++;
		if (Syb_Num >= MAX_SYB) TRACE("Syb_Num=%d\n", Syb_Num);
	}
	else Syb_TNum[i]++;
	return 0;
}

// ################################
