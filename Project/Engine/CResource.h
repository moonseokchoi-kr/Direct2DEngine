#pragma once
#include "CEntity.h"
#include "ptr.h"
/// <summary>
/// ���ӳ����� ���Ǵ� ��� ���ҽ� ���� �ֻ��� �θ� Ŭ����
/// 
/// �ۼ��� �ֹ���
/// 
/// ����
/// 1.0 - �⺻���� �ۼ� 2021-09-27
/// 1.1 - custom shared ptr ���� 2021-10-15
/// </summary>
class CResource :
    public CEntity
{

public:
    CResource();
    virtual ~CResource();
public:
    void SetKey(const wstring& key) { key_ = key; }
    const wstring& GetKey() { return key_; }

    void SetRelativePath(const wstring& path) { relative_path_ = path; }
    const wstring& GetRelativePath() { return relative_path_; }

    CResource* Clone() { return nullptr; }
private:
    void IncreaseReferenceCount() { ++reference_count_; }
    void DecreaseReferenceCount() { --reference_count_; }
private:
    wstring key_;
    wstring relative_path_;
    UINT reference_count_;

    template<typename T>
    friend class Ptr;

};

