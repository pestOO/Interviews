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

void TestStoryBoardSearchByTitleWrong()
{
  Note note;
  note.setTitle("Title");
  note.setText("text");
  note.setTags({"1", "2"});

  Storyboard board;
  board.addNote(note);

  auto foundByTitile = board.searchByTitle("WrongTitle");
  assert(foundByTitile.size() == 0);
}

void TestStoryBoardSearchByTitle()
{
  Note note;
  note.setTitle("Title");
  note.setText("text");
  note.setTags({"1", "2"});

  Storyboard board;
  board.addNote(note);

  note.setTitle("Title2");
  note.setText("text2");
  note.setTags({"3", "4"});
  board.addNote(note);

  auto foundByTitile = board.searchByTitle("Title");
  assert(foundByTitile.size() == 1);
  assert(foundByTitile[0].get().getText() == "text");
  assert(foundByTitile[0].get().getTags() == Note::TTagContainer({"1", "2"}));
}


void TestStoryBoardSearchByWrongText()
{
  Note note;
  note.setTitle("Title");
  note.setText("text");
  note.setTags({"1", "2"});

  Storyboard board;
  board.addNote(note);

  auto foundByText = board.searchByText("Wrongtext");
  assert(foundByText.size() == 0);
}

void TestStoryBoardSearchByText()
{
  Note note;
  note.setTitle("Title");
  note.setText("text");
  note.setTags({"1", "2"});

  Storyboard board;
  board.addNote(note);

  note.setTitle("Title2");
  note.setText("text2");
  note.setTags({"3", "4"});
  board.addNote(note);

  auto foundByText = board.searchByText("text");
  assert(foundByText.size() == 1);
  assert(foundByText[0].get().getTitle() == "Title");
  assert(foundByText[0].get().getTags() == Note::TTagContainer({"1", "2"}));
}


void TestStoryBoardSearchByTextDouble()
{
  Note note;
  note.setTitle("Title");
  note.setText("text");
  note.setTags({"1", "2"});

  Storyboard board;
  board.addNote(note);

  note.setTitle("Title2");
  note.setText("text");
  note.setTags({"3", "4"});
  board.addNote(note);

  auto foundByText = board.searchByText("text");
  assert(foundByText.size() == 2);
  assert(foundByText[0].get().getTitle() == "Title");
  assert(foundByText[1].get().getTitle() == "Title2");

  assert(foundByText[0].get().getTags() == Note::TTagContainer({"1", "2"}));
  assert(foundByText[1].get().getTags() == Note::TTagContainer({"3", "4"}));
}

void TestStoryBoardSearchByTagOne()
{
  Note note;
  note.setTitle("Title");
  note.setTags({"1"});

  Storyboard board;
  board.addNote(note);

  note.setTitle("Title2");
  note.setTags({"3"});
  board.addNote(note);

  auto foundByText = board.searchByTag("1");
  assert(foundByText.size() == 1);
  assert(foundByText[0].get().getTitle() == "Title");
}

void TestStoryBoardSearchByTagTwo()
{
  Note note;
  note.setTitle("Title");
  note.setTags({"1", "2"});

  Storyboard board;
  board.addNote(note);

  note.setTitle("Title2");
  note.setTags({"1", "3"});
  board.addNote(note);

  auto foundByText = board.searchByTag("1");
  assert(foundByText.size() == 2);
  assert(foundByText[0].get().getTitle() == "Title");
  assert(foundByText[1].get().getTitle() == "Title2");
}


void TestStoryBoardSearchByTextAfterRemoval()
{
  Note note;
  note.setTitle("Title");
  note.setText("text");
  note.setTags({"1", "2"});

  Storyboard board;
  board.addNote(note);

  auto foudnByText = board.searchByText("text");
  assert(foudnByText.size() == 1);

  board.deleteNote(foudnByText[0].get());

  assert(board.searchByTitle("text").size() == 0);
}

void TestStoryBoardSearchByAfterRemovalOneAnother()
{
  Note note;
  note.setTitle("Title");
  note.setText("text");
  note.setTags({"1", "2"});

  Storyboard board;
  board.addNote(note);

  note.setTitle("Title2");
  note.setText("text2");
  note.setTags({"3", "4"});
  board.addNote(note);

  auto foudnByText = board.searchByTitle("text");
  assert(foudnByText.size() == 1);

  board.deleteNote(foudnByText[0].get());

  assert(board.searchByTitle("Text").size() == 0);
  assert(board.searchByTitle("Text2").size() == 1);
}

//TODO(EZ):add test for seral equal in the board

void TestStoryBoard()
{
  TestStoryBoardEmptySearch();
  TestStoryBoardSearchByTitleWrong();
  TestStoryBoardSearchByTitle();
  TestStoryBoardSearchByWrongText();
  TestStoryBoardSearchByText();
  TestStoryBoardSearchByTextDouble();
  TestStoryBoardSearchByTagOne();
  TestStoryBoardSearchByTagTwo();
  TestStoryBoardSearchByTextAfterRemoval();
  TestStoryBoardSearchByAfterRemovalOneAnother();

  std::cout << __FUNCTION__ << " finished" << std::endl;
}

int main()
{
    TestStoryBoard();
}
