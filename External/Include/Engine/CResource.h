#pragma once
#include "CEntity.h"

/// <summary>
/// 게임내에서 사용되는 모든 리소스 들의 최상위 부모 클래스
/// 
/// 작성자 최문석
/// 
/// 버전
/// 1.0 - 기본구조 작성 2021-09-27
/// </summary>
class CResource :
    public CEntity
{

public:
    CResource();
    virtual ~CResource();
public:
    void SetKey(const wstring& _key) { m_key = _key; }
    const wstring& GetKey() { return m_key; }

    void SetRelativePath(const wstring& _path) { m_relativePath = _path; }
    const wstring& GetRelativePath() { return m_relativePath; }
private:
    void increaseRefCount() { ++m_refCount; }
    void decreaseRefCount() { --m_refCount; }
private:
    wstring m_key;
    wstring m_relativePath;
    UINT m_refCount;
};

