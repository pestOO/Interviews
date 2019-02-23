#include <Storyboard.hpp>
#include <iostream>

void TestStoryBoardEmptySearch()
{
  Note note;
  note.setTitle("Title");
  note.setText("text");
  note.setTags({"1", "2"});

  Storyboard board;
  assert(boost::size(board.searchByText("text")) == 0);
  assert(boost::size(board.searchByText("")) == 0);
  assert(boost::size(board.searchByTitle("Title")) == 0);
  assert(boost::size(board.searchByTitle(""))== 0);
  assert(boost::size(board.searchByTag({})) == 0);
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
