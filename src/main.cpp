#include <SFML/Graphics.hpp>

int main()
{
    //Initiliaze width and height of screen
	int width = 2560;
	int height = 1600;

    //Create a window and set frame limit 60
	sf::RenderWindow window(sf::VideoMode(width, height), "Test", sf::Style::Default);
	window.setFramerateLimit(60);

    //Create empty sprite, this sprite reference to empty texture
	sf::RenderTexture emptyTexture;
	emptyTexture.create(width, height);
	sf::Sprite emptySprite = sf::Sprite(emptyTexture.getTexture());
    
    //Create shader and get it from file .frag
	sf::Shader shader;
	shader.loadFromFile("src/OutputShader.frag", sf::Shader::Fragment);

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
		}
        //Clearing previous frame
        window.clear(sf::Color::Cyan);

        //Drawing
		window.draw(emptySprite, &shader);

        //Display current frame
		window.display();
	}
	return 0;
}