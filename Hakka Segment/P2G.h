#pragma once

// ###########################################################
// 斷詞系統，使用維特比演算法，靠詞長+詞頻
// int P2Gn(int I, wwchar_t *S11, SENTENCE *Sent)
// I: 腔調
// S11: 輸入文句
// Sent: 輸出斷詞結果
//
// 將斷詞結果Sent轉成文字WordUI、拼音Syb與注音CSyb
// int Sent_Syb(int I, SENTENCE *Sent, wwchar_t *WordUI, wchar_t *Syb, wchar_t *CSyb)
// 
// #######################################################

// 四線無發音，選用南四線發音取代
//#define PINYIN_MAX  MAX_ROMA_LEN


#define MAX_POS_LEN      15
//#define MAX_WORD_LEN      15
#define MAX_SENTENCE_LEN  300//500
#define MAX_TREE_Y (MAX_SENTENCE_LEN*5)
struct SENTENCE
{
	wwchar_t     WordUI;   // 客字代碼
	wchar_t      Word;   // 客字代碼
	int          WLen;     // 詞長
	int          Freq;
	int          SybID[6];    // 聲調*10000+675音
	wchar_t      Syb[6][MAX_WORD_LEN+1];  // Roma Pinyin
	wchar_t      CSyb[6][MAX_WORD_LEN+1]; // 注音符號
	wchar_t      POS[15];  // 詞類
};

struct VTDATA
{
	int          Flag;    // 0:Initial, 1:Used
	wwchar_t     WordUI;  // 每個字都存
	wchar_t      Word;
	int          SybID[6];   // 0:標點、英文
	wchar_t      Syb[6][MAX_WORD_LEN+1]; // 每個字都存音
	wchar_t      POS[MAX_POS_LEN+1]; // 詞類(只存在詞的第一個字)
	int          WLen;    // 詞長(*01,*02,*03,...)
	int          Freq;
	int          From;    // 來源(只存在詞的第一個字)
	float        Score;   // 累積分數(存在詞的最後一個字)
	int          Dict_No; // 第幾個詞編號
};
int    Tree_Y=0; 
VTDATA Tree[MAX_TREE_Y][MAX_SENTENCE_LEN];

void Init_Tree()
{
	Tree_Y = 0;
	for (int y = 0; y < MAX_SENTENCE_LEN * 5; y++) 
	for (int x = 0; x < MAX_SENTENCE_LEN; x++)
			memset(&Tree[y][x], 0, sizeof(VTDATA)); //Tree[i][j].Flag = 0;
}
int Set_Tree(int I,int X,DICT *p)
{
	int    i,j;
	float  MScore;
	int    MFrom;

	// 1.Search Best Path/Score From Past(X-1) 
	if (X == 0) { MScore = 0; MFrom = -1; } // 第0個字，前面沒有可以累加
	else
	{	MScore = -1; MFrom = -1;
		for (j = 0; j < Tree_Y; j++)
		   if ((Tree[j][X - 1].Flag) && (!Tree[j][X].Flag)) // (左上..=T, 上..=F)，X-1是終點
			   if (MScore < Tree[j][X - 1].Score) 
			   {  MScore = Tree[j][X - 1].Score; MFrom = j; /*Tree[j][X - 1].From;*/ }
		if (MFrom < 0) TRACE("找不到前一字路徑 MFrom Error at X=%d\n", X);
	}

	// 2.Set Tree Data
	for (i = 0; i < p->Wrd_Num; i++)
	{
		// 詞內每個字資料一樣
		Tree[Tree_Y][X+i].Flag = 1;
		Tree[Tree_Y][X + i].Dict_No = p->No;
		wstrcpy(Tree[Tree_Y][X+i].POS, MAX_POS_LEN, p->Pos);
		Tree[Tree_Y][X + i].From  = MFrom;
		Tree[Tree_Y][X + i].Freq = p->Freq;
		Tree[Tree_Y][X + i].Score = MScore;
		if (p->SybID[I][i]>=0) Tree[Tree_Y][X + i].Score += (float)(p->Wrd_Num*p->Wrd_Num) + (float)log10(p->Freq+1)/6; // 詞長平方+詞頻(0~1)
		// 詞內每個字資料不一樣
		Tree[Tree_Y][X+i].WordUI = p->WordUI[i];
		Tree[Tree_Y][X + i].Word = (wchar_t)(p->WordUI[i]&0xFFFF); // 特殊字(0xD800~0xDFFF)不處理
		for (I=0;I<6;I++) Tree[Tree_Y][X+i].SybID[I]  = p->SybID[I][i];
		Tree[Tree_Y][X+i].WLen   = p->Wrd_Num*100+i+1;
		for (I = 0; I < 6; I++) wstrcpy(&Tree[Tree_Y][X+i].Syb[I][0], MAX_WORD_LEN,&p->SybRoma[I][i][0]);
		for (I = 0; I < 6; I++) wstrcat(&Tree[Tree_Y][X+i].Syb[I][0], MAX_WORD_LEN,&p->SybTone[I][i][0]);
	}
	Tree_Y++;
	if (Tree_Y>=MAX_TREE_Y) TRACE("MAX_TREE_Y太小錯誤， X=%d, Tree_Y=%d\n", X,Tree_Y);
	return 0;
}

