#include <iostream>
#include <sstream>

// Get strings on input
#include <array>
// This ctype facet classifies commas and endlines as whitespace
struct StringsWhiteSpace : std::ctype<char> {
  static const mask* make_table() {
    static std::array<mask, table_size> table;
    // make a copy of the "C" locale table
    std::copy(classic_table(), classic_table() + table_size, table.begin());
    table['\0'] |= space;     // comma will be classified as whitespace
    table[' '] &= not space;  // space will not be classified as whitespace
    return table.data();
  }
  StringsWhiteSpace(std::size_t refs = 0) : ctype(make_table(), false, refs) {}
};
std::istream& strings(std::istream& in) {
  in.imbue(std::locale(in.getloc(), new StringsWhiteSpace));
  return in;
}
std::istream& nostrings(std::istream& in) {
  in.imbue(std::locale());
  return in;
}

class Person {
 public:
  std::string name;
  std::string city;
  std::string country;
};

std::istream& operator>>(std::istream& in, Person& person) {
  return in >> person.name >> person.city >> person.country;
}
int main(void) {
  Person person;
  char bytes[] =
      "gladiador\0this is sparta\0 \0jack\0neverland\0alaska\0qualquer outra "
      "coisa";
  std::stringstream input(std::string(bytes, sizeof bytes));

  while (input >> strings >> person) {
    std::cout << "Person:\n"
              << "\tname: " << person.name << '\n'
              << "\tcity: " << person.city << '\n'
              << "\tcontry: " << person.country << std::endl;
  }

  return 0;
}
