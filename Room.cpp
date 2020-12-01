#include "Room.h"

SCHEDULE Room::m_slots = {};
DBTool* Room::tool = DBTool::getInstance();

string Room::getRoomType(int i) {
	switch (i) {
		case 1: return "Economy";
		case 2: return "Executive";
		case 3: return "Luxury";
	}
	return "Invalid Room type";
}
bool Room::bookRoom(DATEs begin, DATEs end, int type) {
	for (auto room : m_slots) {
		if (room.first.second == type) {
			sort(room.second.begin(), room.second.end(), compSlots);
			int pos;
			for (pos = 0; pos < room.second.size(); pos++) {
				if (!compDates(room.second[pos].first, begin)) {// begin should be less than the begin of next slot
					if (pos == 0) {// begin date less than 1st entry 
						if (!compDates(room.second[pos].first, end)) {
							cout << " Booking Available(First Slot)" << endl;
							return true;
						}
						else {
							cout << "End date already booked (Boundry Condition)" << endl;
							return false;
						}
					}
					else if (compDates(room.second[pos - 1].second, begin)) {// begin should be greater than end of previous slot
						if (!compDates(room.second[pos].first, end)) {// end should be less than begin of next slot
							cout << " Booking Available" << endl;
							return true;
						}
						else {
							cout << "End date already booked" << endl;
							return false;
						}
					}
					else {
						cout << "Begin date already booked" << endl;
						return false;
					}
				}
			}// begin date greater than last entry
			cout << " Booking Available (Last Slot)" << endl;
			return true;
		}
	}
	cout << "Rooms of this type are Not available" << endl;
	return false;
}

void Room::pupulateMap() {
	SQLHANDLE SQLStatementHandle = Room::tool->getStatementHandle();
	SQLCHAR query_use_db[1024] = "Select s.room_id, r.room_type, s.f_day,s.f_month,s.f_year,s.l_day,s.l_month,s.l_year from slots as s inner join rooms as r on s.room_id= r.room_id;";
	if (SQL_SUCCESS != SQLExecDirect(SQLStatementHandle, (SQLCHAR*)query_use_db, SQL_NTS)) {
		tool->showSQLError(SQL_HANDLE_STMT, SQLStatementHandle);
	}
	else {
		pair <pair<int, int>, vector<SLOT>> row;
		DATEs d1,d2;
		vector<SLOT> vec;
		int prev_id = 0;
		int temp_id = 0;
		bool flag = false;
		while (SQLFetch(SQLStatementHandle) == SQL_SUCCESS) {
			SQLGetData(SQLStatementHandle, 1, SQL_C_DEFAULT, &prev_id, sizeof(prev_id), NULL);
			if (prev_id != temp_id) {
				if (flag) {
					row.second = vec;
					m_slots.insert(BOOKING(row));
				}
				flag = true;
				temp_id = prev_id;
				row.first.first = prev_id;
				SQLGetData(SQLStatementHandle, 2, SQL_C_DEFAULT, &row.first.second, sizeof(row.first.second), NULL);
				vec.clear();
			}
			
			SQLGetData(SQLStatementHandle, 3, SQL_C_DEFAULT, &d1.first, sizeof(d1.first), NULL);
			SQLGetData(SQLStatementHandle, 4, SQL_C_DEFAULT, &d1.second.first, sizeof(d1.second.first), NULL);
			SQLGetData(SQLStatementHandle, 5, SQL_C_DEFAULT, &d1.second.second, sizeof(d1.second.second), NULL);
			SQLGetData(SQLStatementHandle, 6, SQL_C_DEFAULT, &d2.first, sizeof(d2.first), NULL);
			SQLGetData(SQLStatementHandle, 7, SQL_C_DEFAULT, &d2.second.first, sizeof(d2.second.first), NULL);
			SQLGetData(SQLStatementHandle, 8, SQL_C_DEFAULT, &d2.second.second, sizeof(d2.second.second), NULL);
			vec.push_back(SLOT(d1, d2));
		}
	}
