//
//	Created by ABacker on 9/5/2016
//

#include <string>

template<class _Ty>
struct FakeList_node {
public:
	FakeList_node() :data(NULL), size(0), offset(0), next(NULL) {}

	FakeList_node(_Ty *data, int size, int offset, FakeList_node *next)
		:data(data), size(size), offset(offset), next(next) {
	}
	_Ty &operator[](int n) {
		return data[n + offset];
	}
public:
	_Ty *data;
	int size;
	int offset;
	FakeList_node *next;
};

template<class _Ty>
class FakeList_iterator {
public:
	typedef FakeList_node<_Ty> node;
	typedef FakeList_iterator<_Ty> iterator;

public:
	FakeList_iterator() :_cur_pos(0), _cur_node(NULL) {}
	FakeList_iterator(int cur_pos, node *cur_node) :_cur_pos(cur_pos), _cur_node(cur_node) {}
	iterator operator++() {
		if (_cur_pos + 1 < _cur_node->size) ++_cur_pos;
		else {
			_cur_node = _cur_node->next;
			_cur_pos = 0;
		}

		return *this;
	}
	iterator &operator++(int) {
		iterator it = *this;
		++(*this);

		return it;
	}
	iterator operator+(int n) {
		iterator it = *this;
		it += n;

		return it;
	}
	iterator &operator+=(int n) {
		if (_cur_pos + n >= _cur_node->size) {
			n -= (_cur_node->size - _cur_pos - 1);
			_cur_pos = 0;
		}

		while (_cur_pos + n >= _cur_node->size) {
			n -= _cur_node->size;
			_cur_node = _cur_node->next;
		}
		_cur_pos += n;

		return *this;
	}
	_Ty *operator->() {
		return &((*_cur_node)[_cur_pos]);
	}
	_Ty &operator*() {
		return (*_cur_node)[_cur_pos];
	}
	//const _Ty *operator->()const {
	//	return &((*_cur_node)[_cur_pos]);
	//}
	//const _Ty &operator*()const {
	//	return (*_cur_node)[_cur_pos];
	//}
	bool operator==(const iterator &right) {
		return _cur_pos == right._cur_pos && _cur_node == right._cur_node;
	}
	bool operator!=(const iterator &right) {
		return _cur_pos != right._cur_pos || _cur_node != right._cur_node;
	}
protected:
	int _cur_pos;
	node *_cur_node;
};

template<class _Ty>
class FakeList {
public:
	typedef FakeList_node<_Ty> node;
	typedef FakeList_iterator<_Ty> iterator;

public:
	FakeList() {
		_tidy();
	}

	FakeList(const _Ty *elem, int n) {
		_tidy();
		assign(elem, n);
	}

	FakeList(_Ty *&&elem, int n) {
		_tidy();
		assign(std::move(elem), n);
	}

	//mark:private
	FakeList(const FakeList &right) {
		_tidy();
		assign(right);
	}

	FakeList(FakeList &&right) {
		_tidy();
		assign(std::move(right));
	}

	FakeList &operator=(const FakeList &right) {
		assign(right);
	}
	FakeList &operator=(FakeList &&right) {
		assign(std::move(right));
	}

	FakeList &assign(const _Ty *elem, int n) {
		if (_front == NULL)
			_front = new node(NULL, n, 0, NULL);
		else _tidy(_front->next);

		_Ty *data = new _Ty[n];
		memcpy(data, elem, sizeof(_Ty)*n);

		_front->data = data;

		_size = n;
		_back = _front;

		return *this;
	}
	FakeList &assign(_Ty *&&elem, int n) {
		if (_front == NULL)
			_front = new node(NULL, n, 0, NULL);
		else _tidy(_front->next);

		_front->data = elem;
		elem = NULL;

		_size = n;
		_back = _front;

		return *this;
	}
	FakeList &assign(const FakeList &right) {}
	FakeList &assign(FakeList &&right) {
		std::swap(_size, right._size);
		std::swap(_front, right._front);
		std::swap(_back, right._back);

		return *this;
	}

