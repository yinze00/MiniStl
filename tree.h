#ifndef _TREE_H
#define _TREE_H

#include<iostream>
using namespace std;


//set  map  multiset  multimap
template<class _K, class _Ty, class _Kfn, class _Pr, class _A>
class _Tree 
{
public:
	enum _Redbl {_Red, _Black};
	struct _Node;
	typedef _Node* _Nodeptr;
	struct _Node 
	{
		_Nodeptr _Left, _Parent, _Right;
		_Ty _Value;
		_Redbl _Color;
    };

	typedef _Node*&         _Nodepref;
	typedef const _K& _Keyref;
	typedef _Redbl&         _Rbref;
	typedef _Ty&            _Vref;

	static _Rbref _Color(_Nodeptr _P)
	{return ((_Rbref)(*_P)._Color); }
	static _Keyref _Key(_Nodeptr _P)
	{return (_Kfn()(_Value(_P))); }
	static _Nodepref _Left(_Nodeptr _P)
	{return ((_Nodepref)(*_P)._Left); }
	static _Nodepref _Parent(_Nodeptr _P)
	{return ((_Nodepref)(*_P)._Parent); }
	static _Nodepref _Right(_Nodeptr _P)
	{return ((_Nodepref)(*_P)._Right); }
	static _Vref _Value(_Nodeptr _P)
	{return ((_Vref)(*_P)._Value); }
public:
	typedef _Tree<_K, _Ty, _Kfn, _Pr, _A> _Myt;
	typedef _K                            key_type;
	typedef _Ty                           value_type;
	typedef _A::size_type                 size_type;
	typedef _A::difference_type           difference_type;
	typedef _Ty*                          pointer;
	typedef const _Ty*                    const_pointer;
	typedef _Ty&                          reference;
	typedef const _Ty&                    const_reference;
public:
	class iterator;
	friend class iterator;
	class iterator
	{
	public:
		iterator()
		{}
		iterator(_Nodeptr _P) : _Ptr(_P)
		{}

		reference operator*()
		{return _Value(_Ptr);}
		pointer operator->()
		{return &**this;}

		iterator& operator++()
		{
			_Inc();
			return *this;
		}
		iterator operator++(int)
		{
			iterator _Tmp = *this;
			++*this;
			return _Tmp;
		}
		iterator& operator--()
		{
			_Dec();
			return *this;
		}
		iterator operator--(int)
		{
			iterator _Tmp = *this;
			--*this;
			return _Tmp;
		}

		bool operator==(const iterator& _X) const
		{return (_Ptr == _X._Ptr); }
		bool operator!=(const iterator& _X) const
		{return (!(*this == _X)); }
		_Nodeptr _Mynode() const
		{return (_Ptr); }
		
		void _Inc()
		{
			if(_Right(_Ptr) != _Nil)
				_Ptr = _Min(_Right(_Ptr));
			else
			{
				_Nodeptr _P;
				while(_Ptr == _Right(_P = _Parent(_Ptr)))
					_Ptr = _P;
				if(_Right(_Ptr) != _P)
					_Ptr  = _P;
			}
		}
		void _Dec()
		{
			if(_Color(_Ptr)==_Red && _Parent(_Parent(_Ptr))==_Ptr)
				_Ptr = _Right(_Ptr);
			else if(_Left(_Ptr) != _Nil)
				_Ptr = _Max(_Left(_Ptr));
			else
			{
				_Nodeptr _P;
				while(_Ptr == _Left(_P = _Parent(_Ptr)))
					_Ptr = _P;
				_Ptr = _P;
			}
		}
	protected:
		_Nodeptr _Ptr;
	};

