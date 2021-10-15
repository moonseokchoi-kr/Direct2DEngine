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
    void IncreaseReferenceCount() { ++m_refCount; }
    void DecreaseReferenceCount() { --m_refCount; }
private:
    wstring m_key;
    wstring m_relativePath;
    UINT m_refCount;

    template<typename T>
    friend class Ptr;
};

