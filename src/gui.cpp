#include "gui.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System/Vector2.hpp>
#include "context.h"

extern globalContext context;

//ButtonPanel
gui::ButtonPanel::ButtonPanel() {}

gui::ButtonPanel::ButtonPanel(sf::Vector2<float> position) : position(position) {}

void gui::ButtonPanel::processButtons(sf::Vector2i mouse_pos)
{
	printf("ButtonList Size in processButtons = %d", buttonList.size());
	for (Button* button : buttonList)
	{
		if (button->sprite->getGlobalBounds().contains(sf::Vector2<float>((float)mouse_pos.x, (float)mouse_pos.y)))
		{
			printf("Here");
			button->process();
		}
	}
}

void gui::ButtonPanel::renderButtons()
{
	for (Button* button : buttonList)
	{
		button->window->draw(*(button->sprite));
	}
}

void gui::ButtonPanel::addButton(Button* button)
{
	printf("ButtonList Size = %d\n", buttonList.size());
	button->position = { position.x,position.y + 100 * buttonList.size() };
	button->sprite->setPosition(button->position);
	buttonList.push_back(button);
	printf("Button Position is %f, %f\n", buttonList.back()->sprite->getPosition().x, buttonList.back()->sprite->getPosition().y);
}


//Button
gui::Button::Button(std::string fileName, sf::RenderWindow* window)
{
	this->window = window;
	sf::Texture* texture = new sf::Texture();
	if (!texture->loadFromFile("Assets/Icons/" + fileName))
	{
		texture->loadFromFile("Assets/Icons/no-image.png");
	}
	printf(("Assets/Icons/" + fileName).c_str());
	sprite = new sf::Sprite(*texture);
}


//MapButton
gui::MapButton::MapButton(std::string fileName, sf::RenderWindow* window, worldMap* map, mapMode* mode) : Button(fileName, window)
{
	this->map = map;
	this->resourceIndex = resourceIndex;
	this->mode = mode;
}

void gui::MapButton::process()
{
	map->updateMapMode(mode);
}


//StartGameButton
void gui::StartGameButton::process()
{
	// using 30 instead of 100 to test enemy movement;
	context.scene = new worldMapScene(context.window, 30, 30, 2);
}

gui::StartGameButton::StartGameButton(std::string fileName, sf::RenderWindow* window) : Button(fileName, window)
{
	this->window = window;
}


//ActionsMenu
gui::ActionsMenu::ActionsMenu(sf::Vector2f pos, sf::Vector2f size)
{
	menuFont = std::make_unique<sf::Font>("Assets/Fonts/BigShouldersStencil.ttf");
	if (!menuFont)
	{
		std::cout << "ERROR::LOADFROMFILE::ActionsMenu::menuFont\n";
	}

	backgroundRect.setSize(size);
	backgroundRect.setPosition(pos);
	backgroundRect.setFillColor(sf::Color{ 128,128,128,50 });

	currentIndex = 0;
}

void gui::ActionsMenu::draw(sf::RenderWindow* window)
{
	window->draw(backgroundRect);

	for (const auto& text : textActions)
	{
		window->draw(text);
	}

}

void gui::ActionsMenu::addAction(std::string action)
{
	sf::Text temp(*menuFont);
	temp.setString(action);
	temp.setCharacterSize(52);

	textActions.push_back(temp);

	int index = 1;
	for (auto& action : textActions)
	{
		action.setPosition(sf::Vector2f{ backgroundRect.getPosition().x + 0.05f * backgroundRect.getPosition().x,
			backgroundRect.getPosition().y + (index - 1) *
			(backgroundRect.getGlobalBounds().size.y / textActions.size()) });

		index++;
	}

	if (textActions.size() == 1)
	{
		std::string temp{ textActions[currentIndex].getString() };
		temp = ">" + temp;
		textActions[currentIndex].setString(temp);
		textActions[currentIndex].setFillColor(sf::Color::Yellow);
	}

}

void gui::ActionsMenu::changeIndex(int val)
{
	if (currentIndex == (textActions.size() - 1) && val > 0) { return; }
	if (currentIndex == 0 && val < 0) { return; }

	// get sttring without > char
	std::string temp = textActions[currentIndex].getString().toAnsiString().substr(1,
		textActions[currentIndex].getString().getSize() - 1);

	textActions[currentIndex].setString(temp);
	textActions[currentIndex].setFillColor(sf::Color::White);
	currentIndex += val;

	temp = textActions[currentIndex].getString().toAnsiString();
	temp = ">" + temp;

	textActions[currentIndex].setString(temp);
	textActions[currentIndex].setFillColor(sf::Color::Yellow);

}

