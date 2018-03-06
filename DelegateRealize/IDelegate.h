/*
	阅读时配乐：卡农、雨的印记、菊次郎的夏天、雪之梦、悲怆、星空、天空之城、秋日私语、克罗地亚狂想曲、梦中的婚礼、夜的钢琴曲5、寂静之声、泪之曲、梦中的雪、风居住的街道、神秘园之歌
*/

#pragma once
#include <typeinfo.h> 
#include<list>
//基类，定义了接口的调用方式
class IDelegate
{
public:
	IDelegate(){};
	virtual ~IDelegate(){};
	virtual bool isType(const std::type_info& _type)=0; //在type_info当中保有一个类型实现的指定信息
	virtual bool compare(IDelegate* _delegate)const = 0;
	virtual void invoke() = 0;	//无参数返回值的委托调用
private:

};

//派生出能注册普通函数的委托,也就是说普通函数的委托对象
class CStaticDelegate:public IDelegate
{
public:
	typedef void(*Func)();	//无返回值无参数的函数指针类型，而不是指针

	virtual bool isType(const std::type_info& _type);
	virtual bool compare(IDelegate* _delegate)const;
	virtual void invoke();
	CStaticDelegate(Func _func) :m_func(_func){};
	~CStaticDelegate(){};
private:
	Func m_func;

};

//成员函数的委托
template<typename T>
class CMethodDelegate:public IDelegate
{
public:
	typedef void(T::*Func)();  //成员函数的指针类型
	CMethodDelegate(T* _object, Func _func) :m_object(_object), m_func(_func){};
	virtual bool isType(const std::type_info& _type)
	{
		return typeid(CMethodDelegate<T>) == _type;
	}
	virtual bool compare(IDelegate* _delegate) const
	{
		if (_delegate == 0 || !_delegate->isType(typeid(CMethodDelegate<T>)))
			return false;
		CMethodDelegate<T>* cast = static_cast<CMethodDelegate<T>*>(_delegate);
		return cast->m_object == m_object && cast->m_func == m_func;

	}
	virtual void invoke()
	{
		(m_object->*m_func)();
	}

private:
	T* m_object;
	Func m_func;
};

//生成函数指针的对象接口--普通函数和静态函数
inline IDelegate* newDelegate(void(*_func)())
{
	return new CStaticDelegate(_func);
}
template<typename T>//--成员函数
inline IDelegate* newDelegate(T *_object, void(T::*_func)())
{
	return new CMethodDelegate<T>(_object, _func);
}

//实现多播委托，多播即通知给多个对象
class CMultiDelegate
{
public:
	typedef std::list<IDelegate*> ListDelegate;				//委托对象列表
	typedef ListDelegate::iterator ListDelegateIterator;	//委托迭代器
	typedef ListDelegate::const_iterator ConstListDelegateIterator; //委托遍历迭代器
	CMultiDelegate(){};
	~CMultiDelegate(){};
	bool empty()const;
	void clear();
	CMultiDelegate& operator+=(IDelegate* _delegate);
	CMultiDelegate& operator-=(IDelegate* _delegate);
	void operator()();
private:
	CMultiDelegate(const CMultiDelegate& _event){};
	CMultiDelegate& operator=(const CMultiDelegate& _event);
	ListDelegate mListDelegates;
};