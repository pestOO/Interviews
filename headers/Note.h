
#pragma once
#ifndef HEADERS_NOTE_HPP
#define HEADERS_NOTE_HPP

#include <string>
#include <set>

/*!
 * Note is an element of storyboard.
 * Responsible for storing and simple converstion operations only
 * ?? putting sticky notes on a board).
 * Every note has a title, a text and a set of tags. E.g.
 * 	- title: "Test Traceplayer"
 * 	- text: "Implement a unit test for the class Traceplayer of the spark core framework."
 * 	- tags: {"unit test", "traceplayer", "testing", "spark core"}
 */
class Note
{
public:
    /*! Define ouw types to simplify futher performance improvement
     * TODO(EZamakhov): compare performance with cusom allocators,
     *                  when non-functional requirments are avalible
     */
    using TTitle = std::string;
    using TText  = std::string;
    using TTag   = std::string;
    using TTagContainer = std::set<TTag>;

    const TTitle& getTitle() const;
    void setTitle(const TTitle &value);

    const TText& getText() const;
    void setText(const TText &value);

    const TTagContainer& getTags() const;
    void setTags(const TTagContainer &value);

private:
    TTitle iTitle;
    TText iText;
    TTagContainer iTags;
};


// TODO(EZamakhov):move to source file

const Note::TTitle &Note::getTitle() const
{
return iTitle;
}

void Note::setTitle(const TTitle &value)
{
iTitle = value;
}

const Note::TText &Note::getText() const
{
return iText;
}

void Note::setText(const TText &value)
{
iText = value;
}

const Note::TTagContainer &Note::getTags() const
{
return iTags;
}

void Note::setTags(const TTagContainer &value)
{
iTags = value;
}

#endif  // HEADERS_NOTE_HPP