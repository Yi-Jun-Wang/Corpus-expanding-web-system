// Hakka_AddWord_Jimmy.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//
using namespace std;
#include <atltrace.h>
#define ATLTRACE ATL::CTraceFileAndLineInfo(__FILE__, __LINE__)
#define TRACE ATLTRACE
#include <iostream>
#include "TCPIP_Sync.cpp"
// ############### P2G API ###################
#include "wstring.h"    // 基本字串處理
#include "Syllable.h"   // 音節處理
#include "Dictionary.h" // 字典處理
#include "P2G.h"        // 斷詞(音轉字/字轉音)
// ###########################################
#define MAX_WCHAR 300

#include <sstream>
#include <string>
#include <vector>
#include <codecvt>
#include <tuple>
#include <thread>
#include <mysql.h>

string line_u8;
wstring line_u16;
wstringstream ss;
wstring_convert<codecvt_utf8_utf16<wchar_t, 0x10ffff, codecvt_mode::little_endian>, wchar_t> conv;
void Display_List(SENTENCE* Sent, SOCKET Sock, int Size, wchar_t* S12)
{
	int      n, i, j, i1, i2, i3, Len, I = 0; // 四縣腔
	char     S1[200];
	wchar_t  S13[200], S14[200], CSyb[6][200], Syb[6][200];
	SYB_DATA     A[6];

	n = i1 = i2 = 0;
	for (Len = 0; Len < MAX_SENTENCE_LEN; Len++) if (Sent[Len].WordUI == 0) break;
	while (i1 < Len)
	{
		i = Sent[i1].WLen / 100;
		i3 = 0;
		for (I = 0; I < 6; I++) Syb[I][0] = CSyb[I][0] = 0;
		for (j = i1; j < i1 + i; j++)
		{
			if (Sent[j].WordUI > 0x10000)
			{
				S13[i3++] = S12[i2++] = (wchar_t)(Sent[j].WordUI >> 16);
				S13[i3++] = S12[i2++] = (wchar_t)(Sent[j].WordUI & 0xFFFF);
			}
			else
			{
				S13[i3++] = S12[i2++] = (wchar_t)(Sent[j].WordUI & 0xFFFF);
			}
			for (I = 0; I < 6; I++)
			{
				wstrcat(&Syb[I][0], MAX_SENTENCE_LEN, &Sent[j].Syb[I][0]);
				wstrcat(&Syb[I][0], MAX_SENTENCE_LEN, L" ");
				Syb_Converter(&Sent[j].Syb[I][0], &A[I]);
				wstrcat(&CSyb[I][0], MAX_SENTENCE_LEN, A[I].CSybTon);
				wstrcat(&CSyb[I][0], MAX_SENTENCE_LEN, L" ");
			}
		}
		// 詞
		S13[i3] = 0;
		// cout << "輸出開始" << endl;
		ss << S13 << L'_';
		ss << &Syb[0][0] << L'_' << &Syb[1][0] << L'_'
			<< &Syb[2][0] << L'_' << &Syb[3][0] << L'_'
			<< &Syb[4][0] << L'_' << &Syb[5][0] << L'_';
		sprintf_s(S1, "%d", Sent[i1].Freq);
		Big5ToUniCode(S1, S14, strlen(S1));
		ss << S14 << L'_';
		ss << Sent[i1].POS << L'_' << &CSyb[0][0] << L"\n";
		n++;

		// Global
		i1 += i;
		S12[i2++] = L'_';
	}
	S12[i2] = 0;
	ss << S12 << L"\n";
	line_u8 = conv.to_bytes(ss.str());
	send(Sock, line_u8.c_str(), line_u8.size(), 0);
	ss.str(L"");
	ss.clear();
}

tuple< vector< vector<wchar_t> >, long> Row_to_Vec(
	MYSQL_ROW sql_row, 
	unsigned long* Lens, 
	int col_num) 
{
	vector<vector<wchar_t> > result(col_num - 1, vector<wchar_t>(20, 0));
	static string sql_u8;
	static wstring sql_u16;
	long update_time;
	int size;
	for (int i = 0; i < col_num - 1; i++) {
		sql_u8.assign(sql_row[i], sql_row[i] + Lens[i]);
		sql_u16 = conv.from_bytes(sql_u8);
		size = sql_u16.size();
		result[i].assign(sql_u16.cbegin(), sql_u16.cbegin()+size);
		result[i].push_back(0);
	}
	update_time = atol(sql_row[col_num - 1]);
	return make_tuple(result, update_time);
}

