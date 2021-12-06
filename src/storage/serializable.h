#pragma once
template<int storage_size>
class serializable {
public:
	virtual ~serializable() = default;
	int get_storage_size() const { return storage_size_; }
	virtual char* serialize() = 0;
	virtual void unserialize(char* data) = 0;
protected:
	int storage_size_ = storage_size;

	static void serialize_vector(vector<int>& data, int size, int*& buffer) {
		for (int i = 0; i < size; i++)
			if (i < static_cast<int>(data.size())) *buffer++ = data[i];
			else *buffer++ = 0;
	}

	static void serialize_vector(vector<serializable>& data, int size, int*& buffer) {

	}

	static void unserialize_vector(vector<int>& data, int size, int*& buffer) {
		data.resize(size);
		for (int i = 0; i < size; i++)
			data[i] = *buffer++;
	}

	static void unserialize_vector(vector<serializable>& data, int size, int*& buffer) {

	}

};
