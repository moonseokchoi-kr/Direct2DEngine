#pragma  once


template<typename T, int N>

void Safe_Delete_Array(array<T, N> arr)
{
	for (int i = 0; i < arr.size(); ++i)
	{
		if(nullptr != arr[i])
			delete arr[i];
	}
}

template<typename T1, typename T2>

void Safe_Delete_Map(unordered_map<T1,T2> map)
{
	typename unordered_map<T1, T2>::iterator iter = map.begin();

	for (; iter != map.end(); ++iter)
	{
		if (nullptr != iter->second)
			delete iter->second;
	}

	map.clear();
}


template<typename T>
void Safe_Delete_Vec(vector<T> vec)
{
	for (auto& v : vec)
	{
		if (nullptr != v)
		{
			delete v;
		}
	}
	vec.clear();
}
