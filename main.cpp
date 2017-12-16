#include <iostream>
#include <SFML/Graphics.hpp>
#include "config.hpp"

void draw(bool *map, sf::RenderWindow *window) {
    for (int i(0); i < NUMBER_OF_CELLS_X * NUMBER_OF_CELLS_Y; i++) {
        if (map[i]) {
            int x((i % NUMBER_OF_CELLS_X) * CELL_SIZE), y((i / NUMBER_OF_CELLS_X) * CELL_SIZE);
            sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
            cell.setPosition(x, y);
            cell.setFillColor(sf::Color::White);
            (*window).draw(cell);
        }
    }

}

void mapcpy(bool *mapDest, bool *map) {
    for (int i = 0; i < NUMBER_OF_CELLS_X * NUMBER_OF_CELLS_Y; i++) {
        mapDest[i] = map[i];
    }
}

int countNeighbours(bool *map, int index) {
    int count(0);

    int y(index / NUMBER_OF_CELLS_X), yt((y - 1) < 0 ? NUMBER_OF_CELLS_Y - 1 : y - 1),
            yb((y + 1) >= NUMBER_OF_CELLS_Y ? 0 : y + 1),
            x(index % NUMBER_OF_CELLS_X), xl((x - 1) < 0 ? NUMBER_OF_CELLS_X - 1 : x - 1),
            xr((x + 1) >= NUMBER_OF_CELLS_X ? 0 : x + 1);

    if (map[xl + yt * NUMBER_OF_CELLS_X]) count++;
    if (map[x + yt * NUMBER_OF_CELLS_X]) count++;
    if (map[xr + yt * NUMBER_OF_CELLS_X]) count++;

    if (map[xl + y * NUMBER_OF_CELLS_X]) count++;
    if (map[xr + y * NUMBER_OF_CELLS_X]) count++;

    if (map[xl + yb * NUMBER_OF_CELLS_X]) count++;
    if (map[x + yb * NUMBER_OF_CELLS_X]) count++;
    if (map[xr + yb * NUMBER_OF_CELLS_X]) count++;

    return count;
}

void nextStep(bool *map, bool *mapDest) {
    for (int i = 0; i < NUMBER_OF_CELLS_X * NUMBER_OF_CELLS_Y; i++) {
        int neighboursCount(countNeighbours(map, i));
        if (!map[i]) {
            if (neighboursCount == 3) {
                mapDest[i] = true;
            }
        } else {
            if (neighboursCount >= 2 && neighboursCount <= 3) {
                mapDest[i] = true;
            }
        }
    }
}

void sleep(clock_t ms) {
    clock_t end(clock() + 1000 * ms);
    while (clock() <= end);
}

void updateMap(bool *map) {
    bool temp[NUMBER_OF_CELLS_X * NUMBER_OF_CELLS_Y] = {false};
    nextStep(map, temp);
    mapcpy(map, temp);
    sleep(REST_TIME);
}

int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Conway's game of life", sf::Style::Default, settings);

    bool map[NUMBER_OF_CELLS_X * NUMBER_OF_CELLS_Y] = {false};

    bool go(false);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
            go = true;

        if (go) {
            updateMap(map);
        } else {
            sf::Vector2i pos = sf::Mouse::getPosition(window);
            int x(pos.x / CELL_SIZE), y(pos.y / CELL_SIZE);
            if (x >= 0 && x < NUMBER_OF_CELLS_X && y >= 0 && y < NUMBER_OF_CELLS_Y) {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    map[x + y * NUMBER_OF_CELLS_X] = true;
                } else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                    map[x + y * NUMBER_OF_CELLS_X] = false;
                }
            }
        }

        draw(map, &window);
        window.display();
    }
    return 0;
}