std::string gui::ActionsMenu::getCurrentAction() const
{
	return textActions[currentIndex].getString().toAnsiString().substr(1, 
		textActions[currentIndex].getString().getSize());
}

int gui::ActionsMenu::getCurrentIndex() const
{
	return currentIndex;
}

sf::Vector2f gui::ActionsMenu::getPosition() const
{
	return backgroundRect.getPosition();
}

void gui::ActionsMenu::setBackGroundColor(sf::Color color)
{
	backgroundRect.setFillColor(color);
}


// Texts
gui::RealTimePrintText::RealTimePrintText(std::string text, sf::Font& font, float speed, sf::Vector2f pos)
	:m_strText(text), m_printSpeed(speed)
{
	m_addLetterTime = m_animationClock.getElapsedTime();
	m_animationClock.start();

	m_currentIndex = 0;
	m_currentStrText = m_strText[m_currentIndex];

	m_text = std::make_shared<sf::Text>(font);
	m_text->setString(m_currentStrText);
	m_text->setPosition(pos);
	m_text->setCharacterSize(36);
}

void gui::RealTimePrintText::setText(std::string text)
{
	m_strText = text;
	m_currentIndex = 0;
	m_currentStrText = m_strText[m_currentIndex];

	m_text->setString(m_currentStrText);
	m_addLetterTime = m_animationClock.restart();

}

void gui::RealTimePrintText::update()
{
	if (m_currentIndex == m_strText.size() - 1) { return; }

	m_addLetterTime = m_animationClock.getElapsedTime();
	if (m_addLetterTime.asSeconds() >= m_printSpeed)
	{
		m_currentIndex++;
		m_currentStrText += m_strText[m_currentIndex];
		m_text->setString(m_currentStrText);

		m_addLetterTime = m_animationClock.restart();
	}

}

void gui::RealTimePrintText::draw(sf::RenderWindow* window)
{
	update();

	window->draw(*m_text);
}

void gui::FadePanel::update()
{
	if (m_currentTime.asSeconds() >= m_fadeTime)
	{
		m_clock.stop();
		return;
	}

	sf::Color currentColor = m_background.getFillColor();
	m_currentTime = m_clock.getElapsedTime();

	
	float percent = m_currentTime.asSeconds() / m_fadeTime;

	if (m_isShown)
	{
		currentColor.a = m_maxTransparency * percent;
		std::cout << m_maxTransparency * percent << '\n';
	}
	else
	{
		currentColor.a = m_maxTransparency - m_maxTransparency * percent;
		if ((m_maxTransparency - m_maxTransparency * percent) < 0)
		{
			currentColor.a = 0;
		}
		std::cout << m_maxTransparency - m_maxTransparency * percent << '\n';
	}
	

	

	m_background.setFillColor(currentColor);

}

gui::FadePanel::FadePanel(sf::Vector2f pos, sf::Vector2f size, float animationTime, sf::Color color)
	:m_fadeTime(animationTime)
{
	m_maxTransparency = 255;
	color.a = 0;
	m_isShown = false;

	m_background.setSize(size);
	m_background.setPosition(pos);
	m_background.setFillColor(color);

	m_currentTime = m_clock.restart();

}

void gui::FadePanel::draw(sf::RenderWindow* window)
{
	update();

	window->draw(m_background);
}

void gui::FadePanel::fadeBack()
{
	m_isShown = !m_isShown;
	m_currentTime = sf::Time::Zero;
	m_clock.restart();
	update();
}

void gui::FadePanel::setMaxTransparency(int maxT)
{
	if (maxT > 255)
	{
		//throw exception here
		m_maxTransparency = 255;
		return;
	}
	m_maxTransparency = maxT;
}


gui::AbilityInfoPanel::AbilityInfoPanel(sf::Vector2f pos, sf::Vector2f size, float animationTime, sf::Font& textFont, sf::Color color)
	:FadePanel(pos, size, animationTime, color)
{
	m_title = std::make_shared<sf::Text>(textFont, "Title");
	m_description = std::make_shared<sf::Text>(textFont, "Description");

	m_title->setPosition(pos);
	m_description->setPosition(sf::Vector2f{ pos.x, pos.y + m_description->getCharacterSize() });

}

// Ability panel
void gui::AbilityInfoPanel::draw(sf::RenderWindow* window)
{
	FadePanel::draw(window);

	if (m_isShown)
	{
		window->draw(*m_title);
		window->draw(*m_description);
	}
	
}

void gui::AbilityInfoPanel::setTitle(std::string title)
{
	m_title->setString(title);
}

void gui::AbilityInfoPanel::setDescription(std::string descr)
{
	m_description->setString(descr);
}
