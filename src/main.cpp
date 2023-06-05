#include "../include/json.hpp" 

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

json::json() : pimpl(new impl) { // constructor
  pimpl->type = Type::Null;
}

// Sono i due copyconstructor??
json::json(json const& other){
  pimpl = new impl(*other.pimpl);
  other.~json();
}

json::json(json&& other){
  pimpl = other.pimpl;
}

json& json::operator=(json const& other) {
  if(this == &other){
    return *this;
  }

  this->~json();
  pimpl = new impl(*other.pimpl);

  return *this;
}

json& json::operator=(json&& other) {
  if(this == &other){
    return *this;
  }

  this->~json();

  pimpl = other.pimpl;
  other.pimpl = nullptr;
  return *this;
}

json const& json::operator[](std::string const& other) const {
  if(pimpl->type != Type::Dict){
    throw json_exception{ .msg = "Json isn't a type dict! operator[]" };
  }

  for(json::impl::dict* curr = pimpl->dict_head; curr != nullptr; curr = curr->next){
    if(curr->info.first == other){
      return curr->info.second;
    }
  } 

  throw json_exception{ .msg = "Key not found! operator[]"};
}


json& json::operator[](std::string const& other) {
  if(pimpl->type != Type::Dict){
    throw json_exception{ .msg = "Json isn't a type dict! operator[]" };
  }

  for(json::impl::dict* curr = pimpl->dict_head; curr != nullptr; curr = curr->next){
    if(curr->info.first == other){
      return curr->info.second;
    }
  } 

  json new_json;

  insert(std::make_pair(other, new_json));

  throw json_exception{ .msg = "Key not found! operator[]"};

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
  //pimpl->b_value = false;
  //pimpl->d_value = 0.0;
  //pimpl->s_value.clear();
  //pimpl->type = Type::Null;

  // Clean up pimpl itself
  delete pimpl;
}

// list control

bool json::is_list() const {
  return pimpl->type == Type::List;
}

void json::set_list(){
  this->~json();
  new (this) json();
  pimpl->type = Type::List;
}

void json::push_front(json const& x) {
  if( pimpl->list_head == nullptr) {
    pimpl->type = Type::List;
    pimpl->list_head = new json::impl::list{x, nullptr};
    pimpl->list_tail = pimpl->list_head;
  } else {
    json::impl::list* new_head = new json::impl::list{x, nullptr};
    new_head->next = pimpl->list_head;
    pimpl->list_head = new_head;
  }
}

void json::push_back(json const& x) {
  if (pimpl->list_head == nullptr){
    pimpl->type = Type::List;
    pimpl->list_head = new json::impl::list{x, nullptr};
    pimpl->list_tail = pimpl->list_head;
  } else {
    pimpl->list_tail->next = new json::impl::list{x, nullptr};
    pimpl->list_tail = pimpl->list_tail->next; 
  }
}

struct json::list_iterator{
  json::impl::list* current;

  list_iterator(json::impl::list* ptr) : current(ptr) {}

  json& operator*() const {
    return current->info;
  }

  list_iterator& operator++() {
    current = current->next;
    return *this;
  }

  bool operator==(const list_iterator& other) const {
    return current == other.current;
  }

  bool operator!=(const list_iterator& other) const {
    return !(*this == other);
  }
  
  json* operator->() const {
    return &current->info;
  }
  //I need to do the operator-> function
};

struct json::const_list_iterator {
  json::impl::list* current;

  const_list_iterator(json::impl::list* ptr) : current(ptr) {}

  json& operator*() const {
    return current->info;
  }

  const_list_iterator& operator++() {
    current = current->next;
    return *this;
  }

  bool operator==(const const_list_iterator& other) const {
    return current == other.current;
  }

  bool operator !=(const const_list_iterator& other) const {
    return !(*this == other);
  }
  
  json* operator->() const {
    return &current->info;
  }
};

json::list_iterator json::begin_list(){
  if(is_list()){
    return list_iterator(pimpl->list_head);
  }
  
  throw json_exception{ .msg = "Json isn't a list! begin_list"};
}

json::list_iterator json::end_list(){
  if(is_list()){
    return list_iterator(pimpl->list_tail);
  }

  throw json_exception{ .msg = "Json isn't a list! end_list"};
}

json::const_list_iterator json::begin_list() const {
  if(is_list()){

  }
  throw json_exception{ .msg = "Json isn't a list! begin_list const"};
}

json::const_list_iterator json::end_list() const {
  if(is_list()){
    return const_list_iterator(pimpl->list_tail);
  }
  throw json_exception{ .msg = "json isn't a list! end_list const"};
}

// dictionary control

bool json::is_dictionary() const {
  return pimpl->type == Type::Dict;
}

void json::set_dictionary(){
  this->~json();
  new (this) json();
  pimpl->type = Type::Dict;
}

void json::insert(std::pair<std::string, json> const& elem) {
  if(pimpl->dict_head == nullptr){
    pimpl->dict_head = new json::impl::dict{elem, nullptr};
    pimpl->dict_tail = pimpl->dict_head;
  } else {
    pimpl->dict_tail->next = new json::impl::dict{elem, nullptr};
    pimpl->dict_tail = pimpl->dict_tail->next;
  }
}

struct json::dictionary_iterator{
  json::impl::dict* current;

  dictionary_iterator(json::impl::dict* ptr) : current(ptr) {}

  std::pair<std::string, json>& operator*() const {
    return current->info;
  }

  dictionary_iterator& operator++() {
    current = current->next;
    return *this;
  }

  bool operator==(const dictionary_iterator& other) const {
    return current == other.current;
  }

