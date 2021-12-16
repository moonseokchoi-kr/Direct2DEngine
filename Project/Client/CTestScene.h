#pragma once

class CScene;

class CTestScene
{
public:
	static void CreateTestScene();
	static void CreatePrefab();
private:
	static void InitTestMap(CScene* scene);
};

