#include "Enemy.hpp"
#include "GameMap.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>

Enemy::Enemy(float startX, float startY, GameMap* map, EnemyType type) : gameMap(map), enemyType(type) {
    // Position et mouvement
    position = sf::Vector2f(startX, startY);
    velocity = sf::Vector2f(0.0f, 0.0f);
    speed = 150.0f;
    
    // Pathfinding
    currentPathIndex = 0;
    reachedEnd = false;
    
    // Statistiques
    maxHealth = 100;
    currentHealth = 100;
    reward = 10;
    
    // Sprite
    sprite.setPosition(position);
    sprite.setOrigin(16.0f, 16.0f);
    
    // barre de vie
    float barWidth = 32.0f;
    float barHeight = 4.0f;
    
    healthBarBackground.setSize(sf::Vector2f(barWidth, barHeight));
    healthBarBackground.setFillColor(sf::Color::Red);
    healthBarBackground.setPosition(startX - barWidth/2, startY - 20.0f);
    
    healthBarForeground.setSize(sf::Vector2f(barWidth, barHeight));
    healthBarForeground.setFillColor(sf::Color::Green);
    healthBarForeground.setPosition(startX - barWidth/2, startY - 20.0f);
    
    generatePath();
}

std::string Enemy::getTextureFileName() const {
    switch (enemyType) {
        case EnemyType::BASIC:
            return "Assets/Enemy/Enemy1.png";
        case EnemyType::FAST:
            return "Assets/Enemy/Enemy2.png";
        case EnemyType::TANK:
            return "Assets/Enemy/Enemy3.png";
        default:
            return "Assets/Enemy/Enemy1.png";
    }
}

