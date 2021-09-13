#pragma once
/// <summary>
/// Singleton 클래스 제작시 상속을 받아야하는 부모클래스
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

