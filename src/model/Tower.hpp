#include "Tower.cpp"
#include "Enemy.hpp"
#include <cmath>
#include <algorithm>
#include <iostream>

void Tower::initializeFromType() {
    damage = stats.levelStats[0].damage;
    fireRate = stats.levelStats[0].fireRate;
    range = stats.levelStats[0].range;
    rotationSpeed = stats.levelStats[0].rotationSpeed;
}

void Tower::update(float deltaTime, const std::vector<Enemy*>& enemies) {
    timeSinceLastShot += deltaTime;
    
    Enemy* nearestEnemy = findNearestEnemy(enemies);
    
    if (nearestEnemy != nullptr) {
        targetRotation = calculateAngleToTarget(nearestEnemy->getPosition());
        
        rotateTowards(targetRotation, deltaTime);
        
        if (canShoot()) {
            shootAt(nearestEnemy);
            timeSinceLastShot = 0.0f; 
        }
    }
}
void Tower::rotateTowards(float targetAngle, float deltaTime) {
    auto normalizeAngle = [](float angle) {
        while (angle > 180.0f) angle -= 360.0f;
        while (angle < -180.0f) angle += 360.0f;
        return angle;
    };
    
    float currentAngle = normalizeAngle(rotation);
    float target = normalizeAngle(targetAngle);
    
    float angleDiff = normalizeAngle(target - currentAngle);
    
    if (abs(angleDiff) < rotationSpeed * deltaTime) {
        rotation = target;
    } else {
        if (angleDiff > 0) {
            rotation += rotationSpeed * deltaTime;
        } else {
            rotation -= rotationSpeed * deltaTime;
        }
    }
    
    sprite.setRotation(rotation);
}

float Tower::calculateAngleToTarget(sf::Vector2f targetPos) {
    float dx = targetPos.x - position.x;
    float dy = targetPos.y - position.y;
    
    float angle = atan2(dy, dx) * 180.0f / M_PI + 90.0f;
    
    return angle;
}

void Tower::shootAt(Enemy* target) {
    if (target != nullptr && !target->isDead()) {
        bool wasAlive = !target->isDead();
        target->takeDamage(damage);
        
        if (wasAlive && target->isDead()) {
            earnedRewards.push_back(target->getReward());
            std::cout << "Ennemi éliminé par " << getTypeString() 
                     << " niveau " << (int)level 
                     << "! Récompense: " << target->getReward() << std::endl;
        }
    }
}

bool Tower::canShoot() const {
    return timeSinceLastShot >= (1.0f / fireRate);
}

void Tower::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

void Tower::drawRange(sf::RenderWindow& window) {
    window.draw(rangeIndicator);
}

void Tower::setRotation(float angle) {
    rotation = angle;
    sprite.setRotation(angle);
}

sf::Vector2f Tower::getPosition() const {
    return position;
}

float Tower::getRange() const {
    return range;
}

TowerType Tower::getType() const {
    return towerType;
}

std::string Tower::getTypeString() const {
    return TowerConfig::getTypeString(towerType);
}

bool Tower::isInRange(sf::Vector2f targetPosition) const {
    float dx = targetPosition.x - position.x;
    float dy = targetPosition.y - position.y;
    float distance = sqrt(dx * dx + dy * dy);
    return distance <= range;
}

std::vector<int> Tower::collectRewards() {
    std::vector<int> rewards = earnedRewards;
    earnedRewards.clear();
    return rewards;
}

bool Tower::canUpgrade() const {
    return level != TowerLevel::LEVEL_3;
}

int Tower::getUpgradeCost() const {
    switch (level) {
        case TowerLevel::LEVEL_1:
            return UPGRADE_COST_LEVEL_2;
        case TowerLevel::LEVEL_2:
            return UPGRADE_COST_LEVEL_3;
        case TowerLevel::LEVEL_3:
        default:
            return 0; 
    }
}

void Tower::upgrade() {
    if (!canUpgrade()) {
        return;
    }
    
    TowerLevel oldLevel = level;
    
    switch (level) {
        case TowerLevel::LEVEL_1:
            level = TowerLevel::LEVEL_2;
            break;
        case TowerLevel::LEVEL_2:
            level = TowerLevel::LEVEL_3;
            break;
        case TowerLevel::LEVEL_3:
            return; 
    }
    
    updateStatsForLevel();
    updateVisualForLevel();
    
    std::cout << getTypeString() << " améliorée du niveau " << (int)oldLevel 
             << " au niveau " << (int)level << "!" << std::endl;
}

TowerLevel Tower::getLevel() const {
    return level;
}

std::string Tower::getLevelText() const {
    switch (level) {
        case TowerLevel::LEVEL_1:
            return "Niveau 1";
        case TowerLevel::LEVEL_2:
            return "Niveau 2";
        case TowerLevel::LEVEL_3:
            return "Niveau 3";
        default:
            return "Inconnu";
    }
}

void Tower::updateStatsForLevel() {
    int levelIndex = (int)level - 1;
    
    damage = stats.levelStats[levelIndex].damage;
    fireRate = stats.levelStats[levelIndex].fireRate;
    range = stats.levelStats[levelIndex].range;
    rotationSpeed = stats.levelStats[levelIndex].rotationSpeed;
    
    rangeIndicator.setRadius(range);
    rangeIndicator.setOrigin(range, range);
}

void Tower::updateVisualForLevel() {
    sf::Color baseColor;
    sf::Color rangeColor;
    
    switch (towerType) {
        case TowerType::BASIC:
            baseColor = sf::Color::White;
            rangeColor = sf::Color(255, 255, 255, 150);
            break;
        case TowerType::SNIPER:
            baseColor = sf::Color::White;
            rangeColor = sf::Color(100, 150, 255, 150);
            break;
        case TowerType::CANNON:
            baseColor = sf::Color::White;
            rangeColor = sf::Color(255, 100, 100, 150);
            break;
    }


