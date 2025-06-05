#include "accueil.hpp"
#include <iostream>

Menu::Menu(sf::RenderWindow& win) : window(win) {
    loadResources();
    setupButtons();
    setupSprites();
    setupText();

    sound.setBuffer(buffer);
    sound.setLoop(true);
    sound.play();
}

void Menu::loadResources() {
    if (!bgTexture.loadFromFile("C:/Users/hayou/Desktop/Dev.Logiciel/Projet/TowerDefence/img/bg.png") ||
        !bannerTexture.loadFromFile("C:/Users/hayou/Desktop/Dev.Logiciel/Projet/TowerDefence/img/titre.png") ||
        !flecheTexture.loadFromFile("C:/Users/hayou/Desktop/Dev.Logiciel/Projet/TowerDefence/img/retour.png") ||
        !texBtnStart.loadFromFile("C:/Users/hayou/Desktop/Dev.Logiciel/Projet/TowerDefence/img/commencer.png") ||
        !texBtnMusic.loadFromFile("C:/Users/hayou/Desktop/Dev.Logiciel/Projet/TowerDefence/img/musique.png") ||
        !texBtnHelp.loadFromFile("C:/Users/hayou/Desktop/Dev.Logiciel/Projet/TowerDefence/img/aide.png") ||
        !font.loadFromFile("C:/Users/hayou/Desktop/Dev.Logiciel/Projet/TowerDefence/fonts/fredoka-bold.ttf") ||
        !buffer.loadFromFile("C:/Users/hayou/Desktop/Dev.Logiciel/Projet/TowerDefence/sound/son.wav")) {
        std::cerr << "Erreur lors du chargement des ressources." << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Menu::setupButtons() {
    buttonStart.setSize({300, 70});
    buttonMusic.setSize({300, 70});
    buttonHelp.setSize({300, 70});

    buttonStart.setPosition(650, 250);
    buttonMusic.setPosition(650, 350);
    buttonHelp.setPosition(650, 450);

    buttonStart.setFillColor(sf::Color::Black);
    buttonMusic.setFillColor(sf::Color::Black);
    buttonHelp.setFillColor(sf::Color::Black);

    frame.setSize({600, 400});
    frame.setFillColor(sf::Color::Black);
    frame.setPosition(500, 250);

    flecheSprite.setTexture(flecheTexture);
    flecheSprite.setScale(0.1f, 0.1f);
    flecheSprite.setPosition(510, 260);
}

void Menu::setupSprites() {
    bgSprite.setTexture(bgTexture);
    bannerSprite.setTexture(bannerTexture);
    bannerSprite.setPosition(300, 20);

    spriteBtnStart.setTexture(texBtnStart);
    spriteBtnMusic.setTexture(texBtnMusic);
    spriteBtnHelp.setTexture(texBtnHelp);

    spriteBtnStart.setScale(0.3f, 0.3f);
    spriteBtnMusic.setScale(0.3f, 0.3f);
    spriteBtnHelp.setScale(0.2f, 0.2f);

    auto centerSprite = [](sf::Sprite& spr, const sf::RectangleShape& btn) {
        sf::FloatRect btnBounds = btn.getGlobalBounds();
        sf::FloatRect sprBounds = spr.getGlobalBounds();
        spr.setPosition(
            btnBounds.left + (btnBounds.width - sprBounds.width) / 2,
            btnBounds.top + (btnBounds.height - sprBounds.height) / 2
        );
    };

    centerSprite(spriteBtnStart, buttonStart);
    centerSprite(spriteBtnMusic, buttonMusic);
    centerSprite(spriteBtnHelp, buttonHelp);
}

void Menu::setupText() {
    title.setFont(font);
    title.setCharacterSize(60);
    title.setFillColor(sf::Color::Red);
    title.setPosition(500, 100);
}

void Menu::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mouse(event.mouseButton.x, event.mouseButton.y);

            if (showStartFrame || showMusicFrame || showHelpFrame) {
                if (flecheSprite.getGlobalBounds().contains(mouse)) {
                    showStartFrame = false;
                    showMusicFrame = false;
                    showHelpFrame = false;
                }
            } else {
                if (buttonStart.getGlobalBounds().contains(mouse))
                    showStartFrame = true;
                else if (buttonMusic.getGlobalBounds().contains(mouse))
                    showMusicFrame = true;
                else if (buttonHelp.getGlobalBounds().contains(mouse))
                    showHelpFrame = true;
            }
        }
    }
}

void Menu::update() {
    // Ajouter logique plus tard si besoin
}

void Menu::render() {
    window.clear();
    window.draw(bgSprite);
    window.draw(bannerSprite);

    if (!showStartFrame && !showMusicFrame && !showHelpFrame) {
        window.draw(buttonStart);
        window.draw(spriteBtnStart);
        window.draw(buttonMusic);
        window.draw(spriteBtnMusic);
        window.draw(buttonHelp);
        window.draw(spriteBtnHelp);
    } else {
        window.draw(frame);
        window.draw(flecheSprite);
        // Ajouter affichages spécifiques à chaque frame ici
    }

    window.draw(title);
    window.display();
}

void Menu::run() {
    while (window.isOpen()) {
        handleEvents();
        update();
        render();
    }
}