// === 輸入/輸出參數 ===
// I=0~5: 四海大平安南腔, 
// S11:   輸入斷詞文句
// Sent:  輸出斷詞結果
//     :  音調數(24,11,31,55,2,5)、型(ˊ_ˇ_ˋ_ _ˋ._.)
int P2Gn(int I, wwchar_t *S11, SENTENCE *Sent) // I=腔調(0:四縣, 2:海陸,...)
{
	DICT         *p,p1,p2,p3;
	int          X, i, j, k ,Max_k, Max_Dict, Len, Flag1;
	wchar_t      A[100], B[100], C[100], D[100];
	wwchar_t     UniCode[1000];

	// 1.初始化設定 p1:標點，p2:英文，p3:缺詞
	p1.Freq     = p2.Freq     = p3.Freq     = 0;
	wstrcpy(p1.Pos, MAX_POS_LEN, L"標點符號");
	wstrcpy(p2.Pos, MAX_POS_LEN, L"英文");
	wstrcpy(p3.Pos, MAX_POS_LEN, L"缺一字詞");
	p1.Wrd_Num  = p2.Wrd_Num  = p3.Wrd_Num  = 1;
	p1.Wrd_Num1 = p2.Wrd_Num1 = p3.Wrd_Num1 = 1;
	p1.No = -1; p2.No = -2;  p3.No = -3;
	for (j = 0; j < 6; j++)
	{
		p1.Syb[j][0]  = p2.Syb[j][0]  = p3.Syb[j][0]  = 0;
		p1.Syb_Len[j] = p2.Syb_Len[j] = p3.Syb_Len[j] = 0;
		for (i = 0; i < MAX_WORD_LEN; i++)
		{
			//p1.Word[i]   = p2.Word[i]   = p3.Word[i]   = 0;
			p1.WordUI[i] = p2.WordUI[i] = p3.WordUI[i] = 0;
			p1.SybID[j][i] = -1; // 標點 
			p2.SybID[j][i] = -2; // 英文
			p3.SybID[j][i] = -3; // 缺詞
			p1.SybRoma[j][i][0] = p2.SybRoma[j][i][0] = p3.SybRoma[j][i][0] = 0;
			p1.SybTone[j][i][0] = p2.SybTone[j][i][0] = p3.SybTone[j][i][0] = 0;
		}
	}

	Len = wwstrlen(S11);
	wwstrcpy(UniCode, 1000, S11);
	// =======================================
    // ==== 2.Text Normalize-1 ===================
    // 時間、數量、數字、單位、金錢、重量、長度
    // =======================================
    // Text Normalize-2: 45.45 => 四十五點四五

	// ==== 2.1.Text Normalize-2 =====
	// 英文、數字
	for (i = 0; i < Len; i++)
	{
		if      ((UniCode[i] == (wwchar_t)L'0') || (UniCode[i] == (wwchar_t)L'０')) UniCode[i] = (wwchar_t)L'零';
		else if ((UniCode[i] == (wwchar_t)L'1') || (UniCode[i] == (wwchar_t)L'１')) UniCode[i] = (wwchar_t)L'一';
		else if ((UniCode[i] == (wwchar_t)L'2') || (UniCode[i] == (wwchar_t)L'２')) UniCode[i] = (wwchar_t)L'二';
		else if ((UniCode[i] == (wwchar_t)L'3') || (UniCode[i] == (wwchar_t)L'３')) UniCode[i] = (wwchar_t)L'三';
		else if ((UniCode[i] == (wwchar_t)L'4') || (UniCode[i] == (wwchar_t)L'４')) UniCode[i] = (wwchar_t)L'四';
		else if ((UniCode[i] == (wwchar_t)L'5') || (UniCode[i] == (wwchar_t)L'５')) UniCode[i] = (wwchar_t)L'五';
		else if ((UniCode[i] == (wwchar_t)L'6') || (UniCode[i] == (wwchar_t)L'６')) UniCode[i] = (wwchar_t)L'六';
		else if ((UniCode[i] == (wwchar_t)L'7') || (UniCode[i] == (wwchar_t)L'７')) UniCode[i] = (wwchar_t)L'七';
		else if ((UniCode[i] == (wwchar_t)L'8') || (UniCode[i] == (wwchar_t)L'８')) UniCode[i] = (wwchar_t)L'八';
		else if ((UniCode[i] == (wwchar_t)L'9') || (UniCode[i] == (wwchar_t)L'９')) UniCode[i] = (wwchar_t)L'九';
		else if ((UniCode[i] >= (wwchar_t)L'a')  && (UniCode[i] <= (wwchar_t)L'z'))   UniCode[i] +=(wwchar_t)(L'Ａ' - L'a');
		else if ((UniCode[i] >= (wwchar_t)L'A')  && (UniCode[i] <= (wwchar_t)L'Z'))   UniCode[i] +=(wwchar_t)(L'Ａ' - L'A');
		else if ((UniCode[i] >= (wwchar_t)L'ａ') && (UniCode[i] <= (wwchar_t)L'ｚ'))  UniCode[i] +=(wwchar_t)(L'Ａ' - L'ａ');
	}

	// ====================================================
	// ====== 3.斷詞(Viterbi 往前累加分數) ================
	// ====================================================
	Init_Tree();
	for (X = 0; X < Len; X++)
	{
		// 3.1.過濾標點符號，
		k = wstrlen(Punch);
		for (j = 0; j < k; j++) if ((wwchar_t)Punch[j] == UniCode[X]) break;
		if (j < k)
		{	p1.WordUI[0] = UniCode[X];
			Set_Tree(I, X, &p1); // 標點
		}
		// 3.2.過濾標點符號，a~z, A~Z, Ａ~Ｚ
		else if ((UniCode[X] >= (wwchar_t)L'Ａ') && (UniCode[X] <= (wwchar_t)L'Ｚ'))
		{	p2.WordUI[0] = UniCode[X];
			Set_Tree(I, X, &p2); // 英文
		}
		// 3.3.客語字斷詞
		else
		{	// 3.3.1.搜尋客語詞庫
			Max_Dict = -1; Max_k = 0; Flag1 = 0;
		    for (j = 0; j < Dict_Num; j++)
		    {   p = &Dict[j];
			    // == 完整詞長度比對 ==
			    if ((p->Wrd_Num <= (Len - X)) && // 剩餘足夠長的句子
				    (!wwstrncmp(&UniCode[X], p->WordUI, p->Wrd_Num)) && // 詞一樣
				    (wstrlen(&p->Syb[I][0]) > 0) // 四線有發音
				    // && ((wstrlen(&p->Syb[0][0]) > 0) || (wstrlen(&p->Syb[5][0]) > 0)) // 四線或南四線有發音
				   )
			    {
				   //p->Freq++; // 被搜尋到一次，就+1
				   if (1 == p->Wrd_Num) Flag1 = 1;
				   Set_Tree(I, X, &Dict[j]); 
			    }
		    }

			// 找不到一字詞
			if (Flag1== 0)
			{
				p3.WordUI[0] = UniCode[X];
				Set_Tree(I, X, &p3);
			}

		} // else
	} // for-X

	// ========================================================
	// ====== 4.斷詞(Viterbi 往回找出最佳路徑) ================
	// ========================================================
	float MScore;
	int   MFrom,WLen;
	wwchar_t WordUI[100];
	// 4.1.搜尋最大值
	X = Len - 1;
	MScore = -1; MFrom = -1;
	for (i = 0; i < Tree_Y; i++)
		if ((Tree[i][X].Flag) && (MScore < Tree[i][X].Score))
		{
			MFrom = i;
			MScore = Tree[i][X].Score;
			WLen = Tree[i][X].WLen;
		}

	while ((X >= 0)&&(MFrom>=0))
	{   // 4.2.Copy詞資料
		if (MFrom < 0)
		{	X--; TRACE("Viterbi回朔錯誤 at X=%d\n", X);	
		    //MScore = Tree[MFrom][X].Score;
		    //WLen = Tree[MFrom][X].WLen;
		}
		else
		{
			if ((Tree[MFrom][X].Dict_No>=0)&&(Tree[MFrom][X].Dict_No<Dict_Num))
			   Dict[Tree[MFrom][X].Dict_No].Freq++; // 確定搜尋到，累積詞頻之搜尋次數

			// Copy資料從Tree => Sent
			j = WLen / 100;
			for (i = X - j + 1; i <= X; i++)
			{
				Sent[i].WordUI = Tree[MFrom][i].WordUI;
				Sent[i].Word   = (wchar_t)(Tree[MFrom][i].WordUI&0xFFFF);
				Sent[i].WLen   = Tree[MFrom][i].WLen;
				for (I=0;I<6;I++) Sent[i].SybID[I]  = Tree[MFrom][i].SybID[I];
				Sent[i].Freq   = Tree[MFrom][i].Freq;
				for (I = 0; I < 6; I++) wstrcpy(&Sent[i].Syb[I][0], 15, &Tree[MFrom][i].Syb[I][0]);
				wstrcpy(Sent[i].POS, 15, Tree[MFrom][i].POS);

				WordUI[i - X + j - 1] = Tree[MFrom][i].WordUI;
			}
			WordUI[j] = 0;
			// 紀錄詞被引用 Freq2(累積),Freq3(目前)
			for (i=0;i<Dict_Num;i++)
				if (!memcmp(Dict[i].WordUI, WordUI, j * 4))
				{
					Dict[i].Freq2++;
					Dict[i].Freq3++;
			        break;	}

			// 指向下一筆
			MFrom = Tree[MFrom][X].From;
			X -= j;
			// 取出MScore、WLen
			if ((X >= 0) && (MFrom >= 0))
			{
				MScore = Tree[MFrom][X].Score;
				WLen = Tree[MFrom][X].WLen;
			}
		}
	}


	// =================================
	// ====== 斷詞後處理  ==============
	// =================================
	// 處理重複詞 "快 快樂 樂" => "快快樂樂"
	for (i = 0; i < Len - 3; i++)
	{
		if ((Sent[i].WLen == 101) && (Sent[i+1].WLen == 201) && (Sent[i+2].WLen == 202) && (Sent[i+3].WLen == 101) &&
			(Sent[i].WordUI == Sent[i+1].WordUI) && (Sent[i+2].WordUI == Sent[i+3].WordUI))
		{
			Sent[i].WLen = 401; Sent[i+1].WLen = 402; Sent[i+2].WLen = 403; Sent[i+3].WLen = 404;
			wstrcpy(Sent[i].POS, 15, Sent[i + 1].POS); wstrcpy(Sent[i+3].POS, 15, Sent[i + 1].POS);
			Sent[i].Freq = Sent[i + 3].Freq = Sent[i + 1].Freq;
		}
	}
	
	// ============================
	// ======= 破音字處理 =========
	// ============================
	return 0;
}
int Sent_Syb(int I, SENTENCE *Sent, wwchar_t *WordUI, wchar_t *Syb, wchar_t *CSyb)
{
	int      n, i, j, i1, i2, i3, Len;
	char     S1[2000];
	wchar_t  AA, S11[2000], S13[2000], S14[2000];// , CSyb[200], Syb[200];
	wwchar_t S21[2000];
	SYB_DATA     A[6];

	n = i1 = i2 = 0;

	for (Len = 0; Len < MAX_SENTENCE_LEN; Len++) if (Sent[Len].WordUI == 0) break;
	Syb[0] = CSyb[0] = 0;
	for (i1 = 0; i1 < Len; i1++)
	{
		// 產生文字
		WordUI[i1] = Sent[i1].WordUI;
		// 產生客語拼音Syb與注音CSyb
		if (Sent[i1].SybID < 0)
		{
			AA = (wchar_t)WordUI[i1];
			wstrncat(Syb, 2000, &AA, 1);
			wstrcat(Syb, 2000, L" ");
			wstrncat(CSyb, 2000, &AA, 1);
			wstrcat(CSyb, 2000, L" ");
		}
		else
		{
			wstrcat(Syb, 2000, &Sent[i1].Syb[I][0]);
			// 檢查破音字並加上)
			if (Sent[i1].WLen == 101)
			{
				i = Search_Poin(Sent[i1].WordUI);
				if (i >= 0)
				{
					if (Poin[i].SybNo[I] > 1)
					{
						wstrcat(Syb, 2000, L"(");
						for (j = 0; j < Poin[i].SybNo[I]; j++)
							if (wstrcmp(&Poin[i].Syb[I][j][0], &Sent[i1].Syb[I][0]))
							{
								wstrcat(Syb, 2000, &Poin[i].Syb[I][j][0]);
								wstrcat(Syb, 2000, L" ");
							}
						wstrcat(Syb, 2000, L")");
					}
				}
			}

			wstrcat(Syb, 2000, L" ");
			Syb_Converter(&Sent[i1].Syb[I][0], &A[I]);
			wstrcat(CSyb, 2000, A[I].CSybTon);
			// 檢查破音字並加上)
			if (Sent[i1].WLen == 101)
			{
				i = Search_Poin(Sent[i1].WordUI);
				if (i >= 0)
				{
					if (Poin[i].SybNo[I] > 1)
					{
						wstrcat(CSyb, 2000, L"(");
						for (j = 0; j < Poin[i].SybNo[I]; j++)
							if (wstrcmp(&Poin[i].Syb[I][j][0], &Sent[i1].Syb[I][0]))
							{
								Syb_Converter(&Poin[i].Syb[I][j][0], &A[I]);
								wstrcat(CSyb, 2000, A[I].CSybTon);
								wstrcat(CSyb, 2000, L" ");
							}

						wstrcat(CSyb, 2000, L")");
					}
				}
			}
			wstrcat(CSyb, 2000, L" ");
		}
	}
	WordUI[Len] = 0;
	return Len;
}


