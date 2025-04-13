#ifndef buttons_h
#define buttons_h
#include "worldMap.h"
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>

namespace gui
{
	class Button
	{
	public:
		sf::Vector2<float> position;
		sf::Sprite* sprite;
		sf::RenderWindow* window;
		Button(std::string fileName, sf::RenderWindow* window);
		virtual void process() = 0;
	};


	class ButtonPanel
	{
	public:
		std::vector<Button*> buttonList;
		sf::Vector2<float> position;
		ButtonPanel();
		ButtonPanel(sf::Vector2<float> position);
		void addButton(Button* button);
		void processButtons(sf::Vector2i mouse_pos);
		void renderButtons();
	};


	class MapButton : virtual public Button
	{
	public:
		worldMap* map;
		mapMode* mode;
		int resourceIndex;
		MapButton(std::string fileName, sf::RenderWindow* window, worldMap* map, mapMode* mode);
		void process() override;
	};

	class StartGameButton : virtual public Button
	{
	public:
		StartGameButton(std::string fileName, sf::RenderWindow* window);
		void process() override;
	};

	template <class T>
	class ProgressBar
	{
	private:

		T maxNumber, minNumber, currentValue;

		sf::RectangleShape outlineRect;
		sf::RectangleShape inlineRect;

		float maxSizeX;

	public:

		ProgressBar(sf::Vector2f pos, sf::Vector2f barSize)
		{
			maxNumber = static_cast<T>(100);
			minNumber = 0;
			currentValue = 0;
			maxSizeX = barSize.x;

			outlineRect.setPosition(pos);
			outlineRect.setSize(barSize);
			outlineRect.setOutlineColor(sf::Color::White);
			outlineRect.setOutlineThickness(5.f);


			inlineRect.setPosition(pos);
			inlineRect.setSize(barSize);
			inlineRect.setFillColor(sf::Color::Red);
		}

		void draw(sf::RenderWindow* window)
		{
			window->draw(outlineRect);
			window->draw(inlineRect);
		}

		void setRange(T min, T max)
		{
			minNumber = min;
			maxNumber = max;
		}

		void changeValue(T value)
		{
			if (value < minNumber) { value = 0; }

			if (value > maxNumber)
			{
				std::cout << "Value out of range\n";
				return;
			}

			currentValue = value;

			inlineRect.setSize(sf::Vector2f{ maxSizeX * static_cast<float>(value) / static_cast<float>(maxNumber), inlineRect.getSize().y });

		}

		void setInlineColor(sf::Color color) { inlineRect.setFillColor(color); }
		void setOutlineColor(sf::Color color) { outlineRect.setOutlineColor(color); }

	};

	
	class ActionsMenu
	{
		
	private:

		std::unique_ptr<sf::Font> menuFont;
		std::vector<sf::Text> textActions;

		sf::RectangleShape backgroundRect;

		int currentIndex;


	public:

		ActionsMenu(sf::Vector2f pos, sf::Vector2f size);

		void addAction(std::string action);
		void changeIndex(int val);

		std::string getCurrentAction() const;
		int getCurrentIndex() const;
		
		void setBackGroundColor(sf::Color color);

		void draw(sf::RenderWindow* window);

	};

	class RealTimePrintText
	{

	private:
		std::string m_strText;
		std::string m_currentStrText;

		std::shared_ptr<sf::Text> m_text;

		float m_printSpeed;
		int m_currentIndex;


		sf::Clock m_animationClock;
		sf::Time m_addLetterTime;

		void update();

	public:

		RealTimePrintText(std::string text,sf::Font& font, float speed, sf::Vector2f pos);

		void setText(std::string text);

		
		void draw(sf::RenderWindow* window);

	};

	class FadePanel
	{

	private:

		sf::RectangleShape m_background;

		float m_fadeTime;
		bool m_isShown;
		int m_maxTransparency;


		sf::Clock m_clock;
		sf::Time m_currentTime;

		virtual void update();

	public:

		FadePanel(sf::Vector2f pos, sf::Vector2f size, float animationTime, sf::Color color = sf::Color{169,169,169,0});

		virtual void draw(sf::RenderWindow* window);

		void setMaxTransparency(int maxT);

	};

}



#endif