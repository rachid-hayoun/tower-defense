#include "GameMap.hpp"
#include <iostream>
#include <cmath>

GameMap::GameMap() : hoveredTileX(-1), hoveredTileY(-1), isValidHover(false), selectedTower(nullptr) {
    mapData = {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {3,3,3,3,3,3,3,3,3,3,8,0,0,0,0,0,0,0,0,0},
        {1,1,1,1,1,1,1,1,1,1,5,0,0,0,0,0,0,0,0,0},
        {4,4,4,4,4,4,4,4,7,1,5,0,0,0,0,0,0,0,0,0},
        {0,0,0,2,0,0,0,0,6,1,5,0,0,2,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,6,1,12,3,3,3,3,3,3,3,8,0},
        {0,0,0,0,0,0,0,0,6,1,1,1,1,1,1,1,1,1,5,0},
        {0,0,0,0,0,0,0,0,10,4,4,4,4,4,4,4,7,1,5,0},
        {0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,6,1,5,0},
        {0,9,3,3,3,3,3,3,3,3,3,3,3,3,3,3,13,1,5,0},
        {0,6,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5,0},
        {0,6,1,11,4,4,4,4,4,4,4,4,4,4,4,4,4,4,14,0},
        {0,6,1,5,0,0,0,0,2,0,0,0,0,0,2,0,0,0,0,0},
        {0,6,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {0,10,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4}
    };

    // Initialiser la grille de placement des tours
    towerPlacement.resize(MAP_HEIGHT, std::vector<bool>(MAP_WIDTH, false));

    hoverIndicator.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    hoverIndicator.setFillColor(sf::Color::Transparent);
    hoverIndicator.setOutlineThickness(2.0f);
    hoverIndicator.setOutlineColor(sf::Color::Yellow);
}

bool GameMap::loadTextures() {
    if (loadTexturesFromFiles()) {
        grassSprite.setTexture(grassTexture);
        pathSprite.setTexture(pathTexture);
        placementSprite.setTexture(placementTexture);
        grass2Sprite.setTexture(grass2Texture);
        grass3Sprite.setTexture(grass3Texture);
        grass4Sprite.setTexture(grass4Texture);
        grass5Sprite.setTexture(grass5Texture);
        grass6Sprite.setTexture(grass6Texture);
        grass7Sprite.setTexture(grass7Texture);
        grass8Sprite.setTexture(grass8Texture);
        grass9Sprite.setTexture(grass9Texture);
        grass10Sprite.setTexture(grass10Texture);
        grass11Sprite.setTexture(grass11Texture);
        grass12Sprite.setTexture(grass12Texture);
        grass13Sprite.setTexture(grass13Texture);
        return true;
    }
    return false;
}

bool GameMap::loadTexturesFromFiles() {
    if (!grassTexture.loadFromFile("Assets/Grass/Grass1.png")) {
        return false;
    }
    if (!pathTexture.loadFromFile("Assets/Grass/Chemin.png")) {
        return false;
    }
    if (!placementTexture.loadFromFile("Assets/Grass/Placement.png")) {
        return false;
    }
    if (!grass2Texture.loadFromFile("Assets/Grass/Grass2.png")) {
        return false;
    }
    if (!grass3Texture.loadFromFile("Assets/Grass/Grass3.png")) {
        return false;
    }
    if (!grass4Texture.loadFromFile("Assets/Grass/Grass4.png")) {
        return false;
    }
    if (!grass5Texture.loadFromFile("Assets/Grass/Grass5.png")) {
        return false;
    }
    if (!grass6Texture.loadFromFile("Assets/Grass/Grass6.png")) {
        return false;
    }
    if (!grass7Texture.loadFromFile("Assets/Grass/Grass7.png")) {
        return false;
    }
    if (!grass8Texture.loadFromFile("Assets/Grass/Grass8.png")) {
        return false;
    }
    if (!grass9Texture.loadFromFile("Assets/Grass/Grass9.png")) {
        return false;
    }
    if (!grass10Texture.loadFromFile("Assets/Grass/Grass10.png")) {
        return false;
    }
    if (!grass11Texture.loadFromFile("Assets/Grass/Grass11.png")) {
        return false;
    }
    if (!grass12Texture.loadFromFile("Assets/Grass/Grass12.png")) {
        return false;
    }
    if (!grass13Texture.loadFromFile("Assets/Grass/Grass13.png")) {
        return false;
    }
    return true;
}

bool GameMap::loadTowerTextures() {
    // Charger les textures pour chaque type de tour
    if (!towerTextures[TowerType::BASIC].loadFromFile("Assets/Towers/Tower1.png")) {
        std::cout << "Impossible de charger Tower1.png" << std::endl;
        return false;
    }
    
    if (!towerTextures[TowerType::SNIPER].loadFromFile("Assets/Towers/TowerSniper.png")) {
        std::cout << "Impossible de charger TowerSniper.png" << std::endl;
        // Utiliser la texture de base comme fallback
        towerTextures[TowerType::SNIPER] = towerTextures[TowerType::BASIC];
    }
    
    if (!towerTextures[TowerType::CANNON].loadFromFile("Assets/Towers/TowerCannon.png")) {
        std::cout << "Impossible de charger TowerCannon.png" << std::endl;
        // Utiliser la texture de base comme fallback
        towerTextures[TowerType::CANNON] = towerTextures[TowerType::BASIC];
    }
    
    return true;
}

void GameMap::addTower(int tileX, int tileY, TowerType type) {
    if (!isPlace(tileX, tileY) || hasTowerAt(tileX, tileY)) return;

    float posX = tileX * TILE_SIZE + TILE_SIZE / 2.f;
    float posY = tileY * TILE_SIZE + TILE_SIZE / 2.f;
    
    Tower newTower(posX, posY, &towerTextures[type], type);
    
    // Calculer la rotation de la tour
    float angle = calculateTowerRotation(tileX, tileY);
    newTower.setRotation(angle);
    
    towers.push_back(newTower);
    towerPlacement[tileY][tileX] = true;
}

float GameMap::calculateTowerRotation(int tileX, int tileY) {
    float angle = 0.f;
    
    if (tileX > 0 && isPath(tileX - 1, tileY)) {
        angle = -90.f; // Gauche
    }
    else if (tileX < MAP_WIDTH - 1 && isPath(tileX + 1, tileY)) {
        angle = 90.f;   // Droite
    }
    else if (tileY > 0 && isPath(tileX, tileY - 1)) {
        angle = 0.f; // Haut
    }
    else if (tileY < MAP_HEIGHT - 1 && isPath(tileX, tileY + 1)) {
        angle = 180.f;  // Bas
    }
    else if (tileX > 0 && isPath(tileX + 1, tileY - 1)) {
        angle = 45.f; 
    }
    else if (tileX > 0 && isPath(tileX + 1, tileY + 1)) {
        angle = 135.f;
    }  
    else if (tileX < MAP_WIDTH - 1 && isPath(tileX - 1, tileY + 1)){
        angle = -125.f; 
    }
    else if (tileX < MAP_WIDTH - 1 && isPath(tileX - 1, tileY - 1)) {
        angle = -45.f;
    }
    
    return angle;
}

void GameMap::selectTower(int mouseX, int mouseY) {
    int tileX = mouseX / TILE_SIZE;
    int tileY = mouseY / TILE_SIZE;
    
    if (hasTowerAt(tileX, tileY)) {
        // Trouver la tour à cette position
        float targetX = tileX * TILE_SIZE + TILE_SIZE / 2.f;
        float targetY = tileY * TILE_SIZE + TILE_SIZE / 2.f;
        
        for (auto& tower : towers) {
            sf::Vector2f towerPos = tower.getPosition();
            if (abs(towerPos.x - targetX) < 1.0f && abs(towerPos.y - targetY) < 1.0f) {
                selectedTower = &tower;
                break;
            }
        }
    } else {
        selectedTower = nullptr;
    }
}

void GameMap::deselectTower() {
    selectedTower = nullptr;
}

Tower* GameMap::getSelectedTower() {
    return selectedTower;
}

Tower* GameMap::getTowerAt(int mouseX, int mouseY) {
    int tileX = mouseX / TILE_SIZE;
    int tileY = mouseY / TILE_SIZE;
    
    if (hasTowerAt(tileX, tileY)) {
        float targetX = tileX * TILE_SIZE + TILE_SIZE / 2.f;
        float targetY = tileY * TILE_SIZE + TILE_SIZE / 2.f;
        
        for (auto& tower : towers) {
            sf::Vector2f towerPos = tower.getPosition();
            if (abs(towerPos.x - targetX) < 1.0f && abs(towerPos.y - targetY) < 1.0f) {
                return &tower;
            }
        }
    }
    return nullptr;
}

bool GameMap::upgradeTower(int mouseX, int mouseY) {
    Tower* tower = getTowerAt(mouseX, mouseY);
    if (tower != nullptr && tower->canUpgrade()) {
        tower->upgrade();
        return true;
    }
    return false;
}

bool GameMap::hasTowerAt(int tileX, int tileY) const {
    if (tileX < 0 || tileX >= MAP_WIDTH || tileY < 0 || tileY >= MAP_HEIGHT) {
        return false;
    }
    return towerPlacement[tileY][tileX];
}

void GameMap::updateHover(int mouseX, int mouseY) {
    int tileX = mouseX / TILE_SIZE;
    int tileY = mouseY / TILE_SIZE;

    if (tileX >= 0 && tileX < MAP_WIDTH && tileY >= 0 && tileY < MAP_HEIGHT) {
        hoveredTileX = tileX;
        hoveredTileY = tileY;
        isValidHover = true;

        hoverIndicator.setPosition(tileX * TILE_SIZE, tileY * TILE_SIZE);

        if (isPath(tileX, tileY)) {
            hoverIndicator.setOutlineColor(sf::Color::Red);
        } else if (hasTowerAt(tileX, tileY)) {
            hoverIndicator.setOutlineColor(sf::Color::Blue);
        } else {
            hoverIndicator.setOutlineColor(sf::Color::Green);
        }
    } 
    else {
        isValidHover = false;
    }
}

void GameMap::updateTowers(float deltaTime, const std::vector<Enemy*>& enemies) {
    for (auto& tower : towers) {
        tower.update(deltaTime, enemies);
    }
}

void GameMap::draw(sf::RenderWindow& window) {
    // Dessiner les tuiles
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            sf::Sprite* currentSprite;
            
            if (mapData[y][x] == GRASS) {
                currentSprite = &grassSprite;
            } else if (mapData[y][x] == PLAS) {
                currentSprite = &placementSprite;
            } else if (mapData[y][x] == PATH) {
                currentSprite = &pathSprite;
            } else if (mapData[y][x] == GRASSUP) {
                currentSprite = &grass2Sprite;
            } else if (mapData[y][x] == GRASSDOWN) {
                currentSprite = &grass3Sprite;
            } else if (mapData[y][x] == GRASSRIGHT) {
                currentSprite = &grass4Sprite;
            } else if (mapData[y][x] == GRASSLEFT) {
                currentSprite = &grass5Sprite;
            } else if (mapData[y][x] == GRASSLD) {
                currentSprite = &grass6Sprite; 
            } else if (mapData[y][x] == GRASSAR) {
                currentSprite = &grass7Sprite;
            } else if (mapData[y][x] == GRASSAL) {
                currentSprite = &grass8Sprite;
            } else if (mapData[y][x] == GRASSBL) {
                currentSprite = &grass9Sprite;
            } else if (mapData[y][x] == GRASSBR) {
                currentSprite = &grass10Sprite;
            } else if (mapData[y][x] == GRASSHR) {
                currentSprite = &grass11Sprite;
            } else if (mapData[y][x] == GRASSHL) {
                currentSprite = &grass12Sprite;
            } else if (mapData[y][x] == GRASSMO) {
                currentSprite = &grass13Sprite;
            } else {
                currentSprite = &grassSprite;
            }
            
            currentSprite->setPosition(x * TILE_SIZE, y * TILE_SIZE);
            window.draw(*currentSprite);
        }
    }

    // Dessiner les tours
    for (auto& tower : towers) {
        tower.draw(window);
    }

    // Dessiner l'indicateur de survol
    if (isValidHover) {
        window.draw(hoverIndicator);
    }
}

