#include "game.h"
#include <iostream>
#include <thread>
// #include "SDL.h"
#include </usr/include/SDL2/SDL.h>

using std::cout;

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width)),
      random_h(0, static_cast<int>(grid_height)) {
  cout << "grid width " << grid_width << "grid height " << grid_height << std::endl;
  score = std::make_unique<int>();
  SetScore(0);
  PlaceFood();
  PlaceSuperfood();
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  int superFoodMoveDuration = 0;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    if ((*score % 3 == 0) && (*score != 0)) {
      UpdateSuperLevel();
      ++superFoodMoveDuration;
      if (superFoodMoveDuration == 200) {
        PlaceSuperfood();
        superFoodMoveDuration = 0;
      }
      renderer.RenderSuperFood(snake, food, superfood, dangerVector);
    } else {
      Update();
      renderer.Render(snake, food, dangerVector);
    }

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(*score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing food.
    if ((!snake.SnakeCell(x, y)) && (!CheckFood(x, y)) && (!CheckDanger(x, y)) && (!CheckSuperfood(x, y))) {
      food.x = x;
      food.y = y;
      cout << "placing next food x: " << food.x << " y: " << food.y << std::endl;
      return;
    }
  }
}

void Game::PlaceSuperfood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake or food item before placing food.
    if ((!snake.SnakeCell(x, y)) && (!CheckFood(x, y)) && (!CheckDanger(x, y)) && (!CheckSuperfood(x, y))) {
      superfood.x = x;
      superfood.y = y;
      cout << "placing next superfood x: " << superfood.x << " y: " << superfood.y << std::endl;
      return;
    } else {
      cout << "invalid superfood location x: " << superfood.x << " y: " << superfood.y << std::endl;
    }
  }  
}

void Game::PlaceDanger() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake or food item before placing food.
    if ((!snake.SnakeCell(x, y)) && (!CheckFood(x, y)) && (!CheckDanger(x, y)) && (!CheckSuperfood(x, y))) {
      SDL_Point danger = {x, y};
      dangerVector.emplace_back(danger);
      cout << "placing another danger x: " << x << " y: " << y << std::endl;
      return;
    } else {
      cout << "invalid danger location x: " << x << " y: " << y << std::endl;
    }
  }
}

bool Game::CheckFood(int new_x, int new_y) {
  if (food.x == new_x && food.y == new_y)
  {
    return true;
  } else
  {
    return false;
  }
}

bool Game::CheckSuperfood(int new_x, int new_y) {
  if (superfood.x == new_x && superfood.y == new_y)
  {
    return true;
  } else
  {
    return false;
  }
}

bool Game::CheckDanger(int new_x, int new_y) {
  for (SDL_Point danger: dangerVector)
  {
    if (danger.x == new_x && danger.y == new_y)
    {
      return true;
    }
  }
  return false;
}

void Game::ActionSuperLevel(bool hitFood, bool hitSuperfood, bool hitDanger) 
  {
    if (hitFood)
    {
      IncrementScore();
      PlaceFood();
      PlaceDanger();
      // Grow snake and increase speed.
      snake.GrowBody();
      snake.speed += 0.02;
    }
    else if (hitSuperfood)
    {
      IncrementScore();
      PlaceSuperfood();
      PlaceDanger();
      // superfood reduces speed instead
      snake.GrowBody();
      snake.speed -= 0.03;
    }
    else if (hitDanger)
    {
      snake.alive = false;
    }
  }
void Game::Action(bool hitFood, bool hitDanger) 
  {
    if (hitFood)
    {
      IncrementScore();
      PlaceFood();
      // Grow snake and increase speed.
      snake.GrowBody();
      snake.speed += 0.02;
    }
    else if (hitDanger)
    {
      snake.alive = false;
    }
  }
void Game::Update() {  
  if (!snake.alive) return;

  snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  std::future<bool> ftr3 = std::async(&Game::CheckDanger, this, new_x, new_y);
  std::future<bool> ftr1 = std::async(&Game::CheckFood, this, new_x, new_y);
  bool hitFood = ftr1.get();
  bool hitDanger = ftr3.get();
  if (hitDanger || hitFood)
  {
    Action(hitFood, hitDanger);
  }
}

void Game::UpdateSuperLevel() {  
  if (!snake.alive) return;

  snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  std::future<bool> ftr3 = std::async(&Game::CheckDanger, this, new_x, new_y);
  std::future<bool> ftr1 = std::async(&Game::CheckFood, this, new_x, new_y);
  std::future<bool> ftr2 = std::async(&Game::CheckSuperfood, this, new_x, new_y);
  bool hitFood = ftr1.get();
  bool hitSuperfood = ftr2.get();
  bool hitDanger = ftr3.get();
  if (hitDanger || hitFood || hitSuperfood)
  {
    ActionSuperLevel(hitFood, hitSuperfood, hitDanger);
  }
}


int Game::GetScore() { return *score; }
void Game::IncrementScore() {
  int currentScore = GetScore();
  cout << "current score " << currentScore << std::endl;
  SetScore(currentScore + 1);
}
void Game::SetScore(int newScore) {
  *score = newScore;
}
int Game::GetSize() const { return snake.size; }
