#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <ctime>
#include <cstdlib>

int main()
{
    std::srand(static_cast<unsigned int>(std::time(NULL)));

    // Define some constants
    const float pi = 3.14159f;
    const int gameWidth = 800;
    const int gameHeight = 600;
    float ballRadius = 10.f;
    float paddleRadius = 25.f;

    // Create the window of the application
    sf::RenderWindow window(sf::VideoMode(gameWidth, gameHeight, 32), "SFML Pong");
    window.setVerticalSyncEnabled(true);

    // Load the sounds used in the game
    sf::SoundBuffer ballSoundBuffer;
    if (!ballSoundBuffer.loadFromFile("resources/ball.wav"))
        return EXIT_FAILURE;
    sf::Sound ballSound(ballSoundBuffer);

    // Create the left paddle
    sf::CircleShape leftPaddle;
    leftPaddle.setRadius(paddleRadius);
    leftPaddle.setOutlineThickness(3); //контур
    leftPaddle.setOutlineColor(sf::Color::Black);
    leftPaddle.setFillColor(sf::Color::Red);
    leftPaddle.setOrigin(paddleRadius, paddleRadius);//position

    // Create the right paddle
    sf::CircleShape rightPaddle;
    rightPaddle.setRadius(paddleRadius);
    rightPaddle.setOutlineThickness(3); //контур
    rightPaddle.setOutlineColor(sf::Color::Black);
    rightPaddle.setFillColor(sf::Color::Blue);
    rightPaddle.setOrigin(paddleRadius, paddleRadius);//position


    // Create the ball
    sf::CircleShape ball;
    ball.setRadius(ballRadius - 3);
    ball.setOutlineThickness(3);
    ball.setOutlineColor(sf::Color::Black);
    ball.setFillColor(sf::Color::White);
    ball.setOrigin(ballRadius / 2, ballRadius / 2);

    // Load the text font
    sf::Font font;
    if (!font.loadFromFile("resources/sansation.ttf"))
        return EXIT_FAILURE;

    // Initialize the pause message
    sf::Text pauseMessage;
    pauseMessage.setFont(font);
    pauseMessage.setCharacterSize(40);
    pauseMessage.setPosition(170.f, 150.f);
    pauseMessage.setColor(sf::Color::White);
    pauseMessage.setString("Welcome to SFML pong!\nPress space to start the game");

    // Define the paddles properties
    sf::Clock AITimer;
    const sf::Time AITime   = sf::seconds(0.1f);
    const float paddleSpeed = 400.f;
    float rightPaddleSpeed  = 0.f;
    const float ballSpeed   = 400.f;
    float ballAngle         = 0.f; // to be changed later

    sf::Clock clock;
    bool isPlaying = false;
    while (window.isOpen())
    {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Window closed or escape key pressed: exit
            if ((event.type == sf::Event::Closed) ||
               ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)))
            {
                window.close();
                break;
            }

            // Space key pressed: play
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Space))
            {
                if (!isPlaying)
                {
                    // (re)start the game
                    isPlaying = true;
                    clock.restart();

                    // Reset the position of the paddles and ball
                    leftPaddle.setPosition(50, gameHeight / 2);
                    rightPaddle.setPosition(gameWidth - 50, gameHeight / 2);
                    ball.setPosition(gameWidth / 2, gameHeight / 2);

                    // Reset the ball angle
                    do
                    {
                        // Make sure the ball initial angle is not too much vertical
                        ballAngle = (std::rand() % 360) * 2 * pi / 360;
                    }
                    while (std::abs(std::cos(ballAngle)) < 0.7f);
                }
            }
        }

        // Clear the window
        window.clear(sf::Color(50, 200, 50));

        if (isPlaying)
        {
            // Draw the paddles and the ball
            window.draw(leftPaddle);
            window.draw(rightPaddle);
            window.draw(ball);
        }
        else
        {
            // Draw the pause message
            window.draw(pauseMessage);
        }

        // Display things on screen
        window.display();
    }

    return EXIT_SUCCESS;
}
