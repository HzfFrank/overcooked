#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#include <framework.h>

std::string oracle = "pick ingredient";

extern int width, height;
extern char Map[20 + 5][20 + 5];
extern int IngredientCount;
extern struct Ingredient Ingredient[20 + 5];
extern int recipeCount;
extern struct Recipe Recipe[20 + 5];
extern int totalTime, randomizeSeed, totalOrderCount;
extern struct Order totalOrder[20 + 5];
extern int orderCount;
extern struct Order Order[20 + 5];
extern struct Player Players[2 + 5];
extern int entityCount;
extern struct Entity Entity[20 + 5];
extern int remainFrame, Fund;
const double error_l = 0.1;
const double error_h = 1 - error_l;
const double middle = 0.5;
double hell_x = 8.0;
double hell_y = 8.0;
double pan_x;
double pan_y;
double pot_x;
double pot_y;

int waiting = 0;

double dis(double x, double y, double x_, double y_)
{
    double ans = (x - x_) * (x - x_) + (y - y_) * (y - y_);
    return fabs(ans);
}

bool not_move(std::string action)
{
    if (action != "Move U" && action != "Move D" && action != "Move L" && action != "Move R" && action != "Move LU" && action != "Move LD" && action != "Move RU" && action != "Move RD" && action != "Move")
        return true;
    return false;
}

bool dangerous(int id)
{
    double num = 0.25;
    if (Players[id].x >= hell_x && Players[id].x <= hell_x + 1)
    {
        if (Players[id].y >= hell_y - num)
            return true;
    }
    else if (Players[id].y >= hell_y && Players[id].y <= hell_y + 1)
    {
        if (Players[id].x >= hell_x - num)
            return true;
    }
    else
    {
        if (Players[id].y >= hell_y - num && Players[id].x >= hell_x - num)
            return true;
    }
    return false;
}

bool in(double x, double y, double x_, double y_)
{
    if (x_ > x + error_l && x_ < x + error_h && y_ > y + error_l && y_ < y + error_h)
        return true;
    return false;
}

bool in_x(double x, double x_)
{
    if (x_ > x + error_l && x_ < x + error_h)
        return true;
    return false;
}

bool in_y(double y, double y_)
{
    if (y_ > y + error_l && y_ < y + error_h)
        return true;
    return false;
}

std::string move(double goal_x, double goal_y, int id)
{
    if (in_x(goal_x, Players[id].x))
    {
        if (Players[id].y <= goal_y + error_l)
        {
            if (dangerous(id))
                return "Move LD";
            else
                return "Move D";
        }
        else if (Players[id].y >= goal_y + error_h)
        {
            if (dangerous(id))
                return "Move LU";
            else
                return "Move U";
        }
    }
    else if (in_y(goal_y, Players[id].y))
    {
        if (Players[id].x <= goal_x + error_l)
            return "Move R";
        else if (Players[id].x >= goal_x + error_h)
            return "Move L";
    }
    else
    {
        if (goal_x < Players[id].x)
        {
            if (Players[id].y < goal_y + middle)
            {
                if (dangerous(id))
                    return "Move L";
                else
                    return "Move LD";
            }
            else if (Players[id].y > goal_y + middle)
                return "Move LU";
            else
                return "Move L";
        }
        else if (goal_x > Players[id].x)
        {
            if (Players[id].y < goal_y + middle)
                return "Move RD";
            else if (Players[id].y > goal_y + middle)
            {
                if (dangerous(id))
                    return "Move U";
                else
                    return "Move RU";
            }
            else
                return "Move R";
        }
        else if (goal_y < Players[id].y)
        {
            if (Players[id].x < goal_x + middle)
            {
                if (dangerous(id))
                    return "Move U";
                else
                    return "Move RU";
            }
            else if (Players[id].x > goal_x + middle)
                return "Move LU";
            else
                return "Move U";
        }
        else if (goal_y > Players[id].y)
        {
            if (Players[id].x < goal_x + middle)
                return "Move RD";
            else if (Players[id].x > goal_x + middle)
            {
                if (dangerous(id))
                    return "Move L";
                else
                    return "Move LD";
            }
            else
                return "Move D";
        }
    }
    return "Move";
}

