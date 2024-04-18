#include <SFML/Graphics.hpp>

int main()
{
    //Initiliaze width and height of screen
	int width = 1920;
	int height = 1080;
	sf::Clock clock;

	bool mouseHidden = false;

    //Create a window and set frame limit 60
	sf::RenderWindow window(sf::VideoMode(width, height), "Test", sf::Style::Default);
	window.setFramerateLimit(60);

    //Create empty sprite, this sprite reference to empty texture
	sf::RenderTexture emptyTexture;
	emptyTexture.create(width, height);
	sf::Sprite emptySprite = sf::Sprite(emptyTexture.getTexture());
    
    //Create shader and get it from file .frag
	sf::Shader shader;
	shader.loadFromFile("./src/OutputShader.frag", sf::Shader::Fragment);
	shader.setUniform("u_resolution", sf::Vector2f(width, height));

    //Standart window loop
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			else if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape && !mouseHidden)
				{
					window.setMouseCursorVisible(false);
					mouseHidden = true;
				}
				else if (event.key.code == sf::Keyboard::Escape && mouseHidden)
				{
					window.setMouseCursorVisible(true);
					mouseHidden = false;
				}
			}
		}

        //Drawing
		window.draw(emptySprite, &shader);
		shader.setUniform("u_time", clock.getElapsedTime().asSeconds());

        //Display current frame
		window.display();
	}
	return 0;
}