#include <SFML/Graphics.hpp>
#include "GameMap.hpp"
#include "Ennemies.hpp"
#include "Wave.hpp"
#include "GameState.hpp"
#include "ViewUI.hpp"
#include "MainMenu.hpp" 
#include <iostream>

enum AppState {
    MENU,
    GAME,
    EXITING
};

enum TowerBuildMode {
    BASIC_TOWER,
    SNIPER_TOWER,
    CANNON_TOWER
};

int main() {
    sf::RenderWindow window(sf::VideoMode(1680, 960), "Tower Defense", sf::Style::Close);
    window.setFramerateLimit(60);

    AppState appState = MENU;
    TowerBuildMode buildMode = BASIC_TOWER;
    
    MainMenu mainMenu;
    
    GameMap gameMap;
    WaveManager* waveManager = nullptr;
    GameState gameState;
    ModernUI modernUI;

    sf::Clock clock;
    sf::Font font;
    
    if (!font.loadFromFile("modern_font.ttf")) {
        std::cout << "La police marche pas" << std::endl;
    }

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            switch (appState) {
                case MENU:
                    mainMenu.handleInput(event);
                    
                    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                        if (mainMenu.shouldStartGame()) {
                            gameMap.loadTextures();
                            gameMap.loadTowerTextures();
                            waveManager = new WaveManager(&gameMap);
                            gameState = GameState(); 
                            appState = GAME;
                        } else if (mainMenu.shouldExit()) {
                            appState = EXITING;
                        }
                    }
                    break;
                    
                case GAME:
                    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                        delete waveManager;
                        waveManager = nullptr;
                        appState = MENU;
                        break;
                    }
                    
                    if (event.type == sf::Event::MouseMoved) {
                        if (event.mouseMove.x < 1280) {
                            gameMap.updateHover(event.mouseMove.x, event.mouseMove.y);
                        }
                    }

                    if (event.type == sf::Event::MouseButtonPressed && !gameState.isGameOver()) {
                        if (event.mouseButton.button == sf::Mouse::Left) {
                            if (event.mouseButton.x < 1280) {
                                int tileX = event.mouseButton.x / gameMap.getTileSize();
                                int tileY = event.mouseButton.y / gameMap.getTileSize();

                                if (gameMap.hasTowerAt(tileX, tileY)) {
                                    gameMap.selectTower(event.mouseButton.x, event.mouseButton.y);
                                } else if (gameMap.isPlace(tileX, tileY)) {
                            
                                    TowerType towerType;
                                    int cost;
                                    
                                    switch (buildMode) {
                                        case BASIC_TOWER:
                                            towerType = TowerType::BASIC;
                                            cost = TowerConfig::getStatsForType(TowerType::BASIC).baseCost;
                                            break;
                                        case SNIPER_TOWER:
                                            towerType = TowerType::SNIPER;
                                            cost = TowerConfig::getStatsForType(TowerType::SNIPER).baseCost;
                                            break;
                                        case CANNON_TOWER:
                                            towerType = TowerType::CANNON;
                                            cost = TowerConfig::getStatsForType(TowerType::CANNON).baseCost;
                                            break;
                                    }
                                    
                                    if (gameState.canAfford(cost)) {
                                        if (gameState.spendMoney(cost)) {
                                            gameMap.addTower(tileX, tileY, towerType);
                                            gameMap.deselectTower();
                                        }
                                    }
                                }
                            }
                        }

                        if (event.mouseButton.button == sf::Mouse::Right) {
                            gameMap.deselectTower();
                        }
                    }

                    if (event.type == sf::Event::KeyPressed && !gameState.isGameOver()) {
                        if (event.key.code == sf::Keyboard::Space) {
                            if (waveManager && !waveManager->isWaveActive() && waveManager->getTimeUntilNextWave() <= 0) {
                                waveManager->startNextWave();
                            }
                        } 
                        else if (event.key.code == sf::Keyboard::U) {
                            Tower* selectedTower = gameMap.getSelectedTower();
                            if (selectedTower != nullptr && selectedTower->canUpgrade()) {
                                int upgradeCost = selectedTower->getUpgradeCost();
                                if (gameState.canAfford(upgradeCost)) {
                                    if (gameState.spendMoney(upgradeCost)) {
                                        selectedTower->upgrade();
                                    }
                                }
                            }
                        }
                    
                        else if (event.key.code == sf::Keyboard::Num1) {
                            buildMode = BASIC_TOWER;
                        }
                        else if (event.key.code == sf::Keyboard::Num2) {
                            buildMode = SNIPER_TOWER;
                        }
                        else if (event.key.code == sf::Keyboard::Num3) {
                            buildMode = CANNON_TOWER;
                        }
                    }
                    break;
            }
        }

        switch (appState) {
            case MENU:
                mainMenu.update(deltaTime);
                break;
                
            case GAME:
                if (waveManager) {
                    if (!gameState.isGameOver()) {
                        waveManager->update(deltaTime);

                
                        std::vector<int> rewards = gameMap.collectAllTowerRewards();
                        for (int reward : rewards) {
                            gameState.addMoney(reward);
                        }

                        for (const auto& enemy : waveManager->getEnemies()) {
                            if (enemy && enemy->hasReachedEnd()) {
                                gameState.loseLife();
                            }
                        }

                    
                        std::vector<Enemy*> enemyPtrs;
                        for (const auto& enemy : waveManager->getEnemies()) {
                            if (enemy && !enemy->isDead() && !enemy->hasReachedEnd()) {
                                enemyPtrs.push_back(enemy.get());
                            }
                        }

                        gameMap.updateTowers(deltaTime, enemyPtrs);
                    }

                    modernUI.update(deltaTime);
                }
                break;
                
            case EXITING:
                window.close();
                break;
        }

        window.clear();
        
        switch (appState) {
            case MENU:
                mainMenu.draw(window, font);
                break;
                
            case GAME:
                if (waveManager) {
                    
                    modernUI.drawBackground(window);
                    
                    gameMap.draw(window);
                    gameMap.drawTowerRanges(window);

                    for (const auto& enemy : waveManager->getEnemies()) {
                        if (enemy && !enemy->isDead()) {
                            enemy->draw(window);
                        }
                    }

                    modernUI.drawHUD(window, font, 
                                    gameState.getMoney(), 
                                    gameState.getLives(), 
                                    waveManager->getCurrentWaveNumber(), 
                                    waveManager->getTotalWaves());

                    modernUI.drawWaveIndicator(window, font, 
                                              waveManager->isWaveActive(), 
                                              (int)waveManager->getTimeUntilNextWave());

                    modernUI.drawBuildMode(window, font, 
                                          (int)buildMode,
                                          TowerConfig::getStatsForType(TowerType::BASIC).baseCost,
                                          TowerConfig::getStatsForType(TowerType::SNIPER).baseCost,
                                          TowerConfig::getStatsForType(TowerType::CANNON).baseCost);
                    Tower* selectedTower = gameMap.getSelectedTower();
                    if (selectedTower != nullptr) {
                        std::string towerInfo = selectedTower->getTypeString() + "\n";
                        towerInfo += selectedTower->getLevelText() + "\n";
                        towerInfo += "Portée: " + std::to_string((int)selectedTower->getRange());
                        
                        modernUI.drawTowerInfo(window, font, towerInfo, 
                                              selectedTower->canUpgrade(), 
                                              selectedTower->getUpgradeCost());
                    }

                    if (gameState.isGameOver()) {
                        modernUI.drawGameOver(window, font);
                    }
                }
                break;
        }

        window.display();
    }
    if (waveManager) {
        delete waveManager;
    }

    return 0;
}
