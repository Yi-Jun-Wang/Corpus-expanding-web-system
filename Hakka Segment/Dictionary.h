#pragma once

// ######################################
// ###  客語詞庫處理 
#define MAX_DICT_NUM      50000  // 最大詞數量上限
#define MAX_WORD_LEN      15     // 最大詞長
#define MAX_ROMA_LEN      15     // 最大羅馬拼音長度
int Dict_Num = 0;                // 詞數量
struct DICT  // 客語詞資料結構
{	// 詞彙屬性
	int     No;                    // 第幾個詞
	wchar_t Pos[MAX_WORD_LEN];     // 詞類
	int     Freq;     // 詞頻(被比對過就+1)
	int     Freq1;    // 詞頻(最後斷詞結果，再+1)
	int     Freq2;    // 詞頻(測試統計用)
	int     Freq3;    // 詞頻(測試統計用)

	// 詞彙 Unicode
	int     Wrd_Num;             // 詞之音節(客語字)數量
	int     Wrd_Num1;            // 詞之音節(客語字)數量+特殊造字(2Bytes,in D800~DF00)
	wchar_t  Word[MAX_WORD_LEN];  // 詞之字Unicode Code 詞目
	wwchar_t WordUI[MAX_WORD_LEN]; // 詞之字雙Unicode Code

	// "四海大平安南"腔發音
	int     Syb_Len[6];                            // =wstrlen(Syb)
	wchar_t Syb[6][MAX_WORD_LEN*MAX_ROMA_LEN];     // 詞之音節羅馬拼音
	wchar_t SybRoma[6][MAX_WORD_LEN][MAX_ROMA_LEN];// 詞之拼音代碼
	wchar_t SybTone[6][MAX_WORD_LEN][MAX_ROMA_LEN];// 詞之聲調代碼
	int     SybID[6][MAX_WORD_LEN];                // 詞之音節代碼

	// 華客對照
	wchar_t Chinese[MAX_WORD_LEN * 2];
	wchar_t Note[MAX_WORD_LEN];
} Dict[MAX_DICT_NUM];


// #################################################################
// 破音字處理: 讀取詞庫時，順便統計破音字
// void Init_Poin()
// int Search_Poin(wwchar_t Word)
// int Insert_Poin(wwchar_t Word,wchar_t Syb[][225]) MAX_WORD_LEN*MAX_ROMA_LEN

#define MAX_POIN1  7000
struct POIN
{
	int      Flag;
	wwchar_t Word;
	wchar_t  Syb[6][10][20];
	int      SybNo[6];
}Poin[MAX_POIN1];
int Poin_No1 = 0;

void Init_Poin()
{
	memset(Poin, 0, MAX_POIN1 * sizeof(POIN));
}

int Search_Poin(wwchar_t Word)
{
	int  i;
	for (i = 0; i < MAX_POIN1; i++) if ((Poin[i].Flag) && (Poin[i].Word == Word)) break;
	if (i < MAX_POIN1) return i;
	else return -1; // 找不到
}

int Insert_Poin(wwchar_t Word, wchar_t Syb[][225])
{
	int  i, j, I;
	i = Search_Poin(Word);
	if (i >= 0) // 詞已存在
	{
		for (I = 0; I < 6; I++)
		{
			if (wstrlen(&Syb[I][0]) > 0) // 有拚音
			{
				for (j = 0; j < Poin[i].SybNo[I]; j++)
					if (!wstrcmp(&Poin[i].Syb[I][j][0], &Syb[I][0])) break;
				if (j >= Poin[i].SybNo[I]) // 不存在，新發音，新增
				{
					wstrcpy(&Poin[i].Syb[I][Poin[i].SybNo[I]][0], 20, &Syb[I][0]);
					Poin[i].SybNo[I]++;
				}
			}
		}
	}
	else // 新增詞
	{
		for (i = 0; i < MAX_POIN1; i++) if (!Poin[i].Flag) break;

		// Buffer Full
		if (i >= MAX_POIN1) return -2;
		// First Insert
		else
		{
			Poin[i].Flag = 1;
			Poin[i].Word = Word; Poin_No1++;
			for (I = 0; I < 6; I++)
			{
				if (wstrlen(&Syb[I][0]) > 0) // 有拚音
				{
					wstrcpy(&Poin[i].Syb[I][0][0], 20, &Syb[I][0]);
					Poin[i].SybNo[I] = 1;
				}
			}
		}
	}

}

// ##########################################################################
// 讀取詞庫、統計、搜尋、儲存
// int  Add_1Word(char *File); 新增一字詞(二字以上字存在，一字詞不存在
// int  Read_Edu_Hakka_Dict_Hsf(char *File); // Data/Edu_Dict_1110508_edu2.csv
// void Statistic_Syb_Tone(char *File)
// int  Save_Word1_Need(wchar_t *S11)
// int  Save_Edu_Dict(char *File)
// 

