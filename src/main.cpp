#include "../include/json.hpp" 
#include <iostream>
#include <sstream>

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
  
  list* list_head;
  list* list_tail;

  struct dict{
    std::pair<std::string, json> info;
    dict* next;
  };
  
  dict* dict_head;
  dict* dict_tail;
  
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
json::~json(){

}

bool json::is_list() const {
  return pimpl->type == Type::List;
}

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

std::istream& operator>>(std::istream& hs, json& rhs) {
  if(hs.get() == '"'){
    parse_string(hs, rhs);
  } else if(hs.get() >= 0 && hs.get() <= 9) {

  }

}

int main(){

  return 0;
}
