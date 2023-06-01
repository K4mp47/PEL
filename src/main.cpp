#include "../include/json.hpp" 
#include <cctype>
#include <iostream>
#include <istream>
#include <sstream>
#include <string>

// define type of the json struct
enum Type { Boolean, Number, Null, Dict, List, String };



struct json::impl {
  bool b_value;
  
  double d_value;
  
  std::string s_value;
  
  struct list {
    json info;
    list* next;
  };
  
  list* list_head = nullptr;
  list* list_tail = nullptr;

  struct dict{
    std::pair<std::string, json> info;
    dict* next;
  };
  
  dict* dict_head = nullptr;
  dict* dict_tail = nullptr;
  
  Type type;
};

json::json() { // constructor

}

// Sono i due copyconstructor??
json::json(json const&){

}

json::json(json&&){

}

// destructor
json::~json() {
  // Clean up list elements
  impl::list* currentList = pimpl->list_head;
  while (currentList) {
    impl::list* nextList = currentList->next;
    delete currentList;
    currentList = nextList;
  }

  // Clean up dictionary elements
  impl::dict* currentDict = pimpl->dict_head;
  while (currentDict) {
    impl::dict* nextDict = currentDict->next;
    delete currentDict;
    currentDict = nextDict;
  }

  // Reset struct values
  pimpl->b_value = false;
  pimpl->d_value = 0.0;
  pimpl->s_value.clear();
  pimpl->type = Type::Null;

  // Clean up pimpl itself
  delete pimpl;
}

// list control

bool json::is_list() const {
  return pimpl->type == Type::List;
}

// dictionary control

bool json::is_dictionary() const {
  return pimpl->type == Type::Dict;
}

// string control

std::string& json::get_string() {
  if(!is_string()){
    throw json_exception{ .msg = "json is not a string" };
  }

  return pimpl->s_value;
}

std::string const& json::get_string() const {
 if(!is_string()){
    throw json_exception{ .msg = "json is not a string" };
  }

  return pimpl->s_value;
}

bool json::is_string() const {
  return pimpl->type == Type::String;
}

// number control

double& json::get_number() {
  if(!is_number()){
    throw json_exception{ .msg = "json is not a double"};
  }

  return pimpl->d_value;
}

double const& json::get_number() const {
 if(!is_number()){
    throw json_exception{ .msg = "json is not a double"};
  }

  return pimpl->d_value;
}


bool json::is_number() const {
  return pimpl->type == Type::Number;
}

// bool control

bool& json::get_bool(){
  if(!is_bool()) {
    throw json_exception{ .msg = "json is not a boolean" };
  } 

  return pimpl->b_value;
}

bool const& json::get_bool() const {
  if(!is_bool()) {
    throw json_exception{ .msg = "json is not a boolean" };
  }

  return pimpl->b_value;
}

bool json::is_bool() const {
  return pimpl->type == Type::Boolean;
}

void json::set_bool(bool value) {
  this->~json();
  new (this) json();
  
  pimpl->b_value = value;
  pimpl->type = Type::Boolean;
}

// null control

bool json::is_null() const{
  return pimpl->type == Type::Null;
}


// void parse_string()
//  std::string str;
// 
// while(lhs.peek() != "" || lhs.peek() != EOF)
//  str += lhs.get();
//
//  rhs.set_string(str);


//put_back(cosa voglio in char) ributta nella stream quello che voglio

//void parse_boolean(std::istream& hs, json& rhs) {
//  bool value;
//  hs >> value;
//}

std::istream& operator>>(std::istream& hs, json& rhs) {
  if(hs.peek() == '"'){
    hs.get();
    //parse_string(hs, rhs);
  } else if(std::isdigit(hs.peek())) {
    //parse_double(hs, rhs);
  } else if(hs.peek() == 't' || hs.peek() == 'f') {
    //parse_boolean(hs, rhs); ? serve?
    bool value;
    hs >> value;
    rhs.set_bool(value);
  } else if(hs.peek() == '[') {
    char c;
    hs >> c;  // Read '['
    while( hs.peek() == ']'){
      json item;
      hs >> item;
      rhs.push_back(item);
    }

    //rhs.pimpl->type = Type::List;
    //rhs.pimpl->list_head = nullptr;
    //rhs.pimpl->list_tail = nullptr;
  }
  return hs;
}

int main(){

  return 0;
}
