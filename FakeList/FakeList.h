#pragma once

//
//	Created by ABacker on 9/5/2016
//


/*
 *
 *
 *
 */
#ifndef _FAKELIST_ABACKER_NINI_H_
#define _FAKELIST_ABACKER_NINI_H_

#include <string>
#include <assert.h>

#define _NINI_BEGIN namespace nini {
#define _NINI_END }

_NINI_BEGIN

#define DEBUG									0x00011111
#define DEBUG_RANGE_CHECK						0x00000001
#define DEBUG_UNKOWN_ERR_CHECK					0x00000010
#define DEBUG_NEW_DELET_CHECK					0x00000100
#define DEBUG_COUT_ERR_INF						0x00001000
#define DEBUG_PRINT_NODE						0x00010000

#define DEFAULT_SIZE_OF_EACH_NODE				(1000000 / sizeof(_Ty))

#define NOEXCEPT throw()

//#define __STR2__(x) #x
//#define __STR1__(x) __STR2__(x)
//#define __LOC__ __FILE__ "("__STR1__(__LINE__)") : Warning Msg: "

typedef unsigned int size_type;

template<class _Ty>class FakeList;


//TEMPLATE CLASS _FakeList_Ptr
template<class _Ty>
class _FakeList_Ptr {
public:
	_FakeList_Ptr() :ref(0), data(NULL) {
	}

	explicit _FakeList_Ptr(_Ty *data) :ref(1), data(data) {
	}

	~_FakeList_Ptr() {
#if(DEBUG & DEBUG_UNKOWN_ERR_CHECK)
		assert(ref == 0);
#endif
		if (data)
			delete[] data;
	}

public:
	size_type ref;
	_Ty *data;
};


//TEMPLATE CLASS _FakeList_node
template<class _Ty>
class _FakeList_node {

	typedef _FakeList_Ptr<_Ty> Ptr;
	typedef _FakeList_node<_Ty> node;

public:
	_FakeList_node()
		: size(0), offset(0), next(NULL), ptr(NULL) {
	}

	_FakeList_node(_Ty *data, size_type size, size_type offset = 0, node *next = NULL)
		: size(size), offset(offset), next(next), ptr(new Ptr(data)) {
	}

	_FakeList_node(const node &node) {
		size = node.size;
		offset = node.offset;
		ptr = node.ptr;
		next = node.next;

		if (ptr)++ptr->ref;
	}

	_FakeList_node(node &&node) {
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

		return (*this);
	}

	node &operator=(node &&right) {
		_dis();

		size = right.size;
		offset = right.offset;
		ptr = right.ptr;
		next = right.next;

		right._tidy();

		return (*this);
	}

	node &assign(_Ty *data, size_type size, size_type offset = 0, node *next = NULL) {
		if (ptr && (--ptr->ref == 0)) {
			delete[] ptr->data;
			this->ptr->data = data;
			this->ptr->ref = 1;
		}
		else ptr = new Ptr(data);

		this->size = size;
		this->offset = offset;
		this->next = next;

		return (*this);
	}

	node &swap(const node &node) {
		std::swap(size, node.size);
		std::swap(offset, node.offset);
		std::swap(ptr, node.ptr);
		std::swap(next, node.next);

		return (*this);
	}

	//node *clone(_Ty *(*clone_func)(const _Ty *elem, size_type n)) const {
	//	_Ty *data = clone_func(ptr->data, size);
	//	return new node(data, size, offset, next);
	//}

	_Ty &operator[](size_type pos) {
#if(DEBUG & DEBUG_RANGE_CHECK)
		if (pos + 1 > size)
			throw std::out_of_range("FakeList");
#endif
		return ptr->data[pos + offset];
	}

	const _Ty &operator[](size_type pos) const {
#if(DEBUG & DEBUG_RANGE_CHECK)
		if (pos + 1 > size)
			throw std::out_of_range("FakeList");
#endif
		return ptr->data[pos + offset];
	}

