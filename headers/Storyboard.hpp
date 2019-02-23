/*!
 * Storyboard should enable us to search for notes by title, text and tags.
 * E.g.:
 *      searchByTitle("Test Traceplayer")
 *      searchByTag({"testing", "unit test"})
 *   	searchByText("Implement a unit test for the class Traceplayer of the spark core framework.")
 * For the sake of simplicity we don't want to do any similiarity or prefix matching when
 * searching for a title, tag or text. Only an exact match should give results.
 * 
 * Hint: Think about performance versus memory tradeoffs in your design, so you can give good 
 *       reasons for your decision. 
 */

#pragma once
#ifndef HEADERS_STORYBOARD_HPP
#define HEADERS_STORYBOARD_HPP
#include <Note.hpp>

#include <list>
#include <unordered_map>
#include <memory>
#include <vector>

class Storyboard
{
public:
  using THashNote = Note::TTitle;
  //TOD(EZ): change from map to set - to save key memoery
  using TNoteContainer = std::unordered_multimap<THashNote, Note>;
  using TNoteSearchResult = std::vector< std::reference_wrapper<Note> >;
  using TNoteFunctor = std::function<bool(const Note&)>;

  //! Adding note to the board
  void addNote(const Note& aNote);

  //! Delete note from the board
  //! \Note TNoteSearchResult coudl be not valide after this deletion
  void deleteNote(TNoteFunctor&& aFunctor);

  TNoteSearchResult searchByTitle(const Note::TTitle& aTitle);
  TNoteSearchResult searchByText(const Note::TText& aText);
  TNoteSearchResult searchByTag(const Note::TTagContainer& aTags);
	
private:
  TNoteContainer iNotes;

  using TSearchHash = std::size_t;
  using TNoteIndexer = std::unordered_map<TSearchHash, THashNote>;
  TNoteIndexer iTextIndexer;
  TNoteIndexer iTagsIndexer;
};


void Storyboard::addNote(const Note &aNote)
{
  iNotes.insert({aNote.getTitle(), aNote});
}

void Storyboard::deleteNote(Storyboard::TNoteFunctor &&aFunctor)
{

//  const it = std::remove(std::begin(iNotes), std::end(iNotes), aFunctor);
//  for( ; it!= std::end(iNotes); ++it)
//    {

//    }
//  iNotes.erase()
}

Storyboard::TNoteSearchResult Storyboard::searchByTitle(const Note::TTitle &aTitle)
{
  auto range = iNotes.equal_range(aTitle);

  Storyboard::TNoteSearchResult result;
  result.reserve(std::distance(range.first, range.second));

  for (auto it = range.first; it!=range.second; ++it) {
      auto ref = std::ref(it->second);
      result.push_back(ref);
    }
  return result;
}

Storyboard::TNoteSearchResult Storyboard::searchByText(const Note::TText &aText)
{
  return {};
}

Storyboard::TNoteSearchResult Storyboard::searchByTag(const Note::TTagContainer &aTags)
{

  return {};
}

#endif  // HEADERS_STORYBOARD_HPP
