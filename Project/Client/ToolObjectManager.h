#pragma once

class ToolGameObject;

class ToolObjectManager :
    public CSingleton< ToolObjectManager>
{
    SINGLE(ToolObjectManager);
public:
    void Init();
    void Update();
    void Render();

public:

private:
    vector<ToolGameObject*> tool_object_vector_;
};

