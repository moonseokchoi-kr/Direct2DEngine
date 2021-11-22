#pragma once
class CWidget
{
public:
	CWidget(const string& name);
	~CWidget();

public:
	virtual void Update() = 0;

public:
	void Activate() { is_active_ = true; }
	void Deactivate() { is_active_ = false; }
	bool IsActive() { return is_active_; }

	const string& GetName() { return name_; }

protected:
	bool is_active_;

private:
	string name_;
	

};