	_Ty &operator[](int n) {
		node *tmp = _front;
		while (tmp != NULL) {
			if (tmp->size > n)return (*tmp)[n];

			n -= tmp->size;
			tmp = tmp->next;
		}
		throw std::out_of_range("FakeList");
	}
	const _Ty &operator[](int n) const {
		node *tmp = _front;
		while (tmp != NULL) {
			if (tmp->size > n)return (*tmp)[n];

			n -= tmp->size;
			tmp = tmp->next;
		}
		throw std::out_of_range("FakeList");
	}

	FakeList& insert(int pos) {}
	FakeList& erase(int begin, int end) {}
	FakeList& replace(int begin, int end, _Ty *val, int n) {}

	FakeList& insert(iterator pos) {}
	FakeList& erase(iterator begin, iterator end) {}
	FakeList& replace(iterator begin, iterator end, _Ty *val, int n) {}

	FakeList& append(const _Ty *elem, int n) {
		_Ty *data = new _Ty[n];
		memcpy(data, elem, sizeof(_Ty)*n);

		if (_front == NULL && _back == NULL) {
			_front = new node(data, n, 0, NULL);
			_back = _front;
		}
		else {
			_back->next = new node(data, n, 0, NULL);
			_back = _back->next;
		}
		_size += n;

		return *this;
	}
	FakeList& append(const _Ty &val) {
		return append(&val, 1);
	}
	FakeList& append(_Ty &&val) {
		
	}

	FakeList& push_front() {}
	FakeList& push_back(_Ty *val, int n) { return append(val, n); }
	FakeList& push_back(const _Ty &val) { return append(val); }
	FakeList& push_back(_Ty &&val) { return append(std::move(val)); }

	FakeList& pop_front() {}
	FakeList& pop_back() {}

	iterator begin()const {
		return iterator(0, _front);
	}
	iterator end()const {
		return iterator(0, NULL);
	}

	_Ty &front() {}
	_Ty &back() {}
	FakeList& sublist(int begin, int end) {}
	FakeList& sublist(iterator begin, iterator end) {}

	iterator find()const {}

	void swap(FakeList &fakeList) {
		std::swap(_size, fakeList._size);
		std::swap(_front, fakeList._front);
		std::swap(_back, fakeList._back);
	}

	int size() const { return _size; }

	int length() const { return _size; }

	bool empty() const { return _size == 0; }

	void clear() {
		_tidy(_front);

		_size = 0;
		_front = NULL;
		_back = NULL;
	}

	~FakeList() { 
		clear();
	}
protected:
	void _tidy() {
		_size = 0;
		_front = NULL;
		_back = NULL;
	}
	void _tidy(node *first) {
		node *tmp;
		while (first != NULL) {
			tmp = first->next;

			if (first->data)
				delete[]first->data;
			delete first;

			first = tmp;
		}
	}

protected:
	int _size;
	node *_front;
	node *_back;
};

class string_builder :public FakeList<char> {
public:
	string_builder() :FakeList() {}

	string_builder(const char *str) :FakeList(str, strlen(str)) {}

	string_builder(const char *str, int n) :FakeList(str, n) {}

	string_builder(char *&&str) :FakeList(std::move(str), strlen(str)) {}

	string_builder(char *&&str, int n) :FakeList(std::move(str), n) {}

	string_builder &append(const char *str, int n) {
		FakeList::append(str, n);

		return *this;
	}

	string_builder &append(const char *str) {
		FakeList::append(str, strlen(str));

		return *this;
	}

	std::string toString() {
		std::string str;
		str.resize(_size + 1);
		str.clear();
		node *tmp = _front;
		while (tmp != NULL) {
			str.append(tmp->data, tmp->size);
			tmp = tmp->next;
		}
		return str;
	}

	void print(bool addLF = false) {
		for (iterator it = this->begin(); it != this->end(); ++it)printf("%c", *it);
		if (addLF)printf("\n");
	}

	string_builder& substr() {}
protected:
};