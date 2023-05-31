#include "../include/json.hpp" 

struct json::impl {
  bool b_value;
  double d_value;
  std::string s_value;
  // tipo lista
  // tipo dizionario
  // tipo null 
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
  return true;
}

bool json::is_dictionary() const {
  return true;
}

bool json::is_string() const {
  return true;
}
bool json::is_number() const {
  return true;
}

bool json::is_bool() const {
  return true;
}

bool json::is_null() const{
  return true;
}


int main(){
  return 0;
}
