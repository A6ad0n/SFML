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

	//Initiliaze mouse variables
	bool mouseHidden = true;
	int mouseX = 0, mouseY = 0;
	float mx = 0.0f, my = 0.0f;
	float mouseSensivity = 1.0f;

	//Initliaze moving variables
	bool wasdud[6] = {false, false, false, false, false, false};
	sf::Vector3f position = sf::Vector3f(-5.0f, 0.0f, 0.0f);
	float speed = 0.05f;

    //Create a window and set frame limit 60, mouse non visible
	sf::RenderWindow window(sf::VideoMode(width, height), "Demo", sf::Style::Fullscreen);
	window.setFramerateLimit(60);
	window.setMouseCursorVisible(false);

    //Create empty sprite, this sprite reference to empty texture
	sf::RenderTexture emptyTexture;
	emptyTexture.create(width, height);
	sf::Sprite emptySprite = sf::Sprite(emptyTexture.getTexture());

	//Create font
	sf::Font font;
    if (!font.loadFromFile("include/Dudka Regular.ttf"))
	{
		std::cout << "There's no Dudra Regular.ttf in include. Please check it.\n";
        return -1;
	}

    //Create text
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    text.setPosition(20, 20);


    
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
					mouseX = event.mouseMove.x - width / 4;
					mouseY = event.mouseMove.y - height / 4; 
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

				else if (event.key.code == sf::Keyboard::localize(sf::Keyboard::Scancode::O)) mouseSensivity -= 0.1f;
				else if (event.key.code == sf::Keyboard::localize(sf::Keyboard::Scancode::P)) mouseSensivity += 0.1f;

				else if (event.key.code == sf::Keyboard::localize(sf::Keyboard::Scancode::Num0)) speed += 0.1f;
				else if (event.key.code == sf::Keyboard::localize(sf::Keyboard::Scancode::Num9)) speed -= 0.1f;

				else if (event.key.code == sf::Keyboard::localize(sf::Keyboard::Scancode::Enter)) window.close();
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
			sf::Vector3f dir = sf::Vector3f(0.0f, 0.0f, 0.0f);
			sf::Vector3f dirTmp;

			if (wasdud[0]) 
				dir = sf::Vector3f(1.0f, 0.0f, 0.0f);
			else if (wasdud[2]) 
				dir = sf::Vector3f(-1.0f, 0.0f, 0.0f);
			if (wasdud[1]) 
				dir += sf::Vector3f(0.0f, -1.0f, 0.0f);
			else if (wasdud[3]) dir += sf::Vector3f(0.0f, 1.0f, 0.0f);

			mx += (float)mouseX / width * mouseSensivity;
			my += (float)mouseY / height * mouseSensivity;

			dirTmp.z = dir.z * cos(-my) - dir.x * sin(-my);
			dirTmp.x = dir.z * sin(-my) + dir.x * cos(-my);
			dirTmp.y = dir.y;
			dir.x = dirTmp.x * cos(mx) - dirTmp.y * sin(mx);
			dir.y = dirTmp.x * sin(mx) + dirTmp.y * cos(mx);
			dir.z = dirTmp.z;

			position += dir * speed;
			if (wasdud[4]) position.z -= speed;
			else if (wasdud[5]) position.z += speed;

			
			//Debug text
			std::ostringstream ss;
			ss << "x: " << position.x << "\ny: " << position.y << "\nz: " << position.z <<
				"\n\nMouse Sensivity: " << mouseSensivity <<
				"\n\nspeed: " << speed;
			text.setString(ss.str());
			
			//Sending uniforms
			shader.setUniform("u_time", clock.getElapsedTime().asSeconds());
			shader.setUniform("u_position", position);
			shader.setUniform("u_mouse", sf::Vector2f(mx, my));
		}
		window.clear();

        //Drawing
		window.draw(emptySprite, &shader);
		if (debug)
		{
			window.draw(text);
		}

        //Display current frame
		window.display();
	}
	return 0;
}