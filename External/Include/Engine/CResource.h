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
/// 
class CResourceManager;

class CResource :
    public CEntity
{

public:
    CResource(RESOURCE_TYPE type);
    virtual ~CResource();
public:
    void SetKey(const wstring& key) { key_ = key; }
    const wstring& GetKey() { return key_; }

    void SetRelativePath(const wstring& path) { relative_path_ = path; }
    const wstring& GetRelativePath() { return relative_path_; }
    RESOURCE_TYPE GetResourceType() { return resource_type_; }
protected:
    virtual HRESULT Load(const wstring& filePath) { assert(nullptr); return E_FAIL; }
    virtual HRESULT Save(const wstring& relativePath) { assert(nullptr); return E_FAIL; }
    CResource* Clone() { return nullptr; }
    
private:
    void IncreaseReferenceCount() { ++reference_count_; }
    void DecreaseReferenceCount() { --reference_count_; }
    UINT GetReferenceCount() { return reference_count_; }
private:
    wstring key_;
    wstring relative_path_;
    UINT reference_count_;
    RESOURCE_TYPE resource_type_;

    template<typename T>
    friend class Ptr;
    friend class CResourceManager;

};

