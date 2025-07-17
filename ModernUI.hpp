#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

class ModernUI {
private:
    sf::Font font;
    sf::Clock animationClock;
    float pulseTime = 0.0f;

    static const int UI_START_X = 1280; 
    static const int UI_WIDTH = 400;   
    static const int UI_MARGIN = 10;  

    struct Particle {
        sf::Vector2f position;
        sf::Vector2f velocity;
        float life;
        float maxLife;
        sf::Color color;
        float size;
    };
    std::vector<Particle> particles;
    
    sf::RectangleShape glowEffect;
    sf::RectangleShape scanLine;
    float scanLinePosition = 0.0f;
    
public:
    ModernUI() {
        initParticles();
        
        glowEffect.setSize(sf::Vector2f(UI_WIDTH, 960));
        glowEffect.setPosition(UI_START_X, 0);
        
        scanLine.setSize(sf::Vector2f(UI_WIDTH, 2));
        scanLine.setFillColor(sf::Color(50, 255, 100, 120));
    }
    
    void initParticles() {
        particles.clear();
        for (int i = 0; i < 60; ++i) {
            Particle p;
            p.position = sf::Vector2f(
                UI_START_X + rand() % UI_WIDTH,
                rand() % 960
            );
            p.velocity = sf::Vector2f(
                (rand() % 60 - 30) / 100.0f,
                (rand() % 80 - 40) / 100.0f
            );
            p.life = p.maxLife = 4.0f + (rand() % 400) / 100.0f;
            p.size = 1.0f + (rand() % 200) / 100.0f;
            
            int greenType = rand() % 4;
            switch(greenType) {
                case 0: 
                    p.color = sf::Color(46, 204, 113, 80 + rand() % 100);
                    break;
                case 1: 
                    p.color = sf::Color(39, 174, 96, 60 + rand() % 80);
                    break;
                case 2:
                    p.color = sf::Color(26, 188, 156, 70 + rand() % 90);
                    break;
                case 3: 
                    p.color = sf::Color(130, 224, 170, 50 + rand() % 100);
                    break;
            }
            particles.push_back(p);
        }
    }
    
    void update(float deltaTime) {
        pulseTime += deltaTime;
        scanLinePosition += deltaTime * 150.0f; 
        if (scanLinePosition > 960) scanLinePosition = -2;
        
        for (auto& particle : particles) {

            particle.velocity.x += sin(pulseTime * 2.0f + particle.position.y * 0.01f) * 0.1f;
            particle.position += particle.velocity * deltaTime * 30.0f;
            particle.life -= deltaTime;
            
            if (particle.life <= 0 || 
                particle.position.x < UI_START_X || 
                particle.position.x > UI_START_X + UI_WIDTH ||
                particle.position.y > 960) {
                particle.position = sf::Vector2f(
                    UI_START_X + rand() % UI_WIDTH,
                    -10.0f 
                );
                particle.life = particle.maxLife;
                particle.velocity.y = (rand() % 50 + 20) / 100.0f; 
            }
            
            float alpha = (particle.life / particle.maxLife) * 150;
            particle.color.a = static_cast<sf::Uint8>(alpha);
        }
        
        scanLine.setPosition(UI_START_X, scanLinePosition);
    }
    
    void drawBackground(sf::RenderWindow& window) {

        sf::RectangleShape gameBackground(sf::Vector2f(1280, 960));
        float pulse = sin(pulseTime * 1.5f) * 0.05f + 0.95f;
        gameBackground.setFillColor(sf::Color(
            static_cast<sf::Uint8>(25 * pulse), 
            static_cast<sf::Uint8>(20 * pulse),
            static_cast<sf::Uint8>(15 * pulse),
            255
        ));
        window.draw(gameBackground);
        
        sf::RectangleShape uiBackground(sf::Vector2f(UI_WIDTH, 960));
        uiBackground.setPosition(UI_START_X, 0);
        uiBackground.setFillColor(sf::Color(
            static_cast<sf::Uint8>(15 * pulse), 
            static_cast<sf::Uint8>(35 * pulse),
            static_cast<sf::Uint8>(25 * pulse),
            255
        ));
        window.draw(uiBackground);

        for (const auto& particle : particles) {
            sf::CircleShape leaf(particle.size);
            leaf.setPosition(particle.position);
            leaf.setFillColor(particle.color);
            window.draw(leaf);
        }
    }
    