	typedef pair<iterator, bool> _Pairib;
	typedef pair<iterator, iterator> _Pairii;

public:
	explicit _Tree(const _Pr& _Parg, bool _Marg = true, const _A& _Al = _A())
                : allocator(_Al), key_compare(_Parg), _Multi(_Marg)
    {
		_Init(); 
	}
public:
	iterator begin()
	{return iterator(_Lmost());}
	iterator end()
	{return iterator(_Head);}
public:
	///////////////////////////////////////////////////////////
	//�ӿ�Ԥ��
	_Tree(const _Ty *_F, const _Ty *_L,
                const _Pr& _Parg, bool _Marg = true,
                const _A& _Al = _A());
	_Tree(const _Myt& _X);
	//~_Tree()
	size_type size() const;
	bool empty() const;
	iterator insert(iterator _P, const value_type& _V);
	void insert(iterator _F, iterator _L);
	void insert(const value_type *_F, const value_type *_L);
	iterator erase(iterator _F, iterator _L);
	size_type erase(const _K& _X);
	void erase(const _K *_F, const _K *_L);
	void clear();
	iterator find(const _K& _Kv);
	size_type count(const _K& _Kv) const;
	iterator lower_bound(const _K& _Kv);
	iterator upper_bound(const _K& _Kv);
	_Pairii equal_range(const _K& _Kv);
	//////////////////////////////////////////////////////////////

public:
	//pair <iterator bool>
	_Pairib insert(const value_type& _V)
	{
		_Nodeptr _X = _Root();
		_Nodeptr _Y = _Head;
		bool _Ans = true;

		//���Ҳ������ݵĲ���λ��
		while (_X != _Nil)
		{
			_Y = _X;
			_Ans = key_compare(_Kfn()(_V), _Key(_X));
			_X = _Ans ? _Left(_X) : _Right(_X); 
		}
		//set map  multiset  multimap
		///////////////////////////////////////////////////////
		if (_Multi)
			return (_Pairib(_Insert(_X, _Y, _V), true));
		///////////////////////////////////////////////////////
		iterator _P = iterator(_Y);
		if (!_Ans)
			;
		else if (_P == begin())
			return (_Pairib(_Insert(_X, _Y, _V), true));
		else
			--_P;
		if (key_compare(_Key(_P._Mynode()), _Kfn()(_V)))
			return (_Pairib(_Insert(_X, _Y, _V), true));
		return (_Pairib(_P, false)); 
	}

