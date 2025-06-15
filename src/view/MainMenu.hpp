#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

class MainMenu {
private:
    sf::Font font;
    sf::Clock animationClock;
    float pulseTime = 0.0f;
    float titleFloat = 0.0f;
    
    enum MenuState {
        MAIN_MENU,
        INSTRUCTIONS,
        CREDITS
    };
    MenuState currentState = MAIN_MENU;
    
    int selectedOption = 0;
    int maxOptions = 4;
    
    struct NatureParticle {
        sf::Vector2f position;
        sf::Vector2f velocity;
        float life;
        float maxLife;
        sf::Color color;
        float size;
        float rotation;
        float rotationSpeed;
        int type;
    };
    std::vector<NatureParticle> particles;
    
    sf::RectangleShape backgroundGradient;
    std::vector<sf::CircleShape> floatingElements;
    
public:
    MainMenu() {
        initializeParticles();
        setupBackground();
    }
    
    void initializeParticles() {
        particles.clear();
        for (int i = 0; i < 80; ++i) {
            NatureParticle p;
            p.position = sf::Vector2f(
                rand() % 1680,
                rand() % 960
            );
            p.velocity = sf::Vector2f(
                (rand() % 40 - 20) / 100.0f,
                (rand() % 60 - 30) / 100.0f
            );
            p.life = p.maxLife = 6.0f + (rand() % 600) / 100.0f;
            p.size = 2.0f + (rand() % 400) / 100.0f;
            p.rotation = 0.0f;
            p.rotationSpeed = (rand() % 100 - 50) / 100.0f;
            p.type = rand() % 3;
            
            switch(p.type) {
                case 0: // Feuilles
                    p.color = sf::Color(46 + rand() % 50, 174 + rand() % 30, 96, 120 + rand() % 80);
                    break;
                case 1: // Pollen
                    p.color = sf::Color(241, 196 + rand() % 30, 15, 100 + rand() % 100);
                    break;
                case 2: // Pétales
                    p.color = sf::Color(255, 182 + rand() % 50, 193, 80 + rand() % 120);
                    break;
            }
            particles.push_back(p);
        }
    }
    
    void setupBackground() {
        backgroundGradient.setSize(sf::Vector2f(1680, 960));
        backgroundGradient.setPosition(0, 0);
        
        floatingElements.clear();
        for (int i = 0; i < 15; ++i) {
            sf::CircleShape element(20 + rand() % 40);
            element.setPosition(rand() % 1680, rand() % 960);
            element.setFillColor(sf::Color(39, 174, 96, 30 + rand() % 40));
            floatingElements.push_back(element);
        }
    }
    
    void update(float deltaTime) {
        pulseTime += deltaTime;
        titleFloat = sin(pulseTime * 1.5f) * 10.0f;
        
        for (auto& particle : particles) {
            particle.velocity.x += sin(pulseTime * 1.2f + particle.position.y * 0.01f) * 0.05f;
            particle.position += particle.velocity * deltaTime * 40.0f;
            particle.rotation += particle.rotationSpeed * deltaTime * 50.0f;
            particle.life -= deltaTime;
            
            if (particle.life <= 0 || 
                particle.position.x < -50 || particle.position.x > 1730 ||
                particle.position.y < -50 || particle.position.y > 1010) {
                
                int edge = rand() % 4;
                switch(edge) {
                    case 0: // Haut
                        particle.position = sf::Vector2f(rand() % 1680, -30);
                        particle.velocity.y = abs(particle.velocity.y);
                        break;
                    case 1: // Droite
                        particle.position = sf::Vector2f(1710, rand() % 960);
                        particle.velocity.x = -abs(particle.velocity.x);
                        break;
                    case 2: // Bas
                        particle.position = sf::Vector2f(rand() % 1680, 990);
                        particle.velocity.y = -abs(particle.velocity.y);
                        break;
                    case 3: // Gauche
                        particle.position = sf::Vector2f(-30, rand() % 960);
                        particle.velocity.x = abs(particle.velocity.x);
                        break;
                }
                particle.life = particle.maxLife;
            }
            
            float alpha = (particle.life / particle.maxLife) * 200;
            particle.color.a = static_cast<sf::Uint8>(alpha);
        }
        
        for (auto& element : floatingElements) {
            sf::Vector2f pos = element.getPosition();
            pos.y += sin(pulseTime * 0.8f + pos.x * 0.005f) * 0.3f;
            element.setPosition(pos);
        }
    }
    
    void handleInput(sf::Event& event) {
        if (event.type == sf::Event::KeyPressed) {
            switch (currentState) {
                case MAIN_MENU:
                    handleMainMenuInput(event);
                    break;
                case INSTRUCTIONS:
                case CREDITS:
                    if (event.key.code == sf::Keyboard::Escape || 
                        event.key.code == sf::Keyboard::Enter) {
                        currentState = MAIN_MENU;
                    }
                    break;
            }
        }
    }
    
