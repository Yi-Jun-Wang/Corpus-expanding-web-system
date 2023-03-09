#pragma once

wchar_t Punch[100] = { L"，。：「」、（）'～－─【】＂•〔〕＜＞’《》／ˇ′–ˋˊ〈〉．・﹔．！？；" };

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// ###############################
// A. 基本字串處理函式
// ###############################
// ===========================================
// ====== A.1.wwchar_t Process Function ===========
// ===========================================
#define wwchar_t unsigned int

int wwstrlen(wwchar_t *S11)
{
	int i;
	for (i = 0; i < 2000; i++) if (S11[i] == 0x0000) break;
	return i;
}
int wwstrlen(wwchar_t *S11, int Size)
{
	int i;
	for (i = 0; i < Size; i++) if (S11[i] == 0x0000) break;
	return i;
}
int wwstrcpy(wwchar_t *Dst, int Size, wwchar_t *Src)
{
	int i, Len;
	Len = wwstrlen(Src, Size); if (Len > Size - 1) Len = Size - 1;
	for (i = 0; i <= Len; i++) Dst[i] = Src[i];
	return Len;
}
int wwstrcat(wwchar_t *Dst, int Size, wwchar_t *Src)
{
	int i, Len1, Len2;
	Len1 = wwstrlen(Dst, Size);
	Len2 = wwstrlen(Src, Size);
	if ((Len1 + Len2) > (Size - 1)) return -1;
	for (i = 0; i <= Len2; i++) Dst[Len1 + i] = Src[i];
	return 0;
}
int wwstrncmp(wwchar_t *S11, wwchar_t *S22, int Len)
{
	int i;
	for (i = 0; i < Len; i++) if (S11[i] != S22[i]) break;
	if (i < Len) return -1; else return 0;
}
int wwstrcmp(wwchar_t *S11, int Size, wwchar_t *S22)
{

	int i, i1, i2, i3;
	i1 = wwstrlen(S11, Size);
	i2 = wwstrlen(S22, Size);
	if (i1 >= i2) i3 = i2; else i3 = i1;
	for (i = 0; i < i3; i++)
		if (S11[i] > S22[i]) return 1;
		else if (S11[i] < S22[i]) return -1;
	if (i1 == i2) return 0;
	else if (i1 < i2) return -1;
	else  return 1;

}


