#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cstdlib>

using namespace std;

// Initializing Dimensions.
// resolutionX and resolutionY determine the rendering resolution.
// Don't edit unless required. Use functions on lines 43, 44, 45 for resizing the game window.
const int resolutionX = 960;
const int resolutionY = 960;
const int boxPixelsX = 32;
const int boxPixelsY = 32;
const int gameRows = resolutionX / boxPixelsX;    // Total rows on grid
const int gameColumns = resolutionY / boxPixelsY; // Total columns on grid

// Initializing GameGrid.
int gameGrid[gameRows][gameColumns] = {};

// The following exist purely for readability.
const int x = 0;
const int y = 1;
const int exists = 2;

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Write your functions declarations here. Some have been written for you. //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void drawPlayer(sf::RenderWindow &window, float player[], sf::Sprite &playerSprite);
void moveBullet(float bullet[], sf::Clock &bulletClock);
void drawBullet(sf::RenderWindow &window, float bullet[], sf::Sprite &bulletSprite);
void fighterMove(sf::Event &event, float player[], int resolutionX, int resolutionY, int boxPixelsX, int boxPixelsY, int gameColumns);
void drawCentipede(sf::RenderWindow &window, sf::Sprite &centipedeSprite, sf::Texture &centipedeTexture, sf::Texture &centipedeHeadTexture, int totalcentipieces, float centipedeX[], float centipedeY[], int boxPixelsX, int boxPixelsY);
void bulletHitsShroom(float bullet[], int totalshrooms, float mushroomX[], float mushroomY[], int bulletHitshroom[], sf::Sprite mushroomSprites[], int boxPixelsX, int boxPixelsY);
void drawShrooms(sf::RenderWindow &window, int totalshrooms, float mushroomX[], float mushroomY[], sf::Sprite mushroomSprites[]);

