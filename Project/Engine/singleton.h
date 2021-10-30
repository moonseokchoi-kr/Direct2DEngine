#pragma once
/// <summary>
/// Singleton Ŭ���� ���۽� ����� �޾ƾ��ϴ� �θ�Ŭ����
/// </summary>
/// 
template<typename T>
class CSingleton
{
public:
	static T* GetInst()
	{
		if (nullptr == g_inst)
		{
			g_inst = new T;
		}
		return g_inst;
	}

protected:
	CSingleton()
	{
		void(*pFunc)(void) = &CSingleton::Destroy;
		atexit(pFunc);
	}
	~CSingleton(){}
private:
	static void Destroy()
	{
		if (nullptr != g_inst)
		{
			delete g_inst;
			g_inst = nullptr;
		}
	}
private:
	static T* g_inst;
};

template<typename T>
T* CSingleton<T>::g_inst = nullptr;

