#include "Tutorial.h"
#include "../Game.h"

Tutorial::Tutorial(Game* game) : Node(game)
{
}

void Tutorial::LevelLoaded(int levelIndex)
{
	DestroyTexts();
	if (!textsMap.contains(levelIndex))
	{
		return;
	}

	vector<pair<string, Vector2>>& textPairs = textsMap[levelIndex];
	for (auto& pair : textPairs)
	{
		string& t = pair.first;
		Vector2& pos = pair.second;

		Text* text = new Text(game);
		AddChild(text);

		text->pos = pos;
		text->SetText(t);
		text->SetFontSize(fontSize);
		text->SetColor(textColor);

		texts.push_back(text);
	}

}

void Tutorial::DestroyTexts()
{
	for (auto& text : texts)
	{
		text->Destroy(this);
	}
	texts.clear();
}
