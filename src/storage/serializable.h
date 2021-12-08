#pragma once

template<int storage_size>
class serializable {
protected:
	int storage_size_ = storage_size;
public:
	virtual ~serializable() = default;
	int get_storage_size() const { return storage_size_; }
	virtual char* serialize() = 0;
	virtual void unserialize(char* buf) = 0;

	static void serialize_int(const int& i, char*& buf) {
		*reinterpret_cast<int*>(buf) = i;
		buf += 4;
	}

	static void unserialize_int(int& i, char*& buf) {
		i = *reinterpret_cast<int*>(buf);
		buf += 4;
	}

	static void serialize_vector(std::vector<int>& data, int size, char*& buffer) {
		for (int i = 0; i < size; i++) {
			*reinterpret_cast<int*>(buffer) = data[i];
			buffer += 4;
		}
	}

	static void unserialize_vector(std::vector<int>& data, int size, char*& buffer) {
		for (int i = 0; i < size; i++) {
			data[i] = *reinterpret_cast<int*>(buffer);
			buffer += 4;
		}
	}

	template<typename T>
	static void serialize_vector(std::vector<T>& data, int size, char*& buffer) {
		int data_size = data[0].get_storage_size();
		for (int i = 0; i < size; i++) {
			auto tmp = data[i].serialize();
			for (int i = 0; i < data_size; i++)
				*buffer++ = tmp[i];
			delete tmp;
		}
	}

	template<typename T>
	static void unserialize_vector(std::vector<T>& data, int size, char*& buffer) {
		int data_size = data[0].get_storage_size();
		for (int i = 0; i < size; i++) {
			data[i].unserialize(buffer);
			buffer += data_size;
		}
	}

};
