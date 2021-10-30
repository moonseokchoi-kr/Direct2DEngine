#pragma once
#include "CEntity.h"
#include "ptr.h"
/// <summary>
/// 게임내에서 사용되는 모든 리소스 들의 최상위 부모 클래스
/// 
/// 작성자 최문석
/// 
/// 버전
/// 1.0 - 기본구조 작성 2021-09-27
/// 1.1 - custom shared ptr 구성 2021-10-15
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

