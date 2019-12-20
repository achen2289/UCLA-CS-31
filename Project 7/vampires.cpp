//
//  vampires.cpp
//  Project_7
//
//  Created by ALEX CHEN (: on 12/3/19.
//  Copyright © 2019 cs. All rights reserved.
//

#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
#include <cctype>
using namespace std;

///////////////////////////////////////////////////////////////////////////
// Manifest constants
///////////////////////////////////////////////////////////////////////////

const int MAXROWS = 20;                // max number of rows in the arena
const int MAXCOLS = 20;                // max number of columns in the arena
const int MAXVAMPIRES = 100;           // max number of vampires allowed

const int NORTH = 0;
const int EAST  = 1;
const int SOUTH = 2;
const int WEST  = 3;
const int NUMDIRS = 4;

const int EMPTY      = 0;
const int HAS_POISON = 1;

///////////////////////////////////////////////////////////////////////////
// Type definitions
///////////////////////////////////////////////////////////////////////////

class Arena;  // This is needed to let the compiler know that Arena is a
              // type name, since it's mentioned in the Vampire declaration.

class Vampire
{
  public:
      // Constructor
    Vampire(Arena* ap, int r, int c);

      // Accessors
    int  row() const;
    int  col() const;
    bool isDead() const;

      // Mutators
    void move();

  private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
      // FINISHED: You'll probably find that a vampire object needs additional
      // data members to support your implementation of the behavior affected
      // by poisoned blood vials.
    int    m_onVial; // number of vials a vampire has stepped on
    int    m_turn; // which "turn" vampire is on, if stepped on vial, to allow or prevent movement
    bool   m_dead; // to determine if vampire is dead or not
};

class Player
{
  public:
      // Constructor
    Player(Arena* ap, int r, int c);

      // Accessors
    int  row() const;
    int  col() const;
    bool isDead() const;

      // Mutators
    string dropPoisonVial();
    string move(int dir);
    void   setDead();

  private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    bool   m_dead;
};

class Arena
{
  public:
      // Constructor/destructor
    Arena(int nRows, int nCols);
    ~Arena();

      // Accessors
    int     rows() const;
    int     cols() const;
    Player* player() const;
    int     vampireCount() const;
    int     getCellStatus(int r, int c) const;
    int     numberOfVampiresAt(int r, int c) const;
    void    display(string msg) const;

      // Mutators
    void setCellStatus(int r, int c, int status);
    bool addVampire(int r, int c);
    bool addPlayer(int r, int c);
    void moveVampires();

  private:
    int      m_grid[MAXROWS][MAXCOLS];
    int      m_rows;
    int      m_cols;
    Player*  m_player;
    Vampire* m_vampires[MAXVAMPIRES];
    int      m_nVampires;
    int      m_turns;

      // Helper functions
    void checkPos(int r, int c, string functionName) const;
    bool isPosInBounds(int r, int c) const;
};

class Game
{
  public:
      // Constructor/destructor
    Game(int rows, int cols, int nVampires);
    ~Game();

      // Mutators
    void play();

  private:
    Arena* m_arena;

      // Helper functions
    string takePlayerTurn();
};

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function declarations
///////////////////////////////////////////////////////////////////////////

int randInt(int lowest, int highest);
bool decodeDirection(char ch, int& dir);
bool attemptMove(const Arena& a, int dir, int& r, int& c);
bool recommendMove(const Arena& a, int r, int c, int& bestDir);
void clearScreen();

///////////////////////////////////////////////////////////////////////////
//  Vampire implementation
///////////////////////////////////////////////////////////////////////////

