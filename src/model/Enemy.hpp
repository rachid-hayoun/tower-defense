#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class GameMap;

enum class EnemyType {
    BASIC,
    FAST,
    TANK
};

class Enemy {
public:
    Enemy(float startX, float startY, GameMap* map, EnemyType type = EnemyType::BASIC);
    
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    
    void takeDamage(int damage);
    bool isDead() const;
    bool hasReachedEnd() const;

    sf::Vector2f getPosition() const;
    int getHealth() const;
    float getSpeed() const;
    sf::FloatRect getHitBox() const;

    bool loadTexture();
    EnemyType getType() const;

    void setSpeed(float newSpeed);
    void setHealth(int health);
    void setReward(int newReward);
    int getReward() const;

private:
    EnemyType enemyType;
    
    sf::Vector2f position;
    sf::Vector2f velocity;
    float speed;
    
    GameMap* gameMap;
    std::vector<sf::Vector2f> pathPoints;
    int currentPathIndex;
    bool reachedEnd;
    
    int maxHealth;
    int currentHealth;
    int reward;
    
    sf::Texture texture;
    sf::Sprite sprite;
    
    sf::RectangleShape healthBarBackground;
    sf::RectangleShape healthBarForeground;
    void updateHealthBar();
    
    void generatePath();
    void moveAlongPath(float deltaTime);
    void updateSpriteRotation(sf::Vector2f direction);
    sf::Vector2f getNextTarget();
    
    std::string getTextureFileName() const;
};
