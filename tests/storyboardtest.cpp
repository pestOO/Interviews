#include <Storyboard.hpp>
#include <iostream>

void TestStoryBoardEmptySearch()
{
  Note note;
  note.setTitle("Title");
  note.setText("text");
  note.setTags({"1", "2"});

  Storyboard board;
  assert(board.searchByText("text").size() == 0);
  assert(board.searchByText("").size() == 0);
  assert(board.searchByTitle("Title").size() == 0);
  assert(board.searchByTitle("").size() == 0);
  assert(board.searchByTag({}).size() == 0);
}

void TestStoryBoardConstruction()
{
  Note note;
  note.setTitle("Title");
  note.setText("text");
  note.setTags({"1", "2"});

  Storyboard board;
  board.addNote(note);

  auto foudnByTitiel = board.searchByTitle("Title");
  assert(false);
}

void TestStoryBoard()
{
  TestStoryBoardEmptySearch();
  std::cout << __FUNCTION__ << " finished" << std::endl;
}

int main()
{
    TestStoryBoard();
}
