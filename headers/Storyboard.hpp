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

#include <Note.hpp>

#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/iterator_range.hpp>

#include <list>
#include <map>
#include <memory>

class Storyboard
{
public:
  using TNoteContainer = std::list<Note>;
  using TNotesRange =  boost::iterator_range<TNoteContainer::iterator>;
  using TNoteFunctor = std::function<bool(const Note&)>;
  using TNoteFilteredRange = boost::filtered_range<TNoteFunctor, const TNotesRange>;

  void addNote(const Note& aNote);

  void deleteNote(TNoteFunctor&& aFunctor);

  TNoteFilteredRange searchByTitle(const Note::TTitle& aTitle);
  TNoteFilteredRange searchByText(const Note::TText& aText);
  TNoteFilteredRange searchByTag(const Note::TTagContainer& aTags);
	
private:
  TNoteContainer iNotes;

  using TNoteIndexer = std::map<std::size_t, std::reference_wrapper<Note>>;
  TNoteIndexer iTitleIndexer;
  TNoteIndexer iTextIndexer;
  TNoteIndexer iTagsIndexer;
};


