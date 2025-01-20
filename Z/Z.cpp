#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>

const int WIDTH = 1920;
const int HEIGHT = 1080;
const int CELL_SIZE = 20;

struct SnakeSegment 
{
    int x, y;
};

//Global variables
std::vector<SnakeSegment> snake;
sf::Vector2i foodPos;
int direction = 1; //0 - up, 1 - right, 2 - down, 3 - left
int score = 0;
int speed = 150; //Game update in ms
void HandleInput(sf::RenderWindow& window)
{
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::KeyPressed)
        {
            switch (event.key.code)
            {
            case sf::Keyboard::Up:
                if (direction != 2) direction = 0;
                break;
            case sf::Keyboard::Right:
                if (direction != 3) direction = 1;
                break;
            case sf::Keyboard::Down:
                if (direction != 0) direction = 2;
                break;
            case sf::Keyboard::Left:
                if (direction != 1) direction = 3;
                break;
            }
        }
    }
}

bool CheckCollision(SnakeSegment head)
{
    //Wall collisions
    if (head.x < 0 || head.x >= WIDTH / CELL_SIZE || head.y < 0 || head.y >= HEIGHT / CELL_SIZE)
        return true;
    //Body collision
    for (size_t i = 1; i < snake.size(); ++i)
    {
        if (head.x == snake[i].x && head.y == snake[i].y)
            return true;
    }

    return false;
}

bool CheckFoodCollision()
{
    //Check collision with snake
    for (const auto& segment : snake)
    {
        if (foodPos.x == segment.x && foodPos.y == segment.y)
            return true;
    }
    return false;
}

void SpawnFood()
{
    const int maxX = (WIDTH / CELL_SIZE) - 1;
    const int maxY = (HEIGHT / CELL_SIZE) - 1;

    do
    {
        foodPos.x = rand() % maxX;
        foodPos.y = rand() % maxY;
    } while (CheckFoodCollision());

    std::cout << "New food at: " << foodPos.x << " " << foodPos.y << "\n";
}

void InitializeGame()
{
    snake.clear();
    snake.push_back({ 5, 5 }); //Starting pos
    direction = 1;
    score = 0;
    SpawnFood();
}

void GameOver()
{
    // Show go screen
    // Ask name
    // Call SaveScore()
    InitializeGame(); // Restart
}

void UpdateGame()
{
    //Snake move
    SnakeSegment newHead = snake.front();
    switch (direction)
    {
    case 0: newHead.y--; break;
    case 1: newHead.x++; break;
    case 2: newHead.y++; break;
    case 3: newHead.x--; break;
    }

    //Collision checks
    if (CheckCollision(newHead))
    {
        GameOver();
        return;
    }

    snake.insert(snake.begin(), newHead);

    //Eating check
    if (newHead.x == foodPos.x && newHead.y == foodPos.y)
    {
        score++;
        SpawnFood();
    }
    else
    {
        snake.pop_back();
    }
}

void Render(sf::RenderWindow& window)
{
    window.clear(sf::Color::Black);

    //Snake drawing
    sf::RectangleShape segment(sf::Vector2f(CELL_SIZE - 2, CELL_SIZE - 2));
    for (auto& s : snake)
    {
        segment.setPosition(s.x * CELL_SIZE, s.y * CELL_SIZE);
        segment.setFillColor(sf::Color::Green);
        window.draw(segment);
    }

    //Food drawing
    sf::CircleShape food(CELL_SIZE / 2 - 2);
    food.setPosition(foodPos.x * CELL_SIZE, foodPos.y * CELL_SIZE);
    food.setFillColor(sf::Color::Red);
    window.draw(food);

    //Score view
    sf::Font font;
    if (!font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
        // Можна використати fallback-шрифт або вийти
    }
    sf::Text text("Score: " + std::to_string(score), font, 24);
    window.draw(text);

    window.display();
}

enum Difficulty { EASY, MEDIUM, HARD };

void SetDifficulty(Difficulty diff)
{
    switch (diff)
    {
    case EASY:
        speed = 200;
        break;
    case MEDIUM:
        speed = 150;
        break;
    case HARD:
        speed = 100;
        break;
    } 
}

Difficulty currentDifficulty = Difficulty::MEDIUM; // Start level

void SaveScore(const std::string& name)
{
    std::ofstream file("scores.txt", std::ios::app);
    if (file)
    {
        file << name << " " << score << "\n";
    }
}

void LoadScores()
{
    std::ifstream file("scores.txt");
    std::string name;
    int score;
    while (file >> name >> score)
    {
        //Initialize res
    }
}

//void ShowDifficultyMenu(sf::RenderWindow & window)
//{
//    sf::Font font;
//    font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");
//
//    std::vector<sf::Text> menuItems;
//    std::vector<std::string> texts = { "Easy", "Medium", "Hard" };
//
//    for (size_t i = 0; i < texts.size(); ++i)
//    {
//        sf::Text text(texts[i], font, 40);
//        text.setPosition(300, 200 + i * 80);
//        menuItems.push_back(text);
//    }
//
//    int selectedItem = 1;
//
//    while (window.isOpen())
//    {
//        //Event listener
//        sf::Event event;
//        while (window.pollEvent(event))
//        {
//            if (event.type == sf::Event::Closed)
//                window.close();
//
//            if (event.type == sf::Event::KeyPressed)
//            {
//                switch (event.key.code)
//                {
//                case sf::Keyboard::Up:
//                    if (selectedItem > 0) selectedItem--;
//                    break;
//                case sf::Keyboard::Down:
//                    if (selectedItem < 2) selectedItem++;
//                    break;
//                case sf::Keyboard::Enter:
//                    currentDifficulty = static_cast<Difficulty>(selectedItem);
//                    return;
//                }
//            }
//        }
//
//        for (size_t i = 0; i < menuItems.size(); ++i)
//        {
//            menuItems[i].setFillColor(i == selectedItem ? sf::Color::Red : sf::Color::White);
//        }
//
//        //View
//        window.clear();
//        for (const auto& item : menuItems)
//        {
//            window.draw(item);
//        }
//
//        window.display();
//    }
//}
//
//void ApplyDifficultySettings()
//{
//    switch (currentDifficulty)
//    {
//    case Difficulty::EASY:
//        speed = 200; //Slower movement
//        snakeStartLength = 3;
//        break;
//    case Difficulty::MEDIUM:
//        speed = 150; 
//        snakeStartLength = 4;
//        break;
//    case Difficulty::HARD:
//        speed = 100; //Fast movement
//        snakeStartLength = 5;
//        wallCollision = true; //Add killwalls
//        break;
//    }
//}

int main()
{
    srand(static_cast<unsigned int>(time(nullptr)));
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Snake Game");
    sf::Clock clock;

    InitializeGame();

    ShowDifficultyMenu(window);
    ApplyDifficultySettings();

    while (window.isOpen())
    {
        HandleInput(window); 

        if (clock.getElapsedTime().asMilliseconds() > speed) 
        {
            UpdateGame();
            clock.restart();
        }

        Render(window);
    }

    return 0;
}