void Enemy::generatePath() {
    pathPoints.clear();
    
    // Trouve le point de départ
    for (int y = 0; y < gameMap->getMapHeight(); y++) {
        for (int x = 0; x < gameMap->getMapWidth(); x++) {
            if (gameMap->isPath(x, y)) {
                if (x == 0 || !gameMap->isPath(x - 1, y)) {
                    pathPoints.push_back(sf::Vector2f(
                        x * gameMap->getTileSize() + gameMap->getTileSize() / 2.0f,
                        y * gameMap->getTileSize() + gameMap->getTileSize() / 2.0f
                    ));
                    break;
                }
            }
        }
        if (!pathPoints.empty()) break;
    }
    
    while (true) {
        sf::Vector2f lastPoint = pathPoints.back();
        int currentX = (lastPoint.x - gameMap->getTileSize() / 2.0f) / gameMap->getTileSize();
        int currentY = (lastPoint.y - gameMap->getTileSize() / 2.0f) / gameMap->getTileSize();
        
        bool foundNext = false;
        
        int directions[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
        
        for (int i = 0; i < 4; i++) {
            int nextX = currentX + directions[i][0];
            int nextY = currentY + directions[i][1];
            
            if (gameMap->isPath(nextX, nextY)) {
                sf::Vector2f nextPoint(
                    nextX * gameMap->getTileSize() + gameMap->getTileSize() / 2.0f,
                    nextY * gameMap->getTileSize() + gameMap->getTileSize() / 2.0f
                );
                
                
                bool alreadyVisited = false;
                for (const auto& point : pathPoints) {
                    if (abs(point.x - nextPoint.x) < 1.0f && abs(point.y - nextPoint.y) < 1.0f) {
                        alreadyVisited = true;
                        break;
                    }
                }
                
                if (!alreadyVisited) {
                    pathPoints.push_back(nextPoint);
                    foundNext = true;
                    break;
                }
            }
        }
        
        if (!foundNext) {
            break; // Fin
        }
    }
}

void Enemy::updateSpriteRotation(sf::Vector2f direction) {
    // Calculer l'angle en radians puis le convertir en degrés
    float angle = atan2(direction.y, direction.x) * 180.0f / M_PI;
    
    sprite.setRotation(angle);
}

void Enemy::moveAlongPath(float deltaTime) {
    if (reachedEnd || pathPoints.empty() || currentPathIndex >= pathPoints.size()) {
        reachedEnd = true;
        return;
    }
    
    sf::Vector2f target = pathPoints[currentPathIndex];
    sf::Vector2f direction = target - position;
    float distance = sqrt(direction.x * direction.x + direction.y * direction.y);
    
    if (distance < 5.0f) {
        currentPathIndex++;
        if (currentPathIndex >= pathPoints.size()) {
            reachedEnd = true;
            return;
        }
        target = pathPoints[currentPathIndex];
        direction = target - position;
        distance = sqrt(direction.x * direction.x + direction.y * direction.y);
    }
    
    if (distance > 0) {
        direction.x /= distance;
        direction.y /= distance;

        updateSpriteRotation(direction);
        
        velocity = sf::Vector2f(direction.x * speed, direction.y * speed);
        position += velocity * deltaTime;
    }
}

void Enemy::draw(sf::RenderWindow& window) {
    updateHealthBar();
    
    window.draw(sprite);
    
    if (currentHealth < maxHealth) {
        window.draw(healthBarBackground);
        window.draw(healthBarForeground);
    }
}

bool Enemy::loadTexture() {
    std::string fileName = getTextureFileName();
    
    if (!texture.loadFromFile(fileName)) {
        std::cout << "Impossible de charger " << fileName << std::endl;
        return false;
    }
    sprite.setTexture(texture);
    
    // Ajuster la taille selon le type d'ennemi
    switch (enemyType) {
        case EnemyType::BASIC:
            sprite.setScale(1.5f, 1.5f);
            break;
        case EnemyType::FAST:
            sprite.setScale(1.3f, 1.3f); // Plus petit pour montrer qu'il est agile
            break;
        case EnemyType::TANK:
            sprite.setScale(2.0f, 2.0f); // Plus gros pour montrer qu'il est tank
            break;
    }
    
    sf::Vector2u textureSize = texture.getSize();
    sprite.setOrigin(textureSize.x / 2.0f, textureSize.y / 2.0f);
    
    return true;
}

void Enemy::updateHealthBar() {
    float healthPercent = (float)currentHealth / (float)maxHealth;
    
    float fullWidth = 32.0f;
    float currentWidth = fullWidth * healthPercent;
    healthBarForeground.setSize(sf::Vector2f(currentWidth, 4.0f));
    
    float barX = position.x - fullWidth / 2.0f;
    float barY = position.y - 20.0f;
    
    healthBarBackground.setPosition(barX, barY);
    healthBarForeground.setPosition(barX, barY);
}

void Enemy::update(float deltaTime) {
    moveAlongPath(deltaTime);
    sprite.setPosition(position);
}

bool Enemy::hasReachedEnd() const {
    return reachedEnd;
}

// Méthodes manquantes à ajouter :

sf::Vector2f Enemy::getPosition() const {
    return position;
}

bool Enemy::isDead() const {
    return currentHealth <= 0;
}

void Enemy::takeDamage(int damage) {
    currentHealth -= damage;
    if (currentHealth < 0) {
        currentHealth = 0;
    }
}

int Enemy::getHealth() const {
    return currentHealth;
}

float Enemy::getSpeed() const {
    return speed;
}

sf::FloatRect Enemy::getHitBox() const {
    return sprite.getGlobalBounds();
}

sf::Vector2f Enemy::getNextTarget() {
    if (currentPathIndex < pathPoints.size()) {
        return pathPoints[currentPathIndex];
    }
    return position; 
}

void Enemy::setSpeed(float newSpeed) {
    speed = newSpeed;
}

void Enemy::setHealth(int health) {
    maxHealth = health;
    currentHealth = health;
}

void Enemy::setReward(int newReward) {
    reward = newReward;
}

int Enemy::getReward() const {
    return reward;
}

EnemyType Enemy::getType() const {
    return enemyType;
}