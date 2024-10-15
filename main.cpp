#include <iostream>
#include <string>
#include <vector>
#include <conio.h>
#include <map>

// enum for the different states of the program
enum class State
{
    MainMenu,
    SubMenu,
};

// struct for the menu options
struct MenuOption
{
    std::string text;
    State nextState;
};

// struct for the menu
struct Menu
{
    std::vector<MenuOption> options;
    int currentSelection;
};

// mapping for the menu limits
std::map<State, Menu> menus;

State currentState = State::MainMenu;

std::vector<State> stateStack;

// -=== Functions ===-
void buildMenus()
{
    menus[State::MainMenu] = {
        {{"Option One", State::SubMenu},
         {"Option Two", State::SubMenu},
         {"Option Three", State::SubMenu}},
        0};

    menus[State::SubMenu] = {
        {{"Sub Option 1", State::MainMenu},
         {"Sub Option 2", State::MainMenu},
         {"Sub Option 2", State::MainMenu}},
        0};
}

void printCursor()
{
    std::cout << "> ";
}

void renderMenu(Menu &menu)
{
    for (int i = 0; i < menu.options.size(); i++)
    {
        if (i == menu.currentSelection)
        {
            printCursor();
        }
        std::cout << menu.options[i].text << std::endl;
    }
}

bool handleInput(bool &running)
{
    if (_kbhit())
    {
        int key = _getch();
        Menu &currentMenu = menus[currentState];
        State nextState; // Moved outside the switch statement

        switch (key)
        {
        case 72: // Up arrow
            currentMenu.currentSelection = (currentMenu.currentSelection - 1 + currentMenu.options.size()) % currentMenu.options.size();
            return true;
        case 80: // Down arrow
            currentMenu.currentSelection = (currentMenu.currentSelection + 1) % currentMenu.options.size();
            return true;
        case 13: // Enter key
            nextState = currentMenu.options[currentMenu.currentSelection].nextState;
            if (nextState == State::SubMenu && currentState == State::MainMenu)
            {
                stateStack.push_back(currentState);
                currentState = nextState;
            }
            else if (nextState == State::MainMenu && currentState == State::SubMenu)
            {
                if (!stateStack.empty())
                {
                    currentState = stateStack.back();
                    stateStack.pop_back();
                }
            }
            return true;
        case 27: // Esc key
            if (!stateStack.empty())
            {
                currentState = stateStack.back();
                stateStack.pop_back();
            }
            else
            {
                running = false;
            }
            return true;
        }
    }
    return false;
}

void render()
{
    // clear the console
    system("cls");
    // render
    renderMenu(menus[currentState]);
    // hide cursor
    std::cout << "\033[?25l";
}

int main()
{
    buildMenus();
    bool running = true;
    bool shouldRender = true;

    while (running)
    {
        if (shouldRender)
        {
            render();
        }
        // get keyboard input for up and down arrow keys
        shouldRender = handleInput(running);
    }
    // show cursor
    std::cout << "\033[?25h";
    return 0;
}