void GameMap::drawTowerRanges(sf::RenderWindow& window) {
    // Dessiner la portée de la tour sélectionnée
    if (selectedTower != nullptr) {
        selectedTower->drawRange(window);
    }
}

void GameMap::drawTowerInfo(sf::RenderWindow& window, sf::Font& font) {
    if (selectedTower != nullptr) {
        // Position pour l'affichage des informations (coin supérieur droit)
        float infoX = 1300;
        float infoY = 200;
        
        // Créer le texte d'information
        sf::Text infoText;
        infoText.setFont(font);
        infoText.setCharacterSize(20);
        infoText.setFillColor(sf::Color::White);
        infoText.setPosition(infoX, infoY);
        
        std::string info = selectedTower->getTypeString() + "\n";
        info += selectedTower->getLevelText() + "\n";
        info += "Portée: " + std::to_string((int)selectedTower->getRange()) + "\n";
        
        if (selectedTower->canUpgrade()) {
            info += "Amélioration: " + std::to_string(selectedTower->getUpgradeCost()) + " gold\n";
            info += "Appuyez sur U pour améliorer";
        } else {
            info += "Niveau maximum atteint";
        }
        
        infoText.setString(info);
        
        // Dessiner un fond semi-transparent
        sf::RectangleShape background;
        sf::FloatRect textBounds = infoText.getGlobalBounds();
        background.setSize(sf::Vector2f(textBounds.width + 20, textBounds.height + 20));
        background.setPosition(infoX - 10, infoY - 10);
        background.setFillColor(sf::Color(0, 0, 0, 150));
        
        window.draw(background);
        window.draw(infoText);
    }
}

int GameMap::getTileSize() const { return TILE_SIZE; }
int GameMap::getMapWidth() const { return MAP_WIDTH; }
int GameMap::getMapHeight() const { return MAP_HEIGHT; }

bool GameMap::isPath(int x, int y) const {
    return x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT && mapData[y][x] == PATH;
}

bool GameMap::isPlace(int x, int y) const {
    return x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT && mapData[y][x] == PLAS;
}

bool GameMap::isGrass(int x, int y) const {
    return x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT && mapData[y][x] == GRASS;
}

std::vector<int> GameMap::collectAllTowerRewards() {
    std::vector<int> allRewards;
    for (auto& tower : towers) {
        std::vector<int> towerRewards = tower.collectRewards();
        allRewards.insert(allRewards.end(), towerRewards.begin(), towerRewards.end());
    }
    return allRewards;
}