int main()
{
    bool spacePressed = false;
    srand(time(0));

    // Declaring RenderWindow.
    sf::RenderWindow window(sf::VideoMode(resolutionX, resolutionY), "Centipede", sf::Style::Close | sf::Style::Titlebar);

    // Used to resize your window if it's too big or too small. Use according to your needs.
    window.setSize(sf::Vector2u(640, 640)); // Recommended for 1366x768 (768p) displays.

    // Used to position your window on every launch. Use according to your needs.
    window.setPosition(sf::Vector2i(100, 0));

    // Initializing Background.
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    backgroundTexture.loadFromFile("Textures/background.png");
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setColor(sf::Color(255, 255, 255, 255 * 0.20)); // Reduces Opacity to 25%

    // Initializing Player and Player Sprites.
    float player[2] = {};
    player[x] = (gameColumns / 2) * boxPixelsX;
    player[y] = (gameColumns - 5) * boxPixelsY;
    sf::Texture playerTexture;
    sf::Sprite playerSprite;
    playerTexture.loadFromFile("Textures/player.png");
    playerSprite.setTexture(playerTexture);
    playerSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));

    // introducing centipede
    const int totalcentipieces = 12;    // 12 segments according to project
    float centipedeX[totalcentipieces]; // we make an array
    float centipedeY[totalcentipieces]; // for x cordinates and y cordinates
    sf::Texture centipedeTexture;       // loading textures blah blah
    sf::Sprite centipedeSprite;
    centipedeTexture.loadFromFile("Textures/c_body_left_walk.png");
    sf::Texture centipedeHeadTexture;
    centipedeHeadTexture.loadFromFile("Textures/c_head_left_walk.png");
    centipedeSprite.setTexture(centipedeTexture);
    centipedeSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));

    // Initializing Bullet and Bullet Sprites
    float bullet[3] = {};
    bullet[x] = player[x];
    bullet[y] = player[y] - boxPixelsY;
    bullet[exists] = true;
    sf::Clock bulletClock;
    sf::Texture bulletTexture;
    sf::Sprite bulletSprite;
    bulletTexture.loadFromFile("Textures/bullet.png");
    bulletSprite.setTexture(bulletTexture);
    bulletSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));

    // Initializing Mushrooms
    const int totalshrooms = 25; // number of mushrooms
    float mushroomX[totalshrooms];
    float mushroomY[totalshrooms];
    int bulletHitshroom[totalshrooms] = {0};
    sf::Texture mushroomTexture;
    sf::Sprite mushroomSprites[totalshrooms];

    mushroomTexture.loadFromFile("Textures/mushroom.png");

    // spawning mushrooms randomly
    for (int i = 0; i < totalshrooms; i++)
    {
        mushroomX[i] = rand() % resolutionX;
        mushroomY[i] = rand() % (resolutionY - (boxPixelsY * 5) + (boxPixelsY * 5)); // dont come in player region >:(
        mushroomSprites[i].setTexture(mushroomTexture);
        mushroomSprites[i].setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));
    }

    // this is to make it start from random location of the first row
    int positionX = rand() % resolutionX;

    for (int i = 0; i < totalcentipieces; i++)
    {
        centipedeX[i] = positionX - boxPixelsX * (i); // all segments
        centipedeY[i] = 0;
    }

    sf::Event e;
    while (window.isOpen())
    {

        if (spacePressed && bullet[exists] == false)
        {
            // Fire if space and no bullet alr on screen
            bullet[exists] = true;
            bullet[x] = player[x];
            bullet[y] = player[y] - boxPixelsY;
        }

        window.draw(backgroundSprite);
        drawPlayer(window, player, playerSprite);
        if (bullet[exists] == true)
        {
            moveBullet(bullet, bulletClock);
            drawBullet(window, bullet, bulletSprite);
        }

        // handling spacebar
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
            {
                return 0;
            }
            else if (e.type == sf::Event::KeyPressed)
            {
                if (e.key.code == sf::Keyboard::Space)
                {
                    spacePressed = true;
                }
            }
            else if (e.type == sf::Event::KeyReleased)
            {
                if (e.key.code == sf::Keyboard::Space)
                {
                    spacePressed = false;
                }
            }
        }

        drawCentipede(window, centipedeSprite, centipedeTexture, centipedeHeadTexture, totalcentipieces, centipedeX, centipedeY, boxPixelsX, boxPixelsY);

        // bool rightside = true;
        bool rightside = true;
        float oneBoxDown = 0.25;

        for (int i = 0; i < totalcentipieces; ++i)
        {
            if (rightside == true)
            {
                centipedeX[i] -= 0.5;
            }
            else
            {
                centipedeX[i] += 0.5;
            }

            if (centipedeX[i] <= 0)
            {
                centipedeY[i] += oneBoxDown;
                rightside = false;
            }

            else if (centipedeX[i] + boxPixelsX >= resolutionX)
            {
                centipedeY[i] += oneBoxDown;
                rightside = true;
            }

            for (int j = 0; j < totalshrooms; ++j)
            {
                // Check if the centipede segment collides with the mushroom
                if (centipedeX[i] < mushroomX[j] + boxPixelsX &&
                    centipedeX[i] + boxPixelsX > mushroomX[j] &&
                    centipedeY[i] < mushroomY[j] + boxPixelsY &&
                    centipedeY[i] + boxPixelsY > mushroomY[j])
                {
                    // Increment Y to move the centipede segment down
                    centipedeY[i] += oneBoxDown;

                    // Adjust the direction based on the segment's current direction
                    if (rightside)
                    {
                        centipedeX[i] += 0.5; // Move right
                    }
                    else
                    {
                        centipedeX[i] -= 0.5; // Move left
                    }

                    // Toggle the direction
                    rightside = !rightside;
                }
            }
        }

        /*
            int centipedeDirection[totalcentipieces] =  {1};
            float oneBoxDown = 0.25;
            for(int i = 0; i < totalcentipieces; i++)
            {
                  /*
                  if(centipedeDirection[i] == 1)
                    {
                    centipedeX[i] -=0.5;
                    }
                    else if(centipedeDirection[i] == -1)
                    {
                    centipedeX[i] +=0.5;
                    }


            centipedeX[i] += 0.5 * centipedeDirection[i];


                    if(centipedeX[i] < 0)
                    {
                    centipedeY[i] += oneBoxDown;
                    centipedeDirection[i] *= -1;
                    }
                    else if(centipedeX[i] + boxPixelsX > resolutionX )
                    {
                    centipedeY[i] += oneBoxDown;
                    centipedeDirection[i] *= -1;
                    }

            }
            */

        // bullet collision with centipiece
        for (int i = 0; i < totalcentipieces; ++i)
        {
            if (centipedeX[i] < bullet[x] + boxPixelsX &&
                centipedeX[i] + boxPixelsX > bullet[x] &&
                centipedeY[i] < bullet[y] + boxPixelsY &&
                centipedeY[i] + boxPixelsY > bullet[y])
            {

                centipedeX[i] = -1000; // pull another lil sneaky and yeet the segment off screen
                centipedeY[i] = -1000;

                bullet[exists] = false; // Reset bullet

                break;
            }
        }

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        // calling function for movement of player
        fighterMove(event, player, resolutionX, resolutionY, boxPixelsX, boxPixelsY, gameColumns);
        // when mushroom and bullet collide twice, mushroom go disappear
        bulletHitsShroom(bullet, totalshrooms, mushroomX, mushroomY, bulletHitshroom, mushroomSprites, boxPixelsX, boxPixelsY);
        // for ... drawing the mushrooms.
        drawShrooms(window, totalshrooms, mushroomX, mushroomY, mushroomSprites);

        window.display();
        window.clear();
    }
}