    void drawHUD(sf::RenderWindow& window, sf::Font& gameFont, int money, int lives, int wave, int totalWaves) {

        sf::RectangleShape hudPanel(sf::Vector2f(UI_WIDTH - 2 * UI_MARGIN, 940));
        hudPanel.setPosition(UI_START_X + UI_MARGIN, UI_MARGIN);
        hudPanel.setFillColor(sf::Color(20, 50, 30, 200)); 
        hudPanel.setOutlineThickness(2);
        hudPanel.setOutlineColor(sf::Color(46, 204, 113, 180)); 
        window.draw(hudPanel);
        
        window.draw(scanLine);
        
        sf::Text title("TOWER DEFENSE", gameFont, 26);
        title.setPosition(UI_START_X + 30, 30);
        title.setFillColor(sf::Color(46, 204, 113)); 
        title.setStyle(sf::Text::Bold);
    
        sf::Text titleGlow = title;
        titleGlow.setFillColor(sf::Color(46, 204, 113, 60));
        titleGlow.setPosition(UI_START_X + 29, 29);
        window.draw(titleGlow);
        titleGlow.setPosition(UI_START_X + 31, 31);
        window.draw(titleGlow);
        window.draw(title);
        
        drawStatBar(window, gameFont, "ARGENTS", std::to_string(money), UI_START_X + 30, 100, sf::Color(241, 196, 15)); 
        drawStatBar(window, gameFont, "VIES", std::to_string(lives), UI_START_X + 30, 140, sf::Color(231, 76, 60)); 
        drawStatBar(window, gameFont, "VAGUES", std::to_string(wave) + "/" + std::to_string(totalWaves), UI_START_X + 30, 180, sf::Color(46, 204, 113));
        
        drawGrid(window);
    }
    
    void drawStatBar(sf::RenderWindow& window, sf::Font& font, const std::string& label, const std::string& value, float x, float y, sf::Color color) {
        sf::RectangleShape barBg(sf::Vector2f(UI_WIDTH - 60, 25));
        barBg.setPosition(x, y);
        barBg.setFillColor(sf::Color(10, 30, 20, 150)); 
        barBg.setOutlineThickness(1);
        barBg.setOutlineColor(sf::Color(39, 174, 96, 120)); 
        window.draw(barBg);

        sf::Text labelText(label, font, 14);
        labelText.setPosition(x + 10, y + 5);
        labelText.setFillColor(color);
        labelText.setStyle(sf::Text::Bold);
        window.draw(labelText);

        sf::Text valueText(value, font, 14);
        valueText.setPosition(x + UI_WIDTH - 120, y + 5);
        valueText.setFillColor(sf::Color(220, 255, 220)); 
        valueText.setStyle(sf::Text::Bold);
        window.draw(valueText);
        
        float glowIntensity = (sin(pulseTime * 2.5f) + 1.0f) * 0.3f + 0.4f;
        sf::RectangleShape glow(sf::Vector2f(UI_WIDTH - 60, 2));
        glow.setPosition(x, y + 23);
        glow.setFillColor(sf::Color(46, 204, 113, static_cast<sf::Uint8>(180 * glowIntensity)));
        window.draw(glow);
    }
    
    void drawGrid(sf::RenderWindow& window) {
        sf::Color gridColor(39, 174, 96, 40);
 
        for (int i = 0; i < 20; ++i) {
            sf::RectangleShape line(sf::Vector2f(UI_WIDTH - 2 * UI_MARGIN, 1));
            line.setPosition(UI_START_X + UI_MARGIN, UI_MARGIN + i * 47);
            line.setFillColor(gridColor);
            window.draw(line);
        }
      
        int verticalLines = (UI_WIDTH - 2 * UI_MARGIN) / 54;
        for (int i = 0; i <= verticalLines; ++i) {
            sf::RectangleShape line(sf::Vector2f(1, 940));
            line.setPosition(UI_START_X + UI_MARGIN + i * 54, UI_MARGIN);
            line.setFillColor(gridColor);
            window.draw(line);
        }
        
        for (int i = 0; i <= verticalLines; ++i) {
            for (int j = 0; j < 20; ++j) {
                float pulse = sin(pulseTime * 1.8f + i * 0.4f + j * 0.2f) * 0.5f + 0.5f;
                sf::CircleShape dot(2.5f);
                dot.setPosition(UI_START_X + UI_MARGIN + i * 54 - 1.5f, UI_MARGIN + j * 47 - 1.5f);
                dot.setFillColor(sf::Color(46, 204, 113, static_cast<sf::Uint8>(120 * pulse)));
                window.draw(dot);
            }
        }
    }
   
