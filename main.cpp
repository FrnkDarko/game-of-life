#include <cassert>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

enum class State : char
{
  Dead,
  Alive
};

class Board
{
  int n_;
  std::vector<State> board_;

public:
  Board(int n) : n_(n), board_(n * n) {}
  State operator()(int i, int j) const
  {
    return (i >= 0 && i < n_ && j >= 0 && j < n_) ? board_[i * n_ + j]
                                                  : State::Dead;
  } // overload di funzioni, operatore su un tipo const e inizializza tutto a
    // dead

  State &operator()(int i, int j)
  {
    assert(i >= 0 && i < n_ && j >= 0 && j < n_);
    return board_[i * n_ + j];
  }
  int size() const { return n_; }
  void print(Board const& b)
  { int const n_ = b.size();
    for (int i = 0; i != n_; ++i)
    {
      for (int j = 0; j != n_; ++j)
      {
        if (b(i, j) == State::Alive)
        {
          std::cout << "+ ";
        }
        if (b(i, j) == State::Dead)
        {
          std::cout << ". ";
        }
      }
      std::cout << '\n';
    }
    std::cout << '\n';
  }
};

int neigh(Board const &board, int const r, int const c)
{
  int result =
      -static_cast<int>(board(r, c)); // alive and dead are now 1 and 0
  for (int i = r - 1; i != r + 2; ++i)
  {
    for (int j = c - 1; j != c + 2; ++j)
    {
      result +=
          static_cast<int>(board(i, j)); // if the cell is alive, it adds 1,
                                         // if the cell is dead it adds 0
    }
  }
  return result;
}

Board evolve(Board const &current)
{
  int const N = current.size();
  Board next(N);
  int k;
  for (int i = 0; i != N; ++i)
  {
    for (int j = 0; j != N; ++j)
    {
      k = neigh(current, i, j);
      assert(k < 10);
      if (current(i, j) == State::Dead)
      {
        if (k == 3)
        {
          next(i, j) = State::Alive;
        }
        else
        {
          next(i, j) = State::Dead;
        }
      }
      if (current(i, j) == State::Alive)
      {
        k -= 1;
        if (k < 2 || k > 3)
        {
          next(i, j) = State::Dead;
        }
        else
        {
          next(i, j) = State::Alive;
        }
      }
    }
  }

  return next;
}

int main()
{
  int dim = 50;
  Board board(dim);

  board(15, 4) = State::Alive;
  board(16, 5) = State::Alive;
  board(16, 3) = State::Alive;
  board(16, 4) = State::Alive;
  board(16, 6) = State::Alive;
  board.print(board);
 for (int i = 0; i != 10; ++i)
  {
    board = evolve(board);
    board.print(board);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}
