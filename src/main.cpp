#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>

int main()
{
	bool debug = false;

    //Initiliaze width and height of screen
	sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
	int width = desktopMode.width;
	int height = desktopMode.height;
	sf::Clock clock;

	bool mouseHidden = false;
	float mouseX = 0.0f, mouseY = 0.0f;
	sf::Vector2f mouse;
	float mouseSensivity = 1.0f;

	bool wasdud[6] = {false, false, false, false, false, false};
	sf::Vector3f position = sf::Vector3f(-5.0f, 0.0f, 0.0f);
	sf::Vector3f dir = sf::Vector3f(0.0f, 0.0f, 0.0f);
	float speed = 0.2f;

    //Create a window and set frame limit 60
	sf::RenderWindow window(sf::VideoMode(width, height), "Test", sf::Style::Fullscreen);
	window.setFramerateLimit(60);

    //Create empty sprite, this sprite reference to empty texture
	sf::RenderTexture emptyTexture;
	emptyTexture.create(width, height);
	sf::Sprite emptySprite = sf::Sprite(emptyTexture.getTexture());

	sf::Font font;
    if (!font.loadFromFile("include/Dudka Regular.ttf"))
	{
		std::cout << "There's no Dudra Regular.ttf in include. Please check it.\n";
        return -1;
	}

    // Создаем объект текста
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    text.setPosition(100, 100);


    
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
			else if (event.type == sf::Event::MouseMoved)
			{
				if (mouseHidden)
				{
					mouseX += event.mouseMove.x;
					mouseY += event.mouseMove.y;
					sf::Mouse::setPosition(sf::Vector2i(width / 2, height / 2), window);
				}
			}
			else if (event.type == sf::Event::MouseButtonPressed)
			{
				if (!mouseHidden)
				{
					window.setMouseCursorVisible(false);
					mouseHidden = true;
				}
				else if (mouseHidden)
				{
					window.setMouseCursorVisible(true);
					mouseHidden = false;
				}
			}
			else if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape) debug = !debug;
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

			float mx = ((float)mouseX / width) * mouseSensivity;
			float my = ((float)mouseY / height) * mouseSensivity;
			mouse = sf::Vector2f(mx, my);
			
			std::ostringstream ss;
			ss << "x: " << position.x << "\ny: " << position.y << "\nz: " << position.z <<
				"\n\nmouse x: " << mx << "\nmouse y: " << my;
			text.setString(ss.str());
		}
		window.clear();

        //Drawing
		window.draw(emptySprite, &shader);
		if (debug)
		{
			window.draw(text);
		}
		shader.setUniform("u_time", clock.getElapsedTime().asSeconds());
		shader.setUniform("u_position", position);
		shader.setUniform("u_mouse", mouse);

        //Display current frame
		window.display();
	}
	return 0;
}