    void drawBuildMode(sf::RenderWindow& window, sf::Font& font, int buildMode, int basicCost, int sniperCost, int cannonCost) {
        sf::RectangleShape buildPanel(sf::Vector2f(UI_WIDTH - 40, 150));
        buildPanel.setPosition(UI_START_X + 20, 250);
        buildPanel.setFillColor(sf::Color(20, 50, 30, 230)); 
        buildPanel.setOutlineThickness(2);
        buildPanel.setOutlineColor(sf::Color(46, 204, 113, 180)); 
        window.draw(buildPanel);
        
        sf::Text titleText("MODE CONSTRUCTION", font, 16);
        titleText.setPosition(UI_START_X + 40, 260);
        titleText.setFillColor(sf::Color(46, 204, 113)); 
        titleText.setStyle(sf::Text::Bold);
        window.draw(titleText);
        
        float yPos = 290;
        
        drawTowerOption(window, font, "1. TOUR BASIQUE", basicCost, 
                       UI_START_X + 40, yPos, 
                       buildMode == 0, sf::Color::White);
        yPos += 25;
        
        drawTowerOption(window, font, "2. TOUR SNIPER", sniperCost, 
                       UI_START_X + 40, yPos, 
                       buildMode == 1, sf::Color::Cyan);
        yPos += 25;

        drawTowerOption(window, font, "3. TOUR CANON", cannonCost, 
                       UI_START_X + 40, yPos, 
                       buildMode == 2, sf::Color::Red);
    }
    
    void drawTowerOption(sf::RenderWindow& window, sf::Font& font, const std::string& name, int cost, float x, float y, bool selected, sf::Color typeColor) {

        sf::RectangleShape selectionBar(sf::Vector2f(UI_WIDTH - 80, 20));
        selectionBar.setPosition(x, y);
        
        if (selected) {
            float pulse = sin(pulseTime * 4.0f) * 0.3f + 0.7f;
            selectionBar.setFillColor(sf::Color(typeColor.r, typeColor.g, typeColor.b, 
                                               static_cast<sf::Uint8>(100 * pulse)));
            selectionBar.setOutlineThickness(2);
            selectionBar.setOutlineColor(typeColor);
        } else {
            selectionBar.setFillColor(sf::Color(10, 30, 20, 100));
            selectionBar.setOutlineThickness(1);
            selectionBar.setOutlineColor(sf::Color(39, 174, 96, 80));
        }
        
        window.draw(selectionBar);

        sf::Text nameText(name, font, 12);
        nameText.setPosition(x + 5, y + 4);
        if (selected) {
            nameText.setFillColor(sf::Color::White);
            nameText.setStyle(sf::Text::Bold);
        } else {
            nameText.setFillColor(sf::Color(200, 200, 200));
        }
        window.draw(nameText);
        
        sf::Text costText(std::to_string(cost) + " $", font, 12);
        costText.setPosition(x + UI_WIDTH - 120, y + 4);
        costText.setFillColor(sf::Color(241, 196, 15));
        if (selected) {
            costText.setStyle(sf::Text::Bold);
        }
        window.draw(costText);
    }
    
    void drawTowerInfo(sf::RenderWindow& window, sf::Font& font, const std::string& info, bool canUpgrade, int upgradeCost) {
        sf::RectangleShape infoPanel(sf::Vector2f(UI_WIDTH - 40, 150));
        infoPanel.setPosition(UI_START_X + 20, 420);
        infoPanel.setFillColor(sf::Color(20, 60, 40, 230)); 
        infoPanel.setOutlineThickness(2);
        infoPanel.setOutlineColor(sf::Color(39, 174, 96, 180)); 
        window.draw(infoPanel);
        
        sf::Text infoText("INFO DE LA TOUR", font, 16);
        infoText.setPosition(UI_START_X + 40, 430);
        infoText.setFillColor(sf::Color(46, 204, 113)); 
        infoText.setStyle(sf::Text::Bold);
        window.draw(infoText);
        
        sf::Text detailText(info, font, 12);
        detailText.setPosition(UI_START_X + 40, 450);
        detailText.setFillColor(sf::Color(220, 255, 220)); 
        window.draw(detailText);
        
        if (canUpgrade) {

            float buttonPulse = sin(pulseTime * 3.5f) * 0.2f + 0.8f;
            sf::RectangleShape upgradeBtn(sf::Vector2f(200, 30));
            upgradeBtn.setPosition(UI_START_X + 40, 530);
            upgradeBtn.setFillColor(sf::Color(46, 204, 113, static_cast<sf::Uint8>(180 * buttonPulse))); 
            upgradeBtn.setOutlineThickness(2);
            upgradeBtn.setOutlineColor(sf::Color(39, 174, 96));
            window.draw(upgradeBtn);
            
            sf::Text upgradeText("U - AMELIORER (" + std::to_string(upgradeCost) + " $)", font, 12);
            upgradeText.setPosition(UI_START_X + 50, 540);
            upgradeText.setFillColor(sf::Color(255, 255, 255));
            upgradeText.setStyle(sf::Text::Bold);
            window.draw(upgradeText);
        } else {
            sf::Text maxText("NIVEAU MAXIMUM ATTEINT", font, 12);
            maxText.setPosition(UI_START_X + 40, 530);
            maxText.setFillColor(sf::Color(255, 215, 0)); 
            maxText.setStyle(sf::Text::Bold);
            window.draw(maxText);
        }
    }
    