int TableX[65536]; // 調整後一字詞(破音字)
int TableX2[65536]; // 0xD800~DFFF + wchar_t

int Table1[65536]; // 一字詞 字數量出現數量統計
int TableN[65536]; // 多字詞 字數量出現數量統計
int Table1U[65536]; // 一字詞 字數量出現數量統計(0xD800~DFFF)
int TableNU[65536]; // 多字詞 字數量出現數量統計(0xD800~DFFF)

int Add_1Word(char *File)
{
	DICT  *p0, *p1, *p2;
	int   i, j, k, I, jj, DN;


	DN = Dict_Num;// p->No; // DN=現有詞數量
	p0 = &Dict[0];// p - DN;// p0=指向第0筆詞
	for (i = 0; i < 65536; i++) Table1[i] = TableN[i] = TableX[i] = 0;

	// === 針對四縣腔統計一字詞(Table1)與多字詞(TableN)中文字分佈 ===
	// 統計一字詞與多字詞之中文字出現統計(不含特殊字)
	p1 = p0;
	for (i = 0; i < DN; i++)
	{
		if (p1->Wrd_Num == 1)
		{
			if (p1->WordUI[0] < 0x10000) Table1[p1->WordUI[0]]++;
		}
		else
		{
			for (j = 0; j < p1->Wrd_Num; j++)
				if (p1->WordUI[j] < 0x10000)
					TableN[p1->WordUI[j]]++;
			//else

		}
		p1++;
	}

	// === 新增一字詞 ===
	errno_t err;
	FILE    *Out;
	int Max, Maxj;
	char     S1[100];
	wchar_t  S11[100];
	if ((err = fopen_s(&Out, File, "wb")) != 0) { return -1; }
	S11[0] = 0xFEFF; fwrite(S11, 1, 2, Out); // Unicode-LE(0xFEFF)

	I = 0;
	p1 = &Dict[Dict_Num];
	for (i = 0; i < 65536; i++) // 先找2Bytes Word, 暫不考慮 0xD800~0xDFFF
		if ((Table1[i] == 0) && (TableN[i] != 0))
		{   // 統計發音
			Init_Syb();
			p2 = &Dict[0];// p0;
			DN = Dict_Num;// p->No;
			for (j = 0; j < DN; j++)
			{
				for (k = 0; k < p2->Wrd_Num; k++)
					if (p2->WordUI[k] == (wwchar_t)i)
					{
						wstrcpy(S11, 100, &p2->SybRoma[I][k][0]);
						wstrcat(S11, 100, &p2->SybTone[I][k][0]);
						Insert_Syb(S11);
					}
				p2++;
			}
			// 找尋最多發音
			for (jj = 0; jj < Syb_Num; jj++)
			{
				p2 = &Dict[0]; DN = Dict_Num;// (*p)->No;
				for (j = 0; j < DN; j++)
				{   // 找出二字詞中一樣的中文字i，與Syb_Table[jj]一樣的發音
					for (k = 0; k < p2->Wrd_Num; k++)
					{
						wstrcpy(S11, 100, &p2->SybRoma[I][k][0]);
						wstrcat(S11, 100, &p2->SybTone[I][k][0]);
						if ((p2->WordUI[k] == (wwchar_t)i) && (!wstrcmp(S11, &Syb_Table[jj][0]))) break;
					}
					//新增一字詞
					if (k < p2->Wrd_Num)
					{
						// POS, Freq, Chinese, Note
						wstrcpy(p1->Pos, MAX_WORD_LEN - 1, p2->Pos);
						wstrcpy(p1->Chinese, MAX_WORD_LEN - 1, p2->Chinese);
						wstrcpy(p1->Note, MAX_WORD_LEN - 1, L"新增");
						p1->Freq = p2->Freq;
						p1->Freq1 = p2->Freq1;
						p1->Freq2 = p2->Freq2;
						p1->Freq3 = p2->Freq3;
						// Word
						p1->Wrd_Num = p1->Wrd_Num1 = 1;
						p1->Word[0] = (wchar_t)i;
						p1->WordUI[0] = (wwchar_t)i;

						// Syb[6]
						for (int ii = 0; ii < 6; ii++)
						{
							wstrcpy(&p1->SybRoma[ii][0][0], MAX_WORD_LEN - 1, &p2->SybRoma[ii][k][0]);
							wstrcpy(&p1->SybTone[ii][0][0], MAX_WORD_LEN - 1, &p2->SybTone[ii][k][0]);
							wstrcpy(&p1->Syb[ii][0], MAX_WORD_LEN - 1, &p1->SybRoma[ii][0][0]);
							wstrcat(&p1->Syb[ii][0], MAX_WORD_LEN - 1, &p1->SybTone[ii][0][0]);
							p1->Syb_Len[ii] = wstrlen(&p1->Syb[ii][0]);

							p1->SybID[ii][0] = watoi(&p1->SybTone[ii][0][0]) * 10000 + j;
						}

						// 寫入中文字
						fwrite(p1->Word, 2, wstrlen(p1->Word), Out);
						S11[0] = L'\t'; fwrite(S11, 2, 1, Out);

						// 寫入Fre
						sprintf_s(S1, "%d", p1->Freq);
						Big5ToUniCode(S1, S11, strlen(S1));
						fwrite(S11, 2, wstrlen(S11), Out);
						S11[0] = L'\t'; fwrite(S11, 2, 1, Out);

						// 寫入POS
						fwrite(p1->Pos, 2, wstrlen(p1->Pos), Out);
						S11[0] = L'\t'; fwrite(S11, 2, 1, Out);

						for (int ii = 0; ii < 6; ii++)
						{
							wstrcpy(&p1->SybRoma[ii][0][0], MAX_WORD_LEN - 1, &p2->SybRoma[ii][k][0]);
							wstrcpy(&p1->SybTone[ii][0][0], MAX_WORD_LEN - 1, &p2->SybTone[ii][k][0]);
							wstrcpy(&p1->Syb[ii][0], MAX_WORD_LEN - 1, &p1->SybRoma[ii][0][0]);
							wstrcat(&p1->Syb[ii][0], MAX_WORD_LEN - 1, &p1->SybTone[ii][0][0]);
							p1->Syb_Len[ii] = wstrlen(&p1->Syb[ii][0]);

							if (wstrlen(&p1->Syb[ii][0]) > 0)
								fwrite(&p1->Syb[ii][0], wstrlen(&p1->Syb[ii][0]), 2, Out);
							else
							{
								//S11[0] = L'~'; S11[1] = 0x0000;
								wstrcpy(S11, 100, L"~");	fwrite(S11, wstrlen(S11), 2, Out);
							}
							S11[0] = L'\t'; fwrite(S11, 1, 2, Out);
						}

						// 寫入Chinese
						fwrite(p1->Chinese, 2, wstrlen(p1->Chinese), Out);
						S11[0] = L'\t'; fwrite(S11, 2, 1, Out);

						// 寫入Note
						fwrite(p1->Note, 2, wstrlen(p1->Note), Out);
						S11[0] = L'\t'; fwrite(S11, 2, 1, Out);

						// 跳行
						wstrcpy(S11, 100, L"\r\n");	fwrite(S11, wstrlen(S11), 2, Out);
						//S11[0] = 0x000d; fwrite(S11, 1, 2, Out);
						Dict_Num++; p1 = &Dict[Dict_Num]; //*p++;
						j = Dict_Num; // 結束回圈 j
						//break;
					}
					p2++;
				}
			}
		}

	fclose(Out);

	// 統計調整後一字詞數量(不含特殊字0xD800~DFFF)
	/*
	for (i = 0; i < Dict_Num; i++)
	{	p = &Dict[i];
		if (p->Wrd_Num == 1) if ((p->Word[0] < 0xD800) || (p->Word[0] > 0xDFFF)) TableX[p->Word[0]]++;
	}
	*/

	return 0;

}