// ####################################################
// 功能: 從200萬四線文本中，找出南四線差異最大句子
//       並依差異比例排序，
// 輸入: Data/Edu_Dict_1110509_edu2.csv
// 輸入: Data/四縣（含南四縣）語料文本_2021.04.30_C.txt
// 輸出: Data/南四縣差異句.csv
// ####################################################
#define NNN_MAX 140 // 句數
#define SENT_MAX    300 // 句長
struct NNN
{
	int      Flag;
	wwchar_t WordUI[SENT_MAX];
	wchar_t  Word[SENT_MAX];
	wchar_t  Syb0[SENT_MAX*10]; // 四線拼音(句)
	wchar_t  Syb5[SENT_MAX*10]; // 南四線拼音(句)
	wchar_t  RomaTone0[SENT_MAX][10]; // 四線拼音(字)
	wchar_t  RomaTone5[SENT_MAX][10]; // 南四線拼音(字)
	int      NNo[3], SNo[3], Rank;
	float    Rate;
} Nnn[NNN_MAX]; // 南四線差異句資料

#define POIN_MAX  1000
wwchar_t Poin_WordUI[6][POIN_MAX];
wchar_t Poin_Word[6][POIN_MAX];
int      Poin_No[6] = { 0,0,0,0,0,0 };

