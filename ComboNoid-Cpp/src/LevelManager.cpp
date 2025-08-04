#include "LevelManager.h"


LevelManager::LevelManager()
{
	printf("Level Manager Created\n");
}

void LevelManager::Init()
{
	printf("Level Manager Initialized\n");
    currentLevel = LoadLevel("levels/Level1.tmx");
}

shared_ptr<Level> LevelManager::LoadLevel(string path)
{
	shared_ptr<Level> level = make_shared<Level>(path);
	level->Load();

	return level;
}

Level::Level(string path) : path(path)
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

    for (XMLElement* layer = map->FirstChildElement("layer"); layer != nullptr; layer = layer->NextSiblingElement("layer"))
    {
        Layer* l = new Layer();
        string name = layer->Attribute("name");
        layer->QueryIntAttribute("width", &l->width);
        layer->QueryIntAttribute("height", &l->height);

        XMLElement* data = layer->FirstChildElement("data");
        if (!data) continue;

        const char* csvText = data->GetText();
        if (!csvText) continue;

        stringstream ss(csvText);
        string item;
        while (getline(ss, item, ','))
        {
            int id = stoi(item);
            l->data.push_back(id);
        }

        if (name == "Tiles")
        {
            tiles = l;
        }
        else if (name == "Data")
        {
            this->data = l;
        }
    }

}