	~_FakeList_node() {
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

	//_Ty *_clone(const _Ty *elem, size_type n) {
	//	_Ty *data = new _Ty[n];
	//	_Ty *tmp = data;
	//	for (size_type i = 0; i < n; ++i, ++tmp, ++elem)
	//		*tmp = *elem;//mark
	//	return data;
	//}

public:
	size_type size;
	size_type offset;
	node *next;

private:
	_FakeList_Ptr<_Ty> *ptr;
};


//TEMPLATE CLASS _FakeList_const_iterator
template<class _Ty>
class _FakeList_const_iterator {

	typedef _FakeList_node<_Ty> node;
	typedef _FakeList_const_iterator<_Ty> const_iterator;
	friend class FakeList<_Ty>;

public:
	_FakeList_const_iterator()
		:_cur_pos(0), _cur_node(NULL) {
	}

	_FakeList_const_iterator(size_type cur_pos, node *cur_node)
		:_cur_pos(cur_pos), _cur_node(cur_node) {
	}

	const_iterator operator++() {
		if (_cur_pos + 1 < _cur_node->size) ++_cur_pos;
		else {
			_cur_node = _cur_node->next;
			_cur_pos = 0;

#if(DEBUG & DEBUG_PRINT_NODE)
			printf(",");
#endif
		}
		return (*this);
	}

	const_iterator &operator++(int) {
		const_iterator it = *this;
		++(*this);

		return it;
	}

	const_iterator operator+(size_type n) {
		const_iterator it = *this;
		it += n;

		return it;
	}

	const_iterator &operator+=(size_type n) {
		if (_cur_pos + n >= _cur_node->size) {
			n -= (_cur_node->size - _cur_pos - 1);
			_cur_pos = 0;
			_cur_node = _cur_node->next;

			while (_cur_pos + n >= _cur_node->size) {
				n -= _cur_node->size;
				_cur_node = _cur_node->next;
			}
		}
		_cur_pos += n;

		return (*this);
	}

	const _Ty *operator->() const {
		return &((*_cur_node)[_cur_pos]);
	}

	const _Ty &operator*() const {
		return (*_cur_node)[_cur_pos];
	}

	bool operator==(const const_iterator &right) {
		return _cur_pos == right._cur_pos && _cur_node == right._cur_node;
	}

	bool operator!=(const const_iterator &right) {
		return _cur_pos != right._cur_pos || _cur_node != right._cur_node;
	}

protected:
	size_type _cur_pos;
	node *_cur_node;
};


//TEMPLATE CLASS _FakeList_iterator
template<class _Ty>
class _FakeList_iterator :public _FakeList_const_iterator<_Ty>{

	typedef _FakeList_node<_Ty> node;
	typedef _FakeList_iterator<_Ty> iterator;
	typedef _FakeList_const_iterator<_Ty> base;
	friend class FakeList<_Ty>;

public:
	_FakeList_iterator()
		:base(){
	}

	_FakeList_iterator(size_type cur_pos, node *cur_node)
		:base(cur_pos,cur_node) {
	}

	iterator operator++() {
		++(*static_cast<base *>(this));

		return (*this);
	}

	iterator &operator++(int) {
		iterator it = *this;
		++(*this);

		return it;
	}

	iterator operator+(size_type n) {
		iterator it = *this;
		it += n;

		return it;
	}

	iterator &operator+=(size_type n) {
		*static_cast<base *>(this) += n;

		return (*this);
	}

	_Ty *operator->() const {
		return &((*_cur_node)[_cur_pos]);
	}

	_Ty &operator*() const {
		return (*_cur_node)[_cur_pos];
	}
};


//TEMPLATE CLASS FakeList
template<class _Ty>
class FakeList {
protected:
	typedef _FakeList_node<_Ty> node;

public:
	typedef _FakeList_iterator<_Ty> iterator;
	typedef _FakeList_const_iterator<_Ty> const_iterator;