void Expand_Dict(vector<vector<wchar_t> > Row) {
	int	 Len, i, j, k, I, L2[400], col_itr=0;
	static wchar_t S12[MAX_SENTENCE_LEN1];
	char S1[200], S2[200];
	DICT* p;
	p = &Dict[Dict_Num++]; p->No = Dict_Num - 1;

	// Get Word
	Len = wstrlen(&Row[col_itr][0]); // 字串長度，不包含結尾/0.
	wstrncpy(p->Word, &Row[col_itr++][0], Len);
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
	Len = wstrlen(&Row[col_itr][0]);
	wstrncpy(S12, &Row[col_itr++][0], Len);
	p->Freq = watoi(S12);

	// Get POS
	Len = wstrlen(&Row[col_itr][0]);
	wstrncpy(p->Pos, &Row[col_itr++][0], Len);

	// Get Syb*6

	for (I = 0; I < 6; I++)
	{
		p->Syb_Len[I] = wstrlen(&Row[col_itr][0]);
		wstrncpy(&p->Syb[I][0], &Row[col_itr++][0], p->Syb_Len[I]);


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
	Len = wstrlen(&Row[col_itr][0]);
	wstrncpy(p->Chinese, &Row[col_itr++][0], Len);

	// Get Note
	Len = wstrlen(&Row[col_itr][0]);
	wstrncpy(p->Note, &Row[col_itr][0], Len);
}

void Check_DB() {
	MYSQL conn;
	MYSQL_RES* result;
	MYSQL_ROW sql_row;
	const char* user = "Jeremy";
	const char* passwd = "Test0440";
	const char* host = "140.113.170.220";
	char query[200];
	unsigned int port = 3306;
	int res;
	long new_time, update_time = 1677825066;
	unsigned long* lengths;
	vector< vector<wchar_t> > row_vec;
	mysql_init(&conn);
	if (mysql_real_connect(&conn, host, user, passwd, "Hakka_Corp", port, NULL, 0)) {
		cout << "DB connect successful." << endl;
	}
	else {
		cout << "DB connect failed." << endl;
		return;
	}

	while (1) {
		sprintf_s(query, u8"select * from Hakka where 更新時間>%ld", update_time);
		if (!mysql_query(&conn, query)) {
			cout << "query successful." << endl;
			result = mysql_store_result(&conn);
			res = mysql_num_fields(result) - 1; // 最後一欄為ID, 無意義故省略
			while ((sql_row = mysql_fetch_row(result))) {
				lengths = mysql_fetch_lengths(result);
				tie(row_vec, new_time) = Row_to_Vec(sql_row, lengths, res); 
				if (new_time > update_time) update_time = new_time;
				Expand_Dict(row_vec);
			}
			mysql_free_result(result);
		}
		else {
			cout << "query failed." << endl;
		}
		cout << "update done." << endl;
		this_thread::sleep_for(chrono::seconds(30));
	}
}

int main()
{
	SetConsoleCP(65001);
	SetConsoleOutputCP(65001);
	SOCKET    Sock, Sock1;
	sockaddr  Addr;
	SENTENCE     Sent[MAX_SENTENCE_LEN];
	wchar_t      S11[200], S12[200];
	unsigned int S21[200];
	const unsigned int MAX_BUF_LENGTH = 1024;
	int          i, j, Len=sizeof(sockaddr);
	int          I = 0;// 四縣腔
	char         File[100] = { "Data/四縣（含南四縣）語料文本_2021.04.30_C.txt" };
	vector<char> buffer(1024);

	if ((i = Read_SybID()) < 0) cout << u8"675音節表(PinYin_Table_四縣.txt)讀取失敗";
	if ((j = Read_Edu_Hakka_Dict_Hsf("Data/Edu_Dict_1110508_edu2.csv")) < 0) cout << u8"辭庫(Edu_Dict_1110508_edu2.csv)讀取失敗";
	if ((i < 0) || (j < 0)) return 0;
	if ((i = Add_1Word("Add_1Word.csv")) < 0)  cout << u8"寫入一字詞 Add_1Word.csv 失敗";
	
	thread t1(Check_DB);
	t1.detach();

	// 斷詞分析
	int cur;
	Start_TCP_Server(&Sock, 6000);
	while (1) {
		printf(u8"等待連線中\r\n");
		Sock1 = accept(Sock, &Addr, &Len); // 接受他
		printf(u8"連線成功\r\n");
		while (1) {
			i = recv(Sock1, &buffer[0], buffer.size(), 0);
			if (i > 0) {
				//line_u8 = u8"阿爸你愛尞幾久？𠊎愛尞2日，因為阿舅愛討餔娘";
				//cout << line_u8 << endl;
				line_u8.assign(buffer.cbegin(), buffer.cbegin() + i);
				line_u8 += u8'\r';
				line_u16 = conv.from_bytes(line_u8);
				cur = 0;
				//S11[0] = 0xFEFF;
				do {
					S11[cur] = line_u16.c_str()[cur];
					cur++;
				} while (line_u16.c_str()[cur] != 0x0d);

				S11[cur] = 0;

				//wchar_t* S11 = L"阿爸你愛尞幾久？𠊎愛尞2日，因為阿舅愛討餔娘";
				UniCodeToUI(S11, S21, 200);
				memset(Sent, 0, sizeof(SENTENCE) * MAX_SENTENCE_LEN);
				j = P2Gn(0, S21, Sent);

				Display_List(Sent, Sock1, MAX_SENTENCE_LEN, S12);
			}
			else {
				printf(u8"遠端斷線\r\n");
				closesocket(Sock1);
				break;
			}
		}
	}
	return 0;
}

// 執行程式: Ctrl + F5 或 [偵錯] > [啟動但不偵錯] 功能表
// 偵錯程式: F5 或 [偵錯] > [啟動偵錯] 功能表

// 開始使用的提示: 
//   1. 使用 [方案總管] 視窗，新增/管理檔案
//   2. 使用 [Team Explorer] 視窗，連線到原始檔控制
//   3. 使用 [輸出] 視窗，參閱組建輸出與其他訊息
//   4. 使用 [錯誤清單] 視窗，檢視錯誤
//   5. 前往 [專案] > [新增項目]，建立新的程式碼檔案，或是前往 [專案] > [新增現有項目]，將現有程式碼檔案新增至專案
//   6. 之後要再次開啟此專案時，請前往 [檔案] > [開啟] > [專案]，然後選取 .sln 檔案