// 產生南北差異最大之南四縣句子
void Generate_S4_Sentence()
{
	SENTENCE     Sent[2000], Sent1[2000];// MAX_SENTENCE_LEN];
	char     S1[2000];
	wchar_t  S11[2000];
	wwchar_t S21[2000];
	char     File0[100] = { "Data/Edu_Dict_1110509_edu2.csv" };
	char     File1[100] = { "Data/四縣（含南四縣）語料文本_2021.04.30_C.txt" };
	char     File2[100] = { "Data/南四縣差異句_1110519.csv" };
	errno_t  err;
	FILE     *In;
	int      i, j, k, n, j1, j2, WLen, I;
	NNN      *p, *pi, *pj, p0;
	DICT     *p1, *p2;

	// 讀取拼音檔 & 客語辭庫
	if ((i = Read_SybID()) < 0) printf("675音節表(PinYin_Table_四縣.txt)讀取失敗");
	if ((j = Read_Edu_Hakka_Dict_Hsf(File0)) < 0) printf("辭庫(Edu_Dict_1110508_edu2.csv)讀取失敗");

	// 分析破音字
	memset(Poin_WordUI, 0, POIN_MAX * sizeof(wwchar_t));
	memset(Poin_Word, 0, POIN_MAX * sizeof(wchar_t));
	for (I = 0; I < 6; I++)
		for (i = 0; i < Dict_Num; i++)
		{
			if (i >= 115)
				i = i;
			k = 0;
			p1 = &Dict[i];
			if ((p1->Wrd_Num == 1) && (p1->Syb_Len[I] > 0))
			{
				for (j = i + 1; j < Dict_Num; j++)
				{
					p2 = &Dict[j];
					if ((p2->Wrd_Num == 1) && (p2->Syb_Len[I] > 0) &&
						(p1->WordUI[0] == p2->WordUI[0]) &&
						(wstrcmp(&p1->Syb[I][0], &p2->Syb[I][0]))
						)
					{
						k = 1;
					}
					if ((p2->Wrd_Num > 1) || (p1->WordUI[0] != p2->WordUI[0])) { i = j - 1;  break; } // 跳下一個
				}
				if (k == 1)
				{
					Poin_WordUI[I][Poin_No[I]] = p1->WordUI[0];
					Poin_Word[I][Poin_No[I]++] = p1->Word[0];
				}
			}
		}

	// 讀檔分析
	for (i = 0; i < NNN_MAX; i++) memset(&Nnn[i], 0, sizeof(NNN));
	if ((err = fopen_s(&In, File1, "rb")) != 0) { printf("File Err"); return; }

	k = 0; n = 0;
	while ((!feof(In)))//&&(n<100))
	{
		p = &Nnn[n]; p->Flag = 1;

		i = Read_Unicode_Punch(In, S11, 30);
		//wstrcpy(S11,2000, L"雖然有一息仔遠");
		// 刪除\r\n
		WLen = wstrlen(S11);
		for (i = 0; i < WLen; i++)
		{
			if ((S11[i] == L'\r') || (S11[i] == L'\n'))
			{
				for (j = i + 1; j <= WLen; j++) S11[j - 1] = S11[j];
				WLen--; i--;
			}
		}
		//wstrcpy(S11, 30, L"乳a乾假，值傾像先兒");
		wstrcpy(p->Word, 30, S11);
		WLen = UniCodeToUI(S11, S21, 200);
		wwstrcpy(p->WordUI, 30, S21);

		memset(Sent, 0, sizeof(SENTENCE)*MAX_SENTENCE_LEN);
		memset(Sent1, 0, sizeof(SENTENCE)*MAX_SENTENCE_LEN);
		j1 = P2Gn(0, S21, Sent);
		j2 = P2Gn(5, S21, Sent1);

		// 四縣
		p->Syb0[0] = 0;
		for (i = 0; i < WLen - 1; i++)
		{
			for (j = 0; j < Poin_No[0]; j++) if (p->WordUI[i] == Poin_WordUI[0][j]) break;
			if (j < Poin_No[0]) wstrcat(&p->RomaTone5[i][0], 10, L"(");
			wstrcat(&p->RomaTone0[i][0], 10, &Sent[i].Syb[I][0]);
			wstrcat(p->Syb0, 300, &Sent[i].Syb[I][0]); wstrcat(p->Syb0, 300, L" ");
		}
		wstrcat(p->Syb0, 300, &Sent[WLen - 1].Syb[I][0]);

		// 南四縣
		p->Syb5[0] = 0;
		for (i = 0; i < WLen; i++)
		{
			// 1.紀錄單字拼音資料
			wstrcpy(&p->RomaTone5[i][0], 10, &Sent1[i].Syb[I][0]);

			// 2.記錄整句拼音資料
			// 2.1.破音字拼音前加 (
			for (j = 0; j < Poin_No[5]; j++) if (p->WordUI[i] == Poin_WordUI[5][j]) break;
			if (j < Poin_No[5]) wstrcat(p->Syb5, 300, L"(");

			// 2.2.拼音串接
			if (Sent1[i].SybID < 0)
			{
				// 2.2.1.標點符號、a~z
				// 斷不出的中文字，前面加(
				for (j = 0; j < wstrlen(Punch); j++) 
					if (Sent1[i].Word == Punch[j]) break;
				if ((j >= wstrlen(Punch))&&((Sent1[i].Word<L'0')||(Sent1[i].Word > L'z')))
					wstrcat(p->Syb5, 300, L"("); 

				wstrncat(p->Syb5, 300, &Sent1[i].Word, 1); wstrcat(p->Syb5, 300, L" ");
			}
			else
			{
				// 2.2.2.拼音
				wstrcat(p->Syb5, 300, &Sent1[i].Syb[I][0]); wstrcat(p->Syb5, 300, L" ");
			}
		}

		// 紀錄南北四縣差距數據
		for (i = 0; i < WLen; i++)
		{
			// 四縣
			p->NNo[0]++;
			if (Sent[i].SybID > 0)
			{
				p->NNo[1]++;
				if ((Sent1[i].SybID > 0) && (Sent[i].SybID != Sent1[i].SybID)) p->NNo[2]++;
			}
			// 南四縣
			p->SNo[0]++;
			if (Sent1[i].SybID > 0)
			{
				p->SNo[1]++;
				if ((Sent[i].SybID > 0) && (Sent[i].SybID != Sent1[i].SybID)) p->SNo[2]++;
			}
		}
		if (p->SNo[1] != 0) p->Rate = (float)p->SNo[2] / (float)p->SNo[1]; else p->Rate = 0;

		// 進度顯示
		k += WLen;
		if ((n % 100) == 0)
		{
			sprintf_s(S1, "n=%d k=%d\n", n, k);
			Big5ToUniCode(S1, S11, 2000);
			cout << S11;
			TRACE(S1);
		}
		n++;
	}
	fclose(In);

	// 根據南四縣差異值排序
	for (i = 0; i < n - 1; i++)
	{
		pi = &Nnn[i];
		for (j = i + 1; j < n; j++)
		{
			pj = &Nnn[j];
			if (pi->Rate < pj->Rate)
			{
				memcpy(&p0, pi, sizeof(NNN));
				memcpy(pi, pj, sizeof(NNN));
				memcpy(pj, &p0, sizeof(NNN));
			}

		}
	}

	// 列印
	float Rate;
	FILE  *Out;
	if ((err = fopen_s(&Out, File2, "wb")) != 0) { printf("File544 Err"); return; }

	S11[0] = 0xFEFF; fwrite(S11, 1, 2, Out);

	j = k = 0;
	for (i = 0; i < n; i++)
	{
		p = &Nnn[i];
		j += p->SNo[1];
		k += p->SNo[2];
		Rate = (float)k / (float)j;
		if ((i % 100) == 0)
			TRACE("n=% i=%d j=%d k=%d R=%f - %d %d %f\n", n, i, j, k, Rate, p->SNo[1], p->SNo[2], p->Rate);

		sprintf_s(S1, "%6d - %6d - %7d - %7d - %10.8f\t", n, i, j, k, Rate);
		Big5ToUniCode(S1, S11, strlen(S1)); fwrite(S11, 2, wstrlen(S11), Out);
		fwrite(p->Word, 2, wstrlen(p->Word), Out);
		S11[0] = L'\t'; fwrite(S11, 1, 2, Out);
		fwrite(p->Syb5, 2, wstrlen(p->Syb5), Out);
		wstrcpy(S11, 200, L"\r\n"); fwrite(S11, 2, wstrlen(S11), Out);
	}
	fclose(Out);


	//Save_Edu_Dict("Data/Edu_Dict_1110508_edu2.csv");
}