#define MAX_SENTENCE_LEN1 2000
int Read_Edu_Hakka_Dict_Hsf(char *File)
{
	errno_t err;
	FILE    *In;
	char    S1[200], S2[200];
	static wchar_t S11[MAX_SENTENCE_LEN1], S12[MAX_SENTENCE_LEN1];
	int     i, j, k, I, Len, L1[400], L2[400];
	DICT    *p;

	// 讀取六腔調音節，並產生六腔音節表 
	// 讀取 Data/PinYin_Table_四縣.txt，四縣675音節拼音檔
    // 產出 Data/Syllable_Table_1.h
    // 產出 Data/Syllable_Table_unicode.h
	if ((i = Read_SybID()) < 0)    return -1;//                      SetWindowText(L"675音節表(PinYin_Table_四縣.txt)讀取失敗");
	Init_Poin();


	//sprintf_s(S1, File, DIR100);
	if ((err = fopen_s(&In, File, "rb")) != 0) return -321;
	fread(S11, 1, 2, In);

	Dict_Num = 0;
	while (!feof(In))
	{
		// 標訂欄位
		p = &Dict[Dict_Num++]; p->No = Dict_Num - 1;
		Read_Unicode_0D0A(In, S11, MAX_SENTENCE_LEN1);
		for (j = 0, i = 0; i < wstrlen(S11); i++) if (S11[i] == L'\t')	L1[j++] = i;
		for (k = j; k < 200; k++) L1[k] = i;

		// Get Word
		wstrncpy(p->Word, &S11[0], L1[0]);
		Len = wstrlen(p->Word); // 詞目
		i = j = 0;
		while (i < Len)
		{
			if ((p->Word[i] >= 0xD800) && (p->Word[i] <= 0xDFFF))
			{
				p->WordUI[j] = ((unsigned int)p->Word[i] << 16) + (unsigned int)p->Word[i + 1];
				j++; i += 2;
			}
			else
			{
				p->WordUI[j] = (unsigned int)p->Word[i];
				j++; i++;
			}
		}
		p->WordUI[j] = 0;
		p->Wrd_Num = j;
		p->Wrd_Num1 = Len;

		//if (!wstrcmp(p->Word, L"鴨公聲"))
		//	i = i;


		// Get Freq
		wstrncpy(S12, &S11[L1[0] + 1], L1[1] - L1[0] - 1);
		p->Freq = watoi(S12);

		// Get POS
		wstrncpy(p->Pos, &S11[L1[1] + 1], L1[2] - L1[1] - 1);
		// 將\r\n換成、
		Len = wstrlen(p->Pos);
		for (i = 0; i < Len; i++)
		{
			if ((p->Pos[i] == L'\r') || (p->Pos[i] == L'\n')) p->Pos[i] = L'、';
		}
		for (i = 0; i < Len - 1; i++)
		{
			if ((p->Pos[i] == L'、') && (p->Pos[i + 1] == L'、'))
			{
				for (j = i + 2; j < Len; j++) p->Pos[j - 1] = p->Pos[j];
				Len--;		p->Pos[Len] = 0;
			}
		}

		// Get Syb*6
		for (I = 0; I < 6; I++)
		{
			p->Syb_Len[I] = L1[3 + I] - L1[2 + I] - 1;
			wstrncpy(&p->Syb[I][0], &S11[L1[2 + I] + 1], p->Syb_Len[I]);


			// 1.5.將音節聲調 Copy 進入 SybRoma & SybTone
			L2[0] = -1;
			for (j = 1, i = 0; i < p->Syb_Len[I]; i++) if (p->Syb[I][i] == 0x0020) L2[j++] = i;
			L2[j] = p->Syb_Len[I];
			for (i = 0; i < j; i++)
			{
				Len = L2[i + 1] - L2[i] - 1;
				wstrncpy(&p->SybRoma[I][i][0], &p->Syb[I][L2[i] + 1], Len);
				for (k = 0; k < Len; k++)
					if ((p->SybRoma[I][i][k] >= L'0') && (p->SybRoma[I][i][k] <= L'9')) break;
				//p->SybTone[I][i] = 0;
				if ((k < Len) && (Len > 0))
					for (int n = k; n < Len; n++) p->SybTone[I][i][n - k] = p->SybRoma[I][i][n];// -L'0')*pow(10, Len - n - 1);
				p->SybTone[I][i][Len - k] = 0x0000;
				p->SybRoma[I][i][k] = 0x0000;
				// 找出音節代碼
				for (k = 0; k < HAKKA_PINYIN_MAX; k++)
					//if (!wstrcmp(&p->SybRoma[I][i][0], Hakka_Syb_Table[I][k].Syb)) break;
					//if (!wstrcmp(&p->SybRoma[I][i][0], Hakka_Syb_Table[6][k].Syb)) break; // 使用四海大平安總表共1044音，以避免各腔調缺音
					if (!wstrcmp(&p->SybRoma[I][i][0], Hakka_Syb_Table[6][k].Syb)) break; // 使用四海大平安總表共1044音，以避免各腔調缺音
				if (k < HAKKA_PINYIN_MAX) p->SybID[I][i] = k + watoi(&p->SybTone[I][i][0]) * 10000;
				else
				{
					UniCodeToBig5(&p->SybRoma[I][i][0], S1, wstrlen(&p->SybRoma[I][i][0]));
					UniCodeToBig5(&p->SybTone[I][i][0], S2, wstrlen(&p->SybTone[I][i][0]));
					TRACE("I=%d Roma=%s Tone=%s error\n", I, S1, S2);
					p->SybID[I][i] = -1;
				}
			}
		}

		// #################
		// 分析統計破音字
		i = wwstrlen(p->WordUI);
		if (i == 1)
			Insert_Poin(p->WordUI[0], p->Syb);



		// Get Chinese
		wstrncpy(p->Chinese, &S11[L1[8] + 1], L1[9] - L1[8] - 1);

		// Get Note
		wstrncpy(p->Note, &S11[L1[9] + 1], L1[10] - L1[9] - 1);

	}
	fclose(In);

	//p = &Dict[Dict_Num]; p->No = Dict_Num;
	if ((i = Add_1Word("Add_1Word.csv")) < 0)  return -11;// SetWindowText(L"寫入一字詞 Add_1Word.csv 失敗");

	return 0;
}