    void handleMainMenuInput(sf::Event& event) {
        if (event.key.code == sf::Keyboard::Up) {
            selectedOption = (selectedOption - 1 + maxOptions) % maxOptions;
        } else if (event.key.code == sf::Keyboard::Down) {
            selectedOption = (selectedOption + 1) % maxOptions;
        } else if (event.key.code == sf::Keyboard::Enter) {
            switch (selectedOption) {
                case 0:
                    break;
                case 1:
                    currentState = INSTRUCTIONS;
                    break;
                case 2:
                    currentState = CREDITS;
                    break;
                case 3:
                    break;
            }
        }
    }
    
    bool shouldStartGame() const {
        return selectedOption == 0 && currentState == MAIN_MENU;
    }
    
    bool shouldExit() const {
        return selectedOption == 3 && currentState == MAIN_MENU;
    }
    
    void draw(sf::RenderWindow& window, sf::Font& gameFont) {
        float pulse = sin(pulseTime * 0.8f) * 0.1f + 0.9f;
        backgroundGradient.setFillColor(sf::Color(
            static_cast<sf::Uint8>(20 * pulse),
            static_cast<sf::Uint8>(40 * pulse),
            static_cast<sf::Uint8>(25 * pulse),
            255
        ));
        window.draw(backgroundGradient);
        
        for (const auto& element : floatingElements) {
            window.draw(element);
        }
        
        for (const auto& particle : particles) {
            sf::CircleShape shape(particle.size);
            shape.setPosition(particle.position);
            shape.setFillColor(particle.color);
            shape.setOrigin(particle.size, particle.size);
            shape.setRotation(particle.rotation);
            window.draw(shape);
        }
        
        switch (currentState) {
            case MAIN_MENU:
                drawMainMenu(window, gameFont);
                break;
            case INSTRUCTIONS:
                drawInstructions(window, gameFont);
                break;
            case CREDITS:
                drawCredits(window, gameFont);
                break;
        }
    }
    
    void drawMainMenu(sf::RenderWindow& window, sf::Font& font) {
        sf::Text title("TOWER DEFENSE", font, 120);
        title.setPosition(840 - title.getGlobalBounds().width / 2, 150 + titleFloat);
        title.setFillColor(sf::Color(46, 204, 113));
        title.setStyle(sf::Text::Bold);
        
        sf::Text titleGlow = title;
        titleGlow.setFillColor(sf::Color(46, 204, 113, 60));
        titleGlow.setPosition(title.getPosition().x - 2, title.getPosition().y - 2);
        window.draw(titleGlow);
        titleGlow.setPosition(title.getPosition().x + 2, title.getPosition().y + 2);
        window.draw(titleGlow);
        window.draw(title);
        
        sf::Text subtitle("Protege ton chateau de l'invasion!", font, 28);
        subtitle.setPosition(840 - subtitle.getGlobalBounds().width / 2, 280);
        subtitle.setFillColor(sf::Color(130, 224, 170));
        window.draw(subtitle);
        
        std::vector<std::string> menuOptions = {
            "JOUER",
            "INSTRUCTIONS", 
            "CREDITS",
            "QUITTER"
        };
        
        for (int i = 0; i < menuOptions.size(); ++i) {
            sf::Text option(menuOptions[i], font, 36);
            option.setPosition(840 - option.getGlobalBounds().width / 2, 400 + i * 80);
            
            if (i == selectedOption) {
                float selectPulse = sin(pulseTime * 4.0f) * 0.3f + 0.7f;
                option.setFillColor(sf::Color(46, 204, 113, static_cast<sf::Uint8>(255 * selectPulse)));
                option.setStyle(sf::Text::Bold);
                
                sf::Text arrow(">", font, 36);
                arrow.setPosition(option.getPosition().x - 60, option.getPosition().y);
                arrow.setFillColor(sf::Color(46, 204, 113));
                window.draw(arrow);
                
                sf::Text arrow2("<", font, 36);
                arrow2.setPosition(option.getPosition().x + option.getGlobalBounds().width + 20, option.getPosition().y);
                arrow2.setFillColor(sf::Color(46, 204, 113));
                window.draw(arrow2);
                
                sf::RectangleShape highlight(sf::Vector2f(option.getGlobalBounds().width + 40, 50));
                highlight.setPosition(option.getPosition().x - 20, option.getPosition().y);
                highlight.setFillColor(sf::Color(46, 204, 113, static_cast<sf::Uint8>(30 * selectPulse)));
                highlight.setOutlineThickness(2);
                highlight.setOutlineColor(sf::Color(46, 204, 113, static_cast<sf::Uint8>(100 * selectPulse)));
                window.draw(highlight);
            } else {
                option.setFillColor(sf::Color(150, 200, 150));
            }
            
            window.draw(option);
        }
        
        sf::Text controls("Utiliser -> ou <- pour naviguer, ENTRER pour confirmer", font, 20);
        controls.setPosition(840 - controls.getGlobalBounds().width / 2, 800);
        controls.setFillColor(sf::Color(100, 150, 100));
        window.draw(controls);
    }
    
