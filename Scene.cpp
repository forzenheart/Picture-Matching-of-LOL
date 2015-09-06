#include "Scene.h"
#include "UI.h"

bool CScene::LoadUI(const char* id, CUI* pUI)
{
	std::string s = id;
	if (m_UI.end() != m_UI.find(s))
		return false;

	//让本场景的UI的场景指针都指向本场景
	pUI->scene = this;

	m_UI.insert(std::pair<std::string, CUI*>(s, pUI));

	return true;
}

bool CScene::ReleaseUI(const char* id)
{
	std::string s = id;
	std::map<std::string, CUI*>::iterator i = m_UI.find(s);
	if (i == m_UI.end())
		return false;

	i->second->End();
	delete i->second;
	m_UI.erase(i);

	return true;
}

CUI* CScene::GetUI(const char* id)
{
	std::string s = id;
	std::map<std::string, CUI*>::iterator i = m_UI.find(s);
	return i == m_UI.end() ? 0 : i->second;
}

bool CScene::SetKeyUI(const char* id)
{
	if (0 == id)
	{
		m_KeyUI = 0;
		return true;
	}
	else
	{
		std::string s = id;
		std::map<std::string, CUI*>::iterator i = m_UI.find(s);
		if (i == m_UI.end())
			return false;
		else
		{
			m_KeyUI = i->second;
			return true;
		}
	}
}

CScene::CScene():m_KeyUI(0){}
CScene::~CScene()
{
	std::map<std::string, CUI*>::iterator i;
	for (i = m_UI.begin(); i != m_UI.end(); ++i)
		delete i->second;
}
void CScene::Init(){}
void CScene::Enter(){}
void CScene::Run(){}
void CScene::Quit(){}
void CScene::End(){}