// 統計六腔調間差異性
void Statistic_Syb_Tone(char *File)
{
	int     I;
	DICT *p;
	int  i, j, k, l, n;
	int  Tab[6][6][4]; // 0:音and調皆不同, 1:音不同, 2.調不同, 3.音不同或調不同,
	int  Len[30];
	int  Syb_Empty[10] = { 0,0,0,0,0,0,0,0,0,0 }; // 統計六腔調無發音數量
	int Table1[65536], TableN[65536];


	for (i = 0; i < 65536; i++) Table1[i] = TableN[i] = 0;
	for (i = 0; i < 30; i++)    Len[i] = 0;

	// 統計詞長分布
	for (i = 0; i < Dict_Num; i++) Len[Dict[i].Wrd_Num]++;

	// 統計所有詞長之中文字出現分布
	// 統計一字詞  之中文字出現分布
	for (i = 0; i < Dict_Num; i++)
	{
		p = &Dict[i];
		for (j = 0; j < p->Wrd_Num; j++) TableN[p->Word[j]]++;
		if (p->Wrd_Num == 1) Table1[p->Word[0]]++;
		// 1.4.統計各腔調有字無音數量
		for (I = 0; I < 6; I++) if (p->Syb_Len[I] == 0) Syb_Empty[I]++;
	}
	int A = 0, B = 0;
	for (i = 0; i < 65536; i++)
	{
		if (Table1[i] != 0) A++; // 一字詞使用中文字數量=A
		if (TableN[i] != 0) B++; // 多字詞使用中文字數量=B
	}


	// 統計六腔調間差異性
	for (i = 0; i < 6; i++)
		for (j = 0; j < 6; j++)
			for (k = 0; k < 4; k++) Tab[i][j][k] = 0;

	for (n = 0; n < Dict_Num; n++)
	{
		p = &Dict[n];
		for (i = 0; i < 6; i++)
			for (j = 0; j < 6; j++)
			{
				for (k = 0; k < p->Wrd_Num; k++) if (wstrcmp(&p->SybTone[i][k][0], &p->SybTone[j][k][0])) break;
				for (l = 0; l < p->Wrd_Num; l++) if (wstrcmp(&p->SybRoma[i][l][0], &p->SybRoma[j][l][0])) break;
				if ((k < p->Wrd_Num) && (l < p->Wrd_Num)) Tab[i][j][0]++;
				if (l < p->Wrd_Num)  Tab[i][j][1]++;
				if (k < p->Wrd_Num)                      Tab[i][j][2]++;
				if ((k < p->Wrd_Num) || (l < p->Wrd_Num)) Tab[i][j][3]++;
			}
	}

	// 統計南北四縣差異(字)
	int S, T, Syb_Diff[4] = { 0,0,0,0, }; // 依字: 0:相同，1:Syb不同，
	int SS, TT, Syb_Diff1[4] = { 0,0,0,0, }; // 依詞: 0:相同，1:Syb不同，
	for (n = 0; n < Dict_Num; n++)
	{
		p = &Dict[n];
		i = 0; j = 5;
		SS = TT = 0;
		for (k = 0; k < p->Wrd_Num; k++)
		{
			S = T = 0;
			if (wstrcmp(&p->SybTone[i][k][0], &p->SybTone[j][k][0])) { S = SS = 1; }
			if (wstrcmp(&p->SybRoma[i][k][0], &p->SybRoma[j][k][0])) { T = TT = 1; }
			if ((!S) && (!T)) Syb_Diff[0]++;
			else if ((S) && (!T)) Syb_Diff[1]++;
			else if ((!S) && (T))  Syb_Diff[2]++;
			else if ((S) && (T))  Syb_Diff[3]++;
		}
		if ((!SS) && (!TT)) Syb_Diff1[0]++;
		else if ((SS) && (!TT)) Syb_Diff1[1]++;
		else if ((!SS) && (TT))  Syb_Diff1[2]++;
		else if ((SS) && (TT))  Syb_Diff1[3]++;
	}


	// 產生統計檔
	errno_t err;
	FILE    *Out;
	err = fopen_s(&Out, File, "wb");
	char S123[4][100] = { "音與調皆不同數量統計", "音不同數量統計", "調不同數量統計","音或調不同數量統計" };
	char S124[6][100] = { " 四縣 "," 海陸 "," 大埔 "," 饒平 "," 詔安 ","南四縣" };
	for (k = 0; k < 4; k++)
	{
		fprintf(Out, "\n\n == %s ==\n", &S123[k][0]);
		fprintf(Out, "       |     四縣      |      海陸     |      大埔     |      饒平     |       詔安    |      南四縣\n");
		for (i = 0; i < 6; i++)
		{
			fprintf(Out, "%s | ", &S124[i][0]);
			for (j = 0; j < 6; j++)
				fprintf(Out, " %5d(%5.2f%c), ", Tab[i][j][k], (float)Tab[i][j][k] * 100 / (float)Dict_Num, '%');
			fprintf(Out, "\n");
		}
	}
	fprintf(Out, "\n\n == 音調空白統計 ==\n");
	fprintf(Out, "|     四縣      |      海陸     |      大埔     |      饒平     |       詔安    |      南四縣\n");
	for (i = 0; i < 6; i++)	fprintf(Out, "      %5d,    ", Syb_Empty[i]);
	fprintf(Out, "\n");

	fprintf(Out, "\n\n == 詞長分布統計(詞總數=%d) ==\n", Dict_Num);
	for (i = 1; i < 30; i++)	fprintf(Out, "Word_Len[%d]=%d\n", i, Len[i]);
	fprintf(Out, "\n");

	fprintf(Out, "\n\n == 詞長分布統計(詞總數=%d) ==\n", Dict_Num);
	fprintf(Out, "一字詞數量=%d\n", A);
	fprintf(Out, "詞庫中文字出現數量=%d\n", B);

	// 南北四縣統計差異
	fprintf(Out, "\n\n == 南北四縣統計差異(字) ==\n");
	S = Syb_Diff[0] + Syb_Diff[1] + Syb_Diff[2] + Syb_Diff[3];
	fprintf(Out, "音同且調同數量= %6d(%10.3f)\r\n", Syb_Diff[0], (float)Syb_Diff[0] / (float)S);
	fprintf(Out, "音異且調同數量= %6d(%10.3f)\r\n", Syb_Diff[1], (float)Syb_Diff[1] / (float)S);
	fprintf(Out, "音同且調異數量= %6d(%10.3f)\r\n", Syb_Diff[2], (float)Syb_Diff[2] / (float)S);
	fprintf(Out, "音異且調異數量= %6d(%10.3f)\r\n", Syb_Diff[3], (float)Syb_Diff[3] / (float)S);
	fprintf(Out, "音異或調異數量= %6d(%10.3f)\r\n", (Syb_Diff[1] + Syb_Diff[2] + Syb_Diff[3]), (float)(Syb_Diff[1] + Syb_Diff[2] + Syb_Diff[3]) / (float)S);
	fprintf(Out, "音異數量      = %6d(%10.3f)\r\n", (Syb_Diff[1] + Syb_Diff[3]), (float)(Syb_Diff[1] + Syb_Diff[3]) / (float)S);
	fprintf(Out, "調異數量      = %6d(%10.3f)\r\n", (Syb_Diff[2] + Syb_Diff[3]), (float)(Syb_Diff[2] + Syb_Diff[3]) / (float)S);

	fprintf(Out, "\n\n == 南北四縣統計差異(詞) ==\n");
	S = Syb_Diff1[0] + Syb_Diff1[1] + Syb_Diff1[2] + Syb_Diff1[3];
	fprintf(Out, "音同且調同數量= %6d(%10.3f)\r\n", Syb_Diff1[0], (float)Syb_Diff1[0] / (float)S);
	fprintf(Out, "音異且調同數量= %6d(%10.3f)\r\n", Syb_Diff1[1], (float)Syb_Diff1[1] / (float)S);
	fprintf(Out, "音同且調異數量= %6d(%10.3f)\r\n", Syb_Diff1[2], (float)Syb_Diff1[2] / (float)S);
	fprintf(Out, "音異且調異數量= %6d(%10.3f)\r\n", Syb_Diff1[3], (float)Syb_Diff1[3] / (float)S);
	fprintf(Out, "音異或調異數量= %6d(%10.3f)\r\n", (Syb_Diff1[1] + Syb_Diff1[2] + Syb_Diff1[3]), (float)(Syb_Diff1[1] + Syb_Diff1[2] + Syb_Diff1[3]) / (float)S);
	fprintf(Out, "音異數量      = %6d(%10.3f)\r\n", (Syb_Diff1[1] + Syb_Diff1[3]), (float)(Syb_Diff1[1] + Syb_Diff1[3]) / (float)S);
	fprintf(Out, "調異數量      = %6d(%10.3f)\r\n", (Syb_Diff1[2] + Syb_Diff1[3]), (float)(Syb_Diff1[2] + Syb_Diff1[3]) / (float)S);


	fclose(Out);

}



