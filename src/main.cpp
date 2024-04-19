#include <SFML/Graphics.hpp>

int main()
{
    //Initiliaze width and height of screen
	int width = 1920;
	int height = 1080;
	sf::Clock clock;

	bool mouseHidden = false;
	bool wasdud[6] = {false, false, false, false, false, false};
	sf::Vector3f position = sf::Vector3f(-5.0, 0.0, 0.0);
	float speed = 0.2;

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
				else if (event.key.code == sf::Keyboard::localize(sf::Keyboard::Scancode::W)) wasdud[0] = true;
				else if (event.key.code == sf::Keyboard::localize(sf::Keyboard::Scancode::A)) wasdud[1] = true;
				else if (event.key.code == sf::Keyboard::localize(sf::Keyboard::Scancode::S)) wasdud[2] = true;
				else if (event.key.code == sf::Keyboard::localize(sf::Keyboard::Scancode::D)) wasdud[3] = true;
				else if (event.key.code == sf::Keyboard::localize(sf::Keyboard::Scancode::Space)) wasdud[4] = true;
				else if (event.key.code == sf::Keyboard::localize(sf::Keyboard::Scancode::LShift)) wasdud[5] = true;
			}
			else if (event.type == sf::Event::KeyReleased)
			{
				if (event.key.code == sf::Keyboard::localize(sf::Keyboard::Scancode::W)) wasdud[0] = false;
				else if (event.key.code == sf::Keyboard::localize(sf::Keyboard::Scancode::A)) wasdud[1] = false;
				else if (event.key.code == sf::Keyboard::localize(sf::Keyboard::Scancode::S)) wasdud[2] = false;
				else if (event.key.code == sf::Keyboard::localize(sf::Keyboard::Scancode::D)) wasdud[3] = false;
				else if (event.key.code == sf::Keyboard::localize(sf::Keyboard::Scancode::Space)) wasdud[4] = false;
				else if (event.key.code == sf::Keyboard::localize(sf::Keyboard::Scancode::LShift)) wasdud[5] = false;
			}
		}

		if (mouseHidden) //Moving cam
		{
			if (wasdud[0]) position.x += speed;
			else if (wasdud[1]) position.y -= speed;
			else if (wasdud[2]) position.x -= speed;
			else if (wasdud[3]) position.y += speed;
			else if (wasdud[4]) position.z -= speed;
			else if (wasdud[5]) position.z += speed;
		}

        //Drawing
		window.draw(emptySprite, &shader);
		shader.setUniform("u_time", clock.getElapsedTime().asSeconds());
		shader.setUniform("u_position", position);

        //Display current frame
		window.display();
	}
	return 0;
}