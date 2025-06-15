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
    
    // Méthodes principales
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    
    // Combat
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
    // Type d'ennemi
    EnemyType enemyType;
    
    // Position et pathfinding
    sf::Vector2f position;
    sf::Vector2f velocity;
    float speed;
    
    // Pathfinding
    GameMap* gameMap;
    std::vector<sf::Vector2f> pathPoints;
    int currentPathIndex;
    bool reachedEnd;
    
    // Statistiques
    int maxHealth;
    int currentHealth;
    int reward;
    
    // Graphismes
    sf::Texture texture;
    sf::Sprite sprite;
    
    // Barre de vie
    sf::RectangleShape healthBarBackground;
    sf::RectangleShape healthBarForeground;
    void updateHealthBar();
    
    // Pathfinding et rotation
    void generatePath();
    void moveAlongPath(float deltaTime);
    void updateSpriteRotation(sf::Vector2f direction);
    sf::Vector2f getNextTarget();
    
    // Méthode pour obtenir le nom de fichier de texture selon le type
    std::string getTextureFileName() const;
};