// ===============================================
// ====== A.2.wchar_t Process Function ===========
// ===============================================
int wstrlen(wchar_t *S11)
{
	int i;
	for (i = 0; i < 2000; i++) if (S11[i] == 0x0000) break;
	return i;
}
int wstrlen(wchar_t *Src, int Size)
{
	int i;
	for (i = 0; i < Size; i++) if (Src[i] == 0) break;
	if (i <= Size) return i;
	else          return -1;
}
int wstrcpy(wchar_t *Dst, int Size, wchar_t *Src)
{
	int i, Len;
	Len = wstrlen(Src, Size); 
	if (Len > Size - 1) Len = Size - 1;
	else if (Len == 0) { Dst[0] = 0;  return 0; }
	for (i = 0; i <= Len; i++) Dst[i] = Src[i];
	return Len;
}
int wstrncpy(wchar_t *S11, wchar_t *S22, int Len)
{
	int i, d = 0;
	if (Len <= 0)
	{
		S11[0] = 0; return -1;
	}
	if ((Len == 1) && (S22[0] >= 0xD800) && (S22[0] <= 0xDFFF)) d = 1;
	for (i = 0; i < Len + d; i++) S11[i] = S22[i];
	S11[Len + d] = 0x0000;
	return 0;
}
int wstrncmp(wchar_t *S11, wchar_t *S22, int Len)
{
	/*
	int i, d = 0;
	if ((Len == 1) && (S22[0] >= 0xD800) && (S22[0] <= 0xDFFF)) d = 1;
	for (i = 0; i < Len+d; i++) if (S11[i] != S22[i]) break;
	if (i < (Len+d)) return -1; else return 0;
	*/

	int i, i1, i2, i3;

	if (Len == 0) return -1;

	i1 = wstrlen(S11, Len); 
	i2 = wstrlen(S22, Len); 

	if ((i1 == 0) && (i2 == 0)) return 0;
	else if (i1 < 0) return -1;
	else if (i2 < 0) return 1;
	
	if (i1 >= i2) i3 = i2; else i3 = i1;

	if (i3 < Len) return -2;
	if (i3 > Len) i3 = Len;
	for (i = 0; i < i3; i++)
		if (S11[i] > S22[i]) return 1;
		else if (S11[i] < S22[i]) return -1;

	return 0;
	//if (i1 == i2) return 0;
	//else if (i1 < i2) return -1;
	//else  return 1;

}
int wstrcmp(wchar_t *S11, wchar_t *S22)
{
	int i, i1, i2, i3;
	i1 = wstrlen(S11, 200);
	i2 = wstrlen(S22, 200);
	if (i1 >= i2) i3 = i2; else i3 = i1;
	for (i = 0; i < i3; i++)
		if (S11[i] > S22[i]) return 1;
		else if (S11[i] < S22[i]) return -1;
	if (i1 == i2) return 0;
	else if (i1 < i2) return -1;
	else  return 1;

}
int wstrcat(wchar_t *Dst, int Size, wchar_t *Src)
{
	int i, Len1, Len2;
	Len1 = wstrlen(Dst, Size);
	Len2 = wstrlen(Src, Size);
	if ((Len1 + Len2) > (Size - 1)) return -1;
	for (i = 0; i <= Len2; i++) Dst[Len1 + i] = Src[i];
	return 0;
}
int wstrncat(wchar_t *Dst, int Size, wchar_t *Src, int Len)
{
	int i, Len1, Len2, d = 0;
	Len1 = wstrlen(Dst, Size);
	Len2 = wstrlen(Src, Size);
	if (Len > Len2) return -2;
	if ((Len1 + Len) > (Size - 1)) return -1;

	if ((Len == 1) && (Src[0] >= 0xD800) && (Src[0] <= 0xDFFF)) d = 1;
	for (i = 0; i < Len + d; i++) Dst[Len1 + i] = Src[i];
	Dst[Len1 + Len + d] = 0;
	return 0;
}

int watoi(wchar_t *No)
{
	int Len, i, j, k;
	Len = wstrlen(No);
	if (Len <= 0) return 0;// -1;
	for (i = 0; i < Len; i++) if ((No[i] < L'0') || (No[i] > L'9')) break;
	if (i < Len) return 0;// -2;
	k = 1; j = 0;
	for (i = Len - 1; i >= 0; i--)
	{
		j += (int)(No[i] - L'0')*k;
		k *= 10;
	}
	return j;
}


// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// ##################################################
// ##### B.big5轉unicode是用MultiByteToWideChar #####
// ##################################################
void Big5ToUniCode(char *Big5, wchar_t *UniCode, int Len)
{
	MultiByteToWideChar(950, 0, Big5, -1, UniCode, Len + 1);
}
//unique轉big5是用WideCharToMultiByte
void UniCodeToBig5(wchar_t *UniCode, char *Big5, int Len)
{
	WideCharToMultiByte(950, 0, UniCode, -1, Big5, Len, NULL, NULL);
	Big5[Len] = 0;
}
int UniCodeToUI(wchar_t *UniCode, wwchar_t *UICode, int Size)
{
	int i1, i2;
	i1 = i2 = 0;
	int Len = wstrlen(UniCode, Size);
	if (Len >= Size) Len = Size - 1;
	while (i1 < Len)
	{
		if ((UniCode[i1] >= 0xD800) && (UniCode[i1] <= 0xDFFF))
		{
			UICode[i2++] = ((wwchar_t)UniCode[i1] << 16) + (wwchar_t)UniCode[i1 + 1];
			i1 += 2;
		}
		else
		{
			UICode[i2++] = (wwchar_t)UniCode[i1];
			i1 += 1;
		}
	}
	UICode[i2] = 0; //字串結尾
	return i2; // 扣除字串結尾
}