	//private:
	//	//CAUTION with these two functions!!!
	//	//when:
	//	//  FakeList<char>a("123456");
	//	//  FakeList<char>b = a;
	//	//b shares the same node memory with a.
	//	//you can use b = a.clone() if necessary
	//	FakeList(const FakeList &right)
	//		:_size(right._size), _front(right._front), _back(right._back) {
	//
	////#pragma message(__LOC__"CAUTION with this constructor")
	//
	//		//node *tmp = _front;
	//		//while (tmp != NULL) {
	//		//	//++tmp->ptr->ref;
	//
	//		//	tmp = tmp->next;
	//		//}
	//	}
	//
	//	FakeList &operator=(const FakeList &right) {
	//
	////#pragma message(__LOC__"CAUTION with this mutator")
	//
	//		//_size = right._size;
	//		//_front = right._front;
	//		//_back = right._back;
	//
	//		//node *tmp = _front;
	//		//while (tmp != NULL) {
	//		//	//++tmp->ptr->ref;
	//
	//		//	tmp = tmp->next;
	//		//}
	//		return (*this);
	//	}
	//

public:
	FakeList()
		:_size(0), _front(NULL), _back(NULL), _cow(false) {
	}

	FakeList(const _Ty *elem, size_type n)
		: _front(NULL) {
		assign(elem, n);
	}

	FakeList(_Ty *&&elem, size_type n)
		: _front(NULL) {
		assign(std::move(elem), n);
	}

	FakeList(FakeList &&right)
		: _front(NULL) {
		assign(std::move(right));
	}

	FakeList &operator=(FakeList &&right) {
		return assign(std::move(right));
	}

	FakeList &assign(const _Ty *elem, size_type n) {
		_Ty *data = _clone(elem, n);

		if (_front == NULL)
			_front = new node(data, n);
		else {
			_tidy(_front->next);
			_front->assign(data, n);
		}

		_size = n;
		_back = _front;
		_cow = false;

		return *this;
	}

	FakeList &assign(_Ty *&&elem, size_type n) {
		if (_front == NULL)
			_front = new node(elem, n);
		else {
			_tidy(_front->next);
			_front->assign(elem, n, 0, NULL);
		}

		elem = NULL;

		_size = n;
		_back = _front;
		_cow = false;

		return *this;
	}

	/*FakeList &assign(const FakeList &right) {

	}*/

	FakeList &assign(FakeList &&right) {
		std::swap(_size, right._size);
		std::swap(_front, right._front);
		std::swap(_back, right._back);
		std::swap(_cow, right._cow);

		return *this;
	}

	_Ty &operator[](size_type pos) {

#if(DEBUG & DEBUG_RANGE_CHECK)
		if (pos + 1 > _size)
			throw std::out_of_range("FakeList");
#endif

		node *tmp = _front;
		while (tmp != NULL) {
			if (tmp->size > pos)
				return (*tmp)[pos];

			pos -= tmp->size;
			tmp = tmp->next;
		}
		throw std::out_of_range("FakeList");
	}

	const _Ty &operator[](size_type pos) const {

#if(DEBUG & DEBUG_RANGE_CHECK)
		if (pos + 1 > _size)
			throw std::out_of_range("FakeList");
#endif
		node *tmp = _front;
		while (tmp != NULL) {
			if (tmp->size > pos)
				return (*tmp)[pos];

			pos -= tmp->size;
			tmp = tmp->next;
		}
		throw std::out_of_range("FakeList");
	}

	FakeList &insert(const _Ty *elem, size_type n, size_type pos) {

#if(DEBUG & DEBUG_RANGE_CHECK)
		if (pos == 0)return push_front(elem, n);

		if (pos > _size)return append(elem, n);
#endif
		_Ty *data = _clone(elem, n);
		node *tmp = _find_pos_node(&pos);

		_insert(data, n, tmp, pos);

		_size += n;

		return (*this);
	}