// 南四縣句子上拼音
void Generate_S4_2()
{
	//Generate_S4_Sentence();
	SENTENCE     Sent[2000], Sent1[2000];// MAX_SENTENCE_LEN];
	char     S1[2000];
	wchar_t  S10[2000], S11[2000];
	wwchar_t S21[2000];
	char     File0[100] = { "Data/Edu_Dict_1110509_edu2.csv" };
	//char     File1[100] = { "Data/四縣（含南四縣）語料文本_2021.04.30_C.txt" };
	//char     File1[100] = { "Data/1110517_翰俊/103客語認證南四縣(初級+中高級)詞彙例句.txt" };
	//char     File1[100] = { "Data/1110517_翰俊/南四縣差異句2200(待上拼音).txt" };
	//char     File2[100] = { "Data/1110517_翰俊/南四縣差異句2200(待上拼音)_P2G.csv" };
	char     File1[100] = { "Data/1110517_翰俊/南四縣客語造句_0615待標音.txt" };
	char     File2[100] = { "Data/1110517_翰俊/南四縣客語造句_0615待標音_P2G.csv" };
	errno_t  err;
	FILE     *In, *Out;
	int      i, j, k, n, j1, j2, WLen, I;
	NNN      *p, *pi, *pj, p0;
	DICT     *p1, *p2;
	int      L[200];

	// 讀取拼音檔 & 客語辭庫
	if ((i = Read_SybID()) < 0) printf("675音節表(PinYin_Table_四縣.txt)讀取失敗");
	if ((j = Read_Edu_Hakka_Dict_Hsf(File0)) < 0) printf("辭庫(Edu_Dict_1110508_edu2.csv)讀取失敗");

	// 分析破音字
	memset(Poin_WordUI, 0, POIN_MAX * sizeof(wwchar_t));
	memset(Poin_Word, 0, POIN_MAX * sizeof(wchar_t));
	for (I = 0; I < 6; I++)
		for (i = 0; i < Dict_Num; i++)
		{
			if (i >= 115)
				i = i;
			k = 0;
			p1 = &Dict[i];
			if ((p1->Wrd_Num == 1) && (p1->Syb_Len[I] > 0))
			{
				for (j = i + 1; j < Dict_Num; j++)
				{
					p2 = &Dict[j];
					if ((p2->Wrd_Num == 1) && (p2->Syb_Len[I] > 0) &&
						(p1->WordUI[0] == p2->WordUI[0]) &&
						(wstrcmp(&p1->Syb[I][0], &p2->Syb[I][0]))
						)
					{
						k = 1;
					}
					if ((p2->Wrd_Num > 1) || (p1->WordUI[0] != p2->WordUI[0])) { i = j - 1;  break; } // 跳下一個
				}
				if (k == 1)
				{
					Poin_WordUI[I][Poin_No[I]] = p1->WordUI[0];
					Poin_Word[I][Poin_No[I]++] = p1->Word[0];
				}
			}
		}


	int Len, Beg, Column=0; // 客語文字欄位
	if ((err = fopen_s(&In, File1, "rb")) != 0) { printf("File 5498 Err"); return; }
	if ((err = fopen_s(&Out, File2, "wb")) != 0) { printf("File 5798 Err"); return; }
	fread(S11, 1, 2, In);
	fwrite(S11, 1, 2, Out);
	while (!feof(In))
	{
		p = &Nnn[0]; p->Syb5[0] = 0;
		k = Read_Unicode_0D0A(In, S10, 2000);
		
		L[0] = -1; j = 1;
		for (i = 0; i < wstrlen(S10); i++) if (S10[i] == L'\t') L[j++] = i;
		for (i = j; i < 200; i++) L[i] = wstrlen(S10);

		Len = L[1+ Column] - L[0+ Column] - 1; Beg = L[0+Column] + 1;
		if ((Len > 0) && (Len < SENT_MAX))
		{
			wstrncpy(p->Word, &S10[Beg], Len);
			WLen = UniCodeToUI(p->Word, S21, 200);
			wwstrcpy(p->WordUI, 30, S21);

			memset(Sent1, 0, sizeof(SENTENCE)*MAX_SENTENCE_LEN);
			j2 = P2Gn(5, S21, Sent1); // 5:南四縣

			// 南四縣
			p->Syb5[0] = 0;
			for (i = 0; i < WLen; i++)
			{
				// 1.紀錄單字拼音資料
				wstrcpy(&p->RomaTone5[i][0], 10, &Sent1[i].Syb[I][0]);

				// 2.記錄整句拼音資料
				// 2.1.破音字拼音前加 (
				for (j = 0; j < Poin_No[5]; j++) if (p->WordUI[i] == Poin_WordUI[5][j]) break;
				if (j < Poin_No[5]) wstrcat(p->Syb5, 300, L"(");

				// 2.2.拼音串接
				if (Sent1[i].SybID < 0)
				{
					// 2.2.1.標點符號、a~z
					// 斷不出的中文字，前面加(
					for (j = 0; j < wstrlen(Punch); j++)
						if (Sent1[i].Word == Punch[j]) break;
					if ((j >= wstrlen(Punch)) && ((Sent1[i].Word < L'0') || (Sent1[i].Word > L'z')))
						wstrcat(p->Syb5, 300, L"(");

					wstrncat(p->Syb5, 300, &Sent1[i].Word, 1); wstrcat(p->Syb5, 300, L" ");
				}
				else
				{
					// 2.2.2.拼音
					wstrcat(p->Syb5, 300, &Sent1[i].Syb[I][0]); wstrcat(p->Syb5, 300, L" ");
				}
			}
			i = i;

		}
		wstrcat(S10, 2000, L"\t");
		wstrcat(S10, 2000, p->Syb5);
		wstrcat(S10, 2000, L"\r\n");
		fwrite(S10, 2, wstrlen(S10), Out);


	}
	fclose(In);
	fclose(Out);

}