bool check_ingredient(const std::string& s)
{
    for (int j = 0; j < IngredientCount; j++)
        if (Ingredient[j].name == s)
            return true;
    return false;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cerr.tie(nullptr);
    std::cerr << std::nounitbuf;
    std::string s;
    std::stringstream ss;
    int frame;

    init_read();

    bool wash = false;
    std::string dir0_plate;
    std::string dir1;
    std::string dir_cook;
    double pink_x;
    double pink_y;
    double dirty_x;
    double dirty_y;
    double wash_x;
    double wash_y;
    double cut_x;
    double cut_y;
    double plate_x = -1.0;
    double plate_y = -1.0;
    double op_x = width;
    double op_y = height;
    bool completed = true;
    std::string now_food;
    std::string goal_food;
    std::string ori_food;
    std::string opt;
    int finished = 0;
    bool update_food = true;
    oracle = "pick ingredient";

    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
        {
            if (Map[i][j] == '$')
                pink_x = j, pink_y = i;
            if (Map[i][j] == 'k')
                wash_x = j, wash_y = i;
            if (Map[i][j] == 'c')
                cut_x = j, cut_y = i;
        }

    for (int i = 0; i < entityCount; i++)
    {
        if (Entity[i].entity.back() == "Pan")
            pan_x = Entity[i].x, pan_y = Entity[i].y;
        if (Entity[i].entity.back() == "Pot")
            pot_x = Entity[i].x, pot_y = Entity[i].y;
    }

    int totalFrame = 14400;
    for (int i = 0; i < totalFrame; i++)
    {
        bool skip = frame_read(i);
        if (skip) continue;

        std::string player0_Action = "Move";
        std::string player1_Action = "Move";

        double goal_x = width;
        double goal_y = height;
        double or_goal_x = width;
        double or_goal_y = height;

        waiting++;

        if (Players[1].live)
            waiting = 0;

        if (Players[0].live)
            waiting = 0;

        waiting++;
        if (Players[1].live)
            waiting = 0;

        if (completed)
        {
            completed = false;
            now_food = "";
            goal_food = Order[0].recipe[finished];
            ori_food = goal_food;
            while (!check_ingredient(goal_food))
            {
                for (int index = 0; index < recipeCount; index++)
                {
                    if (Recipe[index].nameAfter == goal_food)
                    {
                        opt = Recipe[index].kind;
                        goal_food = Recipe[index].nameBefore;
                        break;
                    }
                }
            }
        }

        //go to pick ingredient
        if (oracle == "pick ingredient")
        {
            for (int index = 0; index < IngredientCount; index++)
            {
                if (Ingredient[index].name == goal_food)
                {
                    goal_x = Ingredient[index].x;
                    goal_y = Ingredient[index].y;
                    or_goal_x = Ingredient[index].x;
                    or_goal_y = Ingredient[index].y;
                    break;
                }
            }
            if (goal_x == 0)
                goal_x += 1;
            else if (goal_x == width - 1)
                goal_x -= 1;
            if (goal_y == 0)
                goal_y += 1;
            else if (goal_y == height - 1)
                goal_y -= 1;
            if (in(goal_x, goal_y, Players[0].x, Players[0].y))
            {
                if (or_goal_x == 0)
                    player0_Action = "PutOrPick L";
                else if (or_goal_x == width - 1)
                    player0_Action = "PutOrPick R";
                else if (or_goal_y == 0)
                    player0_Action = "PutOrPick U";
                else if (or_goal_y == height - 1)
                    player0_Action = "PutOrPick D";
                now_food = goal_food;
                if (!opt.empty())
                {
                    oracle = "go to operate";
                    assert(now_food != ori_food);
                    // renew goal_food
                    goal_food = ori_food;
                    while (true)
                    {
                        bool flag = false;
                        for (int j = 0; j < recipeCount; j++)
                        {
                            if (Recipe[j].nameAfter == goal_food)
                            {
                                opt = Recipe[j].kind;
                                if (Recipe[j].nameBefore != now_food)
                                    goal_food = Recipe[j].nameBefore;
                                else
                                    flag = true;
                                break;
                            }
                        }
                        if (flag)
                            break;
                    }
                }
                else
                    oracle = "go to plate"; //If don't need to operate, then go to pick the plate
            }
            else
            {
                player0_Action = move(goal_x, goal_y, 0);
            }
        }
        //go to the operation console
        else if (oracle == "go to operate")
        {
            if (opt == "-chop->")
            {
                op_x = cut_x;
                op_y = cut_y;
            }
            else if (opt == "-pot->")
            {
                op_x = pot_x;
                op_y = pot_y;
            }
            else if (opt == "-pan->")
            {
                op_x = pan_x;
                op_y = pan_y;
            }
            goal_x = op_x;
            goal_y = op_y;
            if (goal_x == 0)
                goal_x += 1;
            else if (goal_x == width - 1)
                goal_x -= 1;
            if (goal_y == 0)
                goal_y += 1;
            else if (goal_y == height - 1)
                goal_y -= 1;
            if (in(goal_x, goal_y, Players[0].x, Players[0].y))
            {
                if (op_x == 0)
                    player0_Action = "PutOrPick L", dir_cook = "L";
                else if (op_x == width - 1)
                    player0_Action = "PutOrPick R", dir_cook = "R";
                else if (op_y == 0)
                    player0_Action = "PutOrPick U", dir_cook = "U";
                else if (op_y == height - 1)
                    player0_Action = "PutOrPick D", dir_cook = "D";
                oracle = "cooking";
            }
            else
            {
                player0_Action = move(goal_x, goal_y, 0);
            }
        }
        //when cooking
        else if (oracle == "cooking")
        {
            if (dir_cook == "L")
                player0_Action = "Interact L";
            else if (dir_cook == "R")
                player0_Action = "Interact R";
            else if (dir_cook == "U")
                player0_Action = "Interact U";
            else if (dir_cook == "D")
                player0_Action = "Interact D";
            for (int j = 0; j < entityCount; j++)
            {
                if ((Entity[j].x == op_x && Entity[j].y == op_y && Entity[j].entity.front() == goal_food) || (Entity[j].x == op_x && Entity[j].y == op_y && Entity[j].totalFrame > 0 && Entity[j].totalFrame == Entity[j].currentFrame))
                {
                    // end cooking, pick the cooked food
                    opt = "";
                    oracle = "pick cooked food";
                    break;
                }
            }
        }
        //pick up the cooked food
        else if (oracle == "pick cooked food")
        {
            if (dir_cook == "L")
                player0_Action = "PutOrPick L";
            else if (dir_cook == "R")
                player0_Action = "PutOrPick R";
            else if (dir_cook == "U")
                player0_Action = "PutOrPick U";
            else if (dir_cook == "D")
                player0_Action = "PutOrPick D";
            now_food = goal_food;
            opt = "";
            if (now_food == ori_food)
                oracle = "go to plate";
            else
            {
                oracle = "go to operate";
                goal_food = ori_food;
                while (true)
                {
                    bool flag = false;
                    for (int l = 0; l < recipeCount; l++)
                    {
                        if (Recipe[l].nameAfter == goal_food)
                        {
                            opt = Recipe[l].kind;
                            if (Recipe[l].nameBefore != now_food)
                                goal_food = Recipe[l].nameBefore;
                            else
                                flag = true;
                            break;
                        }
                    }
                    if (flag)
                        break;
                }
            }
        }
        else if (oracle == "go to plate")
        {
            //find the position of the plate
            bool have_plate = false;
            if (plate_x < 0 && plate_y < 0)
                for (int j = 0; j < entityCount; j++)
                {
                    if (Entity[j].containerKind == ContainerKind::Plate)
                    {
                        plate_x = Entity[j].x;
                        plate_y = Entity[j].y;
                        have_plate = true;
                        break;
                    }
                }
            else
                have_plate = true;
            goal_x = plate_x;
            goal_y = plate_y;
            or_goal_x = plate_x;
            or_goal_y = plate_y;
            if (goal_x == 0)
                goal_x += 1;
            else if (goal_x == width - 1)
                goal_x -= 1;
            if (goal_y == 0)
                goal_y += 1;
            else if (goal_y == height - 1)
                goal_y -= 1;
            if (have_plate)
            {
                if (in(goal_x, goal_y, Players[0].x, Players[0].y))
                {
                    if (or_goal_x == 0)
                        player0_Action = "PutOrPick L", dir0_plate = "L";
                    else if (or_goal_x == width - 1)
                        player0_Action = "PutOrPick R", dir0_plate = "R";
                    else if (or_goal_y == 0)
                        player0_Action = "PutOrPick U", dir0_plate = "U";
                    else if (or_goal_y == height - 1)
                        player0_Action = "PutOrPick D", dir0_plate = "D";
                    finished++;
                    now_food = "";
                    // If you still have any utensils in your hand, return them
                    if (Players[0].containerKind != ContainerKind::None)
                        oracle = "return container";
                    // If the food on the plate is enough to complete this order, then pick up the food and plate
                    else if (finished == Order[0].recipe.size())
                        oracle = "pick food and plate";
                    // If you still need food, continue to pick it up
                    else
                    {
                        oracle = "pick ingredient";
                        goal_food = Order[0].recipe[finished];
                        ori_food = goal_food;
                        while (!check_ingredient(goal_food))
                        {
                            for (int index = 0; index < recipeCount; index++)
                            {
                                if (Recipe[index].nameAfter == goal_food)
                                {
                                    opt = Recipe[index].kind;
                                    goal_food = Recipe[index].nameBefore;
                                    break;
                                }
                            }
                        }
                    }
                }
                else
                {
                    player0_Action = move(goal_x, goal_y, 0);
                }
            }
        }
        // return container
        else if (oracle == "return container")
        {
            if (Players[0].containerKind == ContainerKind::Pot)
            {
                op_x = pot_x;
                op_y = pot_y;
            }
            else if (Players[0].containerKind == ContainerKind::Pan)
            {
                op_x = pan_x;
                op_y = pan_y;
            }
            goal_x = op_x;
            goal_y = op_y;
            if (goal_x == 0)
                goal_x += 1;
            else if (goal_x == width - 1)
                goal_x -= 1;
            if (goal_y == 0)
                goal_y += 1;
            else if (goal_y == height - 1)
                goal_y -= 1;
            if (in(goal_x, goal_y, Players[0].x, Players[0].y))
            {
                if (op_x == 0)
                    player0_Action = "PutOrPick L", dir_cook = "L";
                else if (op_x == width - 1)
                    player0_Action = "PutOrPick R", dir_cook = "R";
                else if (op_y == 0)
                    player0_Action = "PutOrPick U", dir_cook = "U";
                else if (op_y == height - 1)
                    player0_Action = "PutOrPick D", dir_cook = "D";
                // If the food on the plate is enough to complete this order, then pick up the food and plate
                if (finished == Order[0].recipe.size())
                    oracle = "go to pick food and plate";
                // If you still need food, continue to pick it up
                else
                {
                    oracle = "pick ingredient";
                    goal_food = Order[0].recipe[finished];
                    ori_food = goal_food;
                    while (!check_ingredient(goal_food))
                    {
                        for (int index = 0; index < recipeCount; index++)
                        {
                            if (Recipe[index].nameAfter == goal_food)
                            {
                                opt = Recipe[index].kind;
                                goal_food = Recipe[index].nameBefore;
                                break;
                            }
                        }
                    }
                }
            }
            else
            {
                player0_Action = move(goal_x, goal_y, 0);
            }
        }
        // go to pick food and plate
        else if (oracle == "go to pick food and plate")
        {
            bool have_plate = false;
            if (plate_x < 0 && plate_y < 0)
                for (int j = 0; j < entityCount; j++)
                {
                    if (Entity[j].containerKind == ContainerKind::Plate)
                    {
                        plate_x = Entity[j].x;
                        plate_y = Entity[j].y;
                        have_plate = true;
                        break;
                    }
                }
            else
                have_plate = true;
            goal_x = plate_x;
            goal_y = plate_y;
            or_goal_x = plate_x;
            or_goal_y = plate_y;
            if (goal_x == 0)
                goal_x += 1;
            else if (goal_x == width - 1)
                goal_x -= 1;
            if (goal_y == 0)
                goal_y += 1;
            else if (goal_y == height - 1)
                goal_y -= 1;
            if (have_plate)
            {
                if (in(goal_x, goal_y, Players[0].x, Players[0].y))
                {
                    if (or_goal_x == 0)
                        dir0_plate = "L";
                    else if (or_goal_x == width - 1)
                        dir0_plate = "R";
                    else if (or_goal_y == 0)
                        dir0_plate = "U";
                    else if (or_goal_y == height - 1)
                        dir0_plate = "D";
                    player0_Action = "Move";
                    oracle = "pick food and plate";
                }
                else
                {
                    player0_Action = move(goal_x, goal_y, 0);
                }
            }
        }
        else if (oracle == "pick food and plate")
        {
            if (dir0_plate == "L")
                player0_Action = "PutOrPick L";
            else if (dir0_plate == "R")
                player0_Action = "PutOrPick R";
            else if (dir0_plate == "U")
                player0_Action = "PutOrPick U";
            else if (dir0_plate == "D")
                player0_Action = "PutOrPick D";
            plate_x = -1.0;
            plate_y = -1.0;
            oracle = "serve";
        }
        else if (oracle == "serve")
        {
            goal_x = pink_x;
            goal_y = pink_y;
            or_goal_x = pink_x;
            or_goal_y = pink_y;
            if (goal_x == 0)
                goal_x += 1;
            else if (goal_x == width - 1)
                goal_x -= 1;
            if (goal_y == 0)
                goal_y += 1;
            else if (goal_y == height - 1)
                goal_y -= 1;
            if (in(goal_x, goal_y, Players[0].x, Players[0].y))
            {
                if (or_goal_x == 0)
                    player0_Action = "PutOrPick L";
                else if (or_goal_x == width - 1)
                    player0_Action = "PutOrPick R";
                else if (or_goal_y == 0)
                    player0_Action = "PutOrPick U";
                else if (or_goal_y == height - 1)
                    player0_Action = "PutOrPick D";
                completed = true;
                opt = "";
                now_food = "";
                finished = 0;
                oracle = "pick ingredient";
            }
            else
            {
                player0_Action = move(goal_x, goal_y, 0);
            }
        }

        if (Players[1].containerKind == ContainerKind::DirtyPlates)
        {
            goal_x = wash_x;
            goal_y = wash_y;
            or_goal_x = wash_x;
            or_goal_y = wash_y;
            if (goal_x == 0)
                goal_x += 1;
            else if (goal_x == width - 1)
                goal_x -= 1;
            if (goal_y == 0)
                goal_y += 1;
            else if (goal_y == height - 1)
                goal_y -= 1;
            if (in(goal_x, goal_y, Players[1].x, Players[1].y))
            {
                if (or_goal_x == 0)
                    player1_Action = "PutOrPick L", dir1 = "L";
                else if (or_goal_x == width - 1)
                    player1_Action = "PutOrPick R", dir1 = "R";
                else if (or_goal_y == 0)
                    player1_Action = "PutOrPick U", dir1 = "U";
                else if (or_goal_y == height - 1)
                    player1_Action = "PutOrPick D", dir1 = "D";
                wash = true;
            }
            else
            {
                player1_Action = move(goal_x, goal_y, 1);
            }
        }
        else
        {
            if (wash)
            {
                if (dir1 == "L")
                    player1_Action = "Interact L";
                else if (dir1 == "R")
                    player1_Action = "Interact R";
                else if (dir1 == "U")
                    player1_Action = "Interact U";
                else if (dir1 == "D")
                    player1_Action = "Interact D";
                bool check = false;
                for (int j = 0; j < entityCount; j++)
                {
                    if (Entity[j].x == wash_x && Entity[j].y == wash_y && Entity[j].containerKind == ContainerKind::DirtyPlates)
                    {
                        check = true;
                        break;
                    }
                }
                if (!check)
                    wash = false;
            }
            else
            {
                bool dirty = false;
                for (int j = 0; j < entityCount; j++)
                {
                    if (Entity[j].containerKind == ContainerKind::DirtyPlates)
                    {
                        dirty_x = Entity[j].x;
                        dirty_y = Entity[j].y;
                        dirty = true;
                        break;
                    }
                }
                if (!dirty)
                    player1_Action = "Move";
                else
                {
                    goal_x = dirty_x;
                    goal_y = dirty_y;
                    or_goal_x = dirty_x;
                    or_goal_y = dirty_y;
                    if (goal_x == 0)
                        goal_x += 1;
                    else if (goal_x == width - 1)
                        goal_x -= 1;
                    if (goal_y == 0)
                        goal_y += 1;
                    else if (goal_y == height - 1)
                        goal_y -= 1;
                    if (in(goal_x, goal_y, Players[1].x, Players[1].y))
                    {
                        if (or_goal_x == 0)
                            player1_Action = "PutOrPick L";
                        else if (or_goal_x == width - 1)
                            player1_Action = "PutOrPick R";
                        else if (or_goal_y == 0)
                            player1_Action = "PutOrPick U";
                        else if (or_goal_y == height - 1)
                            player1_Action = "PutOrPick D";
                    }
                    else
                    {
                        player1_Action = move(goal_x, goal_y, 1);
                    }
                }
            }
        }

        if (dis(Players[0].x, Players[0].y, Players[1].x, Players[1].y) < 1.0)
        {
            if (not_move(player0_Action) && !not_move(player1_Action))
                player1_Action = "Move";
            else if (!not_move(player0_Action) && not_move(player1_Action))
                player0_Action = "Move";
        }

        if (dangerous(0) && !not_move(player0_Action))
            player0_Action = "Move LU";

        if (player0_Action == "PutOrPick L" || player0_Action == "PutOrPick R" || player0_Action == "PutOrPick U" || player0_Action == "PutOrPick D" || player0_Action == "Interact L" || player0_Action == "Interact R" || player0_Action == "Interact U" || player0_Action == "Interact D")
            waiting = 0;

        if (!dangerous(0) && !dangerous(1) && !not_move(player0_Action) && !not_move(player1_Action))
            if (waiting > 200)
                player1_Action = "Move R";

        /* Operation of outputting the current frame */
        std::cout << "Frame " << i << "\n";

        /* Synthesize a string before outputting, otherwise the output may be interrupted */
        std::string action = player0_Action + "\n" + player1_Action + "\n";
        std::cout << action;

        /* Don't forget to refresh the output stream, otherwise the game will not receive a response in a timely manner */
        std::cout.flush();
    }
}