	iterator erase(iterator _P)
	{
		//���ҽڵ�
		_Nodeptr _X;
		_Nodeptr _Y = (_P++)._Mynode();
		_Nodeptr _Z = _Y;
		if (_Left(_Y) == _Nil)
			_X = _Right(_Y);
		else if (_Right(_Y) == _Nil)
			_X = _Left(_Y);
		else
			_Y = _Min(_Right(_Y)), _X = _Right(_Y);
		
		
		if (_Y != _Z)
		{
			_Parent(_Left(_Z)) = _Y;
			_Left(_Y) = _Left(_Z);
			if (_Y == _Right(_Z))
				_Parent(_X) = _Y;
			else
			{
				_Parent(_X) = _Parent(_Y);
				_Left(_Parent(_Y)) = _X;
				_Right(_Y) = _Right(_Z);
				_Parent(_Right(_Z)) = _Y;
			}
			if (_Root() == _Z)
				_Root() = _Y;
			else if (_Left(_Parent(_Z)) == _Z)
				_Left(_Parent(_Z)) = _Y;
			else
				_Right(_Parent(_Z)) = _Y;
			_Parent(_Y) = _Parent(_Z);
			std::swap(_Color(_Y), _Color(_Z));
			_Y = _Z;
		}
		else
		{
			_Parent(_X) = _Parent(_Y);
			if (_Root() == _Z)
				_Root() = _X;
			else if (_Left(_Parent(_Z)) == _Z)
				_Left(_Parent(_Z)) = _X;
			else
				_Right(_Parent(_Z)) = _X;

			if (_Lmost() != _Z)
				;
			else if (_Right(_Z) == _Nil)
				_Lmost() = _Parent(_Z);
			else
				_Lmost() = _Min(_X);
			if (_Rmost() != _Z)
				;
			else if (_Left(_Z) == _Nil)
				_Rmost() = _Parent(_Z);
			else
				_Rmost() = _Max(_X);
		}
		////////////////////////////////////////////////////////////
		//
		
		if (_Color(_Y) == _Black)
		{
			while (_X != _Root() && _Color(_X) == _Black)
			{
				if (_X == _Left(_Parent(_X)))  //���֧
				{
					_Nodeptr _W = _Right(_Parent(_X));
					if (_Color(_W) == _Red)
					{
						_Color(_W) = _Black;
						_Color(_Parent(_X)) = _Red;
						_Lrotate(_Parent(_X));
						_W = _Right(_Parent(_X)); 
					}
					if (_Color(_Left(_W)) == _Black && _Color(_Right(_W)) == _Black)
					{
						_Color(_W) = _Red;
						_X = _Parent(_X); 
					}
					else
					{
						if (_Color(_Right(_W)) == _Black)
						{
							_Color(_Left(_W)) = _Black;
							_Color(_W) = _Red;
							_Rrotate(_W);
							_W = _Right(_Parent(_X)); 
						}
						_Color(_W) = _Color(_Parent(_X));
						_Color(_Parent(_X)) = _Black;
						_Color(_Right(_W)) = _Black;
						_Lrotate(_Parent(_X));
						break; 
					}
				}
				else   //�ҷ�֧
				{
					_Nodeptr _W = _Left(_Parent(_X));
					if (_Color(_W) == _Red)
					{
						_Color(_W) = _Black;
						_Color(_Parent(_X)) = _Red;
						_Rrotate(_Parent(_X));
						_W = _Left(_Parent(_X)); 
					}
					if (_Color(_Right(_W)) == _Black
						&& _Color(_Left(_W)) == _Black)
					{
						_Color(_W) = _Red;
						_X = _Parent(_X); 
					}
					else
					{
						if (_Color(_Left(_W)) == _Black)
						{_Color(_Right(_W)) = _Black;
						_Color(_W) = _Red;
						_Lrotate(_W);
						_W = _Left(_Parent(_X)); 
						}
						_Color(_W) = _Color(_Parent(_X));
						_Color(_Parent(_X)) = _Black;
						_Color(_Left(_W)) = _Black;
						_Rrotate(_Parent(_X));
						break; 
					}
				}
			}
			_Color(_X) = _Black;
		}
		
		_Destval(&_Value(_Y));
		_Freenode(_Y);
		--_Size;
		return (_P);
	}

protected:
	iterator _Insert(_Nodeptr _X, _Nodeptr _Y, const _Ty& _V)
	{
		//_X  _Y  _Z
		_Nodeptr _Z = _Buynode(_Y, _Red);
		_Left(_Z) = _Nil, _Right(_Z) = _Nil;
		_Consval(&_Value(_Z), _V);
		++_Size;

		if (_Y == _Head || _X != _Nil || key_compare(_Kfn()(_V), _Key(_Y)))
		{
			_Left(_Y) = _Z;
			if (_Y == _Head)
			{
				_Root() = _Z;
				_Rmost() = _Z; 
			}
			else if (_Y == _Lmost())
				_Lmost() = _Z;
		}
		else
		{
			_Right(_Y) = _Z;
			if (_Y == _Rmost())
				_Rmost() = _Z;
		}

		/////////////////////////////////////////////////////////////////
		//�Ƿ�ƽ�⣿ ����ƽ��
		for(_X=_Z; _X!=_Root()&&_Color(_Parent(_X))==_Red ;) //���֧
		{
			if( _Parent(_X) == _Left(_Parent(_Parent(_X))))
			{
				_Y = _Right(_Parent(_Parent(_X)));//�岮�ڵ�
				if( _Color(_Y) == _Red)           //�岮�ڵ���ɫ��ɫ
				{
					_Color(_Parent(_X)) = _Black;
					_Color(_Y) = _Black;
					_Color(_Parent( _Parent(_X) )) = _Red;
					_X = _Parent(_Parent(_X));
				}
				else  //�岮�ڵ���ɫ��ɫ
				{
					if(_X == _Right(_Parent(_X)))  //   <
					{
						_X = _Parent(_X);
						_Lrotate(_X);   //??
					}    
					_Color(_Parent(_X)) = _Black;
					_Color(_Parent(_Parent(_X))) = _Red;
					_Rrotate(_Parent(_Parent(_X))); //??
				}
			}
			else   //�ҷ�֧
			{
				_Y = _Left(_Parent(_Parent(_X)));
				if (_Color(_Y) == _Red)
				{
					_Color(_Parent(_X)) = _Black;
					_Color(_Y) = _Black;
					_Color(_Parent(_Parent(_X))) = _Red;
					_X = _Parent(_Parent(_X)); 
				}
				else
				{
					if (_X == _Left(_Parent(_X)))
					{
						_X = _Parent(_X);
						_Rrotate(_X); 
					}
					_Color(_Parent(_X)) = _Black;
					_Color(_Parent(_Parent(_X))) = _Red;
					_Lrotate(_Parent(_Parent(_X))); 
				}
			}
		}

		_Color(_Root()) = _Black;
		return iterator(_Z);
	}
protected:
	static _Nodeptr _Max(_Nodeptr _P)
	{
		while(_Right(_P) != _Nil)
			_P = _Right(_P);
		return _P;
	}
	static _Nodeptr _Min(_Nodeptr _P)
	{
		while(_Left(_P) != _Nil)
			_P = _Left(_P);
		return _P;
	}
	static _Nodeptr _Nil;  //1
	static size_t _Nilrefs;
	void _Init()
	{
		_Nodeptr _Tmp = _Buynode(0, _Black);
		{
			//_Lockit _Lk;  //lock  ��Զ��߳�
			if (_Nil == 0)
			{
				_Nil = _Tmp;
				_Tmp = 0;
				_Left(_Nil) = 0, _Right(_Nil) = 0; 
			}
			++_Nilrefs; 
		}
		if (_Tmp != 0)
			_Freenode(_Tmp);
		_Head = _Buynode(_Nil, _Red), _Size = 0;
		_Lmost() = _Head, _Rmost() = _Head; 
	}