#define PINYIN_MAX   20
int Write_PinYin(FILE *Out, int I, SENTENCE *Sent, int Mode, int K) // Mode=0(Syb), 1(CSyb), 2(Both)    K=0(\r\n), 1(\t),
{
	int     i, j, k, Len;
	wchar_t      AA, S14[100];
	wwchar_t     WordUI[MAX_SENTENCE_LEN1];
	wchar_t      Word[MAX_SENTENCE_LEN1];
	wchar_t      Syb[MAX_SENTENCE_LEN1];
	wchar_t      CSyb[MAX_SENTENCE_LEN1];

	if (Out == 0) return -1;

	Sent_Syb(I, Sent, WordUI, Syb, CSyb);
	Len = wwstrlen(WordUI);
	// 寫入原始文句
	for (i = 0; i < Len; i++)
	{
		if (WordUI[i] > 0x10000)
		{
			AA = (wchar_t)(WordUI[i] >> 16); fwrite(&AA, 1, 2, Out);
		}
		AA = (wchar_t)(WordUI[i] & 0xFFFF);	fwrite(&AA, 1, 2, Out);
	}
	if (K == 0)
	{
		wstrcpy(S14, 99, L"\r\n"); fwrite(S14, wstrlen(S14), 2, Out);
	}
	else if (K == 1)
	{
		wstrcpy(S14, 99, L"\t"); fwrite(S14, wstrlen(S14), 2, Out);
	}

	// 寫入拼音
	if ((Mode == 0) || (Mode == 2))
	{
		fwrite(Syb, 2, wstrlen(Syb), Out);
		if (K == 0)
		{
			wstrcpy(S14, 99, L"\r\n"); fwrite(S14, wstrlen(S14), 2, Out);
		}
		else if (K == 1)
		{
			wstrcpy(S14, 99, L"\t"); fwrite(S14, wstrlen(S14), 2, Out);
		}
	}

	// 寫入注音
	if ((Mode == 1) || (Mode == 2))
	{
		fwrite(CSyb, 2, wstrlen(CSyb), Out);
		if (K == 0)
		{
			wstrcpy(S14, 99, L"\r\n"); fwrite(S14, wstrlen(S14), 2, Out);
		}
		else if (K == 1)
		{
			wstrcpy(S14, 99, L"\t"); fwrite(S14, wstrlen(S14), 2, Out);
		}
	}

	wstrcpy(S14, 99, L"\r\n"); fwrite(S14, wstrlen(S14), 2, Out);
	return Len;
}