Vampire::Vampire(Arena* ap, int r, int c)
{
    if (ap == nullptr)
    {
        cout << "***** A vampire must be created in some Arena!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > ap->rows()  ||  c < 1  ||  c > ap->cols())
    {
        cout << "***** Vampire created with invalid coordinates (" << r << ","
             << c << ")!" << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
    m_onVial = 0;
    m_turn = 0;
    m_dead = false;
}

int Vampire::row() const
{
    return m_row;
}

int Vampire::col() const
{
      // FINISHED: TRIVIAL:  Return what column the Vampire is at

    return m_col;
}

bool Vampire::isDead() const
{
      // FINISHED: Return whether the Vampire is dead

    return m_dead;
}

void Vampire::move()
{
      // FINISHED:
      //   Return without moving if the vampire has drunk one vial of
      //   poisoned blood (so is supposed to move only every other turn) and
      //   this is a turn it does not move.

      //   Otherwise, attempt to move in a random direction; if can't
      //   move, don't move.  If it lands on a poisoned blood vial, drink all
      //   the blood in the vial and remove it from the game (so it is no
      //   longer on that grid point).
    
    // random direction
    int dir = randInt(0, NUMDIRS-1);
    // if vampire has stepped on vial, m_turn incremented
    if (m_onVial > 0)
    {
        m_turn++;
    }
    // if m_turn is even, allow attemptMove to be called
    if (m_turn % 2 == 0)
    {
        if (!(attemptMove(*m_arena, dir, m_row, m_col)))
        {
            return;
        }
    }
    // if current cell has poison, remove poison and increment onVial
    if (m_arena->getCellStatus(m_row, m_col) == HAS_POISON)
    {
        m_arena->setCellStatus(m_row, m_col, EMPTY);
        m_onVial++;
    }
    // if onVial is greater than one, vampire is dead
    if (m_onVial > 1)
    {
        m_dead = true;
    }
}

///////////////////////////////////////////////////////////////////////////
//  Player implementation
///////////////////////////////////////////////////////////////////////////

Player::Player(Arena* ap, int r, int c)
{
    if (ap == nullptr)
    {
        cout << "***** The player must be created in some Arena!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > ap->rows()  ||  c < 1  ||  c > ap->cols())
    {
        cout << "**** Player created with invalid coordinates (" << r
             << "," << c << ")!" << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
    m_dead = false;
}

int Player::row() const
{
      // FINISHED: TRIVIAL:  Return what row the Player is at

    return m_row;
}

int Player::col() const
{
      // FINISHED: TRIVIAL:  Return what column the Player is at
    
    return m_col;
}

string Player::dropPoisonVial()
{
    if (m_arena->getCellStatus(m_row, m_col) == HAS_POISON)
        return "There's already a poisoned blood vial at this spot.";
    m_arena->setCellStatus(m_row, m_col, HAS_POISON);
    return "A poisoned blood vial has been dropped.";
}

string Player::move(int dir)
{
      // FINISHED:  Attempt to move the player one step in the indicated
      //        direction.  If this fails,
      //        return "Player couldn't move; player stands."
      //        A player who moves onto a vampire dies, and this
      //        returns "Player walked into a vampire and died."
      //        Otherwise, return one of "Player moved north.",
      //        "Player moved east.", "Player moved south.", or
      //        "Player moved west."
    
    // if player can move in direction, allow that move
    if (dir == 0 && attemptMove(*m_arena, dir, m_row, m_col))
    {
        // if vampires are on that new cell, player dies
        if (m_arena->numberOfVampiresAt(m_row, m_col) > 0)
        {
            setDead();
            return "Player walked into a vampire and died.";
        }
        return "Player moved north.";
    }
    else if (dir == 1 && attemptMove(*m_arena, dir, m_row, m_col))
    {
        if (m_arena->numberOfVampiresAt(m_row, m_col) > 0)
        {
            setDead();
            return "Player walked into a vampire and died.";
        }
        return "Player moved east.";
    }
    else if (dir == 2 && attemptMove(*m_arena, dir, m_row, m_col))
    {
        if (m_arena->numberOfVampiresAt(m_row, m_col) > 0)
        {
            setDead();
            return "Player walked into a vampire and died.";
        }
        return "Player moved south.";
    }
    else if (dir == 3 && attemptMove(*m_arena, dir, m_row, m_col))
    {
        if (m_arena->numberOfVampiresAt(m_row, m_col) > 0)
        {
            setDead();
            return "Player walked into a vampire and died.";
        }
        return "Player moved west.";
    }
    return "Player couldn't move; player stands.";
}

bool Player::isDead() const
{
      // FINISHED: Return whether the Player is dead

    return m_dead;
}

void Player::setDead()
{
    m_dead = true;
}

///////////////////////////////////////////////////////////////////////////
//  Arena implementation
///////////////////////////////////////////////////////////////////////////

Arena::Arena(int nRows, int nCols)
{
    if (nRows <= 0  ||  nCols <= 0  ||  nRows > MAXROWS  ||  nCols > MAXCOLS)
    {
        cout << "***** Arena created with invalid size " << nRows << " by "
             << nCols << "!" << endl;
        exit(1);
    }
    m_rows = nRows;
    m_cols = nCols;
    m_player = nullptr;
    m_nVampires = 0;
    m_turns = 0;
    for (int r = 1; r <= m_rows; r++)
        for (int c = 1; c <= m_cols; c++)
            setCellStatus(r, c, EMPTY);
}

Arena::~Arena()
{
    // FINISHED:  Deallocate the player and all remaining dynamically allocated
    //        vampires.
    for (int a=0; a<m_nVampires; a++)
    {
        delete m_vampires[a];
    }
    delete m_player;
}

int Arena::rows() const
{
      // FINISHED: TRIVIAL:  Return the number of rows in the arena

    return m_rows;
}

int Arena::cols() const
{
      // FINISHED: TRIVIAL:  Return the number of columns in the arena

    return m_cols;
}

Player* Arena::player() const
{
    return m_player;
}

int Arena::vampireCount() const
{
      // FINISHED: TRIVIAL:  Return the number of vampires in the arena
 
    return m_nVampires;
}

int Arena::getCellStatus(int r, int c) const
{
    checkPos(r, c, "Arena::getCellStatus");
    return m_grid[r-1][c-1];
}

int Arena::numberOfVampiresAt(int r, int c) const
{
      // FINISHED:  Return the number of vampires at row r, column c

    int num = 0;
    for (int a=0; a<m_nVampires; a++)
    {
        if (m_vampires[a]->row() == r && m_vampires[a]->col() == c)
        {
            num++;
        }
    }
    return num;
}

void Arena::display(string msg) const
{
    char displayGrid[MAXROWS][MAXCOLS];
    int r, c;
    
      // Fill displayGrid with dots (empty) and stars (poisoned blood vials)
    for (r = 1; r <= rows(); r++)
        for (c = 1; c <= cols(); c++)
            displayGrid[r-1][c-1] = (getCellStatus(r,c) == EMPTY ? '.' : '*');

      // Indicate each vampire's position
      // FINISHED:  If one vampire is at some grid point, set the displayGrid char
      //        to 'V'.  If it's 2 though 8, set it to '2' through '8'.
      //        For 9 or more, set it to '9'.

    for (r = 1; r <= rows(); r++)
    {
        for (c = 1; c <= cols(); c++)
        {
            if (! (m_player->row() == r && m_player->col() == c))
            {
                if (numberOfVampiresAt(r, c) == 1)
                {
                    displayGrid[r-1][c-1] = 'V';
                }
                if (numberOfVampiresAt(r, c) > 1)
                {
                    displayGrid[r-1][c-1] = numberOfVampiresAt(r, c) + 48;
                }
                if (numberOfVampiresAt(r, c) >= 9)
                {
                    displayGrid[r-1][c-1] = '9';
                }
            }
        }
    }
    
      // Indicate player's position
    if (m_player != nullptr)
        displayGrid[m_player->row()-1][m_player->col()-1] = (m_player->isDead() ? 'X' : '@');

      // Draw the grid
    clearScreen();
    for (r = 1; r <= rows(); r++)
    {
        for (c = 1; c <= cols(); c++)
        {
            cout << displayGrid[r-1][c-1];
        }
        cout << endl;
    }
    cout << endl;

      // Write message, vampire, and player info
    if (msg != "")
        cout << msg << endl;
    cout << "There are " << vampireCount() << " vampires remaining." << endl;
    if (m_player == nullptr)
        cout << "There is no player!" << endl;
    else if (m_player->isDead())
        cout << "The player is dead." << endl;
    cout << m_turns << " turns have been taken." << endl;
}

void Arena::setCellStatus(int r, int c, int status)
{
    checkPos(r, c, "Arena::setCellStatus");
    m_grid[r-1][c-1] = status;
}

bool Arena::addVampire(int r, int c)
{
    if (! isPosInBounds(r, c))
        return false;

      // Don't add a vampire on a spot with a poisoned blood vial
    if (getCellStatus(r, c) != EMPTY)
        return false;

      // Don't add a vampire on a spot with a player
    if (m_player != nullptr  &&  m_player->row() == r  &&  m_player->col() == c)
        return false;

      // If there are MAXVAMPIRES existing vampires, return false.  Otherwise,
      // dynamically allocate a new vampire at coordinates (r,c).  Save the
      // pointer to newly allocated vampire and return true.

      // FINISHED:  Implement this.
    
    if (m_nVampires == MAXVAMPIRES)
        return false;
    
    m_vampires[m_nVampires] = new Vampire(this, r, c);
    m_nVampires++;
    
    return true;  // This implementation compiles, but is incorrect.
}

bool Arena::addPlayer(int r, int c)
{
    if (! isPosInBounds(r, c))
        return false;

      // Don't add a player if one already exists
    if (m_player != nullptr)
        return false;

      // Don't add a player on a spot with a vampire
    if (numberOfVampiresAt(r, c) > 0)
        return false;

    m_player = new Player(this, r, c);
    return true;
}

void Arena::moveVampires()
{
      // Move all vampires
      // FINISHED:  Move each vampire.  Mark the player as dead if necessary.
      //        Deallocate any dead dynamically allocated vampire.

      // Another turn has been taken
    for (int a=0; a<m_nVampires; a++)
    {
        m_vampires[a]->move();
        if (m_vampires[a]->row() == m_player->row() && m_vampires[a]->col() == m_player->col())
        {
            m_player->setDead();
        }
        if (m_vampires[a]->isDead())
        {
            delete m_vampires[a];
            for (int b=a; b<m_nVampires-1; b++)
            {
                m_vampires[b] = m_vampires[b+1];
            }
            m_nVampires--;
            a--;
        }
    }
    m_turns++;
}

bool Arena::isPosInBounds(int r, int c) const
{
    return (r >= 1  &&  r <= m_rows  &&  c >= 1  &&  c <= m_cols);
}

void Arena::checkPos(int r, int c, string functionName) const
{
    if (r < 1  ||  r > m_rows  ||  c < 1  ||  c > m_cols)
    {
        cout << "***** " << "Invalid arena position (" << r << ","
             << c << ") in call to " << functionName << endl;
        exit(1);
    }
}

///////////////////////////////////////////////////////////////////////////
//  Game implementation
///////////////////////////////////////////////////////////////////////////

Game::Game(int rows, int cols, int nVampires)
{
    if (nVampires < 0)
    {
        cout << "***** Cannot create Game with negative number of vampires!" << endl;
        exit(1);
    }
    if (nVampires > MAXVAMPIRES)
    {
        cout << "***** Trying to create Game with " << nVampires
             << " vampires; only " << MAXVAMPIRES << " are allowed!" << endl;
        exit(1);
    }
    int nEmpty = rows * cols - nVampires - 1;  // 1 for Player
    if (nEmpty < 0)
    {
        cout << "***** Game created with a " << rows << " by "
             << cols << " arena, which is too small too hold a player and "
             << nVampires << " vampires!" << endl;
        exit(1);
    }

      // Create arena
    m_arena = new Arena(rows, cols);

      // Add player
    int rPlayer;
    int cPlayer;
    do
    {
        rPlayer = randInt(1, rows);
        cPlayer = randInt(1, cols);
    } while (m_arena->getCellStatus(rPlayer, cPlayer) != EMPTY);
    m_arena->addPlayer(rPlayer, cPlayer);

      // Populate with vampires
    while (nVampires > 0)
    {
        int r = randInt(1, rows);
        int c = randInt(1, cols);
        if (r == rPlayer && c == cPlayer)
            continue;
        m_arena->addVampire(r, c);
        nVampires--;
    }
}

Game::~Game()
{
    delete m_arena;
}

string Game::takePlayerTurn()
{
    for (;;)
    {
        cout << "Your move (n/e/s/w/x or nothing): ";
        string playerMove;
        getline(cin, playerMove);

        Player* player = m_arena->player();
        int dir;

        if (playerMove.size() == 0)
        {
            if (recommendMove(*m_arena, player->row(), player->col(), dir))
                return player->move(dir);
            else
                return player->dropPoisonVial();
        }
        else if (playerMove.size() == 1)
        {
            if (tolower(playerMove[0]) == 'x')
                return player->dropPoisonVial();
            else if (decodeDirection(playerMove[0], dir))
                return player->move(dir);
        }
        cout << "Player move must be nothing, or 1 character n/e/s/w/x." << endl;
    }
}

void Game::play()
{
    m_arena->display("");
    Player* player = m_arena->player();
    if (player == nullptr)
        return;
    while ( ! player->isDead()  &&  m_arena->vampireCount() > 0)
    {
        string msg = takePlayerTurn();
        m_arena->display(msg);
        if (player->isDead())
            break;
        m_arena->moveVampires();
        m_arena->display(msg);
    }
    if (player->isDead())
        cout << "You lose." << endl;
    else
        cout << "You win." << endl;
}

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function implementation
///////////////////////////////////////////////////////////////////////////

  // Return a uniformly distributed random int from lowest to highest, inclusive
int randInt(int lowest, int highest)
{
    if (highest < lowest)
        swap(highest, lowest);
    static random_device rd;
    static default_random_engine generator(rd());
    uniform_int_distribution<> distro(lowest, highest);
    return distro(generator);
}

bool decodeDirection(char ch, int& dir)
{
    switch (tolower(ch))
    {
      default:  return false;
      case 'n': dir = NORTH; break;
      case 'e': dir = EAST;  break;
      case 's': dir = SOUTH; break;
      case 'w': dir = WEST;  break;
    }
    return true;
}

  // Return false without changing anything if moving one step from (r,c)
  // in the indicated direction would run off the edge of the arena.
  // Otherwise, update r and c to the position resulting from the move and
  // return true.
bool attemptMove(const Arena& a, int dir, int& r, int& c)
{
      // FINISHED:  Implement this function
    switch (dir)
    {
        case 0:
        {
            if (r-1 >= 1)
            {
                r--;
                break;
            }
            else
                return false;
        }
        case 1:
        {
            if (c+1 <= a.cols())
            {
                c++;
                break;
            }
            else
                return false;
        }
        case 2:
        {
            if (r+1 <= a.rows())
            {
                r++;
                break;
            }
            else
                return false;
        }
        case 3:
        {
            if (c-1 >= 1)
            {
                c--;
                break;
            }
            else
                return false;
        }
    }
    return true;
}

  // Recommend a move for a player at (r,c):  A false return means the
  // recommendation is that the player should drop a poisoned blood vial and
  // not move; otherwise, this function sets bestDir to the recommended
  // direction to move and returns true.
bool recommendMove(const Arena& a, int r, int c, int& bestDir)
{
      // FINISHED:  Implement this function

    // randomizer integer to randomize priority of random movement
    int randomizer = randInt(0, NUMDIRS-1);
    // north, west, south, and east variables declared and
    // initialized to gauge surrouding squares
    int north = a.numberOfVampiresAt(r-1, c);
    int west = a.numberOfVampiresAt(r, c-1);
    int south = a.numberOfVampiresAt(r+1, c);
    int east = a.numberOfVampiresAt(r, c+1);
    // following variables declared to gauge surrounding
    // squares of surrounding squares
    int northnorth = a.numberOfVampiresAt(r-2, c);
    int northwest = a.numberOfVampiresAt(r-1, c-1);
    int northeast = a.numberOfVampiresAt(r-1, c+1);
    int westnorth = a.numberOfVampiresAt(r-1, c-1);
    int westsouth = a.numberOfVampiresAt(r+1, c-1);
    int westwest = a.numberOfVampiresAt(r, c-1);
    int southsouth = a.numberOfVampiresAt(r+2, c);
    int southeast = a.numberOfVampiresAt(r+1, c+1);
    int southwest = a.numberOfVampiresAt(r+1, c-1);
    int eastnorth = a.numberOfVampiresAt(r-1, c+1);
    int eastsouth = a.numberOfVampiresAt(r+1, c+1);
    int easteast = a.numberOfVampiresAt(r-1, c+1);
    // if no vampires surrounding player and no poison on square
    // player will place down square
    if (north + west + south + east == 0)
    {
        if (a.getCellStatus(r, c) != HAS_POISON)
        {
            return false;
        }
    }
    // immediate movement north, west, south, or east is randomized
    // and only occurs if no vampires can potentially move there
    // and if movement occurs on grid
    switch (randomizer)
    {
        case 0:
        {
            if (north == 0 && northnorth == 0 && northwest == 0 && northeast == 0)
            {
                bestDir = NORTH;
                if (r-1 >= 1)
                {
                    return true;
                }
            }
        }
        case 1:
        {
            if (west == 0 && westnorth == 0 && westsouth == 0 && westwest == 0)
            {
                bestDir = WEST;
                if (c-1 >= 1)
                {
                    return true;
                }
            }
        }
        case 2:
        {
            if (south == 0 && southsouth == 0 && southeast == 0 && southwest == 0)
            {
                bestDir = SOUTH;
                if (r+1 <= a.rows())
                {
                    return true;
                }
            }
        }
        case 3:
        {
            if (east == 0 && eastnorth == 0 && eastsouth == 0 && easteast == 0)
            {
                bestDir = EAST;
                if (c+1 <= a.cols())
                {
                    return true;
                }
            }
        }
    }
    // if number of vampires in a certain direction is greater than in
    // all other directions, player will move in opposite direction
    if (north > west && north > south && north > east)
    {
        bestDir = SOUTH;
        if (r+1 <= a.rows())
        {
            return true;
        }
    }
    if (west > north && west > south && west > east)
    {
        bestDir = EAST;
        if (c+1 <= a.cols())
        {
            return true;
        }
    }
    if (south > north && south > west && south > east)
    {
        bestDir = NORTH;
        if (r-1 >= 1)
        {
            return true;
        }
    }
    if (east > north && east > south && east > west)
    {
        bestDir = WEST;
        if (c-1 >= 1)
        {
            return true;
        }
    }
    return false;

      // Your replacement implementation should do something intelligent.
      // You don't have to be any smarter than the following, although
      // you can if you want to be:  If staying put runs the risk of a
      // vampire possibly moving onto the player's location when the vampires
      // move, yet moving in a particular direction puts the player in a
      // position that is safe when the vampires move, then the chosen
      // action is to move to a safer location.  Similarly, if staying put
      // is safe, but moving in certain directions puts the player in
      // danger of dying when the vampires move, then the chosen action should
      // not be to move in one of the dangerous directions; instead, the player
      // should stay put or move to another safe position.  In general, a
      // position that may be moved to by many vampires is more dangerous than
      // one that may be moved to by few.
      //
      // Unless you want to, you do not have to take into account that a
      // vampire might be poisoned and thus sometimes less dangerous than one
      // that is not.  That requires a more sophisticated analysis that
      // we're not asking you to do.
}

///////////////////////////////////////////////////////////////////////////
// main()
///////////////////////////////////////////////////////////////////////////

int main()
{
      // Create a game
      // Use this instead to create a mini-game:   Game g(3, 5, 2);
    // Game g(3, 5, 2);
    Game g(10, 12, 20);
      // Play the game
    g.play();
}

///////////////////////////////////////////////////////////////////////////
//  clearScreen implementation
///////////////////////////////////////////////////////////////////////////

// DO NOT MODIFY OR REMOVE ANY CODE BETWEEN HERE AND THE END OF THE FILE!!!
// THE CODE IS SUITABLE FOR VISUAL C++, XCODE, AND g++/g31 UNDER LINUX.

// Note to Xcode users:  clearScreen() will just write a newline instead
// of clearing the window if you launch your program from within Xcode.
// That's acceptable.  (The Xcode output window doesn't have the capability
// of being cleared.)

#ifdef _MSC_VER  //  Microsoft Visual C++

#pragma warning(disable : 4005)
#include <windows.h>

void clearScreen()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    COORD upperLeft = { 0, 0 };
    DWORD dwCharsWritten;
    FillConsoleOutputCharacter(hConsole, TCHAR(' '), dwConSize, upperLeft,
                                                        &dwCharsWritten);
    SetConsoleCursorPosition(hConsole, upperLeft);
}

#else  // not Microsoft Visual C++, so assume UNIX interface

#include <iostream>
#include <cstring>
#include <cstdlib>

void clearScreen()  // will just write a newline in an Xcode output window
{
    static const char* term = getenv("TERM");
    if (term == nullptr  ||  strcmp(term, "dumb") == 0)
        cout << endl;
    else
    {
        static const char* ESC_SEQ = "\x1B[";  // ANSI Terminal esc seq:  ESC [
        cout << ESC_SEQ << "2J" << ESC_SEQ << "H" << flush;
    }
}

#endif
