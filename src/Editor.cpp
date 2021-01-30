#include "Editor.hpp"

Editor::Editor(sf::RenderWindow& window) : m_window(window) {
    m_view = window.getDefaultView();
    m_staticView = window.getDefaultView();

    m_activeTile.setSize(sf::Vector2f(spacing, spacing));
    m_activeTile.setFillColor(sf::Color(5, 200, 5, 140));
    m_activeTile.setPosition(getTilePos(sf::Mouse::getPosition(m_window)));

    m_grid.resize(window.getSize());
}

void Editor::move(sf::Vector2f v) {
    m_view.move(v);
}

void Editor::update() {
    auto pos = sf::Mouse::getPosition(m_window);

    m_activeTile.setPosition(getTilePos(pos));
}

sf::Vector2f Editor::getTilePos(sf::Vector2i mousePos) {
    int x = (mousePos.x / spacing) * spacing;
    int y = (mousePos.y / spacing) * spacing;

    return sf::Vector2f(x, y);
}

void Editor::draw() {
    // m_window.setView(m_staticView);
    m_window.draw(m_grid);

    m_window.setView(m_view);

    // draw components here

    // m_window.setView(m_staticView);
    m_window.draw(m_activeTile);
}

extern sf::Vector2u previousWindowSize;

void Editor::handleResize(sf::Event::SizeEvent e) {
    auto viewport = sf::FloatRect({0, 0}, sf::Vector2f(e.width, e.height));
    m_staticView.reset(viewport);

    int dw = (int)previousWindowSize.x - (int)m_window.getSize().x;
    int dh = (int)previousWindowSize.y - (int)m_window.getSize().y;

    auto center = m_view.getCenter();
    auto scale = m_view.getSize();

    scale.x /= previousWindowSize.x;
    scale.y /= previousWindowSize.y;

    center.x -= dw / 2;
    center.y -= dh / 2;
    
    m_view.reset(viewport);

    m_view.setCenter(center);

    auto size = m_view.getSize();
    size.x *= scale.x;
    size.y *= scale.y;

    m_view.setSize(size);

    m_window.setView(m_view);

    m_grid.resize(m_window.getSize());
}
