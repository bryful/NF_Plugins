#pragma once
//-----------------------------------------------------------------------------------
// 月名を数値に変換
#define NF_COMPUTE_MONTH \
	(__DATE__[0] == 'J' && __DATE__[1] == 'a' ? "01" : \
	 __DATE__[0] == 'F' ? "02" : \
	 __DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'r' ? "03" : \
	 __DATE__[0] == 'A' && __DATE__[1] == 'p' ? "04" : \
	 __DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'y' ? "05" : \
	 __DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'n' ? "06" : \
	 __DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'l' ? "07" : \
	 __DATE__[0] == 'A' && __DATE__[1] == 'u' ? "08" : \
	 __DATE__[0] == 'S' ? "09" : \
	 __DATE__[0] == 'O' ? "10" : \
	 __DATE__[0] == 'N' ? "11" : \
	 __DATE__[0] == 'D' ? "12" : "??")

#define NF_COMPUTE_DAY \
	(__DATE__[4] == ' ' ? "0" : (char[]){__DATE__[4], '\0'}), \
	(char[]){__DATE__[5], '\0'}

// constexpr関数を使った実装（C++20）
constexpr const char* compute_build_date_yyyymmdd() {
	constexpr char date[] = __DATE__;
	static char result[9] = {};

	// Year
	result[0] = date[7];
	result[1] = date[8];
	result[2] = date[9];
	result[3] = date[10];

	// Month
	const char* month =
		(date[0] == 'J' && date[1] == 'a') ? "01" :
		(date[0] == 'F') ? "02" :
		(date[0] == 'M' && date[1] == 'a' && date[2] == 'r') ? "03" :
		(date[0] == 'A' && date[1] == 'p') ? "04" :
		(date[0] == 'M' && date[1] == 'a' && date[2] == 'y') ? "05" :
		(date[0] == 'J' && date[1] == 'u' && date[2] == 'n') ? "06" :
		(date[0] == 'J' && date[1] == 'u' && date[2] == 'l') ? "07" :
		(date[0] == 'A' && date[1] == 'u') ? "08" :
		(date[0] == 'S') ? "09" :
		(date[0] == 'O') ? "10" :
		(date[0] == 'N') ? "11" : "12";

	result[4] = month[0];
	result[5] = month[1];

	// Day
	result[6] = (date[4] == ' ') ? '0' : date[4];
	result[7] = date[5];
	result[8] = '\0';

	return result;
}

#define NF_BUILD_DATE_YYYYMMDD compute_build_date_yyyymmdd()

//-----------------------------------------------------------------------------------