#include <Note.hpp>
#include <iostream>

void TestNoteConstruction()
{
    Note note;
    assert(note.getTitle().empty());
    assert(note.getText().empty());
    assert(note.getTags().empty());
}

void TestNoteSetGet()
{
    Note note;
    note.setTitle("Title");
    note.setText("Text");
    note.setTags({"Tag1", "Tag2"});

    assert(note.getTitle() == "Title");
    assert(note.getText() == "Text");
    assert(note.getTags() == Note::TTagContainer({"Tag1", "Tag2"}));
}

void TestNote()
{
    TestNoteConstruction();
    TestNoteSetGet();
    std::cout << __FUNCTION__ << " finished" << std::endl;
}


int main()
{
    TestNote();
}