	_Nodeptr& _Lmost()
	{return (_Left(_Head)); }
	_Nodeptr& _Rmost()
	{return (_Right(_Head)); }
	_Nodeptr& _Root()
    {return (_Parent(_Head)); }

	void _Lrotate(_Nodeptr _X)
	{
		_Nodeptr _Y = _Right(_X);
		_Right(_X) = _Left(_Y);
		if(_Left(_Y) != _Nil)
			_Parent(_Left(_Y)) = _X;
		_Parent(_Y) = _Parent(_X);
		if(_X == _Root())
			_Root() = _Y;
		else if(_X == _Left(_Parent(_X)))
			_Left(_Parent(_X)) = _Y;
		else
			_Right(_Parent(_X)) = _Y;
		_Left(_Y) = _X;
		_Parent(_X) = _Y;
	}
	void _Rrotate(_Nodeptr _X)
	{
		_Nodeptr _Y = _Left(_X);
		_Left(_X) = _Right(_Y);
		if(_Right(_Y) != _Nil)
			_Parent(_Right(_Y)) = _X;
		_Parent(_Y) = _Parent(_X);
		if(_X == _Root())
			_Root() = _Y;
		else if(_X == _Right(_Parent(_X)))
			_Right(_Parent(_X)) = _Y;
		else
			_Left(_Parent(_X)) = _Y;
		_Right(_Y) = _X;
		_Parent(_X) = _Y;
	}
protected:
	_Nodeptr _Buynode(_Nodeptr _Parg, _Redbl _Carg)
    {
		_Nodeptr _S = (_Nodeptr)allocator._Charalloc(1 * sizeof (_Node));
		_Parent(_S) = _Parg;
		_Color(_S) = _Carg;
		return (_S); 
	}
	void _Consval(pointer _P, const _Ty& _V)
	{_Construct(&*_P, _V); }
	void _Destval(pointer _P)
	{_Destroy(&*_P); }
	void _Freenode(_Nodeptr _S)
	{allocator.deallocate(_S, 1); }
private:
	_A allocator;
	_Pr key_compare;
	_Nodeptr _Head;
	bool _Multi;
	size_type _Size;
};

template<class _K, class _Ty, class _Kfn, class _Pr, class _A>
_Tree<_K, _Ty, _Kfn, _Pr, _A>::_Nodeptr _Tree<_K, _Ty, _Kfn, _Pr, _A>::_Nil = 0;  //1

template<class _K, class _Ty, class _Kfn, class _Pr, class _A>
size_t _Tree<_K, _Ty, _Kfn, _Pr, _A>::_Nilrefs = 0;

#endif