// 處理字典中缺乏的一字詞，並記錄儲存
#define WORD1_NEED_MAX 2000
FILE    *Out123 = 0;
wchar_t Word1_Need[WORD1_NEED_MAX];
int     Word1_Need_Num = 0;

int Save_Word1_Need(wchar_t *S11)
{
	errno_t err;
	int     j, n, k;
	wchar_t SS[100];
	// 紀錄字典沒有的一字詞
	if (Out123 == 0)
	{
		err = fopen_s(&Out123, "Data/1Word_Need.txt", "wb");
		if (err != 0) { return -187; }
		SS[0] = 0xFEFF;  fwrite(SS, 1, 2, Out123); // UniCode Little Endian Mode
	}
	if (Out123 != 0)
	{
		n = 2;
		if ((S11[0] >= 0xD800) && (S11[0] <= 0xDFFF)) n = 4;  // 特殊字
		for (k = 0; k < Word1_Need_Num; k++)
		{
			if (!memcmp(S11, &Word1_Need[k], n)) break;
		}
		if (k >= Word1_Need_Num)
		{
			fwrite(S11, 1, n, Out123);
			if ((Word1_Need_Num % 30) == 29) { SS[0] = 0x000d; fwrite(SS, 1, 2, Out123); }
			for (j = 0; j < n / 2; j++) Word1_Need[Word1_Need_Num++] = S11[j];
		}
	}
	return 0;
}