  bool operator!=(const dictionary_iterator& other) const {
    return !(*this == other);
  }

  std::pair<std::string, json>* operator->() const {
    return &current->info;
  }

};

struct json::const_dictionary_iterator{
  json::impl::dict* current;

  const_dictionary_iterator(json::impl::dict* ptr) : current(ptr) {}

  std::pair<std::string, json>& operator*() const {
    return current->info;
  }

  const_dictionary_iterator& operator++() {
    current = current->next;
    return *this;
  }

  bool operator==(const const_dictionary_iterator& other) const {
    return current == other.current;
  }

  bool operator!=(const const_dictionary_iterator& other) const {
    return !(*this == other);
  }
  
  std::pair<std::string, json>* operator->() const {
    return &current->info;
  }  
};

json::const_dictionary_iterator json::begin_dictionary() const {
  if(is_dictionary()) {
    return const_dictionary_iterator(pimpl->dict_head);
  }
  throw json_exception{ .msg = "Json isn't a dict! begin_dictionary const" };
}


json::const_dictionary_iterator json::end_dictionary() const {
  if(is_dictionary()) {
    return const_dictionary_iterator(pimpl->dict_tail);
  }
  throw json_exception{ .msg = "Json isn't a dict! end_dictionary const"};
}

json::dictionary_iterator json::begin_dictionary() {
  if(is_dictionary()){
    return dictionary_iterator(pimpl->dict_head);
  }
  throw json_exception{.msg = "Json isn't a dict! begin_dictionary"};  
}

json::dictionary_iterator json::end_dictionary(){
  if(is_dictionary()){
    return dictionary_iterator(pimpl->dict_tail);
  } 
  throw json_exception{ .msg = "Json isn't a dict! end_dictionary"};
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

void json::set_string(std::string const& value){
  this->~json();
  new (this) json();
  pimpl->s_value = value;
  pimpl->type = Type::String;
}

void parse_string(std::istream& hs, std::string& str){
  char c;
  hs >> std::noskipws >> c;

  str.clear();
  while(hs.get(c)) {
    if (c == '\\') {
      if (hs.get(c)){
        switch(c) {
          case 'n': str += '\\'; str += 'n'; break;
          case 'r': str += '\\'; str += 'r'; break;
          case 't': str += '\\'; str += 't'; break;
          case '\"': str += '\\'; str += '"'; break;
          case '\'': str += '\\'; str += '\'';break;
          case '\\': str += '\\'; str += '\\'; break;
          default: str += '\\'; str += c; break;
        }
      } else {
        throw json_exception{ .msg = "Invalid escape sequence"};
      }
    } else if(c == '"'){
      return;
    } else {
      str += c;
    }
  }

  throw json_exception{ .msg = "Error occurred during parsing"};
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


void json::set_number(double number) {
  this->~json();
  new (this) json();
  pimpl->d_value = number;
  pimpl->type = Type::Number;
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
  //pimpl = new impl; 
  pimpl->b_value = value;
  pimpl->type = Type::Boolean;
}

// null control

bool json::is_null() const{
  return pimpl->type == Type::Null;
}

void json::set_null() {
  this->~json();
  new (this) json();  
  pimpl->type = Type::Null;
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

std::ostream& operator<<(std::ostream& lhs, json const& rhs) {
  if(rhs.is_null()) {
    lhs << "null";
  } else if (rhs.is_string()) {
    lhs << rhs.get_string();
  } else if (rhs.is_bool()) {
    lhs << rhs.get_bool();
  } else if (rhs.is_number()){
    lhs << rhs.get_number();
  } else if (rhs.is_list()){
    lhs << "[";
    for(auto iterator = rhs.begin_list(); iterator != rhs.end_list(); ++iterator){
      lhs << *iterator;
      lhs << ",";
    }
    lhs << "]";
  } else if (rhs.is_dictionary()){
    lhs << "{";
    for(auto iterator = rhs.begin_dictionary(); iterator != rhs.end_dictionary(); ++iterator){
      lhs << iterator->first << ":" << iterator->second;
      lhs << ",";
    }
    lhs << "}"; 
  }
  
  return lhs;
}


std::istream& operator>>(std::istream& hs, json& rhs) {
  if(hs.peek() == '"'){
    std::string str;
    parse_string(hs, str);
    rhs.set_string(str);
  } else if(std::isdigit(hs.peek()) || hs.peek() == '-') {
    //parse_double(hs, rhs);
    double num;    
    hs >> num;
    rhs.set_number(num); // Set Num Type
  } else if(hs.peek() == 't' || hs.peek() == 'f') {
    //parse_boolean(hs, rhs); ? serve?
    bool value;
    hs >> value;
    rhs.set_bool(value);
  } else if(hs.peek() == '[') {
    char c;
    hs >> c;  // Read '['
    rhs.set_list(); // Set List Type
    while( hs.peek() != ']'){
      json item;
      hs >> item;
      rhs.push_back(item);
    
      if(hs.peek() == ','){
        hs.ignore();
      }
    }

    hs.ignore();
  } else if(hs.peek() == '{') {
    char c;
    hs >> c;
    rhs.set_dictionary(); // Set Dict Type
    while (hs.peek() != '}'){ // Fill Dict
      std::string key;
      hs >> key;
      if (hs.get() != ':')
        throw json_exception{.msg = "Invalid dictionary format" }; 
      json value;
      hs >> value;
      rhs.insert({key, value});
       
      if(hs.peek() == ',')
        hs.ignore();
    }
    
    hs.ignore();

  }
  return hs;
}

int main(){
  std::ifstream is("in.json");
  std::ofstream os("out.json");
  json j;
  is >> j;
  os << j;
  return 0;
}
