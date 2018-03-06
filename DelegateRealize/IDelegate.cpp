#include "stdafx.h"
#include "IDelegate.h"

bool CStaticDelegate::isType(const std::type_info& _type)
{
	return typeid(CStaticDelegate) == _type;   //typeid����˵�᷵��������ͣ���̬��̬������
}

bool CStaticDelegate::compare(IDelegate* _delegate) const
{
	if (0 == _delegate || !_delegate->isType(typeid(CStaticDelegate)))
		return false;
	CStaticDelegate *cast = static_cast<CStaticDelegate*>(_delegate);
	return cast->m_func = m_func;
	
}

void CStaticDelegate::invoke()
{
	m_func();
}


bool CMultiDelegate::empty() const
{
	for (auto iter = mListDelegates.begin(); iter != mListDelegates.end(); iter++)
	{
		if (*iter)return false;
	}
	return true;
}

void CMultiDelegate::clear()
{
	for (auto iter = mListDelegates.begin(); iter != mListDelegates.end(); iter++)
	{
		if (*iter)
		{
			delete(*iter);
			(*iter) = 0;
		}
		
	}
}

void CMultiDelegate::operator()()
{
	auto iter = mListDelegates.begin();
	while (iter!=mListDelegates.end())
	{
		if (0 == (*iter))
		{
			iter = mListDelegates.erase(iter);
		}
		else
		{
			(*iter)->invoke();
			iter++;
		}

	}
}

CMultiDelegate& CMultiDelegate::operator-=(IDelegate* _delegate)
{
	for (auto iter = mListDelegates.begin(); iter != mListDelegates.end(); iter++)
	{
		if ((*iter) && (*iter)->compare(_delegate))
		{
			if ((*iter) != _delegate) delete (*iter);
			(*iter) = 0;
			break;
		}
	}
	delete _delegate;//???
	return *this;
}

CMultiDelegate& CMultiDelegate::operator+=(IDelegate* _delegate)
{
	for (auto iter = mListDelegates.begin(); iter != mListDelegates.end(); iter++)
	{
		if ((*iter)&&(*iter)->compare(_delegate))
		{
			//���𵽵����þ��Ǽ����󣬲�����ί�ж���
			delete _delegate;	//û�д���Ļ���ɾ��ί�ж�������Ϊ��ί���ﱣ����ί�ж���ĸ�����Ҳ����thisָ��
			return *this;
		}
	}
	mListDelegates.push_back(_delegate);
	return *this;
}