int Save_Edu_Dict(char *File)
{
	FILE    *Out;
	errno_t err;
	wchar_t S11[100];
	char    S1[100], S2[100];
	DICT    *p, *p1, *p2, q;
	int     i, j, k, n, I;

	// 1.修飾拼音格式
	for (i = 0; i < Dict_Num; i++)
	{
		p1 = &Dict[i];
		for (I = 0; I < 6; I++)
			if (p1->Syb_Len[I] > 0)
			{
				// 0xA0=160 => 空格
				for (j = 0; j < p1->Syb_Len[I]; j++) if (p1->Syb[I][j] == 160) p1->Syb[I][j] = L' ';

				// 去頭:空白L' '
				for (j = 0; j < p1->Syb_Len[I]; j++)
				{
					if ((p1->Syb[I][j] != L' ') && (p1->Syb[I][j] != L'　') && (p1->Syb[I][j] != L'\t')) break;
					for (k = j + 1; k <= p1->Syb_Len[I]; k++)
						p1->Syb[I][k - 1] = p1->Syb[I][k];
					p1->Syb_Len[I]--;
				}
				// 去尾:空白L' '
				for (j = p1->Syb_Len[I] - 1; j >= 0; j--)
				{
					if ((p1->Syb[I][j] != L' ') && (p1->Syb[I][j] != L'　') && (p1->Syb[I][j] != L'\t')) break;
					p1->Syb[I][j] = 0;
					p1->Syb_Len[I]--;
				}
				// 去中間重複空白
				n = 1;
				while (n == 1)
				{
					n = 0;
					for (j = 0; j < p1->Syb_Len[I] - 1; j++)
					{
						if ((p1->Syb[I][j] == L' ') && (p1->Syb[I][j + 1] == L' '))
						{
							for (k = j + 1; k <= p1->Syb_Len[I]; k++)
								p1->Syb[I][k - 1] = p1->Syb[I][k];
							p1->Syb_Len[I]--;
							n = 1;
						}
					}
				}
				// 檢查格式
				for (j = 0; j < p1->Syb_Len[I]; j++)
				{
					if (((p1->Syb[I][j] < L'0') || (p1->Syb[I][j] > L'z')) && (p1->Syb[I][j] != L' '))
					{
						UniCodeToBig5(p1->Word, S1, wstrlen(p1->Word));
						UniCodeToBig5(&p1->Syb[I][0], S2, wstrlen(&p1->Syb[I][0]));
						TRACE("詞發音有錯: 詞=%s I=%d Syb=%s\n", S1, I, S2);
					}
				}
			}
	}
	// 2.刪除重複詞
	for (i = 0; i < Dict_Num - 1; i++)
	{
		p1 = &Dict[i];
		if (p1->Wrd_Num > 0)
		{
			for (j = i + 1; j < Dict_Num; j++)
			{
				p2 = &Dict[j];
				if ((i != j) && (p2->Wrd_Num > 0))
				{
					// 詞相同
					if (!wwstrcmp(p1->WordUI, 15, p2->WordUI))
					{
						// 六腔音完全相同
						if ((!wstrcmp(&p1->Syb[0][0], &p2->Syb[0][0])) &&
							(!wstrcmp(&p1->Syb[1][0], &p2->Syb[1][0])) &&
							(!wstrcmp(&p1->Syb[2][0], &p2->Syb[2][0])) &&
							(!wstrcmp(&p1->Syb[3][0], &p2->Syb[3][0])) &&
							(!wstrcmp(&p1->Syb[4][0], &p2->Syb[4][0])) &&
							(!wstrcmp(&p1->Syb[5][0], &p2->Syb[5][0])))
						{
							if ((wstrlen(p1->Chinese) == 0) && (wstrlen(p2->Chinese) > 0)) wstrcpy(p1->Chinese, MAX_WORD_LEN * 2, p2->Chinese);
							if ((wstrlen(p1->Note) == 0) && (wstrlen(p2->Note) > 0)) wstrcpy(p1->Note, MAX_WORD_LEN, p2->Note);
							if ((wstrlen(p1->Pos) == 0) && (wstrlen(p2->Pos) > 0)) wstrcpy(p1->Pos, MAX_WORD_LEN, p2->Pos);
							if (p1->Freq < p2->Freq) p1->Freq = p2->Freq;
							p2->Wrd_Num = 0;
						}

						// 六腔音有無互補
						if (!wstrcmp(p1->Word, L"鯽魚嘴"))
							k = 0;
						for (I = 0; I < 6; I++)
							if ((p1->Syb_Len[I] > 0) && (p2->Syb_Len[I] > 0) && (wstrcmp(&p1->Syb[I][0], &p2->Syb[I][0]))) break;
						if (I >= 6)
						{
							for (I = 0; I < 6; I++)
								if (p2->Syb_Len[I] > 0)
									wstrcpy(&p1->Syb[I][0], MAX_WORD_LEN*MAX_ROMA_LEN, &p2->Syb[I][0]);

							if ((wstrlen(p1->Chinese) == 0) && (wstrlen(p2->Chinese) > 0)) wstrcpy(p1->Chinese, MAX_WORD_LEN * 2, p2->Chinese);
							if ((wstrlen(p1->Note) == 0) && (wstrlen(p2->Note) > 0)) wstrcpy(p1->Note, MAX_WORD_LEN, p2->Note);
							if ((wstrlen(p1->Pos) == 0) && (wstrlen(p2->Pos) > 0)) wstrcpy(p1->Pos, MAX_WORD_LEN, p2->Pos);
							if (p1->Freq < p2->Freq) p1->Freq = p2->Freq;
							p2->Wrd_Num = 0;

						}

					}
				}
			}
		}
	}

	// 刪除被判定重複之詞

	for (j = 0, i = 0; i < Dict_Num; i++)
	{
		p1 = &Dict[j];
		p2 = &Dict[i];
		if (p2->Wrd_Num > 0)
		{
			memcpy(p1, p2, sizeof(DICT));
			j++;
		}
	}
	Dict_Num = j;

	// 1.先做詞庫排序
	for (i = 0; i < Dict_Num - 1; i++)
	{
		p1 = &Dict[i];
		for (j = i + 1; j < Dict_Num; j++)
		{
			p2 = &Dict[j];
			if (wwstrcmp(p1->WordUI, MAX_WORD_LEN, p2->WordUI) > 0)
			{
				memcpy(&q, p1, sizeof(DICT));
				memcpy(p1, p2, sizeof(DICT));
				memcpy(p2, &q, sizeof(DICT));
			}
		}
		if ((i % 100) == 0)
		{
			TRACE("Dict_Num=%d i=%d\n", Dict_Num, i);
			//sprintf_s(S1, "Dict_Num=%d i=%d", Dict_Num, i);
			//Big5ToUniCode(S1, S11, strlen(S1));
			//SetWindowText((wchar_t *)S11);
		}
		p1->No = i;
	}

	// 3.詞庫存檔
	if ((err = fopen_s(&Out, File, "wb")) != 0) return -1;
	S11[0] = 0xFEFF; fwrite(S11, 1, 2, Out);
	for (int i = 0; i < Dict_Num; i++)
	{
		p = &Dict[i];
		if (p->Wrd_Num > 0)
		{

			// 寫入詞(Word)
			fwrite(p->Word, 2, wstrlen(p->Word), Out);
			wstrcpy(S11, 100, L"\t"); fwrite(S11, 2, wstrlen(S11), Out);

			// 寫入詞頻(Frequency)
			sprintf_s(S1, "%d", p->Freq);
			Big5ToUniCode(S1, S11, strlen(S1));
			wstrcat(S11, 100, L"\t"); fwrite(S11, 2, wstrlen(S11), Out);

			// 寫入詞類(POS)
			fwrite(p->Pos, 2, wstrlen(p->Pos), Out);
			wstrcpy(S11, 100, L"\t"); fwrite(S11, 2, wstrlen(S11), Out);

			// 寫入音調(PinYin)*6腔
			for (int j = 0; j < 6; j++)
			{
				fwrite(&p->Syb[j][0], 2, wstrlen(&p->Syb[j][0]), Out);
				wstrcpy(S11, 100, L"\t"); fwrite(S11, 2, wstrlen(S11), Out);
			}

			// 寫入華客對照詞(Chinese)
			fwrite(p->Chinese, 2, wstrlen(p->Chinese), Out);
			wstrcpy(S11, 100, L"\t"); fwrite(S11, 2, wstrlen(S11), Out);

			// 寫入Note
			fwrite(p->Note, 2, wstrlen(p->Note), Out);
			wstrcpy(S11, 100, L"\t"); fwrite(S11, 2, wstrlen(S11), Out);

			wstrcpy(S11, 100, L"\r\n"); fwrite(S11, 2, wstrlen(S11), Out);
		}
	}
	fclose(Out);
	return 0;
}
