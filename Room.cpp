#include "Room.h"

SCHEDULE Room::m_slots = {};

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