// ##################################################
// 產生700音節平衡句
#define MAX_SENTENCE_NO   140000
wchar_t Sentence[MAX_SENTENCE_NO][MAX_SENTENCE_LEN];
int     Select[MAX_SENTENCE_NO];  // 句子被挑選否
int     SybID[MAX_SENTENCE_NO][MAX_SENTENCE_LEN];
int     SybNo1[HAKKA_PINYIN_MAX]; // 文本音節數量
int     SybNo2[HAKKA_PINYIN_MAX]; // 平衡句已挑數量
int     SybNo3[HAKKA_PINYIN_MAX];

void Syb_Stat(int gSybID[][MAX_SENTENCE_LEN], int gSybNo2[], int gSelect[], int *gn1, int *gn2, int *gn3)
{
	// 統計Syb分布
	for (int i = 0; i < HAKKA_PINYIN_MAX; i++) gSybNo2[i] = 0;
	for (int i = 0; i < MAX_SENTENCE_NO; i++)
	{
		if (gSelect[i] != 0)
		{
			for (int k = 0; k < MAX_SENTENCE_LEN; k++)
			{
				if ((gSybID[i][k] > 0) && (gSybID[i][k] < HAKKA_PINYIN_MAX)) gSybNo2[gSybID[i][k]]++;
			}
		}
	}

	// 列印 音節數量 與字數
	int n1 = 0, n2 = 0, n3 = 0;
	for (int i = 0; i < HAKKA_PINYIN_MAX; i++) if (gSybNo2[i] != 0) n1++;
	for (int i = 0; i < MAX_SENTENCE_NO; i++)
	{
		if (gSelect[i] != 0)
		{
			n3++;
			for (int k = 0; k < MAX_SENTENCE_LEN; k++)
			{
				if ((gSybID[i][k] > 0) && (gSybID[i][k] < HAKKA_PINYIN_MAX)) n2++;
			}
		}
	}
	//TRACE("挑選字數=%d  音節數=%d\n", n2, n1);
	*gn1 = n1; *gn2 = n2; *gn3 = n3;
}

