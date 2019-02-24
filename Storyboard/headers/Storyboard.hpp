/*!
 * Storyboard should enable us to search for notes by title, text and tags.
 * E.g.:
 *      searchByTitle("Test Traceplayer")
 *      searchByTag({"testing", "unit test"})
 *   	searchByText("Implement a unit test for the class Traceplayer of the
 * spark core framework.") For the sake of simplicity we don't want to do any
 * similiarity or prefix matching when searching for a title, tag or text. Only
 * an exact match should give results.
 *
 * Hint: Think about performance versus memory tradeoffs in your design, so you
 * can give good reasons for your decision.
 */

#pragma once
#ifndef HEADERS_STORYBOARD_HPP
#define HEADERS_STORYBOARD_HPP
#include <Note.hpp>

#include <list>
#include <memory>
#include <unordered_map>
#include <vector>

class Storyboard {
public:
  using THashNote = Note::TTitle;
  // TODO(EZ): change from map to set - to save memory
  using TNoteContainer = std::unordered_multimap<THashNote, Note>;
  using TNoteSearchResult = std::vector<std::reference_wrapper<Note>>;
  using TNoteFunctor = std::function<bool(const Note &)>;

  //! Adding note to the board
  void addNote(const Note &aNote);

  //! Delete exact note from the board
  //! \Note Note is not valide after insertion
  // TODO(EZ): return shared ptr to avoid UB
  void deleteNote(const Note &aNote);

  // TOD(EZ): add proper doxygen for each searh
  TNoteSearchResult searchByTitle(const Note::TTitle &aTitle);
  TNoteSearchResult searchByText(const Note::TText &aText);
  TNoteSearchResult searchByTag(const Note::TTag &aTag);

private:
  TNoteContainer iNotes;

  // Use hash not string value to save memory
  using TSearchHash = std::size_t;
  using TNoteIndexer = std::unordered_multimap<TSearchHash, THashNote>;
  TNoteIndexer iTextIndexer;
  TNoteIndexer iTagsIndexer;
};

void Storyboard::addNote(const Note &aNote) {
  const auto &title = aNote.getTitle();
  iNotes.insert({title, aNote});

  const auto textHash = std::hash<Note::TText>()(aNote.getText());
  iTextIndexer.insert({textHash, title});

  for (const auto &tag : aNote.getTags()) {
    const auto tagHash = std::hash<Note::TTag>()(tag);
    iTagsIndexer.insert({tagHash, title});
  }
}

void Storyboard::deleteNote(const Note &aNote) {

  assert(false && "Not implmented");
}

Storyboard::TNoteSearchResult
Storyboard::searchByTitle(const Note::TTitle &aTitle) {
  auto range = iNotes.equal_range(aTitle);

  Storyboard::TNoteSearchResult result;
  result.reserve(std::distance(range.first, range.second));

  for (auto it = range.first; it != range.second; ++it) {
    auto ref = std::ref(it->second);
    result.push_back(ref);
  }
  return result;
}

Storyboard::TNoteSearchResult
Storyboard::searchByText(const Note::TText &aText) {

  const auto textHash = std::hash<Note::TText>()(aText);

  auto range = iTextIndexer.equal_range(textHash);

  Storyboard::TNoteSearchResult result;

  for (auto it = range.first; it != range.second; ++it) {
    const auto title = it->second;
    auto partResult = searchByTitle(title);
    result.insert(result.end(), std::make_move_iterator(partResult.begin()),
                  std::make_move_iterator(partResult.end()));
  }
  return result;
}

Storyboard::TNoteSearchResult Storyboard::searchByTag(const Note::TTag &aTag) {

  const auto tagHash = std::hash<Note::TText>()(aTag);

  auto range = iTagsIndexer.equal_range(tagHash);

  Storyboard::TNoteSearchResult result;

  for (auto it = range.first; it != range.second; ++it) {
    const auto title = it->second;
    auto partResult = searchByTitle(title);
    result.insert(result.end(), std::make_move_iterator(partResult.begin()),
                  std::make_move_iterator(partResult.end()));
  }
  return result;
}

#endif // HEADERS_STORYBOARD_HPP
