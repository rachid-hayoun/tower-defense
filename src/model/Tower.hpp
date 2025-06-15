#pragma once
#include <SFML/Graphics.hpp>
#include "TowerType.hpp"

class Enemy; // Forward declaration

enum class TowerLevel {
    LEVEL_1 = 1,
    LEVEL_2 = 2,
    LEVEL_3 = 3
};

class Tower {
public:
    Tower(float x, float y, sf::Texture* texture, TowerType type = TowerType::BASIC);
    
    void update(float deltaTime, const std::vector<Enemy*>& enemies);
    void draw(sf::RenderWindow& window);
    void drawRange(sf::RenderWindow& window);
    void setRotation(float angle);
    
    sf::Vector2f getPosition() const;
    float getRange() const;
    bool isInRange(sf::Vector2f targetPosition) const;
    TowerType getType() const;
    std::string getTypeString() const;

    std::vector<int> collectRewards();
    
    // Système d'amélioration
    bool canUpgrade() const;
    int getUpgradeCost() const;
    void upgrade();
    TowerLevel getLevel() const;
    std::string getLevelText() const;
    
private:
    sf::Vector2f position;
    sf::Sprite sprite;
    sf::CircleShape rangeIndicator;
    TowerType towerType;
    TowerStats stats;
    
    float range;
    float rotation;
    float rotationSpeed;
    float targetRotation;
    
    // Système de combat
    int damage;
    float fireRate;
    float timeSinceLastShot;
    
    // Système d'amélioration
    TowerLevel level;
    static const int UPGRADE_COST_LEVEL_2 = 50;
    static const int UPGRADE_COST_LEVEL_3 = 100;
    
    Enemy* findNearestEnemy(const std::vector<Enemy*>& enemies);
    void rotateTowards(float targetAngle, float deltaTime);
    float calculateAngleToTarget(sf::Vector2f targetPos);
    void shootAt(Enemy* target);
    bool canShoot() const;
    
    void initializeFromType();
    void updateStatsForLevel();
    void updateVisualForLevel();

    std::vector<int> earnedRewards;
};