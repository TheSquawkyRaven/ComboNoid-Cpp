#include "LevelManager.h"

#include <map>
#include <sstream>
#include <memory>


LevelManager::LevelManager()
{
}

void LevelManager::Init()
{
}

shared_ptr<Level> LevelManager::LoadLevel(int levelIndex)
{
    if (levelIndex < 0 || levelIndex >= levels.size())
    {
        printf("Invalid level index: %d\n", levelIndex);
        return nullptr;
	}
    string levelName = levels.at(levelIndex);
    string path = "levels/" + levelName + ".tmx";
    shared_ptr<Level> level = make_shared<Level>(path);
    level->Load();

    return level;
}

Level::Level(const string& path) : path(path)
{
}

void Level::Load()
{
	XMLDocument doc;
	if (doc.LoadFile(path.c_str()) != XML_SUCCESS)
	{
		printf("Failed to load level file: %s\n", path.c_str());
		return;
	}

	XMLElement* map = doc.FirstChildElement("map");
	if (!map)
	{
		printf("No map element found in level file: %s\n", path.c_str());
		return;
	}

    XMLElement* layer = map->FirstChildElement("layer");
    Layer* l = new Layer();
    string name = layer->Attribute("name");
    layer->QueryIntAttribute("width", &l->width);
    layer->QueryIntAttribute("height", &l->height);

    XMLElement* data = layer->FirstChildElement("data");
    const char* csvText = data->GetText();

    stringstream ss(csvText);
    string item;
    while (getline(ss, item, ','))
    {
        int id = stoi(item);
        l->data.push_back(id);
    }

    tiles = l;

}
