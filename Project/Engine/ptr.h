#pragma once

template<typename T>
class Ptr
{

public:
	T* Get() { return resource_; }
public:
	Ptr& operator = (const Ptr<T>& ptr)
	{
		if (nullptr != resource_)
		{
			resource_->DecreaseReferenceCount();
		}
		
		resource_ = ptr.resource_;

		if (nullptr != resource_)
		{
			resource_->IncreaseReferenceCount();
		}

		return *this;
	}
	Ptr& operator = (T* ptr)
	{
		if (nullptr != resource_)
		{
			resource_->DecreaseReferenceCount();
		}

		resource_ = ptr;

		if (nullptr != resource_)
		{
			resource_->IncreaseReferenceCount();
		}

		return *this;
	}

	bool operator == (T* res)
	{
		return res == resource_;
	}
	bool operator !=(T* res)
	{
		return res != resource_;
	}

	bool operator ==(const Ptr<T>& ptr)
	{
		return resource_ == ptr.resource_;
	}
	bool operator !=(const Ptr<T>& ptr)
	{
		return resource_ != ptr.resource_;
	}

	T* operator -> ()
	{
		return resource_;
	}

public:
	Ptr()
		:resource_(nullptr)
	{

	}

	Ptr(const Ptr<T>& origin)
		:resource_(origin.resource_)
	{
		if (nullptr != resource_)
		{
			resource_->IncreaseReferenceCount();
		}
	}

	Ptr(T* res)
		:resource_(res)
	{
		if (nullptr != resource_)
		{
			resource_->IncreaseReferenceCount();
		}
	}

	~Ptr()
	{
		if (nullptr != resource_ && resource_->GetReferenceCount() != 0)
		{
			resource_->DecreaseReferenceCount();
		}

	}
private:

	T* resource_;
};

template<typename T>
bool operator == (void* ptr, Ptr<T> origin)
{
	return ptr == origin.Get();
}

template<typename T>
bool operator != (void* ptr, Ptr<T> origin)
{
	return ptr != origin.Get();
}