    void drawInstructions(sf::RenderWindow& window, sf::Font& font) {
        // Titre
        sf::Text title("Comment Jouer", font, 60);
        title.setPosition(840 - title.getGlobalBounds().width / 2, 50);
        title.setFillColor(sf::Color(46, 204, 113));
        title.setStyle(sf::Text::Bold);
        window.draw(title);
        
        // Panel d'instructions
        sf::RectangleShape panel(sf::Vector2f(1200, 700));
        panel.setPosition(240, 150);
        panel.setFillColor(sf::Color(20, 50, 30, 200));
        panel.setOutlineThickness(3);
        panel.setOutlineColor(sf::Color(46, 204, 113, 150));
        window.draw(panel);
        
        // Texte d'instructions
        std::vector<std::string> instructions = {
            "OBJECTIVE:",
            "   Defend ton chateau de l'invasion !",
            "",
            "CONTROLES:",
            "    Clique sur les cases vide pour placer un tour",
            "    Clique droit pour deselectionner",
            "    Espace pour demarrer l'invasion",
            "    U pour ameliorer ta tour",
            "",
            "RESSOURCES:",
            "    Gagne de l'argent quand tu tue un enemie",
            "    Utilise cette argents pour ameliorer et placer tes tours",
            "",
            "VIES:",
            "    Tu perd une vie a chaque fois que un ennemie atteind la fin",
            "    La partie est fini si tu n'a plus de vies",
            "",
        };
        
        float yPos = 180;
        for (const auto& line : instructions) {
            sf::Text text(line, font, 24);
            text.setPosition(280, yPos);
            
            if (line.find("OBJECTIVE") != std::string::npos || 
                line.find("CONTROLES") != std::string::npos ||
                line.find("RESSOURCES") != std::string::npos ||
                line.find("VIES") != std::string::npos) {
                text.setFillColor(sf::Color(46, 204, 113));
                text.setStyle(sf::Text::Bold);
            } else {
                text.setFillColor(sf::Color(220, 255, 220));
            }
            
            window.draw(text);
            yPos += 35;
        }
        
        // Instruction de retour
        sf::Text backText("ESC ou ENTRER pour retourner au menu", font, 24);
        backText.setPosition(840 - backText.getGlobalBounds().width / 2, 880);
        backText.setFillColor(sf::Color(130, 224, 170));
        window.draw(backText);
    }
    
    void drawCredits(sf::RenderWindow& window, sf::Font& font) {
        // Titre
        sf::Text title("CREDITS", font, 60);
        title.setPosition(840 - title.getGlobalBounds().width / 2, 50);
        title.setFillColor(sf::Color(46, 204, 113));
        title.setStyle(sf::Text::Bold);
        window.draw(title);
        
        // Panel de crédits
        sf::RectangleShape panel(sf::Vector2f(800, 600));
        panel.setPosition(440, 200);
        panel.setFillColor(sf::Color(20, 50, 30, 200));
        panel.setOutlineThickness(3);
        panel.setOutlineColor(sf::Color(46, 204, 113, 150));
        window.draw(panel);
        
        std::vector<std::string> credits = {
            "TOWER DEFENSE",
            "",
            "Devellopeurs:",
            "   Abdel, Rachid, Zaky et Djacem",
            "",
            "Interface:",
            "   Abdel et Rachid",
            "",
            "Cree avec:",
            "   SFML",
            "   C++",
            "",
            "Version 1.0",
            "   Cree avec Amour"
        };
        
        float yPos = 240;
        for (const auto& line : credits) {
            sf::Text text(line, font, 28);
            text.setPosition(840 - text.getGlobalBounds().width / 2, yPos);
            
            if (line == "GARDEN DEFENSE") {
                text.setFillColor(sf::Color(46, 204, 113));
                text.setStyle(sf::Text::Bold);
                text.setCharacterSize(40);
            } else if (line.find("Devellopeurs") != std::string::npos || 
                      line.find("Interface") != std::string::npos ||
                      line.find("Cree avec") != std::string::npos ||
                      line.find("Version") != std::string::npos) {
                text.setFillColor(sf::Color(130, 224, 170));
                text.setStyle(sf::Text::Bold);
            } else {
                text.setFillColor(sf::Color(220, 255, 220));
            }
            
            window.draw(text);
            yPos += 35;
        }
        
        sf::Text backText("ESC ou ENTRER pour retourner au menu", font, 24);
        backText.setPosition(840 - backText.getGlobalBounds().width / 2, 880);
        backText.setFillColor(sf::Color(130, 224, 170));
        window.draw(backText);
    }
};