/*
extern int Dic_F;
extern char Dic_File_Edu[100];
void Generate_Syb_Balance(int I)
{
	// #############################
// 產生各腔調700音平衡句
	int Beg = 0, End = 0;
	errno_t      err;
	FILE         *In, *Out;
	char         S1[2000];
	int          i, j, k, n, n1, n2, n3, Sent_No, Word_No;  // 0:四線，1:海陸，2:大埔，3:饒平，4:詔安，5:南四線
	wchar_t      S11[MAX_SENTENCE_LEN];
	wwchar_t     S21[MAX_SENTENCE_LEN];
	SENTENCE     Sent[MAX_SENTENCE_LEN];
	wwchar_t     WordUI[MAX_SENTENCE_LEN1];
	wchar_t      Word[MAX_SENTENCE_LEN1];
	wchar_t      Syb[MAX_SENTENCE_LEN1];
	wchar_t      CSyb[MAX_SENTENCE_LEN1];


	// 1.讀教育部詞庫
	if (Dic_F == 0)
	{
		Dic_F = 1;
		if ((i = Read_SybID()) < 0) printf(L"675音節表(PinYin_Table_四縣.txt)讀取失敗");
		if ((j = Read_Edu_Hakka_Dict_Hsf(Dic_File_Edu)) < 0) printf(L"辭庫(Edu_Dict_1110508_edu2.csv)讀取失敗");
		if ((i < 0) || (j < 0)) return;
		TRACE("詞庫讀完\n");
	}

	// 2.分析文本(若以分析過，就直接讀取分析結果)
	//char    File1[100] = { "Data/海陸腔語料文本_2021.06.30.txt" };
	char    File1[100] = { "Data/四縣（含南四縣）語料文本_2021.04.30_C.txt" };

	// 讀檔
	if ((err = fopen_s(&In, "SybID_200W_Table.dat", "rb")) == 0)
	{
		for (i = 0; i < MAX_SENTENCE_NO; i++)
		{
			fread(&Sentence[i][0], MAX_SENTENCE_LEN, sizeof(wchar_t), In);
			fread(&SybID[i][0], MAX_SENTENCE_LEN, sizeof(int), In);
		}
		fread(SybNo1, HAKKA_PINYIN_MAX, sizeof(int), In);
		fclose(In);
		TRACE("SybID讀完\n");
	}
	else
	{
		if ((err = fopen_s(&In, File1, "rb")) != 0) { printf(L"海陸腔語料文本_2021.06.30.txt Error"); return; }
		Sent_No = Word_No = 0;
		memset(SybID, 0, MAX_SENTENCE_NO*MAX_SENTENCE_LEN * sizeof(int));
		for (i = 0; i < HAKKA_PINYIN_MAX; i++) SybNo1[i] = SybNo2[i] = SybNo3[i] = 0;
		while ((!feof(In)) && (Sent_No < MAX_SENTENCE_NO))
		{
			// 讀句
			//i = Read_Unicode_0D0A(In, S11, MAX_SENTENCE_LEN);
			i = Read_Unicode_Punch(In, S11, MAX_SENTENCE_LEN);

			Word_No += wstrlen(S11);
			wstrcpy(&Sentence[Sent_No][0], MAX_SENTENCE_LEN, S11);


			// 斷詞
			UniCodeToUI(S11, S21, MAX_SENTENCE_LEN); // wchar_t 轉成 wwchar_t
			memset(Sent, 0, sizeof(SENTENCE)*MAX_SENTENCE_LEN);
			i = P2Gn(I, S21, Sent);

			// 紀錄
			for (i = 0; i < MAX_SENTENCE_LEN; i++)
			{
				j = Sent[i].SybID % 10000;
				if ((j > 0) && (j < HAKKA_PINYIN_MAX))
				{
					SybID[Sent_No][i] = j;	SybNo1[j]++;
				}
			}

			//Sent_Syb(I,Sent,WordUI,Syb,CSyb);
			// 寫檔: S11:文字，S12:PinYin，S13:注音
			//Write_PinYin(Out, II, Sent, JJ, KK);// S11, S12, S13, WLen, 3); // 3:詞間隔開列印, !=3:寫入原始文句
			Sent_No++;
			if ((Sent_No % 1000) == 0) TRACE("Sent_No=%d  Word_No=%d\n", Sent_No, Word_No);
		}
		fclose(In);
		TRACE("斷詞完畢\n");

		// 寫檔
		if ((err = fopen_s(&Out, "SybID_200W_Table.dat", "wb")) != 0)
		{
			printf(L"SybID_200W_Table.dat wb err"); return;
		}
		for (i = 0; i < MAX_SENTENCE_NO; i++)
		{
			fwrite(&Sentence[i][0], MAX_SENTENCE_LEN, sizeof(wchar_t), Out);
			fwrite(&SybID[i][0], MAX_SENTENCE_LEN, sizeof(int), Out);
		}
		fwrite(SybNo1, HAKKA_PINYIN_MAX, sizeof(int), Out);
		fclose(Out);
		TRACE("寫檔SybID完畢\n");
	}

	for (i = 0; i < MAX_SENTENCE_NO; i++) Select[i] = 0;
	// 低於 MIN_SYB_NO 句子優先挑
	for (i = 0; i < HAKKA_PINYIN_MAX; i++)
		if ((SybNo1[i] > 0) && (SybNo1[i] < 4))
		{
			for (k = 0; k < MAX_SENTENCE_NO; k++)
			{
				for (n = 0; n < MAX_SENTENCE_LEN; n++)
				{
					if (SybID[k][n] == i) { Select[k] = 1; break; }
				}
			}
		}

	// 統計Syb分布
	Syb_Stat(SybID, SybNo2, Select, &n1, &n2, &n3);
	TRACE("挑選 音節(Syb)數=%d   字(Word)數=%d   句(Sent)數=%d\n", n1, n2, n3);

	// 挑剩下CP值最高(補空Syb最多)的
	int max_i, max_n1, max_n2, Rept = 0, Min_Bound = 0;
	float r1, max_r1;


	for (Min_Bound = 0; Min_Bound <= 4; Min_Bound++)
	{
		while (Rept < 2000)
		{
			max_r1 = 0;
			for (i = 0; i < MAX_SENTENCE_NO; i++)
			{
				if (Select[i] == 0)
				{
					n1 = n2 = 0;
					for (j = 0; j < MAX_SENTENCE_LEN; j++)
					{
						if ((SybID[i][j] > 0) && (SybID[i][j] < HAKKA_PINYIN_MAX))
						{
							if (SybNo2[SybID[i][j]] <= Min_Bound) n1++;
							n2++;

						}
					}
					r1 = (float)n1 / (float)n2;
					if (max_r1 < r1)
					{
						max_r1 = r1; max_i = i;
						max_n1 = n1; max_n2 = n2;
					}
				}
			}

			Select[max_i] = 1;
			Syb_Stat(SybID, SybNo2, Select, &n1, &n2, &n3);
			Rept++;
			UniCodeToBig5(&Sentence[max_i][0], S1, 2 * wstrlen(&Sentence[max_i][0]));
			TRACE("Min_Bound=%d Rept=%d 挑選 音節(Syb)數=%d   字(Word)數=%d   句(Sent)數=%d  max_r1=%f %d %d %s\n", Min_Bound, Rept, n1, n2, n3, max_r1, max_n1, max_n2, S1);

			if (max_r1 == 0) break; // 已經找不到，中斷 while
		}
	}


	// 確保每個音在 Min_Bound=5 以上

	// 存檔
	if ((err = fopen_s(&Out, "Balance_675.txt", "wb")) != 0)
	{
		printf(L"Balance_675Syb.txt err"); return;
	}
	S11[0] = 0xFEFF; fwrite(S11, 1, 2, Out);
	for (i = 0; i < MAX_SENTENCE_NO; i++)
	{
		if (Select[i])
		{
			// 斷詞
			// 0:四線，1:海陸，2:大埔，3:饒平，4:詔安，5:南四線
			//I = 0; 
			wstrcpy(S11, 2000, &Sentence[i][0]);

			// 消除 <...>
			Beg = End = -1;
			n = wstrlen(S11);
			for (j = 0; j < n; j++)
			{
				if ((S11[j] == L'<') || (S11[j] == L'＜'))
					Beg = j;
				else if ((S11[j] == L'>') || (S11[j] == L'＞')) End = j;
				if ((End >= Beg) && (Beg >= 0))
				{
					for (k = End + 1; k <= n; k++) S11[Beg + (k - End - 1)] = S11[k];
					n -= End - Beg + 1; j = Beg - 1; Beg = End = -1;
				}
			}

			// 消除\r\n
			n = wstrlen(S11);
			for (j = 0; j < n; j++)
				if ((S11[j] == 0x0a) || (S11[j] == 0x0d) ||
					(S11[j] == L'　') ||
					((S11[j] >= L'Ａ') && (S11[j] <= L'Ｚ')) ||
					((S11[j] >= L'A') && (S11[j] <= L'Z')) ||
					((S11[j] >= L'a') && (S11[j] <= L'z'))
					)
				{
					for (k = j + 1; k <= n; k++) S11[k - 1] = S11[k];
					n--; j--;
				}


			UniCodeToUI(S11, S21, MAX_SENTENCE_LEN); // wchar_t 轉成 wwchar_t
			memset(Sent, 0, sizeof(SENTENCE)*MAX_SENTENCE_LEN);
			j = P2Gn(I, S21, Sent);
			Sent_Syb(I, Sent, WordUI, Syb, CSyb);
			// 寫檔: S11:文字，S12:PinYin，S13:注音
			Write_PinYin(Out, I, Sent, 0, 1);// 0: roma, 1:注音, 2:兩者,  0:.txt, 1:錄音用 // 3:詞間隔開列印, !=3:寫入原始文句

		}
	}
	fclose(Out);

}
*/