int UIToUniCode(wwchar_t *UICode, wchar_t *UniCode, int Size)
{
	int i1, i2, Len;
	i1 = i2 = 0;
	Len = wwstrlen(UICode);
	while (i1 < Len)
	{
		if (UICode[i1] >= 0x10000)
		{
			UniCode[i2++] = (wchar_t)(UICode[i1] >> 16);
			UniCode[i2++] = (wchar_t)(UICode[i1++] & 0xFFFF);
		}
		else
		{
			UniCode[i2++] = (wchar_t)(UICode[i1++] & 0xFFFF);
		}
		if (i2 >= (Size - 1)) { i2 = Size - 1; break; }
	}
	UniCode[i2] = 0;
	return i2;
}

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// ####################################################
// C.讀取Unicode LE-16 一行資料
// ####################################################
int Read_Unicode_0D0A(FILE *In, wchar_t *S11, int Size)
{
	int i, F, Cur = 0;
	do {
		fread(&S11[Cur++], 1, 2, In);
		//for (F = 0, i = 0; i < wstrlen(Punch); i++) if (S11[Cur - 1] == Punch[i]) { F = 1; break; }
	} while ((Cur < (Size - 1)) && // 超過長度
			  //((Cur<Size/2)||(F==0)) && // 超過一半長，且遇到標點符號
		(!feof(In)) &&         // 檔案結束
		((S11[Cur - 2] != 0x0d) || (S11[Cur - 1] != 0x0a))//|| (Cur<Size/2) ) // 跳行
		);
	if (S11[Cur - 2] == 0x0d) { S11[Cur - 2] = 0; Cur -= 2; }
	else S11[Cur] = 0;
	return Cur;
}
int Read_Unicode_Punch(FILE *In, wchar_t *S11, int Size)
{
	int F, i, Cur = 0;
	do {
		fread(&S11[Cur++], 1, 2, In);
		for (F = 0, i = 0; i < wstrlen(Punch); i++) if (S11[Cur - 1] == Punch[i]) { F = 1; break; }
	} while (((Cur < (Size / 3 - 1)) || (F == 0)) &&
		(!feof(In)) &&
		(Cur < (Size - 1))
		);
	S11[Cur] = 0;
	return Cur;
}



// 讀取EDU_Unicode LE-16 一行資料
wchar_t S11_Res[100] = { L"\0" };
int Read_Edu_Unicode_Line(FILE *In, wchar_t *S11)
{	// 補前一次預讀的HK00000xxxxx
	int i, j = wstrlen(S11_Res);
	for (i = 0; i < j; i++) S11[i] = S11_Res[i];
	// 讀一行資料，直到檔尾或下一行HK00000xxxxx
	i = j;
	do { fread(&S11[i++], 1, 2, In); } while ((!feof(In)) &&
		(i < 1999) &&
		(memcmp(&S11[i - 12], L"HK00000", 14)));
	// 將末端 H000000xxxxx Copy到保留Buf
	if (!memcmp(&S11[i - 12], L"HK00000", 14))
	{
		wstrncpy(S11_Res, &S11[i - 12], 12);
		S11_Res[12] = 0x0000; S11[i - 12] = 0x0000;
		i -= 12;
	}
	else
	{
		S11_Res[0] = 0x0000;
	}
	return i;// -12;
}

// 讀取 ANSI 一行資料
int Read_ANSI_Line(FILE *In, char *S11)
{
	int i = 0;
	do { fread(&S11[i++], 1, 1, In); } while (S11[i - 1] != 0x0a);
	S11[i] = 0x00;
	return i;
}