	FakeList &insert(_Ty *&&elem, size_type n, size_type pos) {

#if(DEBUG & DEBUG_RANGE_CHECK)
		if (pos == 0)return push_front(elem, n);

		if (pos > _size)return append(elem, n);
#endif
		node *tmp = _find_pos_node(&pos);

		_insert(elem, n, tmp, pos);

		elem = NULL;

		_size += n;

		return (*this);
	}

	FakeList &insert(const _Ty *elem, size_type n, iterator pos) {

#if(DEBUG & DEBUG_RANGE_CHECK)
		if (pos == this->begin())return push_front(elem, n);

		if (pos == this->end())return append(elem, n);
#endif
		_Ty *data = _clone(elem, n);

		_insert(data, n, pos._cur_node, pos._cur_pos);

		_size += n;

		return (*this);
	}

	FakeList &insert(_Ty *&&elem, size_type n, iterator pos) {

#if(DEBUG & DEBUG_RANGE_CHECK)
		if (pos == this->begin())return push_front(elem, n);

		if (pos == this->end())return append(elem, n);
#endif
		_insert(elem, n, pos._cur_node, pos._cur_pos);

		elem = NULL;

		_size += n;

		return (*this);
	}

	/*FakeList &insert(FakeList &&fakeList,size_type pos) {

	}*/

	FakeList &erase(size_type begin, size_type n) {

#if(DEBUG &DEBUG_RANGE_CHECK)
		if (begin + n > _size)
			throw std::out_of_range("FakeList");
#endif
		//node *tmp = _find_pos_node(&begin);


		return (*this);
	}

	//FakeList &erase(iterator begin, iterator end) {}
	//FakeList &replace(iterator begin, iterator end, _Ty *val, int n) {}
	//FakeList &replace(size_type begin, size_type end, _Ty *val, size_type n) {}

	FakeList &append(const _Ty *elem, size_type n) {
		_Ty *data = new _Ty[n];
		memcpy(data, elem, sizeof(_Ty)*n);

		if (_front == NULL) {
			_front = new node(data, n);
			_back = _front;
		}
		else {
			_back->next = new node(data, n);
			_back = _back->next;
		}
		_size += n;

		return (*this);
	}

	FakeList &append(_Ty *&&elem, size_type n) {
		if (_front == NULL) {
			_front = new node(elem, n);
			_back = _front;
		}
		else {
			_back->next = new node(elem, n);
			_back = _back->next;
		}

		elem = NULL;

		_size += n;

		return (*this);
	}

	FakeList &append(FakeList &&fakeList) {
		_back->next = fakeList._front;
		_back = fakeList._back;

		fakeList._front = NULL;
		fakeList._back = NULL;

		return (*this);
	}

	FakeList &append(const _Ty &val) {
		return append(&val, 1);
	}

	FakeList &append(_Ty &&val) {
		return append(std::move(&val), 1);
	}

	//FakeList &append(const FakeList &fakeList) {
	//	if (this == &fakeList) {
	//		FakeList tmp = fakeList.clone();
	//		this->_back = tmp->_front;
	//	}
	//	else this->_back = fakeList->_front;

	//	_size += fakeList._size;

	//	return (*this);
	//}

	FakeList &push_front(const _Ty *elem, size_type n) {
		_Ty *data = new _Ty[n];
		memcpy(data, elem, sizeof(_Ty)*n);

		node *push_node = new node(data, n);

		push_node->next = _front;
		_front = push_node;

		return (*this);
	}

	FakeList &push_front(_Ty *&&elem, size_type n) {
		node *push_node = new node(elem, n);

		elem = NULL;

		push_node->next = _front;
		_front = push_node;

		return (*this);
	}

	FakeList &push_back(_Ty *val, size_type n) {
		return append(val, n);
	}

	FakeList &push_back(const _Ty &val) {
		return append(val);
	}

	FakeList &push_back(_Ty &&val) {
		return append(std::move(val));
	}

