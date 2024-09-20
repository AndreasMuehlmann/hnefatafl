#include "RandomMovesGenerator.hpp"

auto rightPad(std::string string, size_t paddedLength) -> std::string {
  if (paddedLength < string.size()) {
    return string;
  }
  string.insert(string.end(), paddedLength - string.length(), ' ');
  return string;
}
