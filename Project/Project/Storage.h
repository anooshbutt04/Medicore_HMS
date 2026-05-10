#ifndef STORAGE_H
#define STORAGE_H

template<typename T>
class Storage {
private:
	T data[100];//fixed size in qs
	int count;

public:

	Storage() {
		count = 0;
	}

	//add new to array

	bool add(const T& obj) {
		if (count >= 100) {
			return false;
		}
		data[count] = obj;
		count++;
		return true;
	}

	//remove
	bool removeID(int id) {
		int index = -1;
		for (int i = 0; i < count; i++) {
			if (data[i].getID() == id) {
				index = i;
				break;
			}
		}

		if (index == -1) return false;

		for (int i = index; i < count - 1; i++) {
			data[i] = data[i + 1];
		}
		count--;
		return true;
	}

	//find by id
	//T* 

	T* findbyID(int id) {
		for (int i = 0; i < count; i++) {
			if (data[i].getID() == id) {
				return &data[i];
			}
		}
		return nullptr;
	}

	T* getALL() {
		return data;
	}

	int size() {
		return count;
	}

	//update (for filehandler)

	bool updatebyID(int id, const T& updated) {
		for (int i = 0; i < count; i++) {
			if (data[i].getID() == id) {
				data[i] = updated;
				return true;
			}
		}
		return false;
	}

};

#endif 
