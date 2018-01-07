#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
using namespace sf;

Sprite paddle;
Sprite ball;
Sprite brick;

Text info;
Text enter;
float RandomFloat(float a, float b);

float speed = 0.30;
float xVel = -speed*2;
float yVel = -speed;
float ballX = RandomFloat(0, 780);
float ballY = 526;

bool positiveX = false;
bool positiveY = true;

bool isRunning = true;

int scoreInt;

void enterKey();
void reset();
void gameOver();

float RandomFloat(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

template <typename T>
std::string to_string(T value)
{
	std::ostringstream os ;
	os << value ;
	return os.str() ;
}

int main()
{
    //std::ofstream file("lame.txt");

    RenderWindow window(VideoMode(800, 560), "Break Bricker");
    window.setFramerateLimit(144);

    //bg
    Texture bgTexture;
    bgTexture.loadFromFile("bg.png");

    Sprite bg;
    bg.setTexture(bgTexture);


    //the paddle
    Texture paddleTexture;
    paddleTexture.loadFromFile("paddle.png");

    paddle.setTexture(paddleTexture);
    paddle.setPosition(350, 547);
    paddle.setOrigin(Vector2f(50, 0));

    //the ball
    Texture ballTexture;
    ballTexture.loadFromFile("ball.png");

    ball.setTexture(ballTexture);
    ball.setPosition(ballX, ballY);

    //the brick (you)
    Texture brickTexture;
    brickTexture.loadFromFile("brick.png");

    brick.setTexture(brickTexture);
    brick.setPosition(200, 200);

    //Score text
    Font font;
    font.loadFromFile("pirates.otf");

    Text score;
    score.setFont(font);
    score.setString("Speed: 25");
    score.setColor(Color::White);
    score.setCharacterSize(40);
    score.setPosition(10, 0);

    //Game over text
    info.setFont(font);
    info.setColor(Color::White);
    info.setCharacterSize(80);
    info.setString("Game Over!");

    sf::FloatRect textRect = info.getLocalBounds();
    info.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);
    info.setPosition(sf::Vector2f(800/2.0f, 150));
    info.setString("");

    //enter to play again
    enter.setFont(font);
    enter.setString("Press ENTER to Play Again!");
    enter.setColor(Color::White);
    enter.setCharacterSize(40);

    sf::FloatRect enterRect = enter.getLocalBounds();
    enter.setOrigin(enterRect.left + enterRect.width/2.0f, enterRect.top  + enterRect.height/2.0f);
    enter.setPosition(sf::Vector2f(800/2.0f, 216));
    enter.setString("");


    //frame ratecker
    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Event event;

        Time time = clock.getElapsedTime();


        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed) window.close();

            if(event.type == sf::Event::KeyPressed) {
                switch(event.key.code){
                    case sf::Keyboard::Return:
                        enterKey();
                        break;
                }
            }

        }
        if(isRunning){
            //move brick
            if(Keyboard::isKeyPressed(Keyboard::Down) && brick.getPosition().y <= 400) brick.move(0, speed*2*time.asMilliseconds());
            if(Keyboard::isKeyPressed(Keyboard::Up) && brick.getPosition().y >= 0) brick.move(0, -speed*2*time.asMilliseconds());
            if(Keyboard::isKeyPressed(Keyboard::Left) && brick.getPosition().x >= 0) brick.move(-speed*2*time.asMilliseconds(), 0);
            if(Keyboard::isKeyPressed(Keyboard::Right) && brick.getPosition().x <= 720) brick.move(speed*2*time.asMilliseconds(), 0);

            //wall correction
            if(brick.getPosition().y < 0) {
                brick.setPosition(brick.getPosition().x, 0);
            }

            if(brick.getPosition().y > 400) {
                brick.setPosition(brick.getPosition().x, 400);
            }

            if(brick.getPosition().x < 0) {
                brick.setPosition(0, brick.getPosition().y);
            }

            if(brick.getPosition().x > 720) {
                brick.setPosition(720, brick.getPosition().y);
            }

            //ball engine
            if(positiveX) {
                xVel = (speed*2);
            } else {
                xVel = -(speed*2);
            }
            if(positiveY) {
                yVel = -speed;
            } else {
                yVel = speed;
            }

            ballX += xVel*time.asMilliseconds();
            ballY += yVel*time.asMilliseconds();

            ball.setPosition(ballX, ballY);
            speed += 0.00002*time.asMilliseconds();

            //show speed (score)
            scoreInt = (int) ((speed*1000) - 300);
            score.setString("Score: " + to_string(scoreInt));


            if(ballX <= 0) {
                ballX = 0;
                positiveX = true;
            } else if((ballX+20) >= 800) {
                ballX = 780;
                positiveX = false;
            } else if(ballY <= 0) {
                ballY = 0;
                positiveY = false;
            } else if((ballY+20) >= 547) {
                ballY = 527;
                positiveY = true;
            }

            paddle.setPosition(ballX, 547);

            //collision
            if(ball.getGlobalBounds().intersects(brick.getGlobalBounds())) gameOver();
        }

        clock.restart();

        window.clear();

        window.draw(bg);
        window.draw(paddle);
        window.draw(ball);
        window.draw(brick);
        window.draw(score);
        window.draw(info);
        window.draw(enter);

        window.display();
    }
}

void enterKey() {
    if(!isRunning) {
        reset();
    }
}

void reset() {
    isRunning = true;
    brick.setPosition(200, 200);
    speed = 0.30;
    xVel = -speed*2;
    yVel = -speed;
    float ballX = RandomFloat(0, 780);
    ballY = 526;

    positiveX = false;
    positiveY = true;
    info.setString("");
    enter.setString("");
}

void gameOver(){
    isRunning = false;
    info.setString("Game Over!");
    enter.setString("Press ENTER to Play Again!");
}
