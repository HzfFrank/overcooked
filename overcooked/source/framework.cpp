#include <enum.h>
#include <framework.h>
#include <string>
#include <iostream>
#include <sstream>
#include <cassert>

/* Defined in order of reading in */
int width, height;
char Map[20 + 5][20 + 5];
int IngredientCount;
struct Ingredient Ingredient[20 + 5];
int recipeCount;
struct Recipe Recipe[20 + 5];
int totalTime, randomizeSeed, totalOrderCount;
struct Order totalOrder[20 + 5];
int orderCount;
struct Order Order[20 + 5];
int k;
struct Player Players[2 + 5];
int entityCount;
struct Entity Entity[20 + 5];
int remainFrame, Fund;

void init_read()
{
    std::string s;
    std::stringstream ss;
    int frame;

    /* Read initial map information */
    std::getline(std::cin, s, '\0');
    ss << s;

    /* If you follow this reading, accessing coordinates (x, y) is equivalent to accessing Map [y] [x],
     * and you can modify it according to your own habits */
    ss >> width >> height;
    std::cerr << "Map size: " << width << "x" << height << std::endl;
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            ss >> Map[i][j];

    /* Read in the raw material box: location, name, and purchase unit price */
    ss >> IngredientCount;
    for (int i = 0; i < IngredientCount; i++)
    {
        ss >> s;
        assert(s == "IngredientBox");
        ss >> Ingredient[i].x >> Ingredient[i].y >> Ingredient[i].name >> Ingredient[i].price;
    }

    /* Reading in formulas: processing time, string representation before processing, processing container,
     * and string representation after processing */
    ss >> recipeCount;
    for (int i = 0; i < recipeCount; i++)
    {
        ss >> Recipe[i].time >> Recipe[i].nameBefore >> Recipe[i].kind >> Recipe[i].nameAfter;
    }

    /* Total number of frames read in, current random seeds used, and total possible number of orders */
    ss >> totalTime >> randomizeSeed >> totalOrderCount;

    /* Read in the effective frames, price, weight, and order composition of the order */
    for (int i = 0; i < totalOrderCount; i++)
    {
        ss >> totalOrder[i].validFrame >> totalOrder[i].price >> totalOrder[i].frequency;
        getline(ss, s);
        std::stringstream tmp(s);
        while (tmp >> s)
            totalOrder[i].recipe.push_back(s);
    }

    /* Reading player information: initial coordinates */
    ss >> k;
    assert(k == 2);
    for (int i = 0; i < k; i++)
    {
        ss >> Players[i].x >> Players[i].y;
        Players[i].containerKind = ContainerKind::None;
        Players[i].entity.clear();
    }

    /* Reading in entity information: coordinates, entity composition */
    ss >> entityCount;
    for (int i = 0; i < entityCount; i++)
    {
        ss >> Entity[i].x >> Entity[i].y >> s;
        Entity[i].entity.push_back(s);
    }
}

bool frame_read(int nowFrame)
{
    std::string s;
    std::stringstream ss;
    int frame;
    std::getline(std::cin, s, '\0');
    ss.str(s);
    /*
      If there is still data in the input stream, it indicates that the game is already requesting the next frame
      At this point, we should skip the current frame in order to respond to the game in a timely manner
    */
    if (std::cin.rdbuf()->in_avail() > 0)
    {
        std::cerr << "Warning: skipping frame " << nowFrame
                  << " to catch up with the game" << std::endl;
        return true;
    }
    ss >> s;
    assert(s == "Frame");
    int currentFrame;
    ss >> currentFrame;
    assert(currentFrame == nowFrame);
    ss >> remainFrame >> Fund;
    /* Read in the remaining frames, prices, and recipes for the current order */
    ss >> orderCount;
    for (int i = 0; i < orderCount; i++)
    {
        ss >> Order[i].validFrame >> Order[i].price;
        Order[i].recipe.clear();
        getline(ss, s);
        std::stringstream tmp(s);
        while (tmp >> s)
        {
            Order[i].recipe.push_back(s);
        }
    }
    ss >> k;
    assert(k == 2);
    /* Read in player coordinates, x-direction speed, y-direction speed, remaining resurrection time */
    for (int i = 0; i < k; i++)
    {
        ss >> Players[i].x >> Players[i].y >> Players[i].X_Velocity >> Players[i].Y_Velocity >> Players[i].live;
        getline(ss, s);
        std::stringstream tmp(s);
        Players[i].containerKind = ContainerKind::None;
        Players[i].entity.clear();
        assert(Players[i].entity.empty());
        while (tmp >> s)
        {
            if (s == ";" || s == ":" || s == "@" || s == "*")
                continue;

            if (s == "Plate")
                Players[i].containerKind = ContainerKind::Plate;
            else if (s == "DirtyPlates")
                Players[i].containerKind = ContainerKind::DirtyPlates;
            else if (s == "Pot")
                Players[i].containerKind = ContainerKind::Pot;
            else if (s == "Pan")
                Players[i].containerKind = ContainerKind::Pan;
            else
                Players[i].entity.push_back(s);
        }
    }

    ss >> entityCount;
    /* Read in entity coordinates */
    for (int i = 0; i < entityCount; i++)
    {
        ss >> Entity[i].x >> Entity[i].y;
        getline(ss, s);
        std::stringstream tmp(s);
        Entity[i].containerKind = ContainerKind::None;
        Entity[i].entity.clear();
        Entity[i].currentFrame = Entity[i].totalFrame = 0;
        Entity[i].sum = 1;
        while (tmp >> s)
        {
            if (s == ":" || s == "@" || s == "*")
                continue;
            if (s == ";")
            {
                tmp >> Entity[i].currentFrame >> s >> Entity[i].totalFrame;
                assert(s == "/");
                break;
            }

            if (s == "Plate")
                Entity[i].containerKind = ContainerKind::Plate;
            else if (s == "DirtyPlates")
            {
                Entity[i].containerKind = ContainerKind::DirtyPlates;
                tmp >> Entity[i].sum;
            }
            else if (s == "Pot")
            {
                Entity[i].containerKind = ContainerKind::Pot;
            }
            else if (s == "Pan")
            {
                Entity[i].containerKind = ContainerKind::Pan;
            }
            else
                Entity[i].entity.push_back(s);
        }
    }
    return false;
}