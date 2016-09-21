#ifdef _MSC_VER
#include "stdafx.h"
#endif

#define TEST_MODE 1

#include <string>
#include <stack>
#include <string>
#include <memory>
#include <mutex>
#include <iostream>
#include <algorithm>

#ifdef TEST_MODE
#define PRINT_VALUE(VALUE) \
    std::cout << VALUE << std::endl;
#else
#define PRINT_VALUE(VALUE) (void) VALUE;
#endif

// Class provides a simple restoring by saving all values
// Pro: fast implmentation, CPU performance
// Cons: RAM consaption is maximum
template<typename T>
class FastRestorer {
public:
    static void set_last_changed_string(T* pointer)
    {
       std::lock_guard<std::mutex> lock(
          history_lock);
       access_history.push_back(pointer);
    }
    static T* get_last_changed_string()
    {
       std::lock_guard<std::mutex> lock(
          history_lock);
       if (access_history.empty()) {
          return nullptr;
       }
       T* last = access_history.back();
       access_history.pop_back();
       return last;
    }

    static void clear_history(T* pointer)
    {
       std::lock_guard<std::mutex> lck(
          history_lock);
       std::remove(
          access_history.begin(),
          access_history.end(),
          pointer);
    }

    static std::mutex history_lock;
    static std::deque<T*> access_history;
};
template<typename T>
std::mutex FastRestorer<T>::history_lock;
template<typename T>
std::deque<T*> FastRestorer<T>::access_history;


// Class provides a simple interface for manipulating
// * adding new char to the end of string
// * remove last char of the string
// * reject (UnDo) last change with any string of this type
class CustomString {
    typedef FastRestorer<CustomString> RestorerType;
public:
   // Add char to the end of string
   void append(char ch)
   {
      save_value();
      _str.push_back(ch);
      PRINT_VALUE(get_value());
   }
   // remove last char in the string
   void erase()
   {
      save_value();
      if (_str.empty()) {
         return;
      }
      _str.pop_back();
      PRINT_VALUE(get_value());
   }
   // return current string value
   const std::string& get_value() const
   {
      return _str;
   }
   // restore last changes element state
   static void UnDo()
   {
   // TODO(EZamkhov): Move logic to restore_value
   // for redability restore_value<->save_value
      CustomString* last = RestorerType::get_last_changed_string();
      if (last == nullptr) {
         return;
      }
      last->restore_value();
   }
private:
   friend class CustromStringProvider;
   explicit CustomString(const std::string& str) : _str(str)
   {
       PRINT_VALUE(get_value());
   }
   ~CustomString()
   {
      RestorerType::clear_history(this);
   }
   void save_value()
   {
      RestorerType::set_last_changed_string(this);
      _states.push(_str);
   }
   void restore_value()
   {
      if (_states.empty()) {
         return;
      }
      _str = _states.top();
      _states.pop();
      PRINT_VALUE(_str);
   }
   std::string _str;
   std::stack<std::string> _states;
};



// TODO-list(EZamakhov) :
// * add template for string types
// * add logging error states
class CustromStringProvider {
public:
   static std::shared_ptr<CustomString>
      CreateString(const std::string& str = std::string())
   {
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

void TEST(std::shared_ptr<CustomString> cstr, const std::string& expected) {
    const std::string& actual = cstr->get_value();
    if(actual != expected) {
        std::cout << "TEST FAILED!" << std::endl
                  << "Expected: \"" << expected << '"' << std::endl
                  << "Actual:   \"" << actual << '"' << std::endl;
    }
    else
    {
        std::cout << "TEST OK!" << std::endl;
    }
}



int main(int argc, char* argv[])
{
   std::shared_ptr<CustomString> cstr =
      CustromStringProvider::CreateString("Start Value");
   // Test initalization
   TEST(cstr, "Start Value");

   // Test adding char
   cstr->append('!');
   TEST(cstr, "Start Value!");

   //test removing char
   cstr->erase();
   TEST(cstr, "Start Value");

   // Test restore removing char
   cstr->UnDo();
   TEST(cstr, "Start Value!");

   // test restore adding char
   cstr->UnDo();
   TEST(cstr, "Start Value");

   // test restore in case of no states
   cstr->UnDo();
   TEST(cstr, "Start Value");
   // twice
   cstr->UnDo();
   TEST(cstr, "Start Value");

   cstr.reset();
   cstr = CustromStringProvider::CreateString();

   // Test adding chars
   cstr->append('1');
   TEST(cstr, "1");
   cstr->append('2');
   TEST(cstr, "12");
   cstr->append('3');
   TEST(cstr, "123");
   cstr->append('4');
   TEST(cstr, "1234");

   // test restore adding char
   cstr->UnDo();
   TEST(cstr, "123");
   cstr->UnDo();
   TEST(cstr, "12");
   cstr->UnDo();
   TEST(cstr, "1");
   cstr->UnDo();
   TEST(cstr, "");
   cstr->UnDo();
   TEST(cstr, "");

   return 0;
}
