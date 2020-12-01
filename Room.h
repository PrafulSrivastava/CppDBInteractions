#pragma once
#include "DBTool.h"
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

/* Date Manipulations */

typedef pair<int, pair<int, int>> DATEs; /* {day, {month, year}} */
typedef pair<pair<int, pair<int, int>>, pair<int, pair<int, int>>> SLOT;/* {start date, end date} */
typedef pair<pair<int, int>, vector<SLOT>> BOOKING;
typedef map<pair<int,int>, vector<SLOT>> SCHEDULE; /* {{ID, TYPE}, {SLOT1, SLOT2....}} */

bool compDates(const DATEs& a, const DATEs& b) {
	if (a.first == b.first) {
		if (a.second.first == b.second.first) {
			return a.second.second < b.second.second;
		}
		else {
			return a.second.first < b.second.first;
		}
	}
	return a.first < b.first;
}

bool compSlots(const SLOT& a, const SLOT& b) {
	return compDates(a.first, b.first);
}

class Room {
public: 
	static DBTool* tool;
	int room_id;
	int room_type;
	static SCHEDULE m_slots;
	void pupulateMap();
	bool bookRoom(DATEs begin, DATEs end, int type);
	string getRoomType(int i);
};
