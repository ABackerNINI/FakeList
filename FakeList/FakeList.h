#pragma once

//
//	Created by ABacker on 9/5/2016
//

#ifndef _FAKELIST_ABACKER_H_
#define _FAKELIST_ABACKER_H_

#include <string>
#include <assert.h>

#define DEBUG						0x1111		
#define DEBUG_RANGE_CHECK			0x0001
#define DEBUG_UNKOWN_ERR_CHECK		0x0010
#define DEBUG_NEW_DELET_CHECK		0x0100
#define DEBUG_COUT_ERR_INF			0x1000

template<class _Ty>
class FakeList_Ptr {
public:
	FakeList_Ptr() :ref(0), data(NULL) {
	}

	FakeList_Ptr(_Ty *data) :ref(1), data(data) {
	}

	~FakeList_Ptr() {
#if(DEBUG & DEBUG_UNKOWN_ERR_CHECK)
		assert(ref > 0);
#endif;
		if (data)delete[] data;
	}

public:
	int ref;
	_Ty *data;
};

template<class _Ty>
class FakeList_node {
public:
	typedef FakeList_Ptr<_Ty> Ptr;
	typedef FakeList_node<_Ty> node;

public:
	FakeList_node() {
		_tidy();
	}

	FakeList_node(_Ty *data, int size, int offset = 0, node *next = NULL)
		:ptr(new Ptr(data)), size(size), offset(offset), next(next) {
	}

	FakeList_node(const node &node) {
		size = node.size;
		offset = node.offset;
		ptr = node.ptr;
		next = node.next;

		if (ptr)++ptr->ref;
	}

	FakeList_node(node &&node) {
		size = node.size;
		offset = node.offset;
		ptr = node.ptr;
		next = node.next;

		node._tidy();
	}

	node &operator=(const node &right) {
		_dis();

		size = right.size;
		offset = right.offset;
		ptr = right.ptr;
		next = right.next;

		if (ptr)++ptr->ref;

		return *this;
	}

	node &operator=(node &&right) {
		_dis();

		size = right.size;
		offset = right.offset;
		ptr = right.ptr;
		next = right.next;

		right._tidy();

		return *this;
	}

	node &assign(_Ty *data, int size, int offset = 0, node *next = NULL) {
		if (ptr && (--ptr->ref == 0)) {
			delete[] ptr->data;
			this->ptr->data = data;
			this->ptr->ref = 1;
		}
		else ptr = new Ptr(data);

		this->size = size;
		this->offset = offset;
		this->next = next;

		return *this;
	}

	node &swap(const node &node) {
		std::swap(size, node.size);
		std::swap(offset, node.offset);
		std::swap(ptr, node.ptr);
		std::swap(next, node.next);

		return *this;
	}

	_Ty &operator[](int n) {
		return ptr->data[n + offset];
	}

	~FakeList_node() {
		_dis();
	}

protected:
	void _tidy() {
		size = 0;
		offset = 0;
		next = NULL;
		ptr = NULL;
	}

	void _dis() {
		if (ptr && (--ptr->ref == 0)) {
			delete ptr;
		}
	}

public:
	int size;
	int offset;
	FakeList_Ptr<_Ty> *ptr;
	FakeList_node *next;
};

template<class _Ty>
class FakeList_iterator {
public:
	typedef FakeList_node<_Ty> node;
	typedef FakeList_iterator<_Ty> iterator;

public:
	FakeList_iterator() :_cur_pos(0), _cur_node(NULL) {
	}

	FakeList_iterator(int cur_pos, node *cur_node)
		:_cur_pos(cur_pos), _cur_node(cur_node) {
	}

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
	//FakeList(const FakeList &right) {
	//	_tidy();
	//	assign(right);
	//}

	FakeList(FakeList &&right) {
		_tidy();
		assign(std::move(right));
	}

	FakeList &operator=(const FakeList &right) {
		return assign(right);
	}
	FakeList &operator=(FakeList &&right) {
		return assign(std::move(right));
	}

	FakeList &assign(const _Ty *elem, int n) {
		if (_front == NULL)
			_front = new node(NULL, n);
		else _tidy(_front->next);

		_Ty *data = new _Ty[n];
		memcpy(data, elem, sizeof(_Ty)*n);

		_front->ptr->data = data;

		_size = n;
		_back = _front;

		return *this;
	}
	FakeList &assign(_Ty *&&elem, int n) {
		if (_front == NULL)
			_front = new node(NULL, n);
		else _tidy(_front->next);

		_front->ptr-> = elem;
		elem = NULL;

		_size = n;
		_back = _front;

		return *this;
	}

	//FakeList &assign(const FakeList &right) {}

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

	FakeList& pop_front() {
		++_front->offset;
		if (_front->offset == _front->size) {

			_front = _front->next;
		}
	}
	FakeList& pop_back() {}

	iterator begin() const {
		return iterator(0, _front);
	}
	iterator end() const {
		return iterator(0, NULL);
	}

	_Ty &front() {
		return (*_front)[0];
	}

	_Ty &back() {
		return (*_back)[_back->size - 1];
	}

	FakeList& sublist(int begin, int end) {}
	FakeList& sublist(iterator begin, iterator end) {}

	iterator find()const {}

	void swap(FakeList &fakeList) {
		std::swap(_size, fakeList._size);
		std::swap(_front, fakeList._front);
		std::swap(_back, fakeList._back);
	}

	int size() const {
		return _size;
	}

	int length() const {
		return _size;
	}

	int node_size()const {}

	bool empty() const {
		return _size == 0;
	}

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

			if (first->offset == 0 && first->data)
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

#endif //_FAKELIST_ABACKER_H_