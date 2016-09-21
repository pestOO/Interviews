#include "stdafx.h"
#include <string>
#include <stack>
#include <string>
#include <memory>
#include <mutex>


class CustomString {
public:
   // Add char to the end of string
   void append(char ch)
   {
      save_value();
      _str.push_back(ch);
   }
   // remove last char in the string
   void erase()
   {
      save_value();
      if (_str.empty()) {
         return;
      }
      _str.pop_back();
   }
   const std::string& get_value() const
   {
      return _str;
   }
   // TODO(EZamkhov): Move logic to restore_value
   // for redability restore_value<->save_value
   static void UnDo()
   {
      CustomString* last = get_last_changed_string();
      if (last == nullptr) {
         return;
      }
      last->restore_value();
   }
private:
   friend class CustromStringProvider;
   explicit CustomString(const std::string& str) : _str(str)
   {
   }
   ~CustomString()
   {
      clear_history(this);
   }
   void save_value()
   {
      std::lock_guard<std::mutex> lck(
         custom_strings_access_history_lock);
      custom_strings_access_history.push_back(this);
      _states.push(_str);
   }
   void restore_value()
   {
      if (_states.empty()) {
         return;
      }
      _str = _states.top();
      _states.pop();
   }
   std::string _str;
   std::stack<std::string> _states;

   static CustomString* get_last_changed_string()
   {
      std::lock_guard<std::mutex> lck(
         custom_strings_access_history_lock);
      if (custom_strings_access_history.empty()) {
         return nullptr;
      }
      CustomString* cstring = custom_strings_access_history.back();
      custom_strings_access_history.pop_back();
      return cstring;
   }

   static void clear_history(CustomString* pointer)
   {
      std::lock_guard<std::mutex> lck(
         custom_strings_access_history_lock);
      std::remove(
         custom_strings_access_history.begin(),
         custom_strings_access_history.end(),
         pointer);
   }

   mutable static std::mutex custom_strings_access_history_lock;
   static std::deque<CustomString*> custom_strings_access_history;
};
std::mutex CustomString::custom_strings_access_history_lock =
   std::mutex();
std::deque<CustomString*> CustomString::custom_strings_access_history =
   std::deque<CustomString*>();


// TODO-list(EZamakhov) :
// * add template for string types
// * add logging error states
class CustromStringProvider {
public:
   static std::shared_ptr<CustomString>
      CreateString(const std::string& str = std::string())
   {
      new CustomString();

      return std::shared_ptr<CustomString>(
         new CustomString(str),
         [](CustomString*p) {
         DeleteString(p);
      },
         std::allocator<CustomString>());
   }
   static void DeleteString(CustomString* p)
   {
      delete p;
      p = nullptr;
   };
};


namespace {
   void print(const std::shared_ptr<CustomString> cstr)
   {
      std::cout << cstr->get_value() << std::endl;
   }
}

int _tmain(int argc, _TCHAR* argv[])
{
   std::shared_ptr<CustomString> cstr = 
      CustromStringProvider::CreateString("Start Value");
   print(cstr);

   cstr->append('!');
   print(cstr);

   cstr->append('?');
   print(cstr);

   cstr->UnDo();
   print(cstr);

   return 0;
}
