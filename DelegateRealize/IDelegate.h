/*
	�Ķ�ʱ���֣���ũ�����ӡ�ǡ��մ��ɵ����졢ѩ֮�Ρ����롢�ǿա����֮�ǡ�����˽����޵��ǿ����������еĻ���ҹ�ĸ�����5���ž�֮������֮�������е�ѩ�����ס�Ľֵ�������԰֮��
*/

#pragma once
#include <typeinfo.h> 
#include<list>
//���࣬�����˽ӿڵĵ��÷�ʽ
class IDelegate
{
public:
	IDelegate(){};
	virtual ~IDelegate(){};
	virtual bool isType(const std::type_info& _type)=0; //��type_info���б���һ������ʵ�ֵ�ָ����Ϣ
	virtual bool compare(IDelegate* _delegate)const = 0;
	virtual void invoke() = 0;	//�޲�������ֵ��ί�е���
private:

};

//��������ע����ͨ������ί��,Ҳ����˵��ͨ������ί�ж���
class CStaticDelegate:public IDelegate
{
public:
	typedef void(*Func)();	//�޷���ֵ�޲����ĺ���ָ�����ͣ�������ָ��

	virtual bool isType(const std::type_info& _type);
	virtual bool compare(IDelegate* _delegate)const;
	virtual void invoke();
	CStaticDelegate(Func _func) :m_func(_func){};
	~CStaticDelegate(){};
private:
	Func m_func;

};

//��Ա������ί��
template<typename T>
class CMethodDelegate:public IDelegate
{
public:
	typedef void(T::*Func)();  //��Ա������ָ������
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

//���ɺ���ָ��Ķ���ӿ�--��ͨ�����;�̬����
inline IDelegate* newDelegate(void(*_func)())
{
	return new CStaticDelegate(_func);
}
template<typename T>//--��Ա����
inline IDelegate* newDelegate(T *_object, void(T::*_func)())
{
	return new CMethodDelegate<T>(_object, _func);
}

//ʵ�ֶಥί�У��ಥ��֪ͨ���������
class CMultiDelegate
{
public:
	typedef std::list<IDelegate*> ListDelegate;				//ί�ж����б�
	typedef ListDelegate::iterator ListDelegateIterator;	//ί�е�����
	typedef ListDelegate::const_iterator ConstListDelegateIterator; //ί�б���������
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