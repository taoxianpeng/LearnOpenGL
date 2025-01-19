#pragma once
class Node
{
  explicit Node() = default;
  virtual ~Node() = default;

  virtual void loadData() = 0;
  virtual void draw() = 0; 
};
