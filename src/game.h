#ifndef GAME_H
#define GAME_H

#include <random>
// #include "SDL.h"
#include </usr/include/SDL2/SDL.h>
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include <memory>
#include <future>
#include <thread>
#include <vector>

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore();
  void SetScore(int newScore);
  int GetSize() const;

 private:
  Snake snake;
  SDL_Point food;
  SDL_Point superfood;
  std::vector<SDL_Point> dangerVector;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;
  std::unique_ptr<int> score;

  void PlaceFood();
  void PlaceSuperfood();
  void PlaceDanger();
  void Update(bool superLevel);
  void IncrementScore();
  bool CheckFood(int new_x, int new_y);
  bool CheckSuperfood(int new_x, int new_y);
  bool CheckDanger(int new_x, int new_y);
};

#endif