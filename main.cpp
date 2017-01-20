#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <sstream>

int main()
{
    std::srand(static_cast<unsigned int>(std::time(NULL)));

    // Define some constants
    const float pi = 3.14159f;
    const int gameWidth = 800;
    const int gameHeight = 600;
    float ballRadius = 10.f;
    float paddleRadius = 25.f;
    int rightPaddleScore = 0;
    int leftPaddleScore = 0;

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
    leftPaddle.setOutlineThickness(3);
    leftPaddle.setOutlineColor(sf::Color::Black);
    leftPaddle.setFillColor(sf::Color::Red);
    leftPaddle.setOrigin(paddleRadius, paddleRadius);

    // Create the right paddle
    sf::CircleShape rightPaddle;
    rightPaddle.setRadius(paddleRadius);
    rightPaddle.setOutlineThickness(3);
    rightPaddle.setOutlineColor(sf::Color::Black);
    rightPaddle.setFillColor(sf::Color::Blue);
    rightPaddle.setOrigin(paddleRadius, paddleRadius);

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
    pauseMessage.setString("Welcome to SFML air hockey!\nPress space to start the game");

    //Initialize computer score
    sf::Text computerScore;
    computerScore.setFont(font);
    computerScore.setCharacterSize(40);
    computerScore.setRotation(270);
    computerScore.setOrigin(20, 20);
    computerScore.setPosition(gameWidth - 30, gameHeight / 2);
    computerScore.setColor(sf::Color::Blue);
    std::ostringstream scoreComputer;
    scoreComputer << rightPaddleScore;
    computerScore.setString(scoreComputer.str());

    //Initialize player score
    sf::Text playerScore;
    playerScore.setFont(font);
    playerScore.setCharacterSize(40);
    playerScore.setRotation(90);
    playerScore.setOrigin(20, 20);
    playerScore.setPosition(30, gameHeight / 2);
    playerScore.setColor(sf::Color::Red);
    std::ostringstream scorePlayer;
    scorePlayer << leftPaddleScore;
    playerScore.setString(scorePlayer.str());

    // Create a sprite for the background
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("resources/background.jpg"))
        return EXIT_FAILURE;
    sf::Sprite background(backgroundTexture);

    // Define the paddles properties
    sf::Clock AITimer;
    const sf::Time AITime   = sf::seconds(0.1f);
    const float paddleSpeed = 400.f;
    float rightPaddleSpeed  = 0.f;
    const float ballSpeed   = 200.f;
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
                    leftPaddle.setPosition(100, gameHeight / 2);
                    rightPaddle.setPosition(gameWidth - 100, gameHeight / 2);
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

        if (isPlaying)
        {
            float deltaTime = clock.restart().asSeconds();

            // Move the player's paddle
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) &&
               (leftPaddle.getPosition().y - paddleRadius / 2 > 20.f))
            {
                leftPaddle.move(0.f, -paddleSpeed * deltaTime);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) &&
               (leftPaddle.getPosition().y + paddleRadius / 2 < gameHeight - 20.f))
            {
                leftPaddle.move(0.f, paddleSpeed * deltaTime);
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) &&
               (leftPaddle.getPosition().x + paddleRadius / 2 < gameWidth - 100.f))
            {
                leftPaddle.move(paddleSpeed * deltaTime, 0.f);
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
               (leftPaddle.getPosition().x + paddleRadius / 2 > 100.f))
            {
                leftPaddle.move(-paddleSpeed * deltaTime, 0.f);
            }

            // Move the computer's paddle
            if (((rightPaddleSpeed < 0.f) && (rightPaddle.getPosition().y - paddleRadius / 2 > 20.f)) ||
                ((rightPaddleSpeed > 0.f) && (rightPaddle.getPosition().y + paddleRadius / 2 < gameHeight - 20.f)))
            {
                rightPaddle.move(0.f, rightPaddleSpeed * deltaTime);
            }

            if (((rightPaddleSpeed < 0.f) && (rightPaddle.getPosition().x - paddleRadius / 2 > 100.f)) ||
                ((rightPaddleSpeed > 0.f) && (rightPaddle.getPosition().x + paddleRadius / 2 < gameHeight - 100.f)))
            {
                rightPaddle.move(rightPaddleSpeed * deltaTime, 0.f);
            }

            // Update the computer's paddle direction according to the ball position
            if (AITimer.getElapsedTime() > AITime)
            {
                AITimer.restart();
                if (ball.getPosition().y + ballRadius > rightPaddle.getPosition().y + paddleRadius / 2)
                    rightPaddleSpeed = paddleSpeed;
                else if (ball.getPosition().y - ballRadius < rightPaddle.getPosition().y - paddleRadius / 2)
                    rightPaddleSpeed = -paddleSpeed;
                else
                    rightPaddleSpeed = 0.f;
            }

            // Move the ball
            float factor = ballSpeed * deltaTime;
            ball.move(std::cos(ballAngle) * factor, std::sin(ballAngle) * factor);

            // Check collisions between the ball and the screen
            if (((ball.getPosition().x - ballRadius < 0.f) &&
               (ball.getPosition().y - ballRadius > 200.f)) &&
               (ball.getPosition().y - ballRadius < 400.f))

            {
                isPlaying = false;
                pauseMessage.setString("You lost !\nPress space to restart or\nescape to exit");
                leftPaddleScore ++;
                std::ostringstream scorePlayer;
                scorePlayer << leftPaddleScore;
                playerScore.setString(scorePlayer.str());
            }

            if (((ball.getPosition().x + ballRadius > gameWidth) &&
                 (ball.getPosition().y + ballRadius > 200.f)) &&
                 (ball.getPosition().y + ballRadius < 400.f))

            {
                isPlaying = false;
                pauseMessage.setString("You won !\nPress space to restart or\nescape to exit");
                rightPaddleScore ++;
                std::ostringstream scoreComputer;
                scoreComputer << rightPaddleScore;
                computerScore.setString(scoreComputer.str());
            }

            if (ball.getPosition().y - ballRadius < 0.f)
            {
                ballSound.play();
                ballAngle = 2 * pi - ballAngle;
                ball.setPosition(ball.getPosition().x, ballRadius + 0.1f);
            }

            if (ball.getPosition().y + ballRadius > gameHeight)
            {
                ballSound.play();
                ballAngle = 2 * pi - ballAngle;
                ball.setPosition(ball.getPosition().x, gameHeight - ballRadius - 0.1f);
            }

            if (((ball.getPosition().x - ballRadius < 0.f) &&
                 (ball.getPosition().y - ballRadius < 200.f)) ||
                 ((ball.getPosition().x - ballRadius < 0.f) &&
                 (ball.getPosition().y - ballRadius > 400.f)))
            {
                ballSound.play();
                if ((ballAngle > 0) && (ballAngle < pi))
                    ballAngle = pi - ballAngle;
                else
                    ballAngle = 3 * pi - ballAngle;
                ball.setPosition(ballRadius + 0.1f, ball.getPosition().y);
            }

            if (((ball.getPosition().x + ballRadius > gameWidth) &&
                 (ball.getPosition().y + ballRadius < 200.f)) ||
                 ((ball.getPosition().x + ballRadius > gameWidth) &&
                 (ball.getPosition().y + ballRadius > 400.f)))
            {
                ballSound.play();
                if ((ballAngle > 0) && (ballAngle < pi))
                    ballAngle = pi - ballAngle;
                else
                    ballAngle = 3 * pi - ballAngle;
                ball.setPosition(gameWidth - ballRadius - 0.1f, ball.getPosition().y);
            }

            // Check the collisions between the ball and the paddles
            // Left Paddle
            if (ball.getPosition().x - ballRadius < leftPaddle.getPosition().x + paddleRadius / 2 &&
                ball.getPosition().x - ballRadius > leftPaddle.getPosition().x &&
                ball.getPosition().y + ballRadius >= leftPaddle.getPosition().y - paddleRadius / 2 &&
                ball.getPosition().y - ballRadius <= leftPaddle.getPosition().y + paddleRadius / 2)
            {
                if (ball.getPosition().y > leftPaddle.getPosition().y)
                    ballAngle = pi - ballAngle + (std::rand() % 20) * pi / 180;
                else
                    ballAngle = pi - ballAngle - (std::rand() % 20) * pi / 180;

                ballSound.play();
                ball.setPosition(leftPaddle.getPosition().x + ballRadius + paddleRadius/ 2 + 0.1f, ball.getPosition().y);
            }

           // Right Paddle
            if (ball.getPosition().x + ballRadius > rightPaddle.getPosition().x - paddleRadius / 2 &&
                ball.getPosition().x + ballRadius < rightPaddle.getPosition().x &&
                ball.getPosition().y + ballRadius >= rightPaddle.getPosition().y - paddleRadius / 2 &&
                ball.getPosition().y - ballRadius <= rightPaddle.getPosition().y + paddleRadius / 2)
            {
                if (ball.getPosition().y > rightPaddle.getPosition().y)
                    ballAngle = pi - ballAngle + (std::rand() % 20) * pi / 180;
                else
                    ballAngle = pi - ballAngle - (std::rand() % 20) * pi / 180;

                ballSound.play();
                ball.setPosition(rightPaddle.getPosition().x - ballRadius - paddleRadius / 2 - 0.1f, ball.getPosition().y);
            }
        }

        // Clear the window
        //window.clear(sf::Color(50, 200, 50));
        window.clear();

        if (isPlaying)
        {
            // Draw the paddles and the ball
            window.draw(background);
            window.draw(leftPaddle);
            window.draw(rightPaddle);
            window.draw(ball);
            window.draw(computerScore);
            window.draw(playerScore);
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
