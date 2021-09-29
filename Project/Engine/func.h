#pragma  once


template<typename T, int N>

void Safe_Delete_Array(array<T, N> _arr)
{
	for (int i = 0; i < _arr.size(); ++i)
	{
		if(nullptr != _arr[i])
			delete _arr[i];
	}
}

template<typename T1, typename T2>

void Safe_Delete_Map(unordered_map<T1,T2> _map)
{
	typename unordered_map<T1, T2>::iterator iter = _map.begin();

	for (; iter != _map.end(); ++iter)
	{
		if (nullptr != iter->second)
			delete iter->second;
	}

	_map.clear();
}