    void drawControls(sf::RenderWindow& window, sf::Font& font) {
        sf::RectangleShape controlsPanel(sf::Vector2f(UI_WIDTH - 40, 120));
        controlsPanel.setPosition(UI_START_X + 20, 590);
        controlsPanel.setFillColor(sf::Color(20, 50, 30, 200));
        controlsPanel.setOutlineThickness(2);
        controlsPanel.setOutlineColor(sf::Color(46, 204, 113, 150));
        window.draw(controlsPanel);
        
        sf::Text controlsTitle("CONTROLES", font, 16);
        controlsTitle.setPosition(UI_START_X + 40, 600);
        controlsTitle.setFillColor(sf::Color(46, 204, 113));
        controlsTitle.setStyle(sf::Text::Bold);
        window.draw(controlsTitle);
        
        sf::Text controlsText(
            "1/2/3 - Changer type de tour\n"
            "U - Ameliorer tour selectionnee\n"
            "ECHAP - Retour au menu\n"
            "Clic gauche - Placer/Selectionner\n", 
            font, 11
        );
        controlsText.setPosition(UI_START_X + 40, 625);
        controlsText.setFillColor(sf::Color(200, 255, 200));
        window.draw(controlsText);
    }
    
    void drawWaveIndicator(sf::RenderWindow& window, sf::Font& font, bool waveActive, int timeLeft) {
        sf::RectangleShape wavePanel(sf::Vector2f(UI_WIDTH - 40, 80));
        wavePanel.setPosition(UI_START_X + 20, 730);
        
        if (waveActive) {
            float intensity = sin(pulseTime * 5.0f) * 0.3f + 0.7f;
            wavePanel.setFillColor(sf::Color(220, 20, 60, static_cast<sf::Uint8>(200 * intensity))); 
            wavePanel.setOutlineColor(sf::Color(220, 20, 60));
        } else {
            wavePanel.setFillColor(sf::Color(39, 174, 96, 200)); 
            wavePanel.setOutlineColor(sf::Color(46, 204, 113));
        }
        
        wavePanel.setOutlineThickness(2);
        window.draw(wavePanel);
        
        sf::Text statusText;
        statusText.setFont(font);
        statusText.setCharacterSize(14);
        statusText.setPosition(UI_START_X + 40, 750);
        statusText.setFillColor(sf::Color(255, 255, 255));
        statusText.setStyle(sf::Text::Bold);
        
        if (waveActive) {
            statusText.setString("INVASION EN COURS!");
        } else if (timeLeft > 0) {
            statusText.setString("PROCHAINE VAGUE DANS\n" + std::to_string(timeLeft) + " SECONDES");
        } else {
            statusText.setString("APPUYEZ SUR ESPACE\nPOUR DEMARRER");
        }
        
        window.draw(statusText);
    }
    
    void drawGameOver(sf::RenderWindow& window, sf::Font& font) {
        sf::RectangleShape overlay(sf::Vector2f(1680, 960));
        overlay.setFillColor(sf::Color(0, 0, 0, 180));
        window.draw(overlay);

        float pulse = sin(pulseTime * 3.0f) * 0.3f + 0.7f;
        
        sf::Text gameOverText("CHATEAU DETRUIT", font, 70);

        gameOverText.setPosition(380, 400);
        gameOverText.setFillColor(sf::Color(220, 20, 60, static_cast<sf::Uint8>(255 * pulse))); 
        gameOverText.setStyle(sf::Text::Bold);
        
        sf::Text glowText = gameOverText;
        glowText.setFillColor(sf::Color(220, 20, 60, static_cast<sf::Uint8>(100 * pulse)));
        glowText.setPosition(378, 398);
        window.draw(glowText);
        glowText.setPosition(382, 402);
        window.draw(glowText);
        
        window.draw(gameOverText);
  
        sf::Text subText("Toutes les guerres ne sont pas gagnables...", font, 24);
        subText.setPosition(450, 480);
        subText.setFillColor(sf::Color(150, 150, 150, static_cast<sf::Uint8>(200 * pulse)));
        window.draw(subText);

        sf::Text returnText("APPUYEZ SUR ECHAP POUR RETOURNER AU MENU", font, 20);
        returnText.setPosition(400, 520);
        returnText.setFillColor(sf::Color(46, 204, 113, static_cast<sf::Uint8>(200 * pulse)));
        window.draw(returnText);
    }
};