void drawPlayer(sf::RenderWindow &window, float player[], sf::Sprite &playerSprite)
{
    playerSprite.setPosition(player[x], player[y]);
    window.draw(playerSprite);
}

void moveBullet(float bullet[], sf::Clock &bulletClock)
{
    if (bulletClock.getElapsedTime().asMilliseconds() < 20)
        return;

    bulletClock.restart();
    bullet[y] -= 10;
    if (bullet[y] < -32)
        bullet[exists] = false;
}

void drawBullet(sf::RenderWindow &window, float bullet[], sf::Sprite &bulletSprite)
{
    bulletSprite.setPosition(bullet[x], bullet[y]);
    window.draw(bulletSprite);
}

// function to handleplayermovement easy
void fighterMove(sf::Event &event, float player[], int resolutionX, int resolutionY, int boxPixelsX, int boxPixelsY, int gameColumns)
{
    // easy to read
    const int x = 0;
    const int y = 1;

    // move left
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && player[x] > 0)
    {
        player[x] -= 1;
    }
    // move right
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && player[x] < resolutionX - boxPixelsX)
    {
        player[x] += 1;
    }
    // move up
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && player[y] > (gameColumns - 5) * boxPixelsY)
    {
        player[y] -= 1;
    }
    // move down
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && player[y] < resolutionY - boxPixelsY)
    {
        player[y] += 1;
    }
}
// function to draw centipede
void drawCentipede(sf::RenderWindow &window, sf::Sprite &centipedeSprite, sf::Texture &centipedeTexture, sf::Texture &centipedeHeadTexture, int totalcentipieces, float centipedeX[], float centipedeY[], int boxPixelsX, int boxPixelsY)
{
    for (int i = 0; i < totalcentipieces; i++)
    {
        if (i == 11) // for head
        {
            centipedeSprite.setTexture(centipedeHeadTexture);
        }
        else // body
        {
            centipedeSprite.setTexture(centipedeTexture);
        }

        centipedeSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));
        centipedeSprite.setPosition(centipedeX[i], centipedeY[i]);
        window.draw(centipedeSprite);
    }
}

void bulletHitsShroom(float bullet[], int totalshrooms, float mushroomX[], float mushroomY[], int bulletHitshroom[], sf::Sprite mushroomSprites[], int boxPixelsX, int boxPixelsY)
{
    if (!bullet[exists])
    {
        return; // No collision check if the bullet doesn't exist
    }

    for (int i = 0; i < totalshrooms; ++i)
    {
        if (bullet[x] < mushroomX[i] + boxPixelsX &&
            bullet[x] + boxPixelsX > mushroomX[i] &&
            bullet[y] < mushroomY[i] + boxPixelsY &&
            bullet[y] + boxPixelsY > mushroomY[i])
        {

            bulletHitshroom[i]++;

            if (bulletHitshroom[i] == 1)
            {
                mushroomSprites[i].setTextureRect(sf::IntRect(32, 0, boxPixelsX, boxPixelsY));
            }
            else if (bulletHitshroom[i] == 2)
            {
                mushroomX[i] = -987; // pull a lil sneaky just yeet mushroom out of screen
                mushroomY[i] = -875;
            }
            bullet[exists] = false;
        }
    }
}

void drawShrooms(sf::RenderWindow &window, int totalshrooms, float mushroomX[], float mushroomY[], sf::Sprite mushroomSprites[])
{
    for (int i = 0; i < totalshrooms; ++i)
    {
        mushroomSprites[i].setPosition(mushroomX[i], mushroomY[i]);
        window.draw(mushroomSprites[i]);
    }
}
