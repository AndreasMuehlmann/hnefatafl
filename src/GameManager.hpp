#pragma once

class GameManager {

public:
  virtual ~GameManager() = default;
  virtual auto run()-> void = 0;
};