	FakeList &pop_front() {
#if(DEBUG & DEBUG_RANGE_CHECK)
		if (_size == 0)
			throw std::out_of_range("FakeList");
#endif
		if (--_front->size == 0) {
			node *tmp = _front;
			_front = _front->next;
			delete tmp;
		}
		else ++_front->offset;

		if (--_size == 0)_back = NULL;

		return (*this);
	}

	FakeList &pop_back() {
#if(DEBUG & DEBUG_RANGE_CHECK)
		if (_size == 0)
			throw std::out_of_range("FakeList");
#endif
		if (--_size == 0)_front = NULL;

		if (--_back->size == 0) {
			delete _back;
			_back = _find_new_back(_back);
			if (_back)_back->next = NULL;
		}

		return (*this);
	}

	/*

	FakeList &pop_front_n() {

	return (*this);
	}

	FakeList &pop_back_n() {

	return (*this);
	}

	FakeList &pop_back_node() {

	return (*this);
	}

	FakeList &pop_front_node() {
	#if(DEBUG & DEBUG_RANGE_CHECK)
	if (_size == 0)
	throw std::out_of_range("FakeList");
	#endif


	return (*this);
	}
	*/

	iterator begin() NOEXCEPT{
		return iterator(0, _front);
	}

	const_iterator begin()const NOEXCEPT{
		return const_iterator(0, _front);
	}

	iterator end() NOEXCEPT{
		return iterator(0, NULL);
	}

	const_iterator end() const NOEXCEPT{
		return const_iterator(0, NULL);
	}

	_Ty &front() {
#if(DEBUG & DEBUG_RANGE_CHECK)
		if (_back == NULL)
			throw std::out_of_range("FakeList");
#endif
		return (*_front)[0];
	}

	const _Ty &front() const {
#if(DEBUG & DEBUG_RANGE_CHECK)
		if (_back == NULL)
			throw std::out_of_range("FakeList");
#endif
		return (*_front)[0];
	}

	_Ty &back() {
#if(DEBUG & DEBUG_RANGE_CHECK)
		if (_back == NULL)
			throw std::out_of_range("FakeList");
#endif

		return (*_back)[_back->size - 1];
	}

	const _Ty &back() const {
#if(DEBUG & DEBUG_RANGE_CHECK)
		if (_back == NULL)
			throw std::out_of_range("FakeList");
#endif
		return (*_back)[_back->size - 1];
	}

	//FakeList sublist(size_type begin, size_type end) {
	//	return (*this);
	//}

	//FakeList sublist(iterator begin, iterator end) {
	//	return (*this);
	//}

	/*iterator find()const {
		return end();
		}*/

	//DO NOT use it if not necessary
	FakeList clone(size_type size_of_each_node = DEFAULT_SIZE_OF_EACH_NODE)const {
		FakeList ret;

		if (_front) {
			int i = _size / size_of_each_node + (_size%size_of_each_node ? 1 : 0);
			size_type len;
			size_type rest = _size;

			len = (rest >= size_of_each_node ? size_of_each_node : rest);
			ret._front = new node(new char[len], len);
			ret._back = ret._front;
			rest -= len;

			while (--i) {
				len = (rest >= size_of_each_node ? size_of_each_node : rest);
				ret._back->next = new node(new char[len], len);
				ret._back = ret._back->next;

				rest -= len;
			}

			iterator  des = ret.begin();
			for (const_iterator src = this->begin(); src != this->end(); ++src, ++des) {
				*des = *src;
			}

			ret._size = _size;
		}
		return ret;
	}

	void swap(FakeList &fakeList) {
		std::swap(_size, fakeList._size);
		std::swap(_front, fakeList._front);
		std::swap(_back, fakeList._back);
		std::swap(_cow, fakeList._cow);
	}

	void format(size_type size_of_each_node = DEFAULT_SIZE_OF_EACH_NODE) {
		*this = this->clone();
	}

	size_type size() const {
		return _size;
	}

	size_type length() const {
		return _size;
	}

	size_type list_node_len()const {
		size_type n = 0;
		node *tmp = _front;
		while (tmp != NULL) {
			++n;
			tmp = tmp->next;
		}
		return n;
	}

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

			/*if (first->offset == 0 && first->ptr->data)
				delete[]first->ptr->data;*/
			delete first;

			first = tmp;
		}
	}

	void _insert(_Ty *data, size_type n, node *_node, size_type pos) {
		node *insert_node = new node(data, n);
		if (_node->size == pos) {
			insert_node->next = _node->next;
			_node->next = insert_node;
		}
		else {
			node *seperat_node = new node(*_node);

			seperat_node->offset = pos;
			seperat_node->size = _node->size - pos;
			_node->size = pos;

			seperat_node->next = _node->next;
			_node->next = insert_node;
			insert_node->next = seperat_node;
		}
	}

	node *_find_new_back(node *old_back) {
		node *tmp = _front;
		while (tmp != NULL && tmp->next != old_back) {
			tmp = tmp->next;
		}
		return tmp;
	}

	node *_find_pos_node(size_type *pos) {
		node *tmp = _front;
		while (tmp != NULL) {
			if (tmp->size >= *pos) {
				break;
			}
			*pos -= tmp->size;
			tmp = tmp->next;
		}
		return tmp;
	}

	_Ty *_clone(const _Ty *elem, size_type n) {
		_Ty *data = new _Ty[n];
		_Ty *tmp = data;
		for (size_type i = 0; i < n; ++i, ++tmp, ++elem)
			*tmp = *elem;//mark

		return data;
	}

protected:
	size_type _size;
	node *_front;
	node *_back;
	bool _cow;
};


//CLASS string_builder
class string_builder :public FakeList<char> {

	typedef FakeList<char> base;

public:
	string_builder()
		:base() {
	}

	//string_builder(const string_builder &str_builder){}

	string_builder(string_builder &&str_builder)
		:base(std::move(str_builder)) {
	}

	string_builder(const char *str, size_type n)
		:base(str, n) {
	}

	string_builder(char *&&str, size_type n)
		:base(std::move(str), n) {
	}

	explicit string_builder(const char *str)
		:base(str, strlen(str)) {
	}

	explicit string_builder(char *&&str)
		:base(std::move(str), strlen(str)) {
	}

	string_builder &operator=(string_builder &&str_builder){
		base::operator=(std::move(str_builder));

		return (*this);
	}

	string_builder &append(const char *str) {
		base::append(str, strlen(str));

		return (*this);
	}

	string_builder &append(const char *str, size_type n) {
		base::append(str, n);

		return (*this);
	}

	string_builder &append(char *&&str) {
		base::append(std::move(str), strlen(str));

		return (*this);
	}

	string_builder &append(char *&&str, size_type n) {
		base::append(std::move(str), n);

		return (*this);
	}

	string_builder &append(string_builder &&str_builder) {
		base::append(std::move(str_builder));

		return (*this);
	}

	std::string to_string() const {
		std::string str;
		str.resize(_size + 1);
		str.clear();
		node *tmp = _front;
		while (tmp != NULL) {
			str.append(&(*tmp)[0], tmp->size);
			tmp = tmp->next;
		}
		return str;
	}

	string_builder clone() const {
		string_builder ret;

		base *p = &ret;
		p->assign(base::clone());

		return ret;
	}

	/*char *c_str()const {
		return NULL;
		}*/

	void print(bool addLF = false) const {

#if(DEBUG & DEBUG_PRINT_NODE)
		printf("   ");
#endif

		for (const_iterator it = this->begin(); it != this->end(); ++it)
			printf("%c", *it);

		if (addLF)printf("\n");
	}

	string_builder substr() const {
		return string_builder();
	}

protected:
	static char *_clone(const char *str, size_type n) {
		char *p = new char[n];
		memcpy(p, str, sizeof(char)*n);

		return p;
	}
};

_NINI_END

#endif //_FAKELIST_ABACKER_NINI_H_