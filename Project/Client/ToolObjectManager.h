#pragma once


class ToolObjectManager :
    public CSingleton< ToolObjectManager>
{
    SINGLE(ToolObjectManager);
public:
    void Init();
